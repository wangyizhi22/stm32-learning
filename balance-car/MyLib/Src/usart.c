/**
  ******************************************************************************
  * @file    usart.c
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2024年9月1日
  * @brief   串口源文件
  *
  * 移植说明（标准库 -> HAL）：
  *   1. 句柄类型 USART_TypeDef*  ->  UART_HandleTypeDef*
  *   2. 发送：while(TXE) + USART_SendData  ->  HAL_UART_Transmit()
  *   3. 接收：while(RXNE) + USART_ReceiveData  ->  HAL_UART_Receive()
  *   4. My_USART_Printf 里的 __va_start/__va_end 是 ARMCC(Keil) 的私有内建宏，
  *      GCC 下要用标准 C 的 va_start/va_end（<stdarg.h>）。
  ******************************************************************************
  */

#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "delay.h"

// 单次发送/接收允许等待的最长时间（毫秒）
#define USART_TIMEOUT_MS 1000

//
// @简介：使用串口发送一个字节的数据
//
// @参数 huart：串口句柄，如 &huart2, &huart3
// @参数 Data ：要发送的数据
//
void My_USART_SendByte(UART_HandleTypeDef *huart, const uint8_t Data)
{
	My_USART_SendBytes(huart, &Data, 1);
}

//
// @简介：使用串口发送多个字节的数据
//
// @参数 huart：串口句柄
// @参数 pData：要发送的数据（数组）
// @参数 Size ：要发送数据的数量，单位是字节
//
void My_USART_SendBytes(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size)
{
	if(Size == 0) return;

	HAL_UART_Transmit(huart, (uint8_t *)pData, Size, USART_TIMEOUT_MS);
}

//
// @简介：通过串口发送一个字符
//
// @参数 huart：串口句柄
// @参数 C    ：要发送的字符
//
void My_USART_SendChar(UART_HandleTypeDef *huart, const char C)
{
	My_USART_SendBytes(huart, (const uint8_t *)&C, 1);
}

//
// @简介：通过串口发送字符串
//
// @参数 huart：串口句柄
// @参数 Str  ：要发送的字符串
//
void My_USART_SendString(UART_HandleTypeDef *huart, const char *Str)
{
	My_USART_SendBytes(huart, (const uint8_t *)Str, strlen(Str));
}

//
// @简介：通过串口格式化打印字符串
//
// @参数 huart ：串口句柄
// @参数 Format：字符串的格式
// @参数 ...   ：可变参数
//
void My_USART_Printf(UART_HandleTypeDef *huart, const char *Format, ...)
{
	char format_buffer[128];
	va_list argptr;

	va_start(argptr, Format); // 原为 ARMCC 的 __va_start

	vsprintf(format_buffer, Format, argptr);

	va_end(argptr); // 原为 ARMCC 的 __va_end

	My_USART_SendString(huart, format_buffer);
}

//
// @简介：通过串口读取一字节的数据（阻塞）
//
// @参数 huart：串口句柄
//
// @返回值：读取到的字节
//
uint8_t My_USART_ReceiveByte(UART_HandleTypeDef *huart)
{
	uint8_t data = 0;

	HAL_UART_Receive(huart, &data, 1, HAL_MAX_DELAY);

	return data;
}

//
// @简介：通过串口读取多个字节的数据
//
// @参数 huart   ：串口句柄
// @参数 pDataOut：输出参数，读取到的数据将输出到此数组当中
// @参数 Size    ：需要读取的字节数量
// @参数 Timeout ：超时时间，单位是毫秒，负数表示无限长。超时后返回已读到的数量。
//
// @返回值：实际读取到的数据数量
//
uint16_t My_USART_ReceiveBytes(UART_HandleTypeDef *huart, uint8_t *pDataOut, uint16_t Size, int Timeout)
{
	uint32_t timeout = (Timeout < 0) ? HAL_MAX_DELAY : (uint32_t)Timeout;

	if(HAL_UART_Receive(huart, pDataOut, Size, timeout) != HAL_OK)
	{
		// 超时了就返回实际收到的数量
		// HAL_UART_Receive 超时时 RxXferCount 里剩的就是还没收到的字节数
		return Size - (uint16_t)huart->RxXferCount;
	}

	return Size;
}

//
// @简介：通过串口读取一行字符串
//
// @参数 huart       ：串口句柄
// @参数 pStrOut     ：输出参数，读取到的数据将输出到此数组当中
// @参数 MaxLength   ：字符串的最大长度
// @参数 LineSeperator：行分隔符 LINE_SEPERATOR_CR   - 回车 \r
//                                LINE_SEPERATOR_LF   - 换行 \n
//                                LINE_SEPERATOR_CRLF - 回车+换行 \r\n
// @参数 Timeout     ：超时时间，单位是毫秒，负数表示无限长。
//
// @返回值：0  - 成功读到一行字符串
//         -1 - 超时（Timeout 内未读到一行完整的字符串）
//         -2 - 超过字符串的最大长度
//
int My_USART_ReceiveLine(UART_HandleTypeDef *huart, char *pStrOut, uint16_t MaxLength, uint16_t LineSeperator, int Timeout)
{
	// 如果最大长度都不足以装下行分隔符，就直接返回失败
	if(MaxLength < 2 || ((LineSeperator == LINE_SEPERATOR_CRLF) && (MaxLength < 1)))
	{
		return -2;
	}

	int ret = -1;
	uint32_t expireTime;

	Delay_Init(); // 要用到单片机当前时间，所以初始化延迟函数

	if(Timeout >= 0)
	{
		expireTime = GetTick() + Timeout; // 过期时间 = 当前时间 + Timeout
	}

	uint16_t i = 0;

	do
	{
		// 一次最多等 1ms，等不到就回去检查超时
		uint8_t c;
		if(HAL_UART_Receive(huart, &c, 1, 1) == HAL_OK)
		{
			pStrOut[i++] = (char)c;

			if(LineSeperator == LINE_SEPERATOR_CR && c == '\r') // \r
			{
				ret = 0;
				break;
			}
			else if(LineSeperator == LINE_SEPERATOR_LF && c == '\n') // \n
			{
				ret = 0;
				break;
			}
			else if(i >= 2 && pStrOut[i-2] == '\r' && c == '\n') // \r\n
			{
				ret = 0;
				break;
			}

			if(i == MaxLength) // 超过最大长度
			{
				ret = -2;
				break;
			}
		}
	}
	while(Timeout < 0 || GetTick() < expireTime); // 判断是否超时

	// 在字符串末尾增加 '\0'
	if(i == MaxLength)
	{
		pStrOut[i-1] = '\0';
	}
	else
	{
		pStrOut[i] = '\0';
	}

	return ret;
}
