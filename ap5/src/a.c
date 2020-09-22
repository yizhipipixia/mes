

#include <stdio.h>





#include <error.h>





#include <stdlib.h>





#include <pthread.h>





void* thread_run(void* _val)





{





pthread_detach(pthread_self()); //注释这句代码join success





printf("%s\n", (char*)_val);





return NULL;





}

















int main()





{





pthread_t tid;





int tret = pthread_create(&tid, NULL, thread_run, "thread_run~~~~~");





//线程创建成功之后，程序的执行流变成两个，一个执行函数thread_run，一个继续向下执行。





if (tret == 0)





{





sleep(1);





int ret = pthread_join(tid, NULL);





if (ret == 0)





{





printf("pthread_join success\n");





return ret;





}





else





{





printf("pthread_join failed info: %s\n", strerror(ret));





return ret;





}





}





else





{





printf("create pthread failed info: %s", strerror(tret));





return tret;





}





}
