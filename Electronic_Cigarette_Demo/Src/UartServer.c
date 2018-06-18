#include "stm32f0xx_hal.h"
#include <string.h>
#include "UartServer.h"



#define REC_BYTE_TC 5	//相邻字节的最大接收时间
uint8_t ITRXBuff[1];
static uint8_t UartReceiveBUF[16];
static volatile uint8_t RecIndex=0; 
static volatile uint8_t RecTimer=0;
static volatile uint8_t RecReadyFlag=0;
extern UART_HandleTypeDef huart2;


 
__weak void PackProcCallBack(uint8_t *PackBuf)
{
	switch(PackBuf[2])
		{



		}

	
}



void  UartTimeoutProc(void)
{
	if(RecTimer)
	{
		RecTimer--;
		RecReadyFlag=1;//数据包正在接收
	}
	else
	{
		if(RecReadyFlag==1)RecReadyFlag=2;//数据包需要处理
			
	}
			

}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
////  if(huart==&huart2)
//   {
//  /* Increment Index for buffer writing */
//		if(RecIndex<sizeof(UartReceiveBUF))
//		UartReceiveBUF[RecIndex++]=*ITRXBuff;
//    RecTimer=REC_BYTE_TC;

//  }

//  /* Start another reception: provide the buffer pointer with offset and the buffer size */
//  HAL_UART_Receive_IT(huart, (uint8_t *)(ITRXBuff), 1);
//}



void UartReceiveBUF_Proc(void)
{

	if(RecReadyFlag==2)
		{
			uint8_t i,j;
			uint8_t *PackBuf;
			uint8_t PackLength=0;
			uint8_t Sum=0;
		
			for(i=0;i<sizeof(UartReceiveBUF)-1;i++)
			{
				if(!PackLength)//搜寻包头
				{
					if(UartReceiveBUF[i]==PACK_HEAD)
					{
					PackBuf = UartReceiveBUF+i;
					PackLength=i+1;//记录包头下标。
					
					}
				}
				else
				{			
					//搜寻包尾,
					if(UartReceiveBUF[i]==PACK_END)
					{
						PackLength=i-(PackLength-1)+1;//根据包尾下标和记录的包头下标计算一帧数据的长度;
	
						if(PackLength!=(PackBuf[1]+2) )
							continue;//长度不符合					
	
							//校验和
							for(j=0;j<PackLength-3;j++)
								Sum+=PackBuf[j+1];
	
							if(Sum!=PackBuf[PackLength-2])							
								break;
																				
							PackProcCallBack(PackBuf);//处理数据帧
	
							break;
					  } 					
	
					}	

				}
			
//		 HAL_UART_Transmit(&huart2,UartReceiveBUF,RecIndex,0X1000);

		 memset(UartReceiveBUF, 0, sizeof(UartReceiveBUF));//清空接收缓冲 
		 RecReadyFlag=0;//数据处理完成
		 RecIndex=0;//数据处理完成，接收索引归零
		}

  HAL_UART_Receive_IT(&huart2, (uint8_t *)(ITRXBuff), 1);//定时启动接收中断，防止中断里繁忙不启动
}
