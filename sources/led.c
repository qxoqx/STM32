#include "led.h"
#ifdef LED_H

void Led_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏带负载需上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_ResetBits(GPIOE, GPIO_Pin_5);
	
	LED2_OFF();
	LED3_OFF();
}

//void Led2_OFF(void){
//	GPIO_SetBits(GPIOE, GPIO_Pin_5);
//}
//void Led3_OFF(void){
//	GPIO_SetBits(GPIOB, GPIO_Pin_5);
//}
//void Led2_ON(void){
//	GPIO_ResetBits(GPIOE, GPIO_Pin_5);
//}
//void Led3_ON(void){
//	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
//}

#endif
