#include "MY_IIC_S.h"
#include "MYDELAY.h"



void S_IIC_SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
  GPIO_InitStruct.Pin = IIC_SDA_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(IIC_SDA_PIN_GPIO_Port, &GPIO_InitStruct);
}

void S_IIC_SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
  GPIO_InitStruct.Pin = IIC_SDA_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(IIC_SDA_PIN_GPIO_Port, &GPIO_InitStruct);
}

void S_IIC_Start(void)
{
	S_IIC_SDA_OUT();
	S_IIC_SDA_1;
	S_IIC_SCL_1;
	USR_Delay_us(5);
	S_IIC_SDA_0;
	USR_Delay_us(5);
	S_IIC_SCL_0;

}

void S_IIC_Stop(void)
{
	S_IIC_SDA_OUT();
	S_IIC_SCL_1;
	S_IIC_SDA_0;
	USR_Delay_us(5);
	S_IIC_SDA_1;
	USR_Delay_us(5);
	S_IIC_SCL_0;
}

void S_IIC_SendByte(uint8_t val)
{
	uint8_t i;
	S_IIC_SDA_OUT();
	S_IIC_SCL_0;
	for(i=0;i<8;i++)
	{
		if((val&0x80)>>7)
			S_IIC_SDA_1;
		else
			S_IIC_SDA_0;
		val<<=1;
		USR_Delay_us(2);
		S_IIC_SCL_1;
		USR_Delay_us(2);
		S_IIC_SCL_0;
		USR_Delay_us(2);
	}
}
//0应答,1非应答
void S_IIC_SendAck(uint8_t val)
{
	S_IIC_SCL_0;
	S_IIC_SDA_OUT();
	USR_Delay_us(2);	
	if(val==1)S_IIC_SDA_1;
	else			S_IIC_SDA_0;
	USR_Delay_us(2);
	S_IIC_SCL_1;
	USR_Delay_us(2);
	S_IIC_SCL_0;
}
//0 应答，1无应答sh
uint8_t S_IIC_RecAck(void)
{
	uint8_t CNT;
	S_IIC_SDA_IN();
	S_IIC_SDA_1;
	USR_Delay_us(1);		
	S_IIC_SCL_1;
	USR_Delay_us(1);	
	while(S_IIC_SDA_R)	
	{
		CNT++;
		if(CNT == 100)
		{
			S_IIC_Stop();
			return 1;
		}
	}
 	S_IIC_SCL_0;	
	return 0;
}

uint8_t S_IIC_ReadByte(uint8_t ack)
{
	uint8_t i,REC_DAT;
	S_IIC_SDA_IN();
	for(i=0;i<8;i++)
	{
		S_IIC_SCL_1;
		USR_Delay_us(5);	
		REC_DAT<<=1;
		if(S_IIC_SDA_R)REC_DAT++; 		
		S_IIC_SCL_0;
		USR_Delay_us(5); 
	}
	S_IIC_SDA_OUT();	
	S_IIC_SendAck(ack);
	return REC_DAT;
}


















