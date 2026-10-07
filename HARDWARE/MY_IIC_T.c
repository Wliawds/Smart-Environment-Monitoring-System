#include "MY_IIC_T.h"
#include "MYDELAY.h"	 

void T_IIC_GpioInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;	
	
	__HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = T_IIC_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(T_IIC_SCL_GPIO_Port, &GPIO_InitStruct);
  
	GPIO_InitStruct.Pin = T_IIC_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(T_IIC_SDA_GPIO_Port, &GPIO_InitStruct);
	
	
	HAL_GPIO_WritePin(T_IIC_SCL_GPIO_Port, T_IIC_SCL_Pin, GPIO_PIN_SET); 	
	HAL_GPIO_WritePin(T_IIC_SDA_GPIO_Port, T_IIC_SDA_Pin, GPIO_PIN_SET);
}

void T_IIC_SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
  GPIO_InitStruct.Pin = T_IIC_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(T_IIC_SDA_GPIO_Port, &GPIO_InitStruct);
}

void T_IIC_SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
  GPIO_InitStruct.Pin = T_IIC_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(T_IIC_SDA_GPIO_Port, &GPIO_InitStruct);
}


void T_IIC_Start(void)
{
	T_IIC_SDA_OUT();
	T_IIC_SDA_1;
	T_IIC_SCL_1;
	T_Delay_us(5);
	T_IIC_SDA_0;
	T_Delay_us(5);
	T_IIC_SCL_0;

}
void T_IIC_Stop(void)
{
	T_IIC_SDA_OUT();
	T_IIC_SCL_1;
	T_IIC_SDA_0;
	T_Delay_us(5);
	T_IIC_SDA_1;
	T_Delay_us(5);
	T_IIC_SCL_0;
}

void T_IIC_SendByte(uint8_t val)
{
	uint8_t i;
	T_IIC_SDA_OUT();
	T_IIC_SCL_0;
	for(i=0;i<8;i++)
	{
		if((val&0x80)>>7)
			T_IIC_SDA_1;
		else
			T_IIC_SDA_0;
		val<<=1;
		T_Delay_us(2);
		T_IIC_SCL_1;
		T_Delay_us(2);
		T_IIC_SCL_0;
		T_Delay_us(2);
	}
}
uint8_t T_IIC_RecAck(void)
{
	uint8_t CNT;
	T_IIC_SDA_IN();
	T_IIC_SDA_1;
	T_Delay_us(2);		
	T_IIC_SCL_1;
	T_Delay_us(2);	
	while(T_IIC_SDA_R)	
	{
		CNT++;
		if(CNT == 100)
		{
			T_IIC_Stop();
			return 1;
		}
	}
 	T_IIC_SCL_0;	
	return 0;
}
void T_IIC_SendAck(uint8_t val)
{
	T_IIC_SCL_0;
	T_IIC_SDA_OUT();
	T_Delay_us(2);	
	if(val==0)T_IIC_SDA_1;
	else			T_IIC_SDA_0;
	T_Delay_us(2);
	T_IIC_SCL_1;
	T_Delay_us(2);
	T_IIC_SCL_0;
}

uint8_t T_IIC_ReadByte(uint8_t ack)
{
	uint8_t i,REC_DAT;
	T_IIC_SDA_IN();
	for(i=0;i<8;i++)
	{
		T_IIC_SCL_1;
		T_Delay_us(5);	
		REC_DAT<<=1;
		if(T_IIC_SDA_R)REC_DAT++; 		
		T_IIC_SCL_0;
		T_Delay_us(5); 
	}
	T_IIC_SDA_OUT();	
	T_IIC_SendAck(ack);
	return REC_DAT;
}






























