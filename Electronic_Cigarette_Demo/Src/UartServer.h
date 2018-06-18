#ifndef __UART_SERVER_H__
#define __UART_SERVER_H__

#include "stm32f0xx.h"




#define PACK_HEAD 0XAD
#define PACK_END  0XBF





void UartReceiveBUF_Proc(void);
void  UartTimeoutProc(void);
void PackProcCallBack(uint8_t *PackBuf);

#endif
