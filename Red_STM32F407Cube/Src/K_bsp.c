#include "k_bsp.h"
#include "GUI.h"
extern SPI_HandleTypeDef hspi2;	
uint8_t Xcmd=0xD0,Ycmd=0x90,Z1cmd=0xB0;
TouchRawDataType TouchRawData;
GUI_PID_STATE TS_State;

void k_BspInit(void)
{



}






void k_TouchUpdate(void)
{
	uint8_t  readValue[2]={0};
	

	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,&Xcmd,1,0xffff);
	HAL_SPI_Receive(&hspi2,readValue,2,0xffff);
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_SET);	
		
	TouchRawData.X_RAW=readValue[0];
	TouchRawData.X_RAW<<=8;
	TouchRawData.X_RAW|=readValue[1];
	TouchRawData.X_RAW>>=3;
		
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,&Ycmd,1,0xffff);
	HAL_SPI_Receive(&hspi2,readValue,2,0xffff);
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_SET);
		
	TouchRawData.Y_RAW=readValue[0];
	TouchRawData.Y_RAW<<=8;
	TouchRawData.Y_RAW|=readValue[1];
	TouchRawData.Y_RAW>>=3;
		
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,&Z1cmd,1,0xffff);
	HAL_SPI_Receive(&hspi2,readValue,2,0xffff);
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_SET);
		
	TouchRawData.Z1_RAW=readValue[0];
	TouchRawData.Z1_RAW<<=8;
	TouchRawData.Z1_RAW|=readValue[1];
	TouchRawData.Z1_RAW>>=3;	


	
	


	if(TouchRawData.Z1_RAW>500&&TouchRawData.Z1_RAW<2000)
	{		
		TS_State.Pressed=1;	
		TS_State.y=TouchRawData.Y_RAW;
	  TS_State.x=TouchRawData.X_RAW;
	}else
	{
		TS_State.Pressed=0;
	}
	
	GUI_TOUCH_StoreStateEx(&TS_State);
}




















