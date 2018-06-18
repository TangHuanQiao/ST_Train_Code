#ifndef __SYS_TICK_TASK_MANAGER_H__
#define __SYS_TICK_TASK_MANAGER_H__


#include "stm32f0xx.h"

#define ARRAYSIZE(array) (sizeof(array)/sizeof(array[0]))

typedef struct{
	     void (*fTask)(void);
		 uint32_t uNextTick;
	     uint32_t uLenTick;
         
}SYSTICK_TASK;

void SysTickTaskExecute(void);

#endif
