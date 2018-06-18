/******************************************************************
**	  红龙开发板（V1.0）
**	  LCD配置文件
**
**	  论    坛：bbs.openmcu.com
**	  旺    宝：www.openmcu.com
**	  邮    箱：support@openmcu.com
**
**    版    本：V1.0
**	  作    者：openmcu666
**	  完成日期:	2015.6.17
********************************************************************/
#include "ili9481LCD.h"

static __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;
//uint16_t LCD_DeviceID = 0;


void LCD_FSMC_Config(void)
{

}

/**************************************************************************************************************
 * 函数名：LCD_WriteReg()
 * 输入  ：uint8_t LCD_Reg 寄存器地址, uint16_t LCD_RegValue 寄存器的值
 * 输出  ：void
 * 描述  ：写LCD寄存器函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
  /*写寄存器地址*/
  LCD_REG = LCD_Reg;
  /*写入寄存器值*/
  LCD_RAM = LCD_RegValue;
//	LCD_RAM = LCD_RegValue;
}
/**************************************************************************************************************
 * 函数名：LCD_ReadReg()
 * 输入  ：uint8_t LCD_Reg 需要读取的寄存器地址
 * 输出  ：uint16_t 寄存器的值
 * 描述  ：读取LCD寄存器的值
 * 调用  ：外部调用        
 *****************************************************************************************************************/
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
  /*写寄存器地址*/
  LCD_REG = LCD_Reg;
  /*读出寄存器值并返回*/
  return (LCD_RAM);
}

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(u16 regval)
{ 
	LCD_REG=regval;//写入要写的寄存器序号	 
}
/**************************************************************************************************************
 * 函数名：LCD_WriteRAM_Start()
 * 输入  ：void
 * 输出  ：void
 * 描述  ：开始写LCD的RAM
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_WriteRAM_Start(void)
{
  LCD_REG = 0x2C;
}

/**************************************************************************************************************
 * 函数名：LCD_WriteRAM()
 * 输入  ：uint16_t RGB_data  16bit颜色数据
 * 输出  ：void
 * 描述  ：向LCD的RAM写数据
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_WR_DATA(u16 data)
{						
//	u16 Data = data;
	LCD_RAM = data;	
}
/**************************************************************************************************************
 * 函数名：LCD_BGR2RGB()
 * 输入  ：u16 c  16bit颜色数据
 * 输出  ：红蓝交换后的颜色数据
 * 描述  ：将颜色进行红蓝交换
 * 调用  ：外部调用        
 *****************************************************************************************************************/
u16 LCD_BGR2RGB(u16 c)
{
  u16  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);
}	
/**************************************************************************************************************
 * 函数名：LCD_ReadRAM()
 * 输入  ：void
 * 输出  ：uint16_t  16bit颜色值
 * 描述  ：读取LCD的RAM数据
 * 调用  ：外部调用        
 *****************************************************************************************************************/
uint16_t LCD_ReadRAM(void)
{ 
  LCD_REG = 0x2E; /*选择RAM寄存器*/
  /*读取16bit RAM值*/
//	if(LCD_DeviceID==0x9320)
//	{
	//	return LCD_BGR2RGB(LCD_RAM);
//	}
//	else
//	{
		return LCD_RAM;
//	}
}

/**************************************************************************************************************
 * 函数名：LCD_DisplayOn()
 * 输入  ：void
 * 输出  ：void
 * 描述  ：LCD打开显示
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DisplayOn(void)
{
  /*打开显示 */
  LCD_WriteReg(0x07, 0x0173); /*设置LCD为262K色并打开显示*/
}

/**************************************************************************************************************
 * 函数名：LCD_DisplayOff()
 * 输入  ：void
 * 输出  ：void
 * 描述  ：LCD关闭显示
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DisplayOff(void)
{
  /*关闭显示*/
  LCD_WriteReg(0x07, 0x0); 
}


/**************************************************************************************************************
 * 函数名：LCD_Init()
 * 输入  ：void
 * 输出  ：void
 * 描述  ：LCD初始化函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_Init(void)
{ 
  LCD_FSMC_Config();  
   HAL_Delay(50);	
		   
	 LCD_WR_REG(0x0011);		  //退出睡眠模式，0x0010为进入  
   HAL_Delay(50);  
   LCD_WR_REG(0x0013);			//进入普通显示模式	 
	   
   LCD_WR_REG(0x00D0);			//设置电源参数，后续3个参数  
   LCD_WR_DATA(0x0007); 		//1.0xVci 07  
   LCD_WR_DATA(0x0042); 		//40  
   LCD_WR_DATA(0x0018); 		//1c   
	   
   LCD_WR_REG(0x00D1);			//Vcom Control 后续三个参数  
   LCD_WR_DATA(0x0000); 		//00  
   LCD_WR_DATA(0x0007); 		//18  
   LCD_WR_DATA(0x0010); 		//1d  
	   
   LCD_WR_REG(0x00D2);			//设置普通模式下的电源参数，后续两个参数  
   LCD_WR_DATA(0x0001); 		//01  
   LCD_WR_DATA(0x0011); 		//11  
	   
   LCD_WR_REG(0x00C0);			//Panel Driving setting  后续5个参数  
   LCD_WR_DATA(0x0014); 		//设置扫描模式 01  
   LCD_WR_DATA(0x003B); 		//设定行数480行 3B  
   LCD_WR_DATA(0x0000); 		//00  
   LCD_WR_DATA(0x0002); 		//5frames 02  
   LCD_WR_DATA(0x0011); 		//11  
	   
   LCD_WR_REG(0x00C1);			//Timing setting 后续3个参数  
   LCD_WR_DATA(0x0010); 		//10  
   LCD_WR_DATA(0x000B); 		//0B  
   LCD_WR_DATA(0x0088); 		//88  
	   
   LCD_WR_REG(0x00C5);			//Frame Rate and Inversion Control 后续一个参数  
   LCD_WR_DATA(0x0001); 		//100hz    
	   
   LCD_WR_REG(0x00C8);			//伽马校正,后续12个参数  
	 //00 30 36 45 04 16 37 75 77 54 0f 00	
   LCD_WR_DATA(0x0000);LCD_WR_DATA(0x0032);LCD_WR_DATA(0x0036);  
   LCD_WR_DATA(0x0045);LCD_WR_DATA(0x0006);LCD_WR_DATA(0x0016);  
   LCD_WR_DATA(0x0037);LCD_WR_DATA(0x0075);LCD_WR_DATA(0x0077);  
   LCD_WR_DATA(0x0054);LCD_WR_DATA(0x000f);LCD_WR_DATA(0x0000);   
	   
   LCD_WR_REG(0x0036);LCD_WR_DATA(0x000a);	
   LCD_WR_REG(0x003A);LCD_WR_DATA(0x0055);	
	   
   LCD_WR_REG(0x002A); //00 00 01 3F  
   LCD_WR_DATA(0x0000);  
   LCD_WR_DATA(0x0000);  
   LCD_WR_DATA(0x0001);  
   LCD_WR_DATA(0x003F);  
   
   LCD_WR_REG(0x002B); //00 00 01 DF  
   LCD_WR_DATA(0x0000);  
   LCD_WR_DATA(0x0000);  
   LCD_WR_DATA(0x0001);  
   LCD_WR_DATA(0x00df); 	
   
   HAL_Delay(50);  
   LCD_WR_REG(0x0029);	 //开显示  
   LCD_WR_REG(0x002C);	 //开始写数据	
   
 //  LCD_Clear(WHITE);   



	
}


/**************************************************************************************************************
 * 函数名：LCD_SetFont()
 * 输入  ：sFONT *fonts 要设置的字体
 * 输出  ：void
 * 描述  ：设置LCD的字体
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetFont(sFONT *fonts)
{
  Efont = fonts;
}

#if(0)

/**************************************************************************************************************
 * 函数名：LCD_Clear()
 * 输入  ：void
 * 输出  ：void
 * 描述  ：LCD清屏函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_Clear(u16 Color)
{
  uint32_t index = 0;
	
	LCD_SetCursor1(0,0,LCD_W_, LCD_H_);	//设置光标位置 

	LCD_WR_REG(0x002C);   
	for(index=0;index<153600;index++) LCD_WR_DATA(Color);
}


/**************************************************************************************************************
 * 函数名：LCD_SetCursor()
 * 输入  ：uint16_t Xpos, uint16_t Ypos 设定的屏幕的X值和Y值
 * 输出  ：void
 * 描述  ：LCD设置光标位置函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	LCD_WR_REG(0x002A);LCD_WR_DATA(Xpos>>8);
  LCD_WR_DATA(0x00FF&Xpos);                  //??X??
  LCD_WR_DATA(LCD_W>>8);LCD_WR_DATA(LCD_W);

  LCD_WR_REG(0x002B);LCD_WR_DATA(Ypos>>8);
  LCD_WR_DATA(0x00FF&Ypos);                  //??Y??
  LCD_WR_DATA(LCD_H>>8);LCD_WR_DATA(LCD_H);

}

void LCD_SetCursor1(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{

	LCD_WR_REG(0x002A);
	LCD_WR_DATA(x1>>8);
  LCD_WR_DATA(0x00FF&x1);                  //??X??
  LCD_WR_DATA(x2>>8);
	LCD_WR_DATA(x2);

  LCD_WR_REG(0x002B);
	LCD_WR_DATA(y1>>8);
  LCD_WR_DATA(0x00FF&y1);                  //??Y??
  LCD_WR_DATA(y2>>8);
	LCD_WR_DATA(y2);

}

/**************************************************************************************************************
 * 函数名：LCD_SetWindow()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width 区域的起点，宽和高
 * 输出  ：void
 * 描述  ：设置某个特定的填充区域
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint8_t Height)
{
  /*设置水平方向X开始坐标*/
  if(Xpos >= Height)
  {
    LCD_WriteReg(0x50, (Xpos - Height + 1));
  }
  else
  {
    LCD_WriteReg(0x50, 0);
  }
  /*设置水平方向X结束坐标*/
  LCD_WriteReg(0x51, Xpos);
  /*设置竖直方向Y开始坐标*/
  if(Ypos >= Width)
  {
    LCD_WriteReg(0x52, (Ypos - Width + 1));
  }  
  else
  {
    LCD_WriteReg(0x52, 0);
  }
  /*设置竖直方向Y结束坐标*/
  LCD_WriteReg(0x53, Ypos);
  LCD_SetCursor(Xpos, Ypos);
}
/**************************************************************************************************************
 * 函数名：LCD_SetColors()
 * 输入  ：_TextColor 前景色,_BackColor 背景色
 * 输出  ：void
 * 描述  ：设置LCD的前景色和背景色
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor)
{
  TextColor = _TextColor; 
  BackColor = _BackColor;
}
/**************************************************************************************************************
 * 函数名：LCD_GetColors()
 * 输入  ：*_TextColor 前景色的指针,*_BackColor 背景色的指针
 * 输出  ：void
 * 描述  ：获取LCD的前景色和背景色
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor)
{
  *_TextColor = TextColor; *_BackColor = BackColor;
}
/**************************************************************************************************************
 * 函数名：LCD_SetTextColor()
 * 输入  ：uint16_t Color 前景色
 * 输出  ：void
 * 描述  ：设置LCD的前景色
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetTextColor(__IO uint16_t Color)
{
  TextColor = Color;
}


/**************************************************************************************************************
 * 函数名：LCD_SetBackColor()
 * 输入  ：uint16_t Color 背景色
 * 输出  ：void
 * 描述  ：设置LCD的背景色
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetBackColor(__IO uint16_t Color)
{
  BackColor = Color;
}

/**************************************************************************************************************
 * 函数名：LCD_GetFont()
 * 输入  ：void
 * 输出  ：sFONT * 获取字体
 * 描述  ：设置LCD的字体
 * 调用  ：外部调用        
 *****************************************************************************************************************/
sFONT *LCD_GetFont(void)
{
  return Efont;
}
/**************************************************************************************************************
 * 函数名：LCD_DrawHLine()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Length 起点X和Y坐标及长度
 * 输出  ：void
 * 描述  ：画水平线
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
	uint32_t i = 0;
	//#if HORIZONTAL
  //LCD_SetCursor(Ypos, Xpos);
//	#else
	LCD_SetCursor(Xpos, Ypos);
	//#endif
    LCD_WriteRAM_Start(); /* Prepare to write GRAM */
    for(i = 0; i < Length; i++)
    {
      LCD_WR_DATA(TextColor);
    }
}
/**************************************************************************************************************
 * 函数名：LCD_DrawVLine()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Length 起点X和Y坐标及长度
 * 输出  ：void
 * 描述  ：画垂直线
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
	uint32_t i = 0;
  LCD_SetCursor(Xpos, Ypos);
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM_Start(); /* Prepare to write GRAM */
      LCD_WR_DATA(TextColor);
      Ypos++;
      LCD_SetCursor(Xpos, Ypos);
    }
}
/**************************************************************************************************************
 * 函数名：LCD_DrawRect()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint8_t Height 矩形左上角点的坐标及宽和高
 * 输出  ：void
 * 描述  ：画矩形函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint8_t Height)
{
  LCD_DrawHLine(Xpos, Ypos, Width);
  LCD_DrawHLine(Xpos, Ypos+ Height, Width); 
  LCD_DrawVLine(Xpos, Ypos, Height);
  LCD_DrawVLine(Xpos+ Width,Ypos, Height);
}
/**************************************************************************************************************
 * 函数名：LCD_DrawCircle()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Radius 圆心坐标点及半径
 * 输出  ：void
 * 描述  ：画圆函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;/* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  
  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
    LCD_SetCursor(Xpos + CurX, Ypos + CurY);
    LCD_WriteRAM_Start(); /* Prepare to write GRAM */
    LCD_WR_DATA(TextColor);
    LCD_SetCursor(Xpos + CurX, Ypos - CurY);
    LCD_WriteRAM_Start(); /* Prepare to write GRAM */
    LCD_WR_DATA(TextColor);
    LCD_SetCursor(Xpos - CurX, Ypos + CurY);
    LCD_WriteRAM_Start(); /* Prepare to write GRAM */
    LCD_WR_DATA(TextColor);
    LCD_SetCursor(Xpos - CurX, Ypos - CurY);
    LCD_WriteRAM_Start(); /* Prepare to write GRAM */
    LCD_WR_DATA(TextColor);
    LCD_SetCursor(Xpos + CurY, Ypos + CurX);
    LCD_WriteRAM_Start(); /* Prepare to write GRAM */
    LCD_WR_DATA(TextColor);
    LCD_SetCursor(Xpos + CurY, Ypos - CurX);
    LCD_WriteRAM_Start(); /* Prepare to write GRAM */
    LCD_WR_DATA(TextColor);
    LCD_SetCursor(Xpos - CurY, Ypos + CurX);
    LCD_WriteRAM_Start(); /* Prepare to write GRAM */
    LCD_WR_DATA(TextColor);
    LCD_SetCursor(Xpos - CurY, Ypos - CurX);
    LCD_WriteRAM_Start(); /* Prepare to write GRAM */
    LCD_WR_DATA(TextColor);
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}
/**************************************************************************************************************
 * 函数名：LCD_DrawMonoPict()
 * 输入  ：const uint32_t *Pict 画一个全屏单色的取膜数据
 * 输出  ：void
 * 描述  ：画一个单色的全屏图片函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawMonoPict(const uint32_t *Pict)
{
  uint32_t index = 0, i = 0;
  LCD_SetCursor(0, (LCD_PIXEL_WIDTH - 1)); 
  LCD_WriteRAM_Start(); /* Prepare to write GRAM */
  for(index = 0; index < 2400; index++)
  {
    for(i = 0; i < 32; i++)
    {
      if((Pict[index] & (1 << i)) == 0x00)
      {
        LCD_WR_DATA(BackColor);
      }
      else
      {
        LCD_WR_DATA(TextColor);
      }
    }
  }
}
/**************************************************************************************************************
 * 函数名：LCD_FillRect()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height 填充矩形左上角点、宽和高
 * 输出  ：void
 * 描述  ：画一个填充的矩形
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{ 
	uint16_t tempcolor;
  LCD_SetTextColor(TextColor);
  LCD_DrawHLine(Xpos, Ypos, Width);
  LCD_DrawHLine(Xpos, Ypos+ Height, Width);
  
  LCD_DrawVLine(Xpos, Ypos, Height);
  LCD_DrawVLine(Xpos+Width, Ypos, Height);

  Width --;
  Height-=2;
  Xpos++;
  tempcolor=TextColor;
  LCD_SetTextColor(BackColor);

  while(Height--)
  {
    LCD_DrawHLine(Xpos, ++Ypos, Width);    
  }

  LCD_SetTextColor(tempcolor);
}
/**************************************************************************************************************
 * 函数名：LCD_FillCircle()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Radius 填充圆的圆心和半径
 * 输出  ：void
 * 描述  ：画一个填充圆
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;    /* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  uint16_t tempcolor;
  D = 3 - (Radius << 1);

  CurX = 0;
  CurY = Radius;
  tempcolor=TextColor;
  LCD_SetTextColor(BackColor);

  while (CurX <= CurY)
  {
    if(CurY > 0) 
    {
      LCD_DrawHLine(Xpos - CurY, Ypos - CurX, 2*CurY);
      LCD_DrawHLine(Xpos - CurY, Ypos + CurX, 2*CurY);
    }

    if(CurX > 0) 
    {
      LCD_DrawHLine(Xpos - CurX, Ypos -CurY, 2*CurX);
      LCD_DrawHLine(Xpos - CurX, Ypos + CurY, 2*CurX);
    }
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }

  LCD_SetTextColor(tempcolor);
  LCD_DrawCircle(Xpos, Ypos, Radius);
}
/**************************************************************************************************************
 * 函数名：LCD_DrawUniLine()
 * 输入  ：uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 起始点坐标和终点坐标
 * 输出  ：void
 * 描述  ：画任意方向的直线
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    DrawPixel(x, y);             /* Draw the current pixel */
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}
/**************************************************************************************************************
 * 函数名：DrawPixel()
 * 输入  ：int16_t x, int16_t y  点的坐标
 * 输出  ：void
 * 描述  ：画一个象素点
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void DrawPixel(uint16_t x, uint16_t y)
{ 
	LCD_SetCursor(x,y);
//	LCD_SetCursor1(x,y,x,y);//?????? 
	LCD_WR_REG(0x002C);
  LCD_WR_DATA(TextColor);
}

/**************************************************************************************************************
 * 函数名：DrawPixel()
 * 输入  ：int16_t x, int16_t y  点的坐标
 * 输出  ：void
 * 描述  ：画一个象素点
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void DrawPixel1(uint16_t x, uint16_t y)
{ 
	//LCD_SetCursor(x,y);
	LCD_SetCursor1(x,y,x,y);//?????? 
	LCD_WR_REG(0x002C);
  LCD_WR_DATA(TextColor);
}
/**************************************************************************************************************
 * 函数名：GetPixel()
 * 输入  ：int16_t x, int16_t y  点的坐标
 * 输出  ：uint16_t 读到的像素点的颜色
 * 描述  ：获取一个象素点
 * 调用  ：外部调用        
 *****************************************************************************************************************/
uint16_t GetPixel(uint16_t x,uint16_t y)
{
   LCD_SetCursor(x,y);
   return LCD_ReadRAM();	
}
/**************************************************************************************************************
 * 函数名：LCD_DrawChar()
 * 输入  ：const uint16_t *c   字符编码
 * 输出  ：void
 * 描述  ：LCD画一个字符
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, i = 0;
  uint16_t  Yaddress = 0, Xaddress = 0;
  Yaddress = Ypos;
  Xaddress = Xpos;
  LCD_SetCursor(Xaddress, Yaddress);
  	
  for(index = 0; index < Efont->Height; index++)
  {
    LCD_WriteRAM_Start();
    for(i = 0; i <Efont->Width; i++)
    {
  
      if((((c[index] & ((0x80 << ((Efont->Width / 12 ) * 8 ) ) >> i)) == 0x00) &&(Efont->Width <= 12))||
        (((c[index] & (0x1 << i)) == 0x00)&&(Efont->Width > 12 )))

      {
        LCD_WR_DATA(BackColor);
      }
      else
      {
        LCD_WR_DATA(TextColor);
      } 
		//	delay_ms(4);
    }
    LCD_SetCursor(Xaddress, Yaddress++);
  }
}



/**************************************************************************************************************
 * 函数名：LCD_DisplayChar()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint8_t Ascii 显示的位置和字符
 * 输出  ：void
 * 描述  ：LCD显示一个字符
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
	Ascii -= 32;
	LCD_DrawChar(Xpos, Ypos, &Efont->table[Ascii *Efont->Height]);

}



/**************************************************************************************************************
 * 函数名：LCD_DrawString()
 * 输入  ：u16 xpos, u16 ypos, u8 *ptr 显示的位置和字符串
 * 输出  ：void
 * 描述  ：LCD显示一串字符
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DisplayString(u16 Xpos, u16 Ypos, u16 TextColor, u16 TextBackgroundColor, sFONT *font, u8 *ptr)
{
  u16 RefYpos = Xpos;
	u16 i,j;
	LCD_SetTextColor(TextColor);
	LCD_SetBackColor(TextBackgroundColor);
	LCD_SetFont(font);
	while(*ptr!=0)
  {	
		if(Xpos>LCD_W-16)
		{	
			RefYpos = 0;
			Ypos += 24;
		}
		if(Ypos>LCD_H-24)
		{
			RefYpos = 0;
		}
		switch(*ptr)
		{
			case '\n': // 换行
				Ypos += Efont->Height;
				RefYpos = 0;
				break;
			case '\b': // <-BackSpace 回删
				RefYpos -= Efont->Width;
				for(i = RefYpos;i<RefYpos+Efont->Width;i++)
				{
				  	for(j=0;j<Efont->Height;j++){
						LCD_SetCursor(i,Ypos+j);
						LCD_WR_REG(0x002C);
						LCD_WR_DATA(BackColor);
					}
				}	
			break;
			case '\t': // Tab|<- ->|
				Xpos += Efont->Width * 2;
			break;
			default:			
				LCD_DisplayChar(RefYpos,Ypos,*ptr);	
				RefYpos += Efont->Width;
			break;
		}
    ptr++;
  }
}

//m^n函数
u32 mypow(u8 m,u8 n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
/**************************************************************************************************************
 * 函数名：LCD_DrawNum()
 * 输入  ：u16 x, u16 y, s32 num, u8 len   显示的位置，数字及长度
 * 输出  ：void
 * 描述  ：LCD显示一个数字
 * 调用  ：外部调用        
 *****************************************************************************************************************/	 
void LCD_DrawNum(u16 x, u16 y, s32 num, u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	t=0;
	if(num<0)
	{
		LCD_DisplayChar(x+(Efont->Width)*t,y,'-');
		num=-num;
		t++;
	}						   
	for(;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_DisplayChar(x+(Efont->Width)*t,y,' ');
				continue;
			}else enshow=1; 		 	 
		}
	 	LCD_DisplayChar(x+(Efont->Width)*t,y,temp+'0'); 
	}
} 
/**************************************************************************************************************
 * 函数名：ClearAera()
 * 输入  ：uint16_t x,uint16_t y,uint16_t w,uint16_t h   清除的位置，长度和宽度
 * 输出  ：void
 * 描述  ：LCD清除某个区域
 * 调用  ：外部调用        
 *****************************************************************************************************************/	
void ClearAera(uint16_t x,uint16_t y,uint16_t w,uint16_t h)
{
	uint16_t tempcolor;
  tempcolor=TextColor;
  LCD_SetTextColor(BackColor);
  while(h--)
  {
    LCD_DrawHLine(x, y++, w);    
  }
  LCD_SetTextColor(tempcolor);
}
/**************************************************************************************************************
 * 函数名：LCD_DrawImage()
 * 输入  ：u8 *p   图片取膜数据
 * 输出  ：void
 * 描述  ：LCD画一张全屏图片
 * 调用  ：外部调用        
 *****************************************************************************************************************/	
void LCD_DrawImage(const u8 *p)
{ 
	uint16_t i,j,temp;
	for(i=0;i<320;i++)
		{
			for(j=0;j<480;j++)
				{
					temp=(uint16_t)(*(p+1)<<8|*(p));
					//temp<<=8;
					TextColor=temp;
					DrawPixel(j,i);
					p+=2;
				}
		}
}

#endif

