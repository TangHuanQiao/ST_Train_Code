#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "stm32f0xx.h"

//Define message queue constants
#define MSG_QUEUE_SIZE		128				//128 32bit words
#define MSG_INDEX_MASK		0x007f
#define MAX_QUEUE_MSG		64	

//Define message procedure prototypes
//If the message is successfully processed and it should be
//removed from the queue, the function should return true,
//otherwise the function should return false and the message
//will post back to the message queue.
typedef int (*MESSAGE_PROC)(uint32_t uMessage);
 

//Define the message structure
typedef struct TagMessage{
		MESSAGE_PROC	MessageProc;
		uint32_t  		uMessage;
		}MESSAGE;

//Define the data transmtting request structure
//The first byte of the data block is the size of the data block
//that is to be transmitted
typedef struct DataTransReq{
		MESSAGE_PROC	MessageProc;
		uint8_t  		*pData;
		}DATA_TRANS_REQ;

//The message function prototypes
int PostMessage(MESSAGE_PROC MessageProc, uint32_t uMessage);
int GetMessage(MESSAGE* pMessage);
int DispatchMessage(MESSAGE* pMessage);
uint32_t GetNumMessage(void);


/*The end of message.h */

#endif

