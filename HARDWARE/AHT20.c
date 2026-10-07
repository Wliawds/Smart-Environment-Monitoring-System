#include "MY_IIC_S.h"
#include "AHT20.h"
#include "MYDELAY.h"
/*
 * @brief 发送命令触发AHT20内部进行数据测量
 * @param 无
 */

void AHT20_SendAC(void) 
{
	S_IIC_Start();
	S_IIC_SendByte((AHT20_ADDR<<1) | 0);//发送写命令
	S_IIC_RecAck();
	S_IIC_SendByte(0xac);
	S_IIC_RecAck();
	S_IIC_SendByte(0x33);//00110011
	S_IIC_RecAck();
	S_IIC_SendByte(0x00);
	S_IIC_RecAck();
	S_IIC_Stop();
}
/*
 * @brief 发送读命令，读取AHT20状态
 * @param 无
 * @return sensor_status:每位bit解析参考手册
 */
uint8_t AHT20_Read_Status(void)
{
	uint8_t Byte_first;	
	S_IIC_Start();
	S_IIC_SendByte((AHT20_ADDR<<1) | 1);//发送读命令
	S_IIC_RecAck();
	Byte_first = S_IIC_ReadByte(1);
	S_IIC_Stop();
	return Byte_first;
}

/*
 * @brief 读取温湿度函数
 * @param ct 读取后的数据存储地址
 */

void AHT20_Read_CTdata(int32_t *ct)
{
	volatile uint8_t  Byte_1th=0;
	volatile uint8_t  Byte_2th=0;
	volatile uint8_t  Byte_3th=0;
	volatile uint8_t  Byte_4th=0;
	volatile uint8_t  Byte_5th=0;
	volatile uint8_t  Byte_6th=0;
	uint32_t RetuData = 0;
	uint16_t cnt = 0;
	AHT20_SendAC();								//发送开始测量指令
	USR_Delay_ms(80);							//等待80ms（手册）	
	while(((AHT20_Read_Status()&0x80)==0x80))	//[bit7:1忙状态，0空闲状态]，忙则等待
	{
		USR_Delay_ms(2);												//2ms查询一次
		if(cnt++>=100){break;}									//100次后还在忙，跳出
	}
	S_IIC_Start();															//启动测量
	S_IIC_SendByte((AHT20_ADDR<<1) | 1);				//发地址+读
	S_IIC_RecAck();														//等待传感器应答
	Byte_1th = S_IIC_ReadByte(0);//						//从机状态数据
	Byte_2th = S_IIC_ReadByte(0);//						//湿度数据[19:12]位
	Byte_3th = S_IIC_ReadByte(0);//						//湿度数据[11: 4]位
	Byte_4th = S_IIC_ReadByte(0);//						//温度数据[ 3: 0],湿度数据[19:16]
	Byte_5th = S_IIC_ReadByte(0);//						//温度数据[15: 8]
	Byte_6th = S_IIC_ReadByte(1);//						//湿度数据[ 7: 0]
	S_IIC_Stop();																//停止总线数据传输

	RetuData = (RetuData|Byte_2th)<<8;				//...[24:16]
	RetuData = (RetuData|Byte_3th)<<8;				//...[16: 8]
	RetuData = (RetuData|Byte_4th);						//...[ 8: 0]
	RetuData =RetuData >>4;										//移除低4位，完成整合[19:0]
	ct[0] = RetuData;													//暂存
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;				//...[24:16]
	RetuData = (RetuData|Byte_5th)<<8;				//...[16: 8]
	RetuData = (RetuData|Byte_6th);						//...[ 8: 0]
	RetuData = RetuData&0xfffff;							//移除高4位，完成整合[19:0]
	ct[1] =RetuData; 													//暂存

}
/*
 * @brief AHT20初始化函数
 * @param 无
 */
void AHT20_RST_REG(void)
{
	S_IIC_Start();
	S_IIC_SendByte((AHT20_ADDR<<1) | 0);
	S_IIC_RecAck();
	S_IIC_SendByte(0xBE);
	S_IIC_RecAck();
	S_IIC_SendByte(0x08);
	S_IIC_RecAck();
	S_IIC_SendByte(0x00);
	S_IIC_RecAck();
	S_IIC_Stop();
	
  USR_Delay_ms(10);
	
	S_IIC_Start();
	S_IIC_SendByte((AHT20_ADDR<<1) | 0);
	S_IIC_RecAck();
	S_IIC_SendByte(0xBa);
	S_IIC_RecAck();
	S_IIC_Stop();
	

}







