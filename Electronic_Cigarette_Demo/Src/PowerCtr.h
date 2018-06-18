#include "stm32f0xx_hal.h"


typedef struct
{
	
	double dState; // Last position input
	double iState; // Integrator state
	double iMax, iMin; // Maximum and minimum allowable integrator state
	double iGain, // integral gain
	pGain, // proportional gain
	dGain; // derivative gain

} SPid;


#define PWM_CTR_FREQUENCY 150000
#define BUCK_MODE 1
#define BUCK_OFF 0

#define BOOST_MODE 2
#define BOOST_OFF 1

#define FB_STATE_MAX 1.5
#define FB_STATE_MIN 0


#define POWER_CTR_MODE 1
#define TEMPERATURE_CTR_MODE 2

#define PID_CTR_STOP 0
#define PID_CTR_START 1


#define POWER_SCALE 10


void PowerCtrTask(void);
uint16_t GetADC_AverageValuea(uint8_t Channel);
float GetMCP3421_Voltage(void);
void SetPWM_Freq (uint32_t Freq);
void SetBuckBoostPWM(uint8_t Mode ,float Dutycycle);
double UpdatePID(SPid * pid, double error, double position);

void SetTargePower(uint16_t Power);
uint16_t GetTargePower(void);

void SetTargeTemperature(uint16_t Temp);
uint16_t GetTargeTemperature(void);

void SetCurrentResistors(float Resistors);
float GetCurrentResistors(void);

void SetPIDCtrState(uint8_t State);
uint8_t GetPIDCtrState(void);

void SetPIDCtrMode(uint8_t Mode);
uint8_t GetPIDCtrMode(void);

void SetUserResistors(float Resistors);





