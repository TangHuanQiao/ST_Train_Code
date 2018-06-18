#include "PowerCtr.h"
#include "MCP3421.h"
#include "math.h"
#include "SysTickTaskManager.h"



#define LSB_Val ((double)(2*2.048/4096))
	
extern TIM_HandleTypeDef htim1;
extern uint16_t ADC_ValueTab[ADCCN_MAX*ADCSAMPLE_NUM];

const float Nifethal52_Ct_Table[]= {1.00, 1.33, 1.53, 1.73, 1.93, 2.13, 2.32, 2.49, 2.64, 2.77};
const uint16_t Nifethal52_TEMP_Table[]={20, 100, 150, 200,  250,  300,  350,  400 , 450,  500 };
	
static uint16_t TargePower=0;
static uint16_t TargeTemperature=0;

static float CurrentResistors=0;
static float UserResistors=0;

static uint8_t PIDCtrState=0;
static uint8_t PIDCtrMode=POWER_CTR_MODE;



SPid TargePowerPID={	
				0, // Last position input
				0, // Integrator state
				FB_STATE_MAX,FB_STATE_MIN, // Maximum and minimum allowable integrator state
				
				0.0001, // integral gain
				0.008, // proportional gain
				0.00001 // derivative gain
          	};


SPid TargeTemperaturePID={	
				0, // Last position input
				0, // Integrator state
				FB_STATE_MAX,FB_STATE_MIN, // Maximum and minimum allowable integrator state
				
				0.0001, // integral gain
				0.005, // proportional gain
				0.00001 // derivative gain
          	};



void SetUserResistors(float Resistors)
{
	UserResistors=Resistors;
}
	
void SetCurrentResistors(float Resistors)
{
	CurrentResistors=Resistors;
}

float GetCurrentResistors(void)
{

  return CurrentResistors;
}

void SetTargePower(uint16_t Power)
{
		
	TargePower=Power;
}


uint16_t GetTargePower(void)
{
	return TargePower;
}


void SetTargeTemperature(uint16_t Temp)
{
	TargeTemperature=Temp;
}

	
uint16_t GetTargeTemperature(void)
{
	return TargeTemperature;
}


void SetPIDCtrMode(uint8_t Mode)
{
	PIDCtrMode=Mode;
}


uint8_t GetPIDCtrMode(void)
{
	return PIDCtrMode;
}


void SetPIDCtrState(uint8_t State)
{
	PIDCtrState=State;
}

uint8_t GetPIDCtrState(void)
{
	return PIDCtrState;
}

float GetMCP3421_Voltage(void)
{
	uint8_t RedDataBUF[3]={0};
	uint16_t TempData=0;

	
	Read_MCP3421_Data(RedDataBUF,sizeof(RedDataBUF));
	TempData=RedDataBUF[0]&0X0F;
	TempData<<=8;
	TempData|=RedDataBUF[1];

	return TempData*LSB_Val*3/2;

}


uint16_t GetADC_AverageValuea(uint8_t Index)
{
	uint16_t i;
	uint32_t sum=0;
	for(i=0;i<ADCSAMPLE_NUM;i++)
	{
		sum+=ADC_ValueTab[Index+ADCCN_MAX*i];		
	}
	
	sum=sum/ADCSAMPLE_NUM;
	
	return sum;
}


float GetCurrentTemperature(float ResistorsT,float Resistors20)
{

	float Ct_Factor=0;
	uint8_t i=0;
	float coefficient=0;
	float CurrentTemperature=0;
	

	Ct_Factor=ResistorsT/Resistors20;

	for(i=0;i<ARRAYSIZE(Nifethal52_Ct_Table)-1;i++)
		{

			if(Ct_Factor<Nifethal52_Ct_Table[i+1])
				break;
		}

	if(i<ARRAYSIZE(Nifethal52_Ct_Table))
		{

			coefficient=(Nifethal52_Ct_Table[i+1]-Nifethal52_Ct_Table[i])/(Nifethal52_Ct_Table[i]*(Nifethal52_TEMP_Table[i+1]-Nifethal52_TEMP_Table[i]));
			CurrentTemperature=(ResistorsT/(Resistors20*Nifethal52_Ct_Table[i])-1)/coefficient+Nifethal52_TEMP_Table[i];
		}



	return CurrentTemperature;

}

void PowerCtrTask(void)
{

    float CurrentPower=0;
	float fTargePower=0;
	
	float CurrentTemperature=0;
	float I_Current=0;
	
	float OutPutPwm=0;
	float OUT_Voltage=0;
	float MCP3421_Voltage=0;

	
	if(PIDCtrState!=PID_CTR_START) return;
	
	OUT_Voltage=GetADC_AverageValuea(ADC_FB_Voltage_DET_INDEX)*3.3/ADC_FULL_SCALE*4;
	MCP3421_Voltage=GetMCP3421_Voltage();
	I_Current=(MCP3421_Voltage/0.002/50);
	CurrentResistors=(OUT_Voltage-(MCP3421_Voltage/50))/I_Current;
	
	if(CurrentResistors<0)
		CurrentResistors=0;
	
 	switch(PIDCtrMode)
		{

			case POWER_CTR_MODE:
			fTargePower=(float)TargePower/POWER_SCALE;
			CurrentPower=CurrentResistors*I_Current*I_Current;
			OutPutPwm=OutPutPwm+UpdatePID(&TargePowerPID,fTargePower-CurrentPower,CurrentPower);
			break;

			case TEMPERATURE_CTR_MODE:
			CurrentTemperature=GetCurrentTemperature(CurrentResistors,UserResistors);			
			OutPutPwm=OutPutPwm+UpdatePID(&TargeTemperaturePID,TargeTemperature-CurrentTemperature,CurrentTemperature);	
			break;
		}

	




	

	if(OutPutPwm>FB_STATE_MAX)
	   OutPutPwm=FB_STATE_MAX;

	if(OutPutPwm<FB_STATE_MIN)
	   OutPutPwm=FB_STATE_MIN;

	if(OutPutPwm>1)
	{	
		SetBuckBoostPWM(BUCK_MODE,1);
		SetBuckBoostPWM(BOOST_MODE,1-(OutPutPwm-1));
	}else
	{

		SetBuckBoostPWM(BOOST_MODE,BOOST_OFF);
		SetBuckBoostPWM(BUCK_MODE,OutPutPwm);
	}


}









double UpdatePID(SPid * pid, double error, double position)
{
	double pTerm,dTerm, iTerm;
	
	// calculate the proportional term
	pTerm = pid->pGain * error;
	// calculate the integral state with appropriate limiting
	pid->iState += error;
	
	if (pid->iState > pid->iMax) 
		pid->iState = pid->iMax;
	else if (pid->iState < pid->iMin) 
		pid->iState = pid->iMin;
	
	iTerm = pid->iGain * pid->iState; // calculate the integral term
	dTerm = pid->dGain * (position - pid->dState);
	pid->dState = position;
	
	
	return pTerm + iTerm - dTerm;
}


void SetPWM_Freq (uint32_t Freq)
{
	htim1.Instance->ARR=HAL_RCC_GetSysClockFreq()/Freq-1;
}


void SetBuckBoostPWM(uint8_t Mode ,float Dutycycle) 
{
	switch(Mode)
	{
		case BUCK_MODE:
			if(Dutycycle<1)
			htim1.Instance->CCR1=htim1.Instance->ARR*Dutycycle;
			else
			htim1.Instance->CCR1=0xffff;	
		break;

		case BOOST_MODE:
			if(Dutycycle<1)
			htim1.Instance->CCR2=htim1.Instance->ARR*Dutycycle;
			else
			htim1.Instance->CCR2=0xffff;
		break;
	}
}	

