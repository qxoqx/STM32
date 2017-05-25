#ifndef I2C_H
#define I2C_H

#include "stm32f10x.h"
#include "delay.h"

// //IO方向设置
// #define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=4<<12;} //浮空输入
// #define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=7<<12;} //开漏输出50Mhz

// //IO操作函数	 
// #define IIC_SCL_SET GPIO_SetBits(GPIOB,GPIO_Pin_10)
// #define IIC_SCL_CLR GPIO_ResetBits(GPIOB,GPIO_Pin_10)
// #define IIC_SDA_SET GPIO_SetBits(GPIOB,GPIO_Pin_11)
// #define IIC_SDA_CLR GPIO_ResetBits(GPIOB,GPIO_Pin_11) 
// #define READ_SDA    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) //输入SDA 

//IO方向设置
#define SDA_IN()  {GPIOG->CRH &= 0XF0FFFFFF; GPIOG->CRH |= 4 << 24;} //浮空输入
#define SDA_OUT() {GPIOG->CRH &= 0XF0FFFFFF; GPIOG->CRH |= 7 << 24;} //开漏输出50Mhz

//IO操作函数	 
#define IIC_SCL_SET GPIO_SetBits(GPIOG,GPIO_Pin_13)
#define IIC_SCL_CLR GPIO_ResetBits(GPIOG,GPIO_Pin_13)
#define IIC_SDA_SET GPIO_SetBits(GPIOG,GPIO_Pin_14)
#define IIC_SDA_CLR GPIO_ResetBits(GPIOG,GPIO_Pin_14) 
#define READ_SDA    GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_14) //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

//void SDA_IN(void);
//void SDA_OUT(void);

#endif

