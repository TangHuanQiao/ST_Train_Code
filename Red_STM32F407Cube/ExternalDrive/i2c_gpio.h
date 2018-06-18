/****************************************************************************
* Copyright (C), 2009-2010, www.armfly.com
*
* 文件名: i2c_gpio.h
* 内容简述: gpio模拟i2c模块的头文件
*
* 文件历史:
* 版本号 日期       作者    说明
* 1.0    2009-11-16 armfly  创建该文件
*
*/

#ifndef _I2C_GPIO_H
#define _I2C_GPIO_H

#include <inttypes.h>

#define I2C_WR	0
#define I2C_RD	1

#define SLAVE_ADDR 0x94

typedef enum T_IIC_NUM{

	IIC_NUM_1=0,
	IIC_NUM_2,
	IIC_NUM_3,
	IIC_NUM_4,
	IIC_NUM_5,
	IIC_NUM_6,
	IIC_NUM_MAX
}IIC_NUM;







/* 产生I2C总线起始信号 */
void i2c_Start(void);

/* 产生I2C总线停止信号 */
void i2c_Stop(void);

/*
  向I2C总线发送8bit数据
*/
void i2c_SendByte(uint8_t _ucByte);

uint8_t i2c_ReadByte(void);


uint8_t i2c_wait_ack(void);
void i2c_NAck(void);
void i2c_Ack(void);

void i2c_Configuration(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
uint8_t i2c_ReadDataBuf(IIC_NUM port,uint8_t* DataBuf,uint8_t DataLen ,uint8_t RegAddr);
uint8_t i2c_SendDataBuf(IIC_NUM port,uint8_t* DataBuf,uint8_t DataLen ,uint8_t RegAddr);


#endif
