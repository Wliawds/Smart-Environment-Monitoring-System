#include "SGP30.h"
#include "MYDELAY.h"
#include "MY_IIC_S.h"

//初始化空气质量
void SGP30_Init(void)
{
	S_IIC_Start();
	S_IIC_SendByte((SGP30_ADDR<<1) | 0);   //IIC写
	S_IIC_RecAck();
	S_IIC_SendByte(0x20);										//高位
	S_IIC_RecAck();	
	S_IIC_SendByte(0x03);										//低位
	S_IIC_RecAck();
	S_IIC_Stop();
	USR_Delay_ms(100);
}
void SGP20_Read_data(uint32_t *dat)
{
	volatile uint8_t  Byte_1th=0;
	volatile uint8_t  Byte_2th=0;
	volatile uint8_t  Byte_3th=0;
	volatile uint8_t  Byte_4th=0;
	volatile uint8_t  Byte_5th=0;	
	uint32_t RetuData = 0;
	
	S_IIC_Start();
	S_IIC_SendByte((SGP30_ADDR<<1) | 1);
	S_IIC_RecAck();
	Byte_1th = S_IIC_ReadByte(0);
	Byte_2th = S_IIC_ReadByte(0);
	Byte_3th = S_IIC_ReadByte(0);
	Byte_3th = Byte_3th;  //??????????
	Byte_4th = S_IIC_ReadByte(0);
	Byte_5th = S_IIC_ReadByte(1);
	S_IIC_Stop();
	RetuData = (Byte_1th<<8) + Byte_2th;
	dat[0] = RetuData;
	RetuData = (Byte_4th<<8) + Byte_5th;;
	dat[1] = RetuData;
}
//启动测量
void SGP30_START_MEASURE(void)
{
	S_IIC_Start();
	S_IIC_SendByte((SGP30_ADDR<<1) | 0);
	S_IIC_RecAck();
	S_IIC_SendByte(0x20);
	S_IIC_RecAck();
	S_IIC_SendByte(0x08);
	S_IIC_RecAck();
	S_IIC_Stop();
	USR_Delay_ms(100);
}

