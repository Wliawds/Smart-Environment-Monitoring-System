/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FILTER.h"
#include "MYDELAY.h"
#include "PM25_BSP.h"
#include "CH4_BSP.h"
#include "MY_IIC_S.h"
#include "AHT20.h"
#include "SGP30.h"
#include "LCD.h"
#include "LCD_INTERFACE.h"
#include "TOUCH_CON.h"
#include "AT24CXX.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t 	rx_buffer[100];   //???????
volatile 	uint8_t rx_len = 0; //???????
uint32_t  CH4_Ro,CH4_Rs,PM25,CH4_Ppm;
uint32_t  adc_buffer[2];
uint8_t 	test_c = 0;
uint16_t 	line_x = 0xFFFF;
uint16_t 	line_y = 0xFFFF;	
uint8_t 	touch_flag = 0;
uint8_t 	Item2,Item1= 1;
uint8_t 	Page = 1;
uint8_t 	Page2_Item = 1;
uint8_t 	Page1_Item = 1;
set_sensordata EEPROM_ReadData;
set_sensordata EEPROM_WriteData;
RealTime_Data sensor;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch, 1, 0xffff);
	return ch;
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t SGP30_data[2];

int32_t CT_data[2];
int32_t  c1,t1;
volatile uint32_t co2Data,ch2oData;

int32_t Sensor_data[6] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t Alarm_ClFLG = 1;
uint16_t TM12_count = 0;
uint16_t TM11_count = 0;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint32_t last_exti_time = 0;  // 上次中断时间戳

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//触摸屏的中断
	if(GPIO_Pin==GPIO_PIN_2)
	{ 
		if(HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_2)==0)
		{
			touch_flag = 1;
		}
	}	
	
	if(GPIO_Pin==GPIO_PIN_9)
	{ 
		if(HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_9)==0)
		{
			if(HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_9)==0)
			{		

				// 2. 获取当前时间（必须放在前面）
				uint32_t now = HAL_GetTick();       //HAL_GetTick();可以获取毫秒级的时间戳
				
				// 3. 核心滤波：20ms时间窗口
				if ((now - last_exti_time) > 20)  // 20ms消抖，防止误触。
				{
						last_exti_time = now;      // 更新时间戳
						Alarm_ClFLG ++;
						if(Alarm_ClFLG > 1){Alarm_ClFLG = 0;}
				}
			}
		}
	}	

}

//extern:外扩的作用。TIM_HandleTypeDef:定时器的句柄类型（一个结构体）.
//作用获取定时器10 的权限.
extern TIM_HandleTypeDef htim10;      

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM12)
	{
		TM12_count ++;
		if(TM12_count > 50)
		{
			Led_SetLevel(GPIO_PIN_SET);
			HAL_TIM_PWM_Stop(&htim10, TIM_CHANNEL_1);
			if(TM12_count > 100)
			{
				Led_SetLevel(GPIO_PIN_RESET);
				HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
				TM12_count = 0;
			}
		}
	}
	if (htim->Instance == TIM11)
	{
		TM11_count ++;
		if(TM11_count > 200)
		{
			Led_SetLevel(GPIO_PIN_SET);
			if(TM11_count > 2000)
			{
				Led_SetLevel(GPIO_PIN_RESET);
				TM11_count = 0;
			}
		}
	}
	
	
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}
uint32_t lastCheckTime = 0;
const uint32_t CHECK_INTERVAL = 2000; // 2秒 = 2000毫秒
uint32_t currentTime;
uint8_t Test_results = 0;



static uint8_t Get_Page1_Item_From_Touch(uint16_t touch_x, uint16_t touch_y)
{
    // X坐标检查：所有选项的X坐标范围都是40-474
    if (touch_x <= 30 || touch_x >= 452) {
        return 0;
    }
    
    // Y坐标范围检查
    if (touch_y >  80 && touch_y < 134) return 1;  // 选项1
    if (touch_y > 140 && touch_y < 194) return 2;  // 选项2
    if (touch_y > 200 && touch_y < 254) return 3;  // 选项3
    if (touch_y > 260 && touch_y < 314) return 4;  // 选项4
    if (touch_y > 320 && touch_y < 374) return 5;  // 选项5
    if (touch_y > 380 && touch_y < 434) return 6;  // 选项5
    return 0;  // 没有匹配的选项
}
static uint8_t Get_Page2_Item_From_Touch(uint16_t touch_x, uint16_t touch_y)
{
    // X坐标检查：所有选项的X坐标范围都是40-474
    if (touch_x <= 40 || touch_x >= 474) {
        return 0;
    }
    
    // Y坐标范围检查
    if (touch_y > 500 && touch_y < 524) return 1;  // 选项1
    if (touch_y > 545 && touch_y < 569) return 2;  // 选项2
    if (touch_y > 590 && touch_y < 614) return 3;  // 选项3
    if (touch_y > 635 && touch_y < 659) return 4;  // 选项4
    if (touch_y > 680 && touch_y < 704) return 5;  // 选项5
    
    return 0;  // 没有匹配的选项
}

// 限制数值范围的宏
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
/**
 * @brief 调整传感器数值
 * @param sensor 传感器类型 (0:temp, 1:humi, 2:pm25, 3:ch2o, 4:co2, 5:ch4)
 * @param direction 方向 (1:增加, -1:减少)
 */
static void Adjust_Sensor_Value(uint8_t sensor, int8_t direction)
{
    // 增量配置 [传感器][Page2_Item-1]
    static const int32_t increments[6][5] = {
        {1, 10, 0, 0, 0},        // 温度
        {1, 10, 0, 0, 0},        // 湿度
        {1, 10, 100, 0, 0},      // PM2.5
        {1, 10, 100, 1000, 10000}, // CH2O
        {1, 10, 100, 1000, 10000}, // CO2
        {1, 10, 100, 1000, 0}    // CH4
    };
    
    // 限制范围
    static const struct {
        int32_t min, max;
    } limits[6] = {
        {-40, 85},      // 温度
        {0, 95},        // 湿度
        {1, 600},       // PM2.5
        {0, 60000},     // CH2O
        {400, 60000},   // CO2
        {0, 10000}      // CH4
    };
    
    // 参数检查
    if (Page2_Item < 1 || Page2_Item > 5) return;
    
    // 获取增量
    int32_t increment = increments[sensor][Page2_Item - 1];
    if (increment == 0) return;
    
    // 计算变化量
    int32_t delta = direction * increment;
    
    // 更新对应的传感器数值
    switch (sensor) {
        case 0:  // 温度
            EEPROM_WriteData.temp = CLAMP(EEPROM_WriteData.temp + delta, 
                                          limits[0].min, limits[0].max);
            break;
            
        case 1:  // 湿度
            EEPROM_WriteData.humi = CLAMP(EEPROM_WriteData.humi + delta, 
                                          limits[1].min, limits[1].max);
            break;
            
        case 2:  // PM2.5
            EEPROM_WriteData.pm25 = CLAMP(EEPROM_WriteData.pm25 + delta, 
                                          limits[2].min, limits[2].max);
            break;
            
        case 3:  // CH2O
            EEPROM_WriteData.ch2o = CLAMP(EEPROM_WriteData.ch2o + delta, 
                                          limits[3].min, limits[3].max);
            break;
            
        case 4:  // CO2
            EEPROM_WriteData.co2 = CLAMP(EEPROM_WriteData.co2 + delta, 
                                         limits[4].min, limits[4].max);
            break;
            
        case 5:  // CH4
            EEPROM_WriteData.ch4 = CLAMP(EEPROM_WriteData.ch4 + delta, 
                                         limits[5].min, limits[5].max);
            break;
    }
}
/**
 * @brief 处理设置按钮触摸
 * @return true: 按钮已处理, false: 没有按钮被按下
 */
static uint8_t Handle_Setting_Buttons(void)
{
    // 按钮配置表: [x1,x2,y1,y2,传感器,方向(1=加,-1=减)]
    static const struct {
        uint16_t x1, x2, y1, y2;
        uint8_t sensor;  // 0:temp, 1:humi, 2:pm25, 3:ch2o, 4:co2, 5:ch4
        int8_t dir;      // 1:增加, -1:减少
    } buttons[] = {
        // 温度
        {235, 281,  85, 131, 0, 1},
        {385, 442,  87, 130, 0, -1},
        // 湿度
        {235, 281, 145, 191, 1, 1},
        {385, 442, 147, 190, 1, -1},
        // PM2.5
        {235, 281, 205, 251, 2, 1},
        {385, 442, 207, 250, 2, -1},
        // CH2O
        {235, 281, 265, 311, 3, 1},
        {385, 442, 267, 310, 3, -1},
        // CO2
        {235, 281, 325, 371, 4, 1},
        {385, 442, 327, 370, 4, -1},
        // CH4
        {235, 281, 385, 431, 5, 1},
        {385, 442, 387, 430, 5, -1}
    };
    
    // 查找匹配的按钮
    for (int i = 0; i < 12; i++) {
        if (line_x >= buttons[i].x1 && line_x <= buttons[i].x2 &&
            line_y >= buttons[i].y1 && line_y <= buttons[i].y2) {
            
            Adjust_Sensor_Value(buttons[i].sensor, buttons[i].dir);
            return 1;
        }
    }
    
    return 0;
}

static uint8_t Save_And_Exit(void)
{
    if (line_x > 420 && line_x < 470 && line_y > 740 && line_y < 790) {
        EEPROM_WriteData.sta = 0x55;
        Eeprom_WriteData(0, &EEPROM_WriteData);
        Eeprom_ReadData(0, &EEPROM_ReadData);
        printf("Saved - T:%d H:%d P:%d C2:%d CO:%d C4:%d\n",
               EEPROM_ReadData.temp, EEPROM_ReadData.humi, EEPROM_ReadData.pm25,
               EEPROM_ReadData.ch2o, EEPROM_ReadData.co2, EEPROM_ReadData.ch4);
        Page = 0;
        return 1;
    }
    return 0;
}
static uint8_t Page1_To_Page2(void)
{
    if (line_x > 420 && line_x < 470 && line_y > 740 && line_y < 790) {
        Page = 2;
        return 1;
    }
    return 0;
}




void Handle_Touch(void)
{
    if (!touch_flag) return;
    touch_flag = 0;
    GT911_Scan(0);
    if (line_x == 0xFFFF || line_y == 0xFFFF) goto end;
    
    if (Page == 1)
		{
      uint8_t Item1 = Get_Page1_Item_From_Touch(line_x, line_y);
			if (Item1) {Page1_Item = Item1;}
			Page1_To_Page2();
    } 
    else if (Page == 3) 
		{
			uint8_t item2 = Get_Page2_Item_From_Touch(line_x, line_y);
			if (item2) {
					Page2_Item = item2;
					Page2_ParChoose(item2);
			} 
			else if (Handle_Setting_Buttons()) {}
			else if (line_x > 420 && line_x < 470 && line_y > 740 && line_y < 790) 
			{
					Save_And_Exit();
			}
    }
    end:line_x = line_y = 0xFFFF;  
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
//	uint8_t send_data1[1]={0X11};
//	uint8_t recv_buff[1]={0};
//	int16_t temp =25;
//	int16_t read_temp;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM14_Init();
  MX_TIM10_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_FSMC_Init();
  MX_TIM13_Init();
  MX_I2C1_Init();
  MX_TIM12_Init();
  MX_IWDG_Init();
	HAL_IWDG_Refresh(&hiwdg);
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
	
	LCD_Init();
	LCD_Clear(WHITE);	
	LCD_Splash_Screen();

	
	Sensor_PowCon(GPIO_PIN_SET);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //??????
	HAL_UART_Receive_DMA(&huart1,rx_buffer,100);  //??DMA????
	//HAL_ADC_Start_DMA(&hadc1, adc_buffer, 2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	CH4_Ro = CH4_RoVal(Moving_Average_Filter,adc_buffer);
	printf("CH4_Ro = %dΩ\n",CH4_Ro);	
	
	
	HAL_IWDG_Refresh(&hiwdg);
	PM25 = PM25_Getmg_Init(Composite_Filter,adc_buffer);
	printf("PM25 = %d\n",PM25);
	SGP30_Init();

	USR_Delay_ms(800);
	if((AHT20_Read_Status()&0x08)!=0x08)
	{
		AHT20_RST_REG(); 
		USR_Delay_ms(10);
	}
		printf("aht20_sta   = %x\r\n",AHT20_Read_Status());	
	LCD_Clear(WHITE);	
	Page_draw_all_img(&I_pages[0]);
	Page_draw_all_text(&T_pages[0]);
	TP_Init();
	//


	Touch_Exit_Init();
	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	
	
	EEPROM_WriteData.co2 =400;
	Eeprom_ReadData(0,&EEPROM_ReadData);
	if(EEPROM_ReadData.sta != 0x55)
	{
		EEPROM_WriteData.sta 	= 0x55;
		EEPROM_WriteData.temp	= 40;
		EEPROM_WriteData.humi	= 10;
		EEPROM_WriteData.pm25 = 100;
		EEPROM_WriteData.ch2o = 100;
		EEPROM_WriteData.co2  = 1000;
		EEPROM_WriteData.ch4  = 10000;
		Eeprom_WriteData(0,&EEPROM_WriteData);
	}
	Eeprom_ReadData(0,&EEPROM_ReadData);
	memcpy(&EEPROM_WriteData, &EEPROM_ReadData, sizeof(set_sensordata));
	printf("sta = %d\r\ntemp = %d\r\n,humi = %d\r\npm25 = %d\r\nch2o = %d\r\nco2 = %d\r\n,ch4 = %d\r\n",
	EEPROM_WriteData.sta,EEPROM_WriteData.temp,EEPROM_WriteData.humi,EEPROM_WriteData.pm25,
	EEPROM_WriteData.ch2o,EEPROM_WriteData.co2,EEPROM_WriteData.ch4);
	HAL_IWDG_Refresh(&hiwdg);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/*ch4  value*/
		Handle_Touch();
		switch(Page)
		{
			case 0:		
				LCD_Clear(WHITE);	
				Page_draw_all_img(&I_pages[0]);
				Page_draw_all_text(&T_pages[0]);		
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);	
				HAL_TIM_Base_Start_IT(&htim11);
				LCD_Select_Category(Page1_Item,1);
				Sensor_DegreeCalibrate(Page1_Item,&sensor,1);	
				Page = 1;
				printf("Page1_Item=%d\r\n",Page1_Item);
				break;
			case 1:
				currentTime = HAL_GetTick();
				// 检查是否达到2秒间隔
				if ((currentTime - lastCheckTime) >= CHECK_INTERVAL)
				{
					// 更新最后检测时间
					lastCheckTime = currentTime;
					//		/*temp&humi value*/
					AHT20_Read_CTdata(CT_data);
					c1 = (CT_data[0]*100*10/1024/1024)/10;  
					t1 = (CT_data[1]*200*10/1024/1024-500)/10;
					LCD_TempInterface(t1);
					LCD_HumiInterface(c1);				
					sensor.temp = t1;
					sensor.humi = c1;

					PM25 =PM25_Getmg(Composite_Filter,adc_buffer);
					LCD_PM25Interface(PM25);	
					sensor.pm25 = PM25;

					SGP30_START_MEASURE();
					SGP20_Read_data(SGP30_data);
					co2Data = SGP30_data[0];  
					ch2oData = SGP30_data[1];
					LCD_CH2OInterface(ch2oData);
					LCD_CO2Interface(co2Data);	

					sensor.ch2o = ch2oData;
					sensor.co2 = co2Data;

					CH4_Ppm =	CH4_CalculatePpmVal(Moving_Average_Filter,adc_buffer);
					LCD_CH4Interface(CH4_Ppm);

					sensor.ch4 = CH4_Ppm;				
					printf("TEMP:%d HUMI:%d PM25:%d CH2O:%d CO2:%d CH4:%d \r\n",
					sensor.temp,sensor.humi,sensor.pm25,sensor.ch2o,sensor.co2,sensor.ch4);
				}
				LCD_Select_Category(Page1_Item,0);				
				Sensor_DegreeCalibrate(Page1_Item,&sensor,0);	
				Test_results = Data_Compare(&sensor,&EEPROM_ReadData);
			//	printf("Test_results = %d\r\n",Test_results);

				if(Test_results > 0&&Alarm_ClFLG == 1 )
				{
					HAL_TIM_Base_Stop_IT(&htim11);
					HAL_TIM_Base_Start_IT(&htim12);
				}
				else
				{
					HAL_TIM_Base_Start_IT(&htim11);
					HAL_TIM_Base_Stop_IT(&htim12); 
					Led_SetLevel(GPIO_PIN_SET);
					HAL_TIM_PWM_Stop(&htim10, TIM_CHANNEL_1);	
					TM12_count = 0;						
				}
				 Alarm_Pic(Alarm_ClFLG);
				break;
			case 2:
				LCD_Clear(WHITE);	
				Page_draw_all_img(&I_pages[1]);
				Page_draw_all_text(&T_pages[1]);
				Page2_ParChoose(Page2_Item);
			
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				HAL_TIM_Base_Stop_IT(&htim12); 
				Led_SetLevel(GPIO_PIN_SET);
				HAL_TIM_PWM_Stop(&htim10, TIM_CHANNEL_1);
				HAL_TIM_Base_Stop_IT(&htim11);			
				TM12_count = 0;	
				Page = 3;
				break;
			case 3:

				LCD_TempSet(EEPROM_WriteData.temp);
				LCD_HumiSet(EEPROM_WriteData.humi);
				LCD_PM25Set(EEPROM_WriteData.pm25);
				LCD_CH2OSet(EEPROM_WriteData.ch2o);
				LCD_CO2Set (EEPROM_WriteData.co2);
				LCD_CH4Set (EEPROM_WriteData.ch4);
				
				break;
		}		
		HAL_IWDG_Refresh(&hiwdg);				
  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
