#ifndef __QUEUE_H
#define __QUEUE_H

#include "stm32f4xx.h"

#define MAX_QUEUE_LEN  (1024) // 1K
#define RW_OK         0
#define FULL_ERROR    1
#define EMPTY_ERROR   2

typedef uint8_t boolean;

typedef struct
{
    uint16_t MemFrontSendIndex ;
    uint16_t MemRearRecvIndex ;
    uint16_t MemLength ;
    uint8_t  MemDataBuf[MAX_QUEUE_LEN];
} Queue_Mem_Struct , * Queue_Mem_Struct_p ;

extern Queue_Mem_Struct Queue_Recv ;

boolean QueueMemDataInsert(uint8_t data);
boolean QueueMemDataDel(uint8_t *data);

#endif
