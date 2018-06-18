#include "User.h"
#include "lcd.h"
#include "MCP3421.h"
#include "PowerCtr.h"
#include "stdio.h"
#include "Keyboard.h"


extern TIM_HandleTypeDef htim1;


void CheckCurrentResistorsProc(void);



int UserSystemStartPoc(uint32_t msg)
{
	
		
    SYSTEM_POWER_ON();	

	LCD_POWER_ON();
	HAL_Delay(5);
	Init_LCD();
	HAL_Delay(10);
	LCD_Clear();
 	LCD_P8x16Str(0,0,"==SGMICRO==");
	HAL_Delay(1000);
	LCD_Clear();
	
	Write_MCP3421_Data(0X10);
		
	PWMDR_ON();
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);	  
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	SetPWM_Freq(PWM_CTR_FREQUENCY);
	SetBuckBoostPWM(BUCK_MODE,BUCK_OFF);
	SetBuckBoostPWM(BOOST_MODE,BOOST_OFF);

 	SetTargePower(0*POWER_SCALE);
	SetTargeTemperature(200);


  return -1;
}



void CheckCurrentResistorsProc(void)
{

	float OutputVoltage=0;	
	float VL_TYPE_Voltage=0;
	static uint8_t ProcState=1;
	static uint8_t ProcCount=0;
	float UserResistors=0;
	static uint8_t FirstState=1;
	
	if(GetPIDCtrState()==PID_CTR_STOP)
	{


		switch(ProcState)
			{
				
				case 1:
				PWMDR_OFF();
				SetBuckBoostPWM(BOOST_MODE,0);
				RL_TYPE_ON();
				ProcCount++;			 
				if(ProcCount>1)//ÑÓÊ±
				{
			  	 ProcState=2;
				 break;
				}			
				return;
				 



				case 2:
				ProcCount++;	
				if(ProcCount>20)
				{
				 ProcState=1;
				 ProcCount=0;
				}				
			    return;
			

		    }
			

			OutputVoltage=GetADC_AverageValuea(ADC_FB_Voltage_DET_INDEX)*3.3/ADC_FULL_SCALE*4;
			VL_TYPE_Voltage=GetADC_AverageValuea(VL_TYPE_DET_INDEX)*3.3/ADC_FULL_SCALE*4;
			UserResistors=OutputVoltage/((VL_TYPE_Voltage-OutputVoltage)/10)-0.002;
			RL_TYPE_OFF();	

			
			if(UserResistors<=100)
			{
				SetCurrentResistors(UserResistors);	

					if(FirstState)
						{
							FirstState=0;
							SetUserResistors(UserResistors);
						}
			}
			else
			{
				UserResistors=0;
				SetCurrentResistors(UserResistors);						
			}

					
    }


}

void UpdateDisPlayResistors(void)
{

	if(GetCurrentResistors()==0)
	{
	 LCD_P6x8Str(0,0,"R=NO  ");
	}
	else
	{
	 LCD_P6x8Str(0,0,"R");
	 Disp_Num(6*1,0,0,GetCurrentResistors()*100);
	}

	 if(GetPIDCtrMode()==POWER_CTR_MODE)
	 {
	 LCD_P6x8Str(6*6,0,"P=");
	 Disp_Num(6*8,0,0,GetTargePower());
	 }
	 
	 if(GetPIDCtrMode()==TEMPERATURE_CTR_MODE)
	 {
	  LCD_P6x8Str(6*6,0,"T=");
	  Disp_Num(6*8,0,0,GetTargeTemperature());
	 }
	 
	

}



void KeyEventCallBack(uint32_t KeyVal)
{

	switch(KeyVal)
		{

			case KEY_VAL_POWER_DOUBLE_CLICK:
				if(GetPIDCtrMode()==POWER_CTR_MODE)
					SetPIDCtrMode(TEMPERATURE_CTR_MODE);
				else
					SetPIDCtrMode(POWER_CTR_MODE);
			break;



			case KEY_VAL_POWER_PRESS:
			  PWMDR_ON();
			  RL_TYPE_OFF();	
			  SetPIDCtrState(PID_CTR_START);
			break;



			

			case KEY_VAL_POWER_SHORT_UP:
			case KEY_VAL_POWER_LONG_UP:
			  PWMDR_OFF();
			  SetPIDCtrState(PID_CTR_STOP);
			break;
			




			case KEY_VAL_UP_PRESS:
			  if(GetPIDCtrMode()==POWER_CTR_MODE)
			  SetTargePower(GetTargePower()+POWER_SCALE);
			  else
			  SetTargeTemperature(GetTargeTemperature()+1);
				
			break;





			case KEY_VAL_UP_SHORT_UP:
			  if(GetPIDCtrMode()==POWER_CTR_MODE)
			  SetTargePower(GetTargePower()+1);
			  else
			   SetTargeTemperature(GetTargeTemperature()+1);

			break;





			case KEY_VAL_DOWN_PRESS:
			  if(GetPIDCtrMode()==POWER_CTR_MODE)
			  SetTargePower(GetTargePower()-POWER_SCALE);
			  else
			  SetTargeTemperature(GetTargeTemperature()-1);
				
			break;




			case KEY_VAL_DOWN_SHORT_UP:
			  if(GetPIDCtrMode()==POWER_CTR_MODE)
			  SetTargePower(GetTargePower()-1);
			  else
			  SetTargeTemperature(GetTargeTemperature()-1);			  
			break;

		}



}

void KeyValConvert(UINT32 *pKeyVal)
{

	if(CheckPowerKey()!=0)
		*pKeyVal|=KEY_VAL_POWER;

	if(CheckKey1()!=0)
		*pKeyVal|=KEY_VAL_UP;

	if(CheckKey2()!=0)
		*pKeyVal|=KEY_VAL_DOWN;


}




