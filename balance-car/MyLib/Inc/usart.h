/**
  ******************************************************************************
  * @file    usart.h
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2024年9月1日
  * @brief   串口头文件
  *
  * 移植说明（标准库 -> HAL）：
  *   标准库的串口函数以 USART_TypeDef*（外设寄存器基址）作句柄，
  *   HAL 则以 UART_HandleTypeDef*（句柄结构体）作句柄。
  *   所以下面所有函数的第一个参数都由 USART_TypeDef* 改成 UART_HandleTypeDef*，
  *   调用时传 &huart2 / &huart3。
  ******************************************************************************
  */

#ifndef _USART_H_
#define _USART_H_

#include "main.h"

// 这两个句柄定义在 Core/Src/main.c 里
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

#define LINE_SEPERATOR_CR   0x00 // 回车 \r
#define LINE_SEPERATOR_LF   0x01 // 换行 \n
#define LINE_SEPERATOR_CRLF 0x02 // 回车+换行 \r\n

void My_USART_SendByte(UART_HandleTypeDef *huart, const uint8_t Data);
void My_USART_SendBytes(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size);
void My_USART_SendChar(UART_HandleTypeDef *huart, const char C);
void My_USART_SendString(UART_HandleTypeDef *huart, const char *Str);
void My_USART_Printf(UART_HandleTypeDef *huart, const char *Format, ...);

    uint8_t My_USART_ReceiveByte(UART_HandleTypeDef *huart);
   uint16_t My_USART_ReceiveBytes(UART_HandleTypeDef *huart, uint8_t *pDataOut, uint16_t Size, int Timeout);
        int My_USART_ReceiveLine(UART_HandleTypeDef *huart, char *pStrOut, uint16_t MaxLength, uint16_t LineSeperator, int Timeout);

#endif
