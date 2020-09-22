#include "list.h"

unsigned char OSUserCtr = 0;

OS_TCB OSTCBTbl[OS_MAX_POINTS]={{0,0,0,0,},}; //用户数组初始化 

void  OS_InitTCBList (void)
{
	unsigned char   ix;
	unsigned char   ix_Next;
	OS_TCB  *ptcb1;
	OS_TCB  *ptcb2;
	
	memset((unsigned char *)&OSTCBTbl[0],0,sizeof(OS_TCB)*OS_MAX_POINTS); // Clear all the TCBs
	for (ix = 0u; ix < (OS_MAX_POINTS  - 1u); ix++)                       // Init. list of free TCBs
	{
		ix_Next =  ix + 1u;
		ptcb1   = &OSTCBTbl[ix];
		ptcb2   = &OSTCBTbl[ix_Next];
		ptcb1->Next = ptcb2;
	}
	ptcb1 = &OSTCBTbl[ix];
	ptcb1->Next = (OS_TCB *)0; // Last OS_TCB
	OSTCBList = (OS_TCB *)0;        //真正用户使用时的链表     // TCB lists initializations
	OSTCBFreeList = &OSTCBTbl[0];   //这是空闲节点结构链表 
}

OS_TCB *GetNode()
{
	OS_TCB *ptcb = (OS_TCB *)0;
	ptcb=OSTCBFreeList;
	if (OSTCBFreeList != (OS_TCB *)0)
	{ 
		OSTCBFreeList=OSTCBFreeList->Next;
	}
	memset((unsigned char*)ptcb,0,sizeof(OS_TCB));
	return ptcb;
}

int PutNode(OS_TCB *ptcb)
{
	memset((unsigned char*)ptcb,0,sizeof(OS_TCB));
	ptcb->Next=OSTCBFreeList;
	OSTCBFreeList=ptcb;
	return 0; 
}

int InsertUserInfo(unsigned int id,int fd)
{
	OS_TCB *ptcb=GetNode();

	if(ptcb == (OS_TCB*)0)
	{
	    printf("InsertUserInfo:get node faile!\n");
		return -1;//如果申请空闲节点没有了，退出 
	}
		
	ptcb->UserId = id;
	ptcb->connfd = fd;
	if(OSTCBList == (OS_TCB*)0)
	{
		OSTCBList=ptcb;	
	}
	else
	{
		ptcb->Next=OSTCBList;
		ptcb->Prev=OSTCBList->Prev;
		OSTCBList->Prev=ptcb;
		OSTCBList=ptcb;		
	}
	OSUserCtr++;
	return 0;
}

int DelUserInfo(unsigned int id)
{
	OS_TCB *ptcb;
	      
	ptcb = OSTCBList;
	
	if(ptcb == (OS_TCB *)0)
	{
		printf("DelUserInfo:list already empty!\n");
		return -1;		//链表已经空了
	}
		 
	while(ptcb->UserId!=id)
	{
		ptcb = ptcb->Next;
		if(ptcb==(OS_TCB*)0)break;
	}
	if(ptcb==(OS_TCB *)0)
	{
		printf("DelUserInfo:no node!\n");
		return -1;	//查找的节点不存在 
	}
    //如果节点存在，删除节点 
   	if (ptcb == OSTCBList && ptcb->Next!=(OS_TCB*)0) 
	{   
	    //是头节点，但后面还有节点的情况                                             
        ptcb->Next->Prev = (OS_TCB *)0;
        OSTCBList = ptcb->Next;
    } 
	else if(ptcb->Next==(OS_TCB *)0 && ptcb->Prev!=(OS_TCB*)0)
	{
		//是尾节点，但前面还有节点的情况 
   		ptcb->Prev->Next=ptcb->Next;		
    }
    else if(ptcb->Next==(OS_TCB *)0 && ptcb->Prev==(OS_TCB*)0)
    {
    	//即是头节点又是尾节点的情况
		OSTCBList=(OS_TCB*)0; 
    }
    else    
   	{
   		//中间节点 
        ptcb->Prev->Next = ptcb->Next;
        ptcb->Next->Prev = ptcb->Prev;	   	
   	}
    PutNode(ptcb);
   	OSUserCtr--; 
	return 0;		
}

int SeachId(unsigned int id)
{
	OS_TCB *ptcb;
	      
	ptcb = OSTCBList;
		
	if(ptcb == (OS_TCB *)0)
	{
		printf("SeachId:list already empty!\n");
		return -1;		//链表已经空了	
	}

    while(ptcb->UserId!=id)
	{
		printf("ptcb->UserId is %d\n",ptcb->UserId);
		ptcb = ptcb->Next;
		if(ptcb==(OS_TCB*)0)break;
	}
	
	if(ptcb==(OS_TCB *)0)
	{
		printf("SeachId:no node!\n");
		return -1;	//查找的节点不存在 	
	}
	printf("User fd is %d\n",ptcb->connfd);
	return 0;
}
