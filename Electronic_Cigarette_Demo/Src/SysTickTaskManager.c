#include "SysTickTaskManager.h"
#include "UartServer.h"
#include "message.h"
#include "PowerCtr.h"
#include "Keyboard.h"
#include "User.h"
#include "task_protocol.h"

static SYSTICK_TASK SysTickTask_Tab[]=
{ 
//	 {UartReceiveBUF_Proc,0,2},
	 {AppTaskProtoParse,0,5},
	 {PowerCtrTask,3000,1},
	 {keyScanTask,0,20},
	 {CheckCurrentResistorsProc,0,100},
	 {UpdateDisPlayResistors,2000,200}
};





void HAL_SYSTICK_Callback(void)
{
//	UartTimeoutProc();
	
}






void SysTickTaskExecute() 
{
	uint8_t i;

 	for(i=0;i<ARRAYSIZE(SysTickTask_Tab);i++)
	{   
		if((int)(HAL_GetTick()-SysTickTask_Tab[i].uNextTick)>=0)
		{		
			SysTickTask_Tab[i].fTask();
			SysTickTask_Tab[i].uNextTick=HAL_GetTick()+SysTickTask_Tab[i].uLenTick;	
		}
	}
	
}	





