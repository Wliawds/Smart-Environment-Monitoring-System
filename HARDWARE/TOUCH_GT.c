#include "TOUCH_GT.h"
#include "TOUCH_CON.h"
#include "MY_IIC_T.h"

#include "MYDELAY.h" 
#include "string.h" 
#include "lcd.h" 
#include <stdio.h>




extern uint16_t line_x,line_y;	

void Touch_Address_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	GT_RST_1;
	GT_INT_1;
	/*配置触摸芯片地址为0x28/0x29*/
	GT_RST_0;
	GT_INT_0;
	T_Delay_ms(10);
	GT_INT_1;
	T_Delay_ms(100);
	GT_RST_1;
}

void Touch_Exit_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStruct;	
	__HAL_RCC_GPIOI_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
}


//GT9147配置参数表
//第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部
//flash原有版本号,才会更新配置.
const uint8_t GT9147_CFG_TBL[]=
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
};  
const uint8_t GT911_CFG_TBL[]=
{ 
	0x61,0xe0,0x01,0x20,0x03,0x05,0xf5,0x00,0x01,0x08,
	0x1e,0x05,0x3c,0x3c,0x03,0x05,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x1a,0x1c,0x1e,0x14,0x87,0x29,0x0a,
	0x2a,0x28,0xeb,0x04,0x00,0x00,0x01,0x61,0x03,0x2c,
	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x14,0x3c,0x94,0xc5,0x02,0x08,0x00,0x00,0x04,
	0xb7,0x16,0x00,0x9f,0x1b,0x00,0x8b,0x22,0x00,0x7b,
	0x2b,0x00,0x70,0x36,0x00,0x70,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,
	0x12,0x14,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x1d,
	0x1e,0x1f,0x20,0x21,0x22,0x24,0x26,0x28,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00

}; 



//向GT9147写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	T_IIC_Start();	
 	T_IIC_SendByte(GT_CMD_WR);   	//发送写命令 	 
	T_IIC_RecAck();
	T_IIC_SendByte(reg>>8);   	//发送高8位地址
	T_IIC_RecAck(); 	 										  		   
	T_IIC_SendByte(reg&0XFF);   	//发送低8位地址
	T_IIC_RecAck();  
	for(i=0;i<len;i++)
	{	   
    	T_IIC_SendByte(buf[i]);  	//发数据
		ret=T_IIC_RecAck();
		if(ret)break;  
	}
    T_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}
//从GT9147读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i; 
 	T_IIC_Start();	
 	T_IIC_SendByte(GT_CMD_WR);   //发送写命令 	 
	T_IIC_RecAck();
 	T_IIC_SendByte(reg>>8);   	//发送高8位地址
	T_IIC_RecAck(); 	 										  		   
 	T_IIC_SendByte(reg&0XFF);   	//发送低8位地址
	T_IIC_RecAck(); 
	
 	T_IIC_Start();  	 	   
	T_IIC_SendByte(GT_CMD_RD);   //发送读命令		   
	T_IIC_RecAck();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=T_IIC_ReadByte(i==(len-1)?0:1); //发数据	  
	} 
    T_IIC_Stop();//产生一个停止条件    
} 
//发送GT9147配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
uint8_t GT9147_Send_Cfg(uint8_t mode)
{
	uint8_t buf[2];
	uint8_t i=0;
	buf[0]=0;
	buf[1]=mode;	//是否写入到GT9147 FLASH?  即是否掉电保存
	for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i];//计算校验和
    buf[0]=(~buf[0])+1;
	GT9147_WR_Reg(GT_CFGS_REG,(uint8_t*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//发送寄存器配置
	GT9147_WR_Reg(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
	return 0;
} 
//发送GT911配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
uint8_t GT911_Send_Cfg(uint8_t mode)
{
	uint8_t buf[2];
	uint8_t i=0;
	buf[0]=0;
	buf[1]=mode;	//是否写入到GT9147 FLASH?  即是否掉电保存
	for(i=0;i<sizeof(GT911_CFG_TBL);i++)
		buf[0]+=GT911_CFG_TBL[i];//计算校验和
   
	buf[0]=(~buf[0])+1;
	GT9147_WR_Reg(GT_CFGS_REG,(uint8_t*)GT911_CFG_TBL,sizeof(GT911_CFG_TBL));//发送寄存器配置
	GT9147_WR_Reg(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
	return 0;
} 

//初始化GT9147触摸屏
//返回值:0,初始化成功;1,初始化失败 
uint8_t GT9147_Init(void)
{
	uint8_t temp[200]; 
	Touch_Address_Init();
	T_IIC_GpioInit();      	//初始化电容屏的I2C总线  
	GT9147_RD_Reg(GT_PID_REG,temp,4);//读取产品ID
	temp[4]=0;
	printf("CTP ID:%s\r\n",temp);	//打印ID

	if(strcmp((char*)temp,"9147")==0)//ID==9147
	{
		temp[0]=0X02;			
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//软复位GT9147
		GT9147_RD_Reg(GT_CFGS_REG,temp,1);//读取GT_CFGS_REG寄存器
		if(temp[0]<0X60)//默认版本比较低,需要更新flash配置
		{
			printf("Default Ver:%d\r\n",temp[0]);
			GT9147_Send_Cfg(1);//更新并保存配置
		}
		T_Delay_ms(10);
		temp[0]=0X00;	 
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//结束复位   
		return 0;
	} 
		if(strcmp((char*)temp,"911")==0)//ID==9147
	{
		temp[0]=0X02;			
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//软复位GT9147
		GT9147_RD_Reg(GT_CFGS_REG,temp,1);//读取GT_CFGS_REG寄存器
		if(temp[0]<=0X60)//默认版本比较低,需要更新flash配置
		{
			printf("Default Ver:%02x\r\n",temp[0]);
			GT911_Send_Cfg(1);//更新并保存配置
		}
		T_Delay_ms(10);
		temp[0]=0X00;	 
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//结束复位   
		return 0;
	} 
	return 1;
}
const uint16_t GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};
//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
uint8_t GT911_Scan(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint8_t temp;
	uint8_t tempsta;
 	static uint8_t t=0;//控制查询间隔,从而降低CPU占用率   
	t++;
	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
	{
		GT9147_RD_Reg(GT_GSTID_REG,&mode,1);	//读取触摸点的状态  
 		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;
			GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//清标志 		
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//将点的个数转换为1的位数,匹配tp_dev.sta定义 
			tempsta=tp_dev.sta;			//保存当前的tp_dev.sta值
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[4]=tp_dev.x[0];	//保存触点0的数据
			tp_dev.y[4]=tp_dev.y[0];
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值
					if(tp_dev.touchtype&0X01)//横屏
					{
						tp_dev.y[i]=((uint16_t)buf[1]<<8)+buf[0];
						tp_dev.x[i]=800-(((uint16_t)buf[3]<<8)+buf[2]);
					}else
					{
						tp_dev.x[i]=((uint16_t)buf[1]<<8)+buf[0];
						tp_dev.y[i]=((uint16_t)buf[3]<<8)+buf[2];
					}  
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]>lcddev.width||tp_dev.y[0]>lcddev.height)//非法数据(坐标超出了)
			{ 
				if((mode&0XF)>1)		//有其他点有数据,则复第二个触点的数据到第一个触点.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//触发一次,则会最少连续监测10次,从而提高命中率
				}else					//非法数据,则忽略此次数据(还原原来的)  
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					mode=0X80;		
					tp_dev.sta=tempsta;	//恢复tp_dev.sta
				}
			}else t=0;					//触发一次,则会最少连续监测10次,从而提高命中率
		}
	}
	if((mode&0X8F)==0X80)//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
			line_x=tp_dev.x[0];
			line_y=tp_dev.y[0];
		}else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	if(t>240)t=10;//重新从10开始计数
	return res;
}
 

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
	
}

























