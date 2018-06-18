/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32f0xx_hal.h"

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define BAT_DET_Pin GPIO_PIN_0
#define BAT_DET_GPIO_Port GPIOA
#define VL_TYPE_DET_Pin GPIO_PIN_1
#define VL_TYPE_DET_GPIO_Port GPIOA
#define NTC_TEMP_DET_Pin GPIO_PIN_4
#define NTC_TEMP_DET_GPIO_Port GPIOA
#define VRIN_Current_DET_Pin GPIO_PIN_5
#define VRIN_Current_DET_GPIO_Port GPIOA
#define ADC_FB_Voltage_DET_Pin GPIO_PIN_6
#define ADC_FB_Voltage_DET_GPIO_Port GPIOA
#define CH_FULL_DET_Pin GPIO_PIN_1
#define CH_FULL_DET_GPIO_Port GPIOB
#define USB_DET_Pin GPIO_PIN_2
#define USB_DET_GPIO_Port GPIOB
#define ON_PWMDR_Pin GPIO_PIN_11
#define ON_PWMDR_GPIO_Port GPIOB
#define LDO_EN_Pin GPIO_PIN_12
#define LDO_EN_GPIO_Port GPIOB
#define POWER_KEY_Pin GPIO_PIN_13
#define POWER_KEY_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_14
#define KEY1_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_15
#define KEY2_GPIO_Port GPIOB
#define RL_TYPE_SW_Pin GPIO_PIN_15
#define RL_TYPE_SW_GPIO_Port GPIOA
#define DC_EN_PWMDR_Pin GPIO_PIN_3
#define DC_EN_PWMDR_GPIO_Port GPIOB
#define RES_Pin GPIO_PIN_4
#define RES_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_5
#define LCD_EN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define LCD_POWER_ON()  HAL_GPIO_WritePin(LCD_EN_GPIO_Port,LCD_EN_Pin,GPIO_PIN_SET)
#define LCD_POWER_OFF() HAL_GPIO_WritePin(LCD_EN_GPIO_Port,LCD_EN_Pin,GPIO_PIN_RESET)
#define SYSTEM_POWER_ON() HAL_GPIO_WritePin(LDO_EN_GPIO_Port,LDO_EN_Pin,GPIO_PIN_SET)
#define SYSTEM_POWER_OFF() HAL_GPIO_WritePin(LDO_EN_GPIO_Port,LDO_EN_Pin,GPIO_PIN_RESET)
#define PWMDR_ON() {HAL_GPIO_WritePin(ON_PWMDR_GPIO_Port,ON_PWMDR_Pin,GPIO_PIN_SET);HAL_GPIO_WritePin(DC_EN_PWMDR_GPIO_Port,DC_EN_PWMDR_Pin,GPIO_PIN_SET);}
#define PWMDR_OFF() {HAL_GPIO_WritePin(ON_PWMDR_GPIO_Port,ON_PWMDR_Pin,GPIO_PIN_RESET);HAL_GPIO_WritePin(DC_EN_PWMDR_GPIO_Port,DC_EN_PWMDR_Pin,GPIO_PIN_RESET);}
	
#define RL_TYPE_ON() HAL_GPIO_WritePin(RL_TYPE_SW_GPIO_Port,RL_TYPE_SW_Pin,GPIO_PIN_SET)
#define RL_TYPE_OFF() HAL_GPIO_WritePin(RL_TYPE_SW_GPIO_Port,RL_TYPE_SW_Pin,GPIO_PIN_RESET)

#define CheckPowerKey() HAL_GPIO_ReadPin(POWER_KEY_GPIO_Port,POWER_KEY_Pin)
#define CheckKey1() HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)
#define CheckKey2() HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)


#define ADCSAMPLE_NUM 10

#define ADCCN_MAX 5
#define	BAT_DET_CHANNEL 0
#define VL_TYPE_DET_INDEX 1
#define NTC_TEMP_DET_INDEX 2
#define VRIN_Current_DET_INDEX 3
#define ADC_FB_Voltage_DET_INDEX 4


#define VREFINT_CAL (*(uint16_t *)0x1FFFF7BA)
#define ADC_FULL_SCALE 4096


/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
