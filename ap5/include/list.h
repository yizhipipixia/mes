#ifndef  _USER_LIST_H
#define  _USER_LIST_H

#include <string.h>
#include <stdio.h>

#define OS_MAX_POINTS 20

typedef struct node		            //节点结构体                                                                                                
{                                                                                                                                            
	unsigned char UserId;    
	unsigned char connfd;                                                                                                                       

    struct node *Next;         // Pointer to next 
	struct node *Prev;			// Pointer to prev
}OS_TCB;

OS_TCB *OSTCBList;
OS_TCB *OSTCBFreeList;

unsigned char OSUserCtr;// 用户节点数量

OS_TCB OSTCBTbl[OS_MAX_POINTS]; //节点数组

void  OS_InitTCBList(void);
OS_TCB *GetNode();
int PutNode(OS_TCB *ptcb);
int InsertUserInfo(unsigned int id,int fd);
int DelUserInfo(unsigned int id);
#endif

