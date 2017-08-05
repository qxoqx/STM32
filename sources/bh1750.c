#include "bh1750.h"
#ifdef BH1750_H

void BH1750_Send(u8 command)
{
	
	IIC_Start();
	IIC_Send_Byte(BHAddWrite);
	while(IIC_Wait_Ack());
	IIC_Send_Byte(command);
	IIC_Wait_Ack();
	IIC_Stop();
	Delay_ms(5);
}

u16 HB1750_Read(void)
{
	u16 buf = 0;

	IIC_Start();
	IIC_Send_Byte(BHAddRead);	//从机地址+最后读方向位
	while(IIC_Wait_Ack());
	buf = IIC_Read_Byte(1);
	buf = buf << 8;
	buf += 0x00ff & IIC_Read_Byte(0);
	IIC_Stop();
	Delay_ms(5);
	return buf;
}

void BH1750_Start(void)
{
	BH1750_Send(BHPowOn);	//打开模块等待测量指令
	BH1750_Send(BHReset);	//重置数据寄存器值在PowerOn模式下有效
	BH1750_Send(BHSigModeH);//一次高分辨率 测量 测量后模块转到 PowerDown模式
}

float HB1750_Read_Data(void)
{
	BH1750_Start();
	Delay_ms(180);
	return (HB1750_Read() / 1.2);
}

void HB1750_Init(void)
{
	IIC_Init();
}

#endif
