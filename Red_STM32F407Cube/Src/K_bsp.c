#include "k_bsp.h"

void k_BspInit(void)
{



}




extern SPI_HandleTypeDef hspi2;	
uint8_t cmd=0xD0;
uint8_t readValue[2]={0};
void k_TouchUpdate(void)
{
HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_RESET);
HAL_SPI_Transmit(&hspi2,&cmd,1,0xffff);
HAL_SPI_Receive(&hspi2,readValue,2,0xffff);
HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_SET);

}




















