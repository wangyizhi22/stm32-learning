/**
  ******************************************************************************
  * @file    app_usart2.c
  * @brief   USART2 —— 向上位机打印调试信息
  *
  * 移植说明（标准库 -> HAL）：
  *   原标准库版本在这里手写 RCC/GPIO/USART 的初始化。
  *   HAL 把这些都搬到了 CubeMX 生成的 MX_USART2_UART_Init() 里：
  *       - 参数（921600、8N1、收发）  -> MX_USART2_UART_Init()
  *       - 引脚 PA2(AF_PP) / PA3(上拉) -> HAL_UART_MspInit()
  *   所以本函数不需要再做任何初始化，保留它只是为了与原工程的调用结构一致。
  ******************************************************************************
  */

#include "app_usart2.h"

void App_USART2_Init(void)
{
	// USART2 的初始化已由 MX_USART2_UART_Init() 完成，这里无需重复配置。
	// 打印用 My_USART_Printf(&huart2, ...)。
}
