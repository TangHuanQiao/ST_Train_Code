/**
  ******************************************************************************
  * @file    message.c
  * @author  Luo Zhi Xiang
  * @version V1.1.0
  * @date    2012-05-20
  * @brief   The Message Manipulation Source File.
  ******************************************************************************  
  **/

#include "message.h"

//Define the global message variables
volatile uint32_t	ReadIndex  = 0;
volatile uint32_t	WriteIndex = 0;
volatile uint32_t	NumMessage = 0;
volatile uint32_t	MessageQueue[MSG_QUEUE_SIZE];

/*******************************************************************************
* Function Name  : PostMessage
* Description    : Post message to the message queue
* Input          : MessageProc--The address of the message processing procedure
*				 : uMessage-----The message prarameter
* Output         : None
* Return         : If the message is successfully posted to the message queue
*				   the function returns ture, otherwise returns false
*******************************************************************************/
int PostMessage(MESSAGE_PROC MessageProc, uint32_t uMessage)
{
	uint32_t PriMaskBackup;

	PriMaskBackup = __get_PRIMASK();
	__disable_irq();
	if(	NumMessage < MAX_QUEUE_MSG)
	{ 
		MessageQueue[WriteIndex++] = (uint32_t)MessageProc;
		MessageQueue[WriteIndex++] = uMessage;
		NumMessage++;
		WriteIndex &= MSG_INDEX_MASK;
		__set_PRIMASK(PriMaskBackup);
		return -1;
	}
	else
	{
		//Initiate system reset
 		NVIC_SystemReset();
		return 0;
	}	
}


//Define the get message function
int GetMessage(MESSAGE* pMessage)
{
	uint32_t PriMaskBackup;

	if(!NumMessage) return 0;
	pMessage->MessageProc 	= (MESSAGE_PROC)MessageQueue[ReadIndex++];
	pMessage->uMessage 		= MessageQueue[ReadIndex++];
	ReadIndex &= MSG_INDEX_MASK;
	PriMaskBackup = __get_PRIMASK();
	__disable_irq();
	NumMessage--;
	__set_PRIMASK(PriMaskBackup);
	return -1;
}

//Define dispatch message function
int DispatchMessage(MESSAGE* pMessage)
{
	return (*(pMessage->MessageProc))(pMessage->uMessage);
}

 
 uint32_t GetNumMessage(void)
 {
	return NumMessage;
 }

/* The end of message.c */



