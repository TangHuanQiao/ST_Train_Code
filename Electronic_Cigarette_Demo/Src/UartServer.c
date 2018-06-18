#include "stm32f0xx_hal.h"
#include <string.h>
#include "UartServer.h"



#define REC_BYTE_TC 5	//�����ֽڵ�������ʱ��
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
		RecReadyFlag=1;//���ݰ����ڽ���
	}
	else
	{
		if(RecReadyFlag==1)RecReadyFlag=2;//���ݰ���Ҫ����
			
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
				if(!PackLength)//��Ѱ��ͷ
				{
					if(UartReceiveBUF[i]==PACK_HEAD)
					{
					PackBuf = UartReceiveBUF+i;
					PackLength=i+1;//��¼��ͷ�±ꡣ
					
					}
				}
				else
				{			
					//��Ѱ��β,
					if(UartReceiveBUF[i]==PACK_END)
					{
						PackLength=i-(PackLength-1)+1;//���ݰ�β�±�ͼ�¼�İ�ͷ�±����һ֡���ݵĳ���;
	
						if(PackLength!=(PackBuf[1]+2) )
							continue;//���Ȳ�����					
	
							//У���
							for(j=0;j<PackLength-3;j++)
								Sum+=PackBuf[j+1];
	
							if(Sum!=PackBuf[PackLength-2])							
								break;
																				
							PackProcCallBack(PackBuf);//��������֡
	
							break;
					  } 					
	
					}	

				}
			
//		 HAL_UART_Transmit(&huart2,UartReceiveBUF,RecIndex,0X1000);

		 memset(UartReceiveBUF, 0, sizeof(UartReceiveBUF));//��ս��ջ��� 
		 RecReadyFlag=0;//���ݴ������
		 RecIndex=0;//���ݴ�����ɣ�������������
		}

  HAL_UART_Receive_IT(&huart2, (uint8_t *)(ITRXBuff), 1);//��ʱ���������жϣ���ֹ�ж��ﷱæ������
}
