#include "k_bsp.h"
#include "GUI.h"
#include "k_calibration.h"

extern SPI_HandleTypeDef hspi2;	
static uint16_t ts_x_boundary, ts_y_boundary; 
static uint8_t  ts_orientation;


uint8_t BSP_TS_Init(uint16_t xSize, uint16_t ySize)
{
  uint8_t ret = TS_ERROR;
  
    /* Initialize x and y positions boundaries */
    ts_x_boundary  = xSize;
    ts_y_boundary  = ySize;
    ts_orientation = TS_SWAP_XY;
    ret = TS_OK;
  
  
  return ret;
}







/**
  * @brief  Returns status and positions of the touch screen.
  * @param  TS_State: Pointer to touch screen current state structure
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_TS_GetState(TS_StateTypeDef *TS_State)
{
	static uint32_t _x = 0, _y = 0;
	uint16_t xDiff, yDiff , x , y;
	uint16_t swap;

	uint8_t  readValue[2]={0};
	uint8_t Xcmd=0xD0,Ycmd=0x90,Z1cmd=0xB0;
	
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,&Xcmd,1,0xffff);
	HAL_SPI_Receive(&hspi2,readValue,2,0xffff);
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_SET);	
		
	x=readValue[0];
	x<<=8;
	x|=readValue[1];
	x>>=3;
		
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,&Ycmd,1,0xffff);
	HAL_SPI_Receive(&hspi2,readValue,2,0xffff);
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_SET);

		
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,&Z1cmd,1,0xffff);
	HAL_SPI_Receive(&hspi2,readValue,2,0xffff);
	HAL_GPIO_WritePin(TCS_GPIO_Port,TCS_Pin,GPIO_PIN_SET);
		
	TS_State->z=readValue[0];
	TS_State->z<<=8;
	TS_State->z|=readValue[1];
	TS_State->z>>=3;	

	if(TS_State->z>500&&TS_State->z<2000)
		TS_State->TouchDetected = 1;
	else
		TS_State->TouchDetected = 0;
	
		
 
  
	if(TS_State->TouchDetected)
	{

			
		y=readValue[0];
		y<<=8;
		y|=readValue[1];
		y>>=3;


		if(ts_orientation & TS_SWAP_X)
		{
		  x = 4096 - x;  
		}

		if(ts_orientation & TS_SWAP_Y)
		{
		  y = 4096 - y;
		}

		if(ts_orientation & TS_SWAP_XY)
		{
		  swap = y; 
		  y = x;      
		  x = swap;      
		}

		xDiff = x > _x? (x - _x): (_x - x);
		yDiff = y > _y? (y - _y): (_y - y); 

		if (xDiff + yDiff > 5)
		{
		  _x = x;
		  _y = y; 
		}

		TS_State->x = (ts_x_boundary * _x) >> 12;
		TS_State->y = (ts_y_boundary * _y) >> 12; 
	}  

	return TS_OK;
}







void k_BspInit(void)
{
  /* Initialize the Touch screen */
  BSP_TS_Init(320, 480);
  


}



void k_TouchUpdate(void)
{
  GUI_PID_STATE TS_State;
  static TS_StateTypeDef prev_state;
  __IO TS_StateTypeDef  ts;
  uint16_t xDiff, yDiff;  
  
  BSP_TS_GetState((TS_StateTypeDef *)&ts);
  
  TS_State.Pressed = ts.TouchDetected;

  xDiff = (prev_state.x > ts.x) ? (prev_state.x - ts.x) : (ts.x - prev_state.x);
  yDiff = (prev_state.y > ts.y) ? (prev_state.y - ts.y) : (ts.y - prev_state.y);
  
  if((prev_state.TouchDetected != ts.TouchDetected )||
     (xDiff > 3 )||
       (yDiff > 3))
  {
    prev_state.TouchDetected = ts.TouchDetected;
    
    if((ts.x != 0) &&  (ts.y != 0)) 
    {
      prev_state.x = ts.x;
      prev_state.y = ts.y;
    }
      
    if(k_CalibrationIsDone())
    {
      TS_State.Layer = 0;
      TS_State.x = k_CalibrationGetX (prev_state.x);
      TS_State.y = k_CalibrationGetY (prev_state.y);
    }
    else
    {
      TS_State.Layer = 0;
      TS_State.x = prev_state.x;
      TS_State.y = prev_state.y;
    }
    
    GUI_TOUCH_StoreStateEx(&TS_State);
  }
}






