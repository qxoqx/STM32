#include "ds18b20.h"
#ifdef DS18B20_H

// void Delay_us(u32 Nus) 
// {  
// 	SysTick->LOAD=Nus*9;          //时间加载       
// 	SysTick->CTRL|=0x01;             //开始倒数     
// 	while(!(SysTick->CTRL&(1<<16))); //等待时间到达  
// 	SysTick->CTRL=0X00000000;        //关闭计数器 
// 	SysTick->VAL=0X00000000;         //清空计数器      
// } 


 

unsigned char ResetDS18B20(void)
{
	unsigned char resport;
	SetDQ();
	Delay_us(50);

	ResetDQ();
	Delay_us(500);  //500us （该时间的时间范围可以从480到960微秒）
	SetDQ();
	Delay_us(40);  //40us
	//resport = GetDQ();
	while(GetDQ());
	Delay_us(500);  //500us
	SetDQ();
	return resport;
}

void DS18B20WriteByte(unsigned char Dat)
{
	unsigned char i;
	for(i=8;i>0;i--)
	{
		ResetDQ();     //在15u内送数到数据线上，DS18B20在15-60u读数
		Delay_us(5);    //5us
		if(Dat & 0x01)
			SetDQ();
		else
			ResetDQ();
		Delay_us(65);    //65us
		SetDQ();
		Delay_us(2);    //连续两位间应大于1us
		Dat >>= 1; 
	} 
}


unsigned char DS18B20ReadByte(void)
{
	unsigned char i,Dat;
	SetDQ();
	Delay_us(5);
	for(i=8;i>0;i--)
	{
		Dat >>= 1;
		ResetDQ();     //从读时序开始到采样信号线必须在15u内，且采样尽量安排在15u的最后
		Delay_us(5);   //5us
		SetDQ();
		Delay_us(5);   //5us
		if(GetDQ())
			Dat|=0x80;
		else
			Dat&=0x7f;  
		Delay_us(65);   //65us
		SetDQ();
	}
	return Dat;
}


void ReadRom(unsigned char *Read_Addr)
{
 	unsigned char i;

 	DS18B20WriteByte(ReadROM);
  
 	for(i=8;i>0;i--)
	{
 		*Read_Addr=DS18B20ReadByte();
  		Read_Addr++;
	}
}


void DS18B20Init(unsigned char Precision,unsigned char AlarmTH,unsigned char AlarmTL)
{
	DisableINT();
	ResetDS18B20();
	DS18B20WriteByte(SkipROM); 
	DS18B20WriteByte(WriteScratchpad);
	DS18B20WriteByte(AlarmTL);
	DS18B20WriteByte(AlarmTH);
	DS18B20WriteByte(Precision);

	ResetDS18B20();
	DS18B20WriteByte(SkipROM); 
	DS18B20WriteByte(CopyScratchpad);
	EnableINT();

	while(!GetDQ());  //等待复制完成 ///////////
}


void DS18B20StartConvert(void)
{
	DisableINT();
	ResetDS18B20();
	DS18B20WriteByte(SkipROM); 
	DS18B20WriteByte(StartConvert); 
	EnableINT();
}

void DS18B20_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(DS_RCC_PORT, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DS_DQIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //2M时钟速度
	GPIO_Init(DS_PORT, &GPIO_InitStructure);
}


void DS18B20_Init(void)
{
	DS18B20_Configuration();
	DS18B20Init(DS_PRECISION, DS_AlarmTH, DS_AlarmTL);
	DS18B20StartConvert();
}


float DS18B20_Read(void)
{
	unsigned char DL, DH;
	unsigned short TemperatureData;
	float Temperature;

	DisableINT();
	DS18B20StartConvert();
	ResetDS18B20();
	DS18B20WriteByte(SkipROM); 
	DS18B20WriteByte(ReadScratchpad);
	DL = DS18B20ReadByte();
	DH = DS18B20ReadByte(); 
	EnableINT();

	TemperatureData = DH;
	TemperatureData <<= 8;
	TemperatureData |= DL;

	Temperature = (float)((float)TemperatureData * 0.0625); //分辨率为0.0625度

	return  Temperature;
}
 

#endif
