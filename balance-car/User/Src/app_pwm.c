/**
  ******************************************************************************
  * @file    app_pwm.c
  * @brief   TB6612 电机驱动 —— PWM 调速 + 方向控制
  *
  * 引脚：PA8  TIM1_CH1  -> PWMA (左电机)
  *       PB6  TIM4_CH1  -> PWMB (右电机)
  *       PA1            -> STBY
  *       PA9/PA10       -> AIN1/AIN2 (左电机方向)
  *       PB5/PB7        -> BIN1/BIN2 (右电机方向)
  *
  * 移植说明（标准库 -> HAL）：
  *   定时器本体的初始化（周期 999、预分频 0、PWM1 模式）已经搬到
  *   MX_TIM1_Init() / MX_TIM4_Init()，方向引脚和 STBY 引脚搬到 MX_GPIO_Init()。
  *   所以本文件只剩「启动 PWM」和「运行中改占空比/方向」这两件事。
  ******************************************************************************
  */

#include "app_pwm.h"
#include "qmath.h"

#define PERIOD 999

// 外设句柄，定义在 Core/Src/main.c 中
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

//
// @简介：启动两路 PWM 输出
//
void App_PWM_Init(void)
{
	// 开启 CCR 预加载：写 CCR 在下一个更新事件才生效，避免波形出现毛刺
	__HAL_TIM_ENABLE_OCxPRELOAD(&htim1, TIM_CHANNEL_1);
	__HAL_TIM_ENABLE_OCxPRELOAD(&htim4, TIM_CHANNEL_1);

	// HAL_TIM_PWM_Start 内部会：使能 CC1E、使能 MOE(高级定时器 TIM1 需要)、使能计数器
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

//
// @简介：控制 TB6612 进入待机还是工作状态
// @参数：State - 0 待机(STBY 低)，非零 工作(STBY 高)
//
void App_PWM_Cmd(uint8_t State)
{
	if(State)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	}
}

//
// @简介：设置左电机的占空比
// @参数：Duty - 占空比，范围 -100.0f ~ +100.0f，负号表示反转
//
void App_PWM_Set_L(float Duty)
{
	if(Duty >  100) Duty = 100;
	if(Duty < -100) Duty = -100;

	if(Duty >= 0)
	{
		// IN1 低，IN2 高
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	}
	else
	{
		// IN1 高，IN2 低
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	}

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,
	                      (uint16_t)(fabsf(Duty) / 100.0f * (PERIOD+1)));
}

//
// @简介：设置右电机的占空比
// @参数：Duty - 占空比，范围 -100.0f ~ +100.0f，负号表示反转
//
void App_PWM_Set_R(float Duty)
{
	if(Duty >  100) Duty = 100;
	if(Duty < -100) Duty = -100;

	if(Duty >= 0)
	{
		// IN1 低，IN2 高
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	}
	else
	{
		// IN1 高，IN2 低
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	}

	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,
	                      (uint16_t)(fabsf(Duty) / 100.0f * (PERIOD + 1)));
}
