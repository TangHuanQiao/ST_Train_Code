/*
*
* 文件名: i2c_gpio.c
* 内容简述: 用gpio模拟i2c总线, 适用于STM32系列CPU
*
* 文件历史:
* 版本号 日期       作者    说明
* 1.0    2009-11-15 armfly  创建该文件
*
*/

#include "stm32f4xx_hal.h"

#include "i2c_gpio.h"


GPIO_TypeDef *SCL_GPIO_PortX[]={ GPIOA};
const uint16_t SCL_PinX[]= {GPIO_PIN_4};


GPIO_TypeDef *SDA_GPIO_PortX[]={ GPIOA};
const uint16_t SDA_PinX[]= {GPIO_PIN_4};
									 

static IIC_NUM i2cPort=IIC_NUM_1;



#define I2C_SCL_1()  HAL_GPIO_WritePin(SCL_GPIO_PortX[i2cPort], SCL_PinX[i2cPort], GPIO_PIN_SET)		/* SCL = 1 */
#define I2C_SCL_0()  HAL_GPIO_WritePin(SCL_GPIO_PortX[i2cPort], SCL_PinX[i2cPort], GPIO_PIN_RESET)	/* SCL = 0 */

#define I2C_SDA_1()  HAL_GPIO_WritePin(SDA_GPIO_PortX[i2cPort], SDA_PinX[i2cPort], GPIO_PIN_SET)		/* SDA = 1 */
#define I2C_SDA_0()  HAL_GPIO_WritePin(SDA_GPIO_PortX[i2cPort], SDA_PinX[i2cPort], GPIO_PIN_RESET)	/* SDA = 0 */

#define I2C_SDA_READ()  HAL_GPIO_ReadPin(SDA_GPIO_PortX[i2cPort], SDA_PinX[i2cPort])



/*******************************************************************************
	函数名：i2c_Delay
	输  入: 无
	输  出: 无
	功能说明：I2C总线位延迟，最快400KHz
*/
static void i2c_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 300; i++);
}

/*******************************************************************************
	函数名：i2c_Start
	输  入: 无
	输  出: 无
	功能说明：产生I2C总线起始信号
*/
void i2c_Start(void)
{
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*******************************************************************************
	函数名：i2c_Stop
	输  入: 无
	输  出: 无
	功能说明：产生I2C总线停止信号
*/
void i2c_Stop(void)
{
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*******************************************************************************
	函数名：i2c_Stop
	输  入: _ucByte ：发送的数据
	输  出: 无
	功能说明：向I2C总线发送8bit数据
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();
		I2C_SCL_0();
		_ucByte <<= 1;
		i2c_Delay();
	}
}

/*******************************************************************************
	函数名：i2c_Stop
	输  入: 无
	输  出: 返回独到的数据
	功能说明：从I2C总线读取8bit数据
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*******************************************************************************
	函数名：i2c_Ack
	输  入: 无
	输  出: 返回0表示正确应答，1表示无器件响应
	
*/
uint8_t i2c_wait_ack(void)
{
	uint8_t re;

	I2C_SDA_1();
	i2c_Delay();
	I2C_SCL_1();
	i2c_Delay();
	if (I2C_SDA_READ())
	{
		re = 1;
	}
	else
	{
		re = 0;
	}

	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();

	return re;
}

/*******************************************************************************

*/
void i2c_Ack(void)
{
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();
}



void i2c_NAck(void)
{
	I2C_SDA_1();
	i2c_Delay();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();
}




/*******************************************************************************
	函数名：i2c_Configuration
	输  入: 无
	输  出: 无
	功能说明：配置I2C总线，采用模拟IO的方式实现
*/
void i2c_Configuration(void)
{

	/* 开漏输出 */

}

/*******************************************************************************
	函数名：i2c_Configuration
	输  入: _Address：设备的I2C总线地址
	输  出: 返回值 0 表示正确， 返回1表示未探测到
	功能说明：	检测I2C总线设备，发送指定地址数据，然后读取设备应答
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;


	/* 发送起始位 */
	i2c_Start();

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(_Address | I2C_WR);

	/* 检测ACK */
	ucAck = i2c_wait_ack();

	/* 发送停止位 */
	i2c_Stop();

	return ucAck;
}






uint8_t i2c_ReadDataBuf(IIC_NUM port,uint8_t* DataBuf,uint8_t DataLen ,uint8_t RegAddr)
{
	uint8_t i;
	
	i2cPort=port;

	i2c_Start();
	i2c_SendByte(SLAVE_ADDR);
	if(i2c_wait_ack()){i2c_Stop(); return 0;}
	
	i2c_SendByte(RegAddr);	
	if(i2c_wait_ack()){i2c_Stop(); return 0;}
	
	i2c_Start();				
	i2c_SendByte(SLAVE_ADDR+1);	
	if(i2c_wait_ack()){i2c_Stop(); return 0;}
	
	for(i=0; i<DataLen; i++)
	{
		DataBuf[i] = i2c_ReadByte();
		if(i == DataLen-1) 	
		i2c_NAck();			
		else 			
		i2c_Ack();
			
	}
	i2c_Stop();



	return DataLen;
	
}



uint8_t i2c_SendDataBuf(IIC_NUM port,uint8_t* DataBuf,uint8_t DataLen ,uint8_t RegAddr)
{
	uint8_t i;

	i2cPort=port;
	
	i2c_Start();
	i2c_SendByte(SLAVE_ADDR);
	if(i2c_wait_ack()) {i2c_Stop(); return 0;}
	
	i2c_SendByte(RegAddr);	
	if(i2c_wait_ack()) {i2c_Stop(); return 0;}
	
	for(i=0; i<DataLen; i++)
	{
		i2c_SendByte(DataBuf[i]);
		if(i2c_wait_ack()) {i2c_Stop(); return 0;}			
	}
	
	i2c_Stop();



	return DataLen;
	
}









