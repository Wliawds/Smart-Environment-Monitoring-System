#include "LCD_INTERFACE.h"
#include "LCD.h"
#include "IMAGE.h"

#include "stdio.h"


UI_img page1_ui_img[] = {
	{76, 10,404, 41,gImage_MD			},
	{30, 80,452,134,gImage_KT			},
	{40, 86, 81,127,gImage_TEMP		},
	{30,140,452,194,gImage_KT			},
	{40,146, 81,187,gImage_HUMI		},
	{30,200,452,254,gImage_KT			},
	{40,206, 81,247,gImage_PM25		},
	{30,260,452,314,gImage_KT			},	
	{40,266, 81,307,gImage_CH2O		},
	{30,320,452,374,gImage_KT			},
	{40,326, 81,367,gImage_CO2		},	
	{30,380,452,434,gImage_KT			},
	{40,386, 81,427,gImage_CH4  	},	
	{40,470,280,720,gImage_DJT1 	},
	{310,620,436,728,gImage_DJT2	},
	{310,478,438,589,gImage_DJT3	},
	{116,750,364,790,gImage_LOGO	},
	{420,740,470,790,gImage_ENTER	},
	{425, 10,470, 45,gImage_AON	 	}
};
UI_img page2_ui_img[] = {
	{30, 80, 452,134,gImage_KT  	},
	{40, 86,  81,127,gImage_TEMP	},
	{240,90, 276,126,gImage_ADD		},
	{400,102,427,115,gImage_DEC		},
	{30, 140,452,194,gImage_KT		},
	{40, 146, 81,187,gImage_HUMI	},
	{240,150,276,186,gImage_ADD		},
	{400,162,427,175,gImage_DEC		},
	{30, 200,452,254,gImage_KT		},
	{40, 206, 81,247,gImage_PM25	},
	{240,210,276,246,gImage_ADD		},
	{400,222,427,235,gImage_DEC		},
	{30, 260,452,314,gImage_KT		},
	{40, 266, 81,307,gImage_CH2O	},
	{240,270,276,306,gImage_ADD		},
	{400,282,427,295,gImage_DEC		},
	{30, 320,452,374,gImage_KT		},
	{40, 326, 81,367,gImage_CO2		},
	{240,330,276,366,gImage_ADD		},
	{400,342,427,355,gImage_DEC		},
	{30, 380,452,434,gImage_KT		},
	{40, 386, 81,427,gImage_CH4	  },
	{240,390,276,426,gImage_ADD		},
	{400,402,427,415,gImage_DEC		},
	{78,  10,402, 50,gImage_PS		},
	{116,750,364,790,gImage_LOGO	},
	{420,740,470,790,gImage_ESC		}
};

UI_Text page1_ui_text[] = {
	
	{100, 445,300, 24,24,"--Current data [    ]--"},

};

UI_Text page2_ui_text[] = {
	{80,445,300,24,24,"--Quick setting options--"},
	{30,474,440,24,24,"***********************************"},
	{40,500,400,24,24,"Add [    1] to the parameter.[   ]"},
	{40,545,400,24,24,"Add [   10] to the parameter.[   ]"},
	{40,590,400,24,24,"Add [  100] to the parameter.[   ]"},
	{40,635,400,24,24,"Add [ 1000] to the parameter.[   ]"},
	{40,680,400,24,24,"Add [10000] to the parameter.[   ]"},
	{30,720,440,24,24,"***********************************"}
};

Page_imgInfo I_pages[] = {
    {page1_ui_img, sizeof(page1_ui_img)/sizeof(page1_ui_img[0])},
    {page2_ui_img, sizeof(page2_ui_img)/sizeof(page2_ui_img[0])}
};

Page_textInfo T_pages[] = {
    {page1_ui_text, sizeof(page1_ui_text)/sizeof(page1_ui_text[0])},
    {page2_ui_text, sizeof(page2_ui_text)/sizeof(page2_ui_text[0])}
};

void Page_draw_all_text(const Page_textInfo* page)
{
    for (int i = 0; i < page->text_count; i++) 
    {
        const UI_Text* text = &page->text_array[i];
        LCD_ShowString(text->x, text->y, 
                      text->width, 
                      text->height, 
                      text->font_size, 
                      (uint8_t*)text->text);
    }
}

void Page_draw_all_img(const Page_imgInfo* page) 
{
	for (int i = 0; i < page->img_count; i++) 
	{
		const UI_img* img = &page->img_array[i];
		LCD_DrawPicture(img->x1, img->y1, img->x2, img->y2, (uint8_t*)img->img);
	}
}

// 温度阈值配置
static const ThresholdRange temp_thresholds[] = 
{
    {22, 25, 1},      // 优
    {18, 21, 2},      // 良(注意：修改了范围，原逻辑有重叠)
    {22, 22, 2},      // 良(补充分界点)
    {25, 27, 2},      // 良
    {16, 17, 3},      // 中
    {18, 18, 3},      // 中(补充分界点)
    {27, 29, 3},      // 中
    {10, 15, 4},      // 较差
    {29, 32, 4},      // 较差
    { 0,  9, 5},      // 差
    {32, 38, 5},      // 差
    {-1000, -1, 6},   // 极差(使用一个很低的数表示负无穷)
    {39, 1000,  6}    // 极差(使用一个很高的数表示正无穷)
};

// 湿度阈值配置
static const ThresholdRange humi_thresholds[] = 
{
    {45, 55, 1	},      // 优
    {40, 44, 2	},      // 良
    {56, 60, 2	},      // 良
    {35, 39, 3	},      // 中
    {61, 70, 3	},      // 中
    {30, 34, 4	},      // 较差
    {71, 80, 4	},      // 较差
    {20, 29, 5	},      // 差
    {81, 90, 5	},      // 差
    { 0, 19, 6	},      // 极差
    {91, 1000, 6}     	// 极差
};

// PM2.5阈值配置
static const ThresholdRange pm25_thresholds[] =
{
    {0, 10, 1		},     	// 优
    {10, 19, 2	},      // 良
    {20, 34, 3	},      // 中
    {35, 54, 4	},      // 较差
    {55, 149, 5	},     	// 差
    {150, 1000, 6}    	// 极差
};

// CH2O阈值配置
static const ThresholdRange ch2o_thresholds[] = 
{
    {0, 10, 1		},   		// 优
    {10, 32, 2	},      // 良
    {33, 80, 3	},      // 中
    {81, 163, 4	},     	// 较差
    {164, 327, 5},    	// 差
    {328, 1000, 6}    	// 极差
};

// CO2阈值配置
static const ThresholdRange co2_thresholds[] = 
{
    {0, 450, 1	},      // 优
    {450, 699, 2},    	// 良
    {700, 999, 3},    	// 中
    {1000, 1499, 4},  	// 较差
    {1500, 2499, 5},  	// 差
    {2500, 10000, 6}  	// 极差
};

// CH4阈值配置
static const ThresholdRange ch4_thresholds[] =
{
    {0, 2, 1		},   		// 优
    {2, 9, 2		},    	// 良
    {10, 49, 3	},      // 中
    {50, 9999, 4},    	// 较差
    {10000, 49999, 5},	// 差
    {50000, 100000, 6}	// 极差
};

// 显示位置配置
static const struct {
    uint16_t x1, y1, x2, y2;
} display_pos[6] = {
    {275,  93, 307, 125},  // 温度
    {275, 153, 307, 185},  // 湿度
    {275, 213, 307, 245},  // PM2.5
    {275, 273, 307, 305},  // CH2O
    {275, 333, 307, 365},  // CO2
    {275, 393, 307, 425}   // CH4
};

// 统一的配置结构
typedef struct {
    uint16_t set_x, set_y, set_width;
    uint16_t int_x, int_y, int_width;
    const char* set_format;
    const char* int_format;
} SensorDisplayConfig;

// 配置数组
static const SensorDisplayConfig sensor_configs[] = {
    {308,  95, 100, 320,  95, 100, "%03d", "%03dC"	},    // 温度
    {308, 155, 100, 320, 155, 100, "%03d", "%03d%%"	},    // 湿度
    {308, 215, 120, 320, 215, 120, "%04d", "%04dug/m3"},  // PM2.5
    {308, 275, 120, 320, 275, 120, "%05d", "%05dPPB"},    // CH2O
    {308, 335, 120, 320, 335, 120, "%05d", "%05dPPM"},    // CO2
    {308, 395, 120, 320, 395, 120, "%05d", "%05dPPM"}     // CH4
};

// 内部统一的显示函数
void LCD_DisplayValue(int sensor_idx, int is_interface, int32_t val)
{
    if (sensor_idx < 0 || sensor_idx >= sizeof(sensor_configs)/sizeof(sensor_configs[0])) {
        return;
    }
    
    const SensorDisplayConfig* cfg = &sensor_configs[sensor_idx];
    const char* format;
    uint16_t x, y, width;
    
    if (is_interface) {
        format = cfg->int_format;
        x = cfg->int_x;
        y = cfg->int_y;
        width = cfg->int_width;
    } else {
        format = cfg->set_format;
        x = cfg->set_x;
        y = cfg->set_y;
        width = cfg->set_width;
    }
    
    uint8_t buffer[16];
    snprintf((char*)buffer, sizeof(buffer), format, val);
    LCD_ShowString(x, y, width, 24, 24, buffer);
}

uint8_t Data_Compare(RealTime_Data* redat, set_sensordata* sedat)
{
    warn_dev Cp_val = {.byte_value = 0};

    // 实时值和设定值的数组（方便遍历）
    int32_t real_values[SENSOR_COUNT] = {
        redat->temp, redat->humi, redat->pm25, 
        redat->ch2o, redat->co2, redat->ch4
    };
    
    int32_t set_values[SENSOR_COUNT] = {
        sedat->temp, sedat->humi, sedat->pm25, 
        sedat->ch2o, sedat->co2, sedat->ch4
    };
    
    // 遍历所有传感器
    for (int i = 0; i < SENSOR_COUNT; i++) {
        if (real_values[i] > set_values[i]) {
            // 显示报警图标
            LCD_DrawPicture(display_pos[i].x1, display_pos[i].y1,
                           display_pos[i].x2, display_pos[i].y2,
                           (uint8_t *)gImage_BJ);
            
            // 设置报警位
            switch(i) {
                case 0: Cp_val.bits.warn_temp = 1; break;
                case 1: Cp_val.bits.warn_humi = 1; break;
                case 2: Cp_val.bits.warn_pm25 = 1; break;
                case 3: Cp_val.bits.warn_ch2o = 1; break;
                case 4:  Cp_val.bits.warn_co2  = 1; break;
                case 5:  Cp_val.bits.warn_ch4  = 1; break;
            }
        } else {
            // 清除显示
            LCD_Fill(display_pos[i].x1, display_pos[i].y1,
                    display_pos[i].x2, display_pos[i].y2,
                    WHITE);
        }
    }
    
    return Cp_val.byte_value;
}
static uint8_t CalculateLevel(int32_t value, const ThresholdRange* thresholds, int count)
{
    for (int i = 0; i < count; i++) {
        if (value >= thresholds[i].min_value && value <= thresholds[i].max_value) {
            return thresholds[i].level;
        }
    }
    return 6; // 默认返回最低等级
}
// 优化后的主函数
void Sensor_DegreeCalibrate(uint8_t item, RealTime_Data *senddata,uint8_t force_refresh)
{
    int32_t value = 0;
    uint8_t level = 0;
    
    switch(item) {
        case SENSOR_TEMP:
            value = senddata->temp;
            level = CalculateLevel(value, temp_thresholds, sizeof(temp_thresholds)/sizeof(temp_thresholds[0]));
            break;
        case SENSOR_HUMI:
            value = senddata->humi;
            level = CalculateLevel(value, humi_thresholds, sizeof(humi_thresholds)/sizeof(humi_thresholds[0]));
            break;
        case SENSOR_PM25:
            value = senddata->pm25;
            level = CalculateLevel(value, pm25_thresholds, sizeof(pm25_thresholds)/sizeof(pm25_thresholds[0]));
            break;
        case SENSOR_CH2O:
            value = senddata->ch2o;
            level = CalculateLevel(value, ch2o_thresholds, sizeof(ch2o_thresholds)/sizeof(ch2o_thresholds[0]));
            break;
        case SENSOR_CO2:
            value = senddata->co2;
            level = CalculateLevel(value, co2_thresholds, sizeof(co2_thresholds)/sizeof(co2_thresholds[0]));
            break;
        case SENSOR_CH4:
            value = senddata->ch4;
            level = CalculateLevel(value, ch4_thresholds, sizeof(ch4_thresholds)/sizeof(ch4_thresholds[0]));
            break;
        default:
            return;
    }
    
    LCD_LevelSwitch(level,force_refresh);
}

void LCD_Splash_Screen(void)
{
	LCD_ShowString(80,300,480,24,24,(uint8_t *)"The system is initializing, ");
	LCD_ShowString(80,325,480,24,24,(uint8_t *)"please wait... ");
	LCD_DrawPicture(116,750,364,790,(uint8_t *)gImage_LOGO);
}

void Alarm_Pic(uint8_t val)
{
	switch(val)
	{
		case 0:LCD_DrawPicture(425, 10,470,45,(uint8_t *)gImage_AOFF);
			break;
		case 1:LCD_DrawPicture(425, 10,470,45,(uint8_t *)gImage_AON);
			break;
		default:break;
	
	}
}


void Page2_ParChoose(uint8_t item)
{
    // 所有选项的Y坐标数组
    static const uint16_t option_y_positions[5] = {
        503, 548, 593, 638, 683
    };
    
    // 固定参数
    const uint16_t x1 = 404;
    const uint16_t x2 = 430;
    const uint16_t height = 22;  // 525-503=22
    
    // 参数验证
    if (item < 1 || item > 5) {
        return;
    }
    
    // 当前选择的索引（从0开始）
    uint8_t selected_index = item - 1;
    
    // 遍历所有选项
    for (uint8_t i = 0; i < 5; i++) {
        uint16_t y = option_y_positions[i];
        
        if (i == selected_index) {
            // 绘制选中项的图标
            LCD_DrawPicture(x1, y, x2, y + height, (uint8_t *)gImage_DG);
        } else {
            // 清除未选中项
            LCD_Fill(x1, y, x2, y + height, WHITE);
        }
    }
}

static uint8_t last_level = 0;  // 上一次显示的等级

void LCD_LevelSwitch(uint8_t level,uint8_t force_refresh)
{

        if (!force_refresh && level == last_level) {
        return;
    }
    static const uint16_t left_y[6] = {480, 518, 559, 600, 641, 677};
    static uint8_t* right_img[6] = {
        (uint8_t *)gImage_excellent,
        (uint8_t *)gImage_good,
        (uint8_t *)gImage_average,
        (uint8_t *)gImage_normal,
        (uint8_t *)gImage_serious,
        (uint8_t *)gImage_critical
    };
    
    if (level < 1 || level > 6) {
        return;
    }
    
    uint8_t index = level - 1;
    
    // 如果有上一次选择，清除旧的左侧标记
    if (last_level > 0) {
        uint8_t last_idx = last_level - 1;
        LCD_Fill(5, left_y[last_idx], 38, left_y[last_idx] + 33, WHITE);
    } else {
        // 第一次调用，清除所有左侧标记
        for (int i = 0; i < 6; i++) {
            LCD_Fill(5, left_y[i], 38, left_y[i] + 33, WHITE);
        }
    }
    
    // 绘制新的左侧标记
    LCD_DrawPicture(5, left_y[index], 38, left_y[index] + 33, (uint8_t *)gImage_JT);
    
    // 更新右侧图片（总是绘制，因为图片会变化）
    LCD_DrawPicture(335, 515, 419, 557, right_img[index]);
    
    // 更新状态
    last_level = level;
}
static uint8_t last_selection = 0;

// 添加force参数，强制刷新
void LCD_Select_Category(uint8_t select, uint8_t force_refresh)
{
    // 如果不是强制刷新且选择没有变化，直接返回
    if (!force_refresh && select == last_selection) {
        return;
    }
    
    static const uint16_t y_pos[6] = {98, 158, 218, 278, 338, 398};
    static const char* texts[6] = {"TEMP", "HUMI", "PM25", "CH2O", " CO2", " CH4"};
    
    if (select < 1 || select > 6) {
        return;
    }
    
    uint8_t index = select - 1;
    
    // 显示选中的文本
    LCD_ShowString(293, 445, 96, 24, 24, (uint8_t *)texts[index]);
    
    // 清除所有标记（简化逻辑）
    for (int i = 0; i < 6; i++) {
        LCD_Fill(250, y_pos[i], 270, y_pos[i] + 20, WHITE);
    }
    
    // 绘制新的红色标记
    LCD_Fill(250, y_pos[index], 270, y_pos[index] + 20, RED);
    
    // 更新状态
    last_selection = select;
}



