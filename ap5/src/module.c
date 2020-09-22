#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>

#include "module.h"
#include "list.h"

#define MAX_USER_NUM 40
#define BUFFER_SIZE 128

extern pthread_mutex_t sync_mutex; 

pthread_t client_tid;

int cnt = 0;

void *thread_listen(void *arg)
{
	int listen_fd,connfd=0;
	int ret;
	struct sockaddr_in server_addr;
	
	//创建套结字
	listen_fd = socket(AF_INET,SOCK_STREAM,0);//ipv4 tcp 
	
	if(listen_fd < 0)
	{
		perror("create socket fail");
		exit(-1);
	}
	
	//优化4： 允许绑定地址快速重用 
	int b_reuse = 1;
	  (listen_fd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof (int));
	
	//封装一个套结字地址结构	
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.250");
	//绑定：将套结字和套结字地址结构绑定在一起
	ret = bind(listen_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret < 0)
	{
		perror("bind fail");
		exit(-1);
	}
	
	//将普通套结字变成监听套结字
	//监听套结字的功能：只接受新的连接，不能收发消息
	ret = listen(listen_fd,MAX_USER_NUM);
	if(ret < 0)
	{
		perror("listen fail");
		exit(-1);
	}
	while(1)
	{		
      	connfd=accept(listen_fd,NULL,NULL);
		if(connfd < 0)
		{
			perror("accept fail");
			continue;
		}
		/* client thread */
		ret = pthread_create(&client_tid, NULL, thread_client, (void*)connfd);
		if (ret) {
			errno = ret;
			perror("create client thread");
			continue;
		} else
			printf("create client thread success\n");

		ret = pthread_detach(client_tid);
		if (ret) {
			errno = ret;
			perror("detach client thread");
			continue;
		} else
			printf("detach client thread success\n");
	}
	close(listen_fd);
}

//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	void SeachFriendId()//查询一个在线用户的connfd 通过你输入的id号
	{
		memset(recv_buf,0,sizeof(recv_buf));
		unsigned int rat
		recv(connfd,recv_buf,BUFFER_SIZE,0);//输入你要查询用户的id号
		rat = SeachId((unsigned int)atoi(recv_buf));
		memset(send_buf,0,sizeof(send_buf));
		if(rat==-1)
		{       
				strcat(send_buf,"seach fail,try to other value\r\n");//原型extern char *strcat(char *dest, const char *src);

				send(connfd,send_buf,BUFFER_SIZE,0);	
		}
		else 
		{
			strcat(send_buf,"seach success,the user`s connfd is \r\n");
			send(connfd,send_buf,BUFFER_SIZE,0);
			memset(send_buf,0,sizeof(send_buf));
			strcat(send_buf,rat);
			send(connfd,send_buf,BUFFER_SIZE,0);//回显查询节点的connfd			
		}
	    
		
		
	}
	//0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
void *thread_client(void *arg)
{
	char send_buf[BUFFER_SIZE];
	char recv_buf[BUFFER_SIZE];
	
	int ret;
	int connfd = (int)arg;
	
	memset(send_buf,0,sizeof(send_buf));
	strcat(send_buf,"input 1 to seach user`s connfd by id,2 is contact\r\n");
	//++++++++++++++++++++上面是输入提示 (contact 中文是 联系人)
	send(connfd,send_buf,BUFFER_SIZE,0);	
	
	memset(recv_buf,0,sizeof(recv_buf));
	recv(connfd,recv_buf,BUFFER_SIZE,0);
	//下面将recv_buf 地址给到双链表节点内
	InsertUserInfo((unsigned int)atoi(recv_buf),connfd);//atoi是把字符串转换成整型数的一个函数
	printf("%d\n",(unsigned int)atoi(recv_buf));	
	//.........................................................................................
	unsigned int key ;
	while(1)
	{
		memset (recv_buf,0,sizeof(recv_buf));
		recv(connfd,recv_buf,BUFFER_SIZE,0);
		key =(unsigned int)atoi(recv_buf);
		switch(key)
		{
			
			case 1:
			SeachFriendId();
			break;
			
		
		    case 2://老师写的以下
			memset(send_buf,0,sizeof(send_buf));
		    strcat(send_buf,"Please input Friendid:\r\n");
			send(connfd,send_buf,BUFFER_SIZE,0);
			recv(connfd,recv_buf,BUFFER_SIZE,0);
			
			printf("%d\n",(unsigned int)atoi(recv_buf));
			Frined_fd = SeachId()
			
			memset(send_buf,0,sizeof(send_buf));
			strcat(send_buf,recv_buf);
		    strcat(send_buf,"\r\n");
			send(connfd,send_buf,BUFFER_SIZE,0);
		break;
		
		}
		
		if(key==2)break;
		
	}	
	
//.....................................................................	
	
	
	
	while(1)
	{
		memset(recv_buf,0,sizeof(recv_buf));
		ret = recv(connfd,recv_buf,BUFFER_SIZE,0);
		if(ret == -1)
		{
			perror("recv fail");
			pthread_exit(NULL);
		}
		if(ret == 0)
		{
			//管道破裂，recv函数读出值为0，而且不再阻塞
			printf("recv return %d\n",ret);
		}
		//当我们去读取一个破裂的数据管道时，会收到SIGPIPE信号，默认会终止进程
		

		printf("Recv data is :%s\n",recv_buf);
		if(strncmp("quit",recv_buf,4)==0)break;
		//strcpy(buf,"WELCOME!\n");
		ret = send(connfd,send_buf,BUFFER_SIZE,0);	
		printf("send return %d\n",ret);
		if(ret == -1)
		{
			//管道破裂，send函数返回值为0，而且不再阻塞，程序收到SIGPIPE信号，默认关闭进程
			break;
		}
	}
	printf("client %d quit!\n",connfd);
	close(connfd);
	pthread_exit(NULL);
}


