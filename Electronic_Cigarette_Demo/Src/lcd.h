#ifndef _LCD_H_
#define _LCD_H_

#include "stm32f0xx.h"







void SET_SDA_OUT(void);
void SET_SDA_IN(void);
void IC_IIC_Start(void);
void IC_IIC_Stop(void);
uint8_t Write_IIC_Data(uint8_t Data);
void Write_Data(uint8_t date);
void Write_Command(uint8_t command);
void Write_Command_V2(unsigned char  dat1,unsigned char  dat2);
void Init_LCD(void);
void LCD_Clear(void);
void LCD_Set_Pos(unsigned char x, unsigned char y);
void Disp_Batter_Bmp2(unsigned char x,unsigned char ch);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char size,unsigned char chr);
void LCD_P16x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void Disp_Num(unsigned char x,unsigned char y,unsigned char size, unsigned int num);
void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void Change_direction(void);
void Disp_Batter_Bmp(unsigned char x, unsigned char ch);


#endif


