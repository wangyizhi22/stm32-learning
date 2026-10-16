/**
  ******************************************************************************
  * @file    app_lights.c
  * @brief   WS2812 彩灯驱动（5颗灯珠，PA0 = TIM2_CH1）
  *
  * 原理：WS2812 每一位数据用一个 PWM 周期表示，
  *       高电平时间短 = 0，高电平时间长 = 1。
  *       先算好 120 位(5灯 x 24位)各自的高电平时间，放进数组，
  *       让定时器的每次更新事件(UEV)通过 DMA 自动把下一个值写进 CCR1，
  *       这一串 PWM 就变成了 WS2812 的数据流。
  *
  * 移植说明（标准库 -> HAL）：
  *   1. TIM2 的时基(周期 63 / 预分频 0 / PWM1 模式)搬到 MX_TIM2_Init()，
  *      PA0 复用推挽搬到 HAL_TIM_MspPostInit()，
  *      DMA1_Channel2 的参数搬到 HAL_TIM_Base_MspInit()。
  *   2. CubeMX 没有为「TIM + DMA」生成 HAL 函数，所以下面这几件事要手写：
  *        - TIM2->DCR  ：选择 DMA 搬运的目标基址为 CCR1，且一次搬 1 个
  *        - __HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_UPDATE)：让 UEV 触发 DMA 请求
  *        - HAL_DMA_Start()：每次刷新灯带时重新武装一次 DMA（普通模式，传完就停）
  ******************************************************************************
  */

#include "app_lights.h"
#include "main.h"
#include "delay.h"
#include "task.h"

typedef struct{
	uint8_t R[8];
	uint8_t G[8];
	uint8_t B[8];
}RGB;

typedef struct
{
	uint8_t dummy1;

	RGB rgbs[5];

	uint8_t dummy2;
}RGBs;

#pragma pack()

static RGBs rgbs;

// 外设句柄，定义在 Core/Src/main.c 中
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_up;

static void Downmap(void);
static void Set(uint8_t Which, uint8_t R, uint8_t G, uint8_t B);

void App_Lights_Init(void)
{
	// #1. 配置 DMA 的搬运目标：定时器的 CCR1，一次更新事件搬 1 个半字
	//     （等价于标准库的 TIM_DMAConfig(TIM2, TIM_DMABase_CCR1, TIM_DMABurstLength_1Transfer)）
	//     DBA = 0x0D 表示 CCR1，DBL = 0 表示一次搬 1 个
	TIM2->DCR = (0x0D << TIM_DCR_DBA_Pos) | (0x00 << TIM_DCR_DBL_Pos);

	// #2. 允许更新事件(UEV)产生 DMA 请求
	//     （等价于 TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE)）
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_UPDATE);

	// #3. 启动 PWM 输出（此时 CCR1 还是 0，灯带会保持复位状态）
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

static uint8_t enabled = 1;

void App_Lights_Cmd(uint8_t on)
{
	enabled = on;
}

static uint8_t stage = 0;

void App_Lights_Proc(void)
{
	PERIODIC(100)

	for(uint32_t i=0;i<5;i++)
	{
		Set(i, 0, 0, 0);
	}

	if(enabled)
	{
		Set(stage, 255, 255, 255);

		stage++;

		if(stage >= 5) stage = 0;
	}

	Downmap();
}

#define CODE_ZERO_H_TIME 21
#define CODE_ONE_H_TIME  43

static void Set(uint8_t Which, uint8_t R, uint8_t G, uint8_t B)
{
	uint32_t i;

	for(i=0;i<8;i++)
	{
		rgbs.rgbs[Which].R[i] = (R & (1<<(7-i))) ? CODE_ONE_H_TIME : CODE_ZERO_H_TIME;
	}

	for(i=0;i<8;i++)
	{
		rgbs.rgbs[Which].G[i] = (G & (1<<(7-i))) ? CODE_ONE_H_TIME : CODE_ZERO_H_TIME;
	}

	for(i=0;i<8;i++)
	{
		rgbs.rgbs[Which].B[i] = (B & (1<<(7-i))) ? CODE_ONE_H_TIME : CODE_ZERO_H_TIME;
	}
}

//
// @简介：把 rgbs 数组里的 122 个高电平时间，通过 DMA 依次写进 TIM2->CCR1
//
// @注意：DMA 工作在普通模式(Normal)，传完一轮就停，
//        所以每刷新一次灯带都要重新武装一次。
//
static void Downmap(void)
{
//	DelayUs(80); // Reset

	// 如果上一轮还没传完，先终止（正常情况下不会发生：一轮只要 200us 左右）
	if(hdma_tim2_up.State != HAL_DMA_STATE_READY)
	{
		HAL_DMA_Abort(&hdma_tim2_up);
	}

	// 等价于标准库的 DMA_Init + DMA_ClearFlag + DMA_Cmd(...ENABLE)
	HAL_DMA_Start(&hdma_tim2_up,
	              (uint32_t)&rgbs,          // 源：内存里的时间数组
	              (uint32_t)&TIM2->DMAR,    // 目的：TIM2 的 DMA 寄存器
	              sizeof(rgbs));            // 122 个

//	while(DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);

//	DelayUs(80); // Reset
}
