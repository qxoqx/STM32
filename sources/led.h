#ifndef LED_H
#define LED_H

#include "stm32f10x.h"

#define LED2_OFF() GPIO_SetBits(GPIOE,GPIO_Pin_5)
#define LED2_ON() GPIO_ResetBits(GPIOE,GPIO_Pin_5)
#define LED2_REV() GPIO_WriteBit(GPIOE, GPIO_Pin_5,(BitAction)(1-(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5))))

#define LED3_OFF() GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define LED3_ON() GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define LED3_REV() GPIO_WriteBit(GPIOB, GPIO_Pin_5,(BitAction)(1-(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5))))

extern void Led_Init(void);
//extern void Led2_OFF(void);
//extern void Led3_OFF(void);
//extern void Led2_ON(void);
//extern void Led3_ON(void);

#endif
