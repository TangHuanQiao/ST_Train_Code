#include "MCP3421.h"


#define DeviceAddress 0XD0

extern I2C_HandleTypeDef hi2c1;

void 	Write_MCP3421_Data(uint8_t data)
{

	HAL_I2C_Master_Transmit(&hi2c1,DeviceAddress,&data,1,1000);

	
}


void Read_MCP3421_Data(uint8_t *pData, uint16_t Size)
{
	
	HAL_I2C_Master_Receive(&hi2c1,DeviceAddress,pData,Size,1000);
	
	
	
}


