#ifndef __TOUCH_GT_H
#define __TOUCH_GT_H	
#include "stm32f4xx_hal.h"


//IO操作函数	 
#define GT_RST_1 	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET)
#define GT_RST_0 	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET)
#define GT_INT_1 	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_2, GPIO_PIN_SET)
#define GT_INT_0 	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_2, GPIO_PIN_RESET)
   	   	
 
//I2C读写命令	
#define GT_CMD_WR 		0X28     	//写命令
#define GT_CMD_RD 		0X29		//读命令
  
//GT9147 部分寄存器定义 
#define GT_CTRL_REG 	0X8040   	//GT9147控制寄存器
#define GT_CFGS_REG 	0X8047   	//GT9147配置起始地址寄存器
#define GT_CHECK_REG 	0X80FF   	//GT9147校验和寄存器
#define GT_PID_REG 		0X8140   	//GT9147产品ID寄存器

#define GT_GSTID_REG 	0X814E   	//GT9147当前检测到的触摸情况
#define GT_TP1_REG 		0X8150  	//第一个触摸点数据地址
#define GT_TP2_REG 		0X8158		//第二个触摸点数据地址
#define GT_TP3_REG 		0X8160		//第三个触摸点数据地址
#define GT_TP4_REG 		0X8168		//第四个触摸点数据地址
#define GT_TP5_REG 		0X8170		//第五个触摸点数据地址  
 
void Touch_Exit_Init(void);
uint8_t GT9147_Send_Cfg(uint8_t mode);
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len); 
uint8_t GT9147_Init(void);
uint8_t GT911_Scan(uint8_t mode); 
#endif













