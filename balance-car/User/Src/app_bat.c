#include "app_bat.h"
#include "main.h"
#include "task.h"
#include "usart.h"

static volatile uint8_t first_compute = 1;
static volatile float volt = 0;

// 外设句柄，定义在 Core/Src/main.c 中
extern TIM_HandleTypeDef htim3;
extern ADC_HandleTypeDef hadc1;

//
// @简介：初始化电源监控模块
//
// @注意：ADC1 和 TIM3 的本体初始化已经由 CubeMX 生成的
//        MX_ADC1_Init() / MX_TIM3_Init() 完成（引脚 PB0、分频、
//        采样时间、外部触发源 T3_TRGO 都在那里）。
//        这里只负责「把外设打开」：
//          - HAL_TIM_Base_Start(&htim3)  让 TIM3 开始 10ms 周期地产生 TRGO
//          - HAL_ADC_Start_IT(&hadc1)    打开 ADC 并使能 EOC 中断
//
void App_Bat_Init(void)
{
	HAL_TIM_Base_Start(&htim3); // 开启定时器3，定期溢出以触发ADC采样
	HAL_ADC_Start_IT(&hadc1);   // 开启ADC1并使能转换完成中断
}

//
// @简介：ADC 转换完成回调函数
//
// @注意：原标准库版本是自己写 ADC1_2_IRQHandler() 并判断 EOC 标志；
//        HAL 里 ADC1_2_IRQHandler() 在 Core/Src/stm32f1xx_it.c 中
//        （它调用 HAL_ADC_IRQHandler()），HAL 判断完标志后会回调到这里，
//        所以我们只需要实现这个回调即可。
//
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1)
	{
		uint16_t dr = HAL_ADC_GetValue(hadc);

		volt = dr / 4095.0f * 8.4;
	}
}

float App_Bat_Get(void)
{
	return volt;
}

void App_Bat_Proc(void)
{
	PERIODIC(20)
	
	// 5.8 6.6 7.4 8.2
	
	static uint32_t nxtBlinkTime = 0;
	static uint8_t blinkStage = 0; // 灭
	
	if(volt < 6) // 亏电，闪灯100ms
	{
		if(GetTick() > nxtBlinkTime)
		{
			nxtBlinkTime = GetTick() + 100;
			
			if(blinkStage == 0)
			{
				blinkStage = 1;
				// 全亮
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
			}
			else
			{
				blinkStage = 0;
				
				// 全灭
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			}
		}
	}
	else if(volt < 6.6) // 亮0颗灯 <33 %
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	}
	else if(volt < 7.3) // 亮1颗灯 <66 %
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	}
	else if(volt < 8.0) // 亮2颗灯 <99 %
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	}
	else // 全亮
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	}
}
