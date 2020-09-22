#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#include <signal.h>

#include "module.h"
#include "list.h"

pthread_t listen_tid;

pthread_mutex_t sync_mutex; 		//互斥锁

void my_func(int sign_no)
{
	if(sign_no==SIGPIPE)
		printf("Program recvied SIGPIPE\n");
}

int main(int argc, char *argv[])
{
	int ret;
	
	OS_InitTCBList();
	signal(SIGPIPE,my_func);
	
	pthread_mutex_init(&sync_mutex, NULL);	  
	
	/* listen thread */
	ret = pthread_create(&listen_tid, NULL, thread_listen, NULL);
	if (ret) {
		errno = ret;
		perror("create listen thread");
		exit(EXIT_FAILURE);
	} else
		printf("create listen thread success\n");

	ret = pthread_detach(listen_tid);
	if (ret) {
		errno = ret;
		perror("detach listen thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach listen thread success\n");	
	

	//pthread_mutex_lock(&sync_mutex); //然后再建立TCP数据收发和心跳包发送线程	
	//pthread_mutex_unlock(&sync_mutex);
	
	
	/* main thread */
	while (1) {
		sleep(60);
	}
 	pthread_mutex_destroy(&sync_mutex);
	exit(EXIT_SUCCESS);
}
