/**
  ******************************************************************************
  * @file    app_cmd.c
  * @brief   遥控指令接收与解析
  *
  * 引脚：PB10 -> USART3_TX (9600, 8N1)
  *       PB11 -> USART3_RX
  *
  * 协议：以 '\n' 结尾的一行文本，例如 "move 50 -30\n"
  *       解析后调用 App_Control_Move(speed, turn)
  *
  * 移植说明（标准库 -> HAL）：
  *   1. 引脚（PB10 复用推挽 / PB11 上拉输入）和 USART3 的本体参数
  *      已经搬到 HAL_UART_MspInit() 和 MX_USART3_UART_Init()。
  *   2. 接收从「裸 RXNE 中断」改成 HAL 的「中断接收」两步走：
  *         HAL_UART_Receive_IT(&huart3, &rxByte, 1)   -> 开始等一个字节
  *         HAL_UART_RxCpltCallback()                  -> 收满一个字节时回调
  *      收完一个字节要再调一次 Receive_IT，否则只收到第一个字节就停了。
  *   3. USART3_IRQHandler 仍然留在本文件，保持模块自包含。
  ******************************************************************************
  */

#include "app_cmd.h"
#include "usart.h"
#include <string.h>
#include <strings.h> // strcasecmp 在 GCC/newlib 里属于 <strings.h>
#include <stdlib.h>
#include "app_control.h"

// 当前正在接收的一个字节
static volatile uint8_t rxByte = 0;

//
// @简介：初始化遥控指令接收
//
// @注意：USART3 的初始化已经由 MX_USART3_UART_Init() 完成，
//        这里只需要启动一次中断接收。
//
void App_Cmd_Init(void)
{
	HAL_UART_Receive_IT(&huart3, (uint8_t *)&rxByte, 1);
}

static char buffer[64];
static char cmd[64];
static uint16_t cursor = 0;
static volatile uint8_t cmdPending = 0;

//
// @简介：USART3 的中断响应函数
//
void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart3);
}

//
// @简介：串口接收完成回调（HAL 在收到指定数量的字节后调用）
//
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3)
	{
		uint8_t dataRcvd = rxByte;

		if(dataRcvd == '\n')
		{
			buffer[cursor++] = '\0';
			strcpy(cmd, buffer);
			cursor = 0; // 清空
			cmdPending = 1;
		}
		else
		{
			if(cursor >= 254)
			{
				cursor = 0; // 清空
			}

			buffer[cursor++] = dataRcvd;
		}

		// 继续接收下一个字节
		HAL_UART_Receive_IT(&huart3, (uint8_t *)&rxByte, 1);
	}
}

static void Move_Handler(const char *Args);

void App_Cmd_Proc(void)
{
	char cmdCpy[64];

	if(!cmdPending) return;

	__disable_irq();

	strcpy(cmdCpy, cmd); // 取出消息
	cmdPending = 0;

	__enable_irq();


	// 对消息进行解析

	// 将所有的空格替换成'\0'
	uint16_t len = strlen(cmdCpy);

	for(uint16_t i=0;i<len; i++)
	{
		if(cmdCpy[i] == ' ')
		{
			cmdCpy[i] = '\0';
		}
	}

	const char *name = cmdCpy;

	if(strcasecmp(name, "move") == 0)
	{
		Move_Handler(cmdCpy);
	}
}

static void Move(int8_t Speed, int8_t Turn)
{
//	My_USART_Printf(&huart2, "%d,%d\n", Speed, Turn);

	App_Control_Move(Speed, Turn);
}

static void Move_Handler(const char *Args)
{
	const char *ptr = Args + strlen(Args) + 1;

	int8_t turn = atoi(ptr);

	ptr = ptr + strlen(ptr) + 1;

	int8_t speed = atoi(ptr);

	Move(speed, turn);
}
