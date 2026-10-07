#include "PM25_BSP.h"
#include "GPIO.h"
#include "MYDELAY.h"
#include "stdio.h"

extern ADC_HandleTypeDef hadc1;



uint32_t Get_PM25Val(uint32_t *buf)
{
	uint32_t pm25_code;

	HAL_ADC_Stop_DMA(&hadc1);
	Pm_LEDSetLevel(GPIO_PIN_SET);
	USR_Delay_us(280);
	Pm_LEDSetLevel(GPIO_PIN_RESET);
	USR_Delay_us(5);
	HAL_ADC_Start_DMA(&hadc1, buf, 2);
	USR_Delay_us(30);
	pm25_code = buf[0];
	USR_Delay_ms(10);

	return  pm25_code;
}

uint32_t PM25_Getmg(uint32_t (*filter)(uint32_t),uint32_t *buf)
{
	uint32_t adc_val,filter_val;
	double 	 pm25_ppm;
	
	adc_val = Get_PM25Val(buf);
	filter_val = filter(adc_val);
	
	pm25_ppm = (double)(filter_val)*3300.f/4096.f*3.f;
	return (uint32_t)(pm25_ppm*0.18-162);//1290
}


uint32_t PM25_Getmg_Init(uint32_t (*filter)(uint32_t),uint32_t *buf)
{
	uint32_t adc_val,filter_val,val;
	uint8_t s_count=0,f_count=0,t_count=0;
	double 	 pm25_ppm;
	while(1)
	{
		do{
			adc_val = Get_PM25Val(buf);
			filter_val = filter(adc_val);
			pm25_ppm = (double)(filter_val)*3300.f/4096.f*3.f;
			val= (uint32_t)(pm25_ppm*0.18-162);//1290
			USR_Delay_ms(100);
			t_count ++;
			if(t_count > 10){t_count = 0;break;}
			printf("pm2.5 = %d\n",val);
		}while(val > 50);
		if(c > 0 && val <50)
		{
			s_count ++;
			printf("success count = %d\n",s_count);
			if(s_count >= 10)
			{
				printf("Sampling completed.pm25 val = %d\n",val);
				return val;
			}
		}			
			else
			{
				s_count = 0;
				f_count ++;
				if(f_count > 5){break;}
				printf("Counting failed, starting to resample.=%d\n",f_count);			
			}
		}
			printf("Sampling failure,adc code using constant value.=%d\n",PM25_CONSTANT);
			return PM25_CONSTANT;
	
}









