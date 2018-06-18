/*
*
* �ļ���: i2c_gpio.c
* ���ݼ���: ��gpioģ��i2c����, ������STM32ϵ��CPU
*
* �ļ���ʷ:
* �汾�� ����       ����    ˵��
* 1.0    2009-11-15 armfly  �������ļ�
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
	��������i2c_Delay
	��  ��: ��
	��  ��: ��
	����˵����I2C����λ�ӳ٣����400KHz
*/
static void i2c_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 300; i++);
}

/*******************************************************************************
	��������i2c_Start
	��  ��: ��
	��  ��: ��
	����˵��������I2C������ʼ�ź�
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
	��������i2c_Stop
	��  ��: ��
	��  ��: ��
	����˵��������I2C����ֹͣ�ź�
*/
void i2c_Stop(void)
{
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*******************************************************************************
	��������i2c_Stop
	��  ��: _ucByte �����͵�����
	��  ��: ��
	����˵������I2C���߷���8bit����
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
	��������i2c_Stop
	��  ��: ��
	��  ��: ���ض���������
	����˵������I2C���߶�ȡ8bit����
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
	��������i2c_Ack
	��  ��: ��
	��  ��: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
	
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
	��������i2c_Configuration
	��  ��: ��
	��  ��: ��
	����˵��������I2C���ߣ�����ģ��IO�ķ�ʽʵ��
*/
void i2c_Configuration(void)
{

	/* ��©��� */

}

/*******************************************************************************
	��������i2c_Configuration
	��  ��: _Address���豸��I2C���ߵ�ַ
	��  ��: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
	����˵����	���I2C�����豸������ָ����ַ���ݣ�Ȼ���ȡ�豸Ӧ��
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;


	/* ������ʼλ */
	i2c_Start();

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | I2C_WR);

	/* ���ACK */
	ucAck = i2c_wait_ack();

	/* ����ֹͣλ */
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









