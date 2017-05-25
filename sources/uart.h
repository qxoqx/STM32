#ifndef UART_H
#define UART_H
#include "stm32f10x.h"
#include "stdio.h"

extern __IO uint8_t Uart1Front;
extern __IO uint8_t Uart1Rear;
extern __IO uint8_t Uart1Cnt;
extern __IO uint8_t Uart1Sta;
extern __IO char Uart1Buffer[256];
extern char Uart1Buff[64];

extern __IO uint8_t Uart2Front;
extern __IO uint8_t Uart2Rear;
extern __IO uint8_t Uart2Cnt;
extern __IO uint8_t Uart2Sta;
extern __IO char Uart2Buffer[256];
extern char Uart2Buff[64];

extern __IO uint8_t Uart3Front;
extern __IO uint8_t Uart3Rear;
extern __IO uint8_t Uart3Cnt;
extern __IO uint8_t Uart3Sta;
extern __IO char Uart3Buffer[256];
extern char Uart3Buff[64];

void Uart1_Init(void);
void Uart2_Init(void);
void Uart3_Init(void);
void USART1_IRQHandler(void); //ÖÐ¶Ï·þÎñº¯Êý
void USART2_IRQHandler(void); //ÖÐ¶Ï·þÎñº¯Êý
void USART3_IRQHandler(void); //ÖÐ¶Ï·þÎñº¯Êý
void USART1_Putchar(char ch);
void USART1_Puts(char *string);
void USART2_Puts(char *string);
void USART3_Puts(char *string);
void USART3_PutData(char *string, uint8_t num);

#endif
