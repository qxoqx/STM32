#ifndef KEY_H
#define KEY_H

#include "delay.h"
#include "stm32f10x.h"

#define S1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define S2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define S3 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define S4 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

#define KEY4 	4
#define KEY3	3
#define KEY2	2
#define KEY1	1

extern void KEY_Init(void);
extern uint8_t KEY_Scan(uint8_t mode);

#endif
