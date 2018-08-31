/*
***************************************************************************
*    模块：Queue
*    描述：Queue 循环队列
*    作者：Huo
*    时间：2017.09.06
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "Queue.h"

/* 循环队列全局变量定义 */
Queue_Mem_Struct Queue_Recv ;

/*
***************************************************************************
*	函 数 名: QueueMemDataInsert
*	功能说明: 循环队列 插入数据函数
*	形    参: data 插入的数据
*	返 回 值: 插入数据成功与否
***************************************************************************
*/
boolean QueueMemDataInsert(uint8_t data)
{
    if (MAX_QUEUE_LEN == Queue_Recv.MemLength)
    {
        return FULL_ERROR;
    }
    else
    {
        Queue_Recv.MemDataBuf[Queue_Recv.MemRearRecvIndex] = data ;
        /*
        		if(++Queue_Recv.MemRearRecvIndex >= MAX_QUEUE_LEN)
        			{Queue_Recv.MemRearRecvIndex = 0;}
        */
        Queue_Recv.MemRearRecvIndex =
            (Queue_Recv.MemRearRecvIndex + 1) % MAX_QUEUE_LEN;
        Queue_Recv.MemLength ++ ;
        return RW_OK;
    }
}

/*
***************************************************************************
*	函 数 名: QueueMemDataDel
*	功能说明: 循环队列 读出数据函数
*	形    参: *data 读出数据存放地址
*	返 回 值: 读出数据成功与否
***************************************************************************
*/
boolean QueueMemDataDel(uint8_t *data)
{
    if (0 == Queue_Recv.MemLength)
    {
        return EMPTY_ERROR;
    }
    else
    {
        *data = Queue_Recv.MemDataBuf[Queue_Recv.MemFrontSendIndex];
        Queue_Recv.MemFrontSendIndex =
            (Queue_Recv.MemFrontSendIndex + 1) % MAX_QUEUE_LEN;
        Queue_Recv.MemLength -- ;
        return RW_OK;
    }
}
