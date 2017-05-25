#include "co2.h"
#ifdef CO2_H

char Kmode0[COMMAND_DATA_SUM] = 			{0x4B,0x20,0x30,0x30,0x30,0x30,0x30,0x0d,0x0a};  //模式0 命令模式  指令: K 00000
char Kmode1[COMMAND_DATA_SUM] = 			{0x4B,0x20,0x30,0x30,0x30,0x30,0x31,0x0d,0x0a};  //模式1 串流模式  指令: K 00001   4B2030303030310d0a
char Kmode2[COMMAND_DATA_SUM] = 			{0x4B,0x20,0x30,0x30,0x30,0x30,0x32,0x0d,0x0a};  //模式2 轮询模式  指令: K 00002   4B2030303030320d0a
char MGetCo2Data[MCOMMAND_DATA_SUM] = 	{0x4D,0x20,0x30,0x30,0x30,0x30,0x34,0x0d,0x0a};  //发送指令 M 4 返回 Z  4D2030303030360d0a
char ZToZeroCo2[ZCOMMAND_DATA_SUM] = 	{0x58,0x20,0x34,0x35,0x30,0x0d,0x0a};//发送 x 450\r\n 校准CO2值      58203435300d0a
char ReadCmd[3] = {0x5A, 0x0D, 0x0A};

void Co2_Init(void)
{
	USART2_Puts(Kmode2);
	USART2_Puts(MGetCo2Data);
	USART2_Puts(ZToZeroCo2);
}

u16 Co2_Read(void)
{
	u16 co2ppm;
	USART2_Puts(ReadCmd);
	while(Uart2Sta == 0){}
	memset(Uart2Buff, 0, 64);
	memcpy(Uart2Buff, (char*)&Uart2Buffer[Uart2Front + 1], Uart2Cnt);
	co2ppm =  (Uart2Buff[1] & 0x0f) * 10000;
	co2ppm += (Uart2Buff[2] & 0x0f) * 1000;
	co2ppm += (Uart2Buff[3] & 0x0f) * 100;
	co2ppm += (Uart2Buff[4] & 0x0f) * 10;
	co2ppm += (Uart2Buff[5] & 0x0f) * 1;

	Uart2Front = 0;
	Uart2Rear = 0;
	Uart2Sta = 0;
	return co2ppm;
}

#endif
