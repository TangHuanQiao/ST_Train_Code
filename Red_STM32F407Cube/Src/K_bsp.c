#include "k_bsp.h"
#include "GUI.h"


void k_BspInit(void)
{



}




extern SPI_HandleTypeDef hspi2;	
uint8_t Xcmd=0xD0,Ycmd=0x90,Z1cmd=0xB0;
uint16_t X_RAW=0, Y_RAW=0,Z1_RAW=0;
uint8_t  readValue[2]={0};
int tUIx,tUIy;
GUI_PID_STATE TS_State;
void k_TouchUpdate(void)
{
	
	
	
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,&Xcmd,1,0xffff);
	HAL_SPI_Receive(&hspi2,readValue,2,0xffff);
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_SET);	
		
	X_RAW	=readValue[0];
	X_RAW<<=8;
	X_RAW|=readValue[1];
	X_RAW>>=3;
		
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,&Ycmd,1,0xffff);
	HAL_SPI_Receive(&hspi2,readValue,2,0xffff);
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_SET);
		
	Y_RAW	=readValue[0];
	Y_RAW<<=8;
	Y_RAW|=readValue[1];
	Y_RAW>>=3;
		
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,&Z1cmd,1,0xffff);
	HAL_SPI_Receive(&hspi2,readValue,2,0xffff);
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_SET);
		
	Z1_RAW	=readValue[0];
	Z1_RAW<<=8;
	Z1_RAW|=readValue[1];
	Z1_RAW>>=3;	


	tUIx=(X_RAW-240)/11.25;
	if(tUIx<0)
		tUIx=0;
	if(tUIx>320)
		tUIx=320;
	
	tUIy=(Y_RAW-257)/6.94;
		if(tUIy<0)
		tUIy=0;
	if(tUIy>480)
		tUIy=480;
	
	

	
	
	TS_State.Layer=0;
	TS_State.Pressed=0;
	if(Z1_RAW>500&&Z1_RAW<2000)
	{		
		TS_State.Pressed=1;	
		TS_State.y=480-tUIy;
	  TS_State.x=320-tUIx;
	}
	
	GUI_TOUCH_StoreStateEx(&TS_State);
}




















