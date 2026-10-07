#include "MYDELAY.h"

extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim13;


void USR_Delay_us(uint32_t nus)
{
	uint16_t  differ = 0xffff-nus-5;
	__HAL_TIM_SetCounter(&htim14,differ);
	HAL_TIM_Base_Start(&htim14);
	while( differ<0xffff-5)
	{
		differ = __HAL_TIM_GetCounter(&htim14);
	};
	HAL_TIM_Base_Stop(&htim14);
}

void USR_Delay_ms(uint16_t nms)
{
 while (nms--)
 {	 
	 USR_Delay_us(1000);
 };
}


void T_Delay_us(uint32_t nus)
{
	uint16_t  differ = 0xffff-nus-5;
	__HAL_TIM_SetCounter(&htim13,differ);
	HAL_TIM_Base_Start(&htim13);
	while( differ<0xffff-5)
	{
		differ = __HAL_TIM_GetCounter(&htim13);
	};
	HAL_TIM_Base_Stop(&htim13);
}

void T_Delay_ms(uint16_t nms)
{
 uint32_t i;
 for(i=0;i<nms;i++) T_Delay_us(1000);
}
















