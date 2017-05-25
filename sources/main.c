#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "uart.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "flash.h"
#include "spi.h"
#include "stdio.h"
#include "string.h"
#include "dht11.h"
#include "ds18b20.h"
#include "soil.h"
#include "bh1750.h"
#include "co2.h"
#include "pwm.h"

// void Delay(int ms){
// 	int a;
// 	while(ms > 0){
// 		a = 10000;
// 		while(a > 0)
// 			a --;
// 		ms -- ;
// 	}
// }
typedef struct 
{
	u8 front;
	u8 plantName;			//植物种类
	u8 fanStatus;			//风扇状态
	u8 pumpStatus;			//水泵状态
	u8 lightStatus;			//灯光状态
	int airHumidity;		//空气湿度
	float temp;				//空气温度
	float soilMoisture;		//土壤湿度
	float lightLux;			//光照强度
	int co2ppm;				//CO2浓度
	u8 rear;					//空
}SaveData;

typedef struct 
{
	u8 fanlock;
	u8 fancnt;
	u8 pumplock;
	u8 pumpcnt;
	u8 lightlock;
	u8 lightcnt;
}ControlData;

typedef struct 
{
	u8 getStatus;
	u8 isChangePlant;
	u8 plantName;
	u8 fanControl;
	u8 pumpControl;
	u8 lightControl;	
}ReceiveData;

const u8 TEXT_Buffer[]={"STM32 SPI TEST"};
#define FLASH_SIZE 8388608 //8 * 1024 * 1024
#define SIZE sizeof(TEXT_Buffer)

int main(void){
//	uint16_t i = 0;
//	uint8_t key;
//	uint8_t datatemp[SIZE];
	SaveData saveData;
	ControlData controlData;
	ReceiveData *receiveData;
	char uartBuff[50];
	float temperature;
	float soilMoisture[3];     //保存计算后的电压值
	float lightLux;
	u16 co2ppm;
	u8 wd=0;
	u8 sd=0;
	saveData.plantName = 1;


	SysTick_Init();
	Led_Init();
	Uart1_Init();
	Uart2_Init();
	Uart3_Init();
	Delay_ms(100);
	USART_ClearFlag(USART1, USART_FLAG_TC);//清除传输完成标志位,否则可能会丢失第1个字节的数据.
	USART_ClearFlag(USART2, USART_FLAG_TC);//清除传输完成标志位,否则可能会丢失第1个字节的数据.
	USART_ClearFlag(USART3, USART_FLAG_TC);//清除传输完成标志位,否则可能会丢失第1个字节的数据.
	//printf("Uart Init Successful!\r\n");	
	USART1_Puts("USART1_Init Successful!\r\n");
	USART3_Puts("USART3_Init Successful!\r\n");
	KEY_Init();
	SPI_Flash_Init();
	DHT11_Init();
	DS18B20_Init();
	LED2_OFF();
	LED3_OFF();
	Soil_Moisture_Init();
	HB1750_Init();
	Co2_Init(); //co2初始化之前先需要初始化串口
	Pwm_Init();

	USART3_Puts("Init Successful!\r\n");

	while(SPI_Flash_ReadID() != W25Q16 && SPI_Flash_ReadID() != W25Q32 && SPI_Flash_ReadID() != W25Q64)	
	{
		Delay_ms(1000);
		printf("25Q64 Check Failed!\r\n");
		LED3_ON();
	}
	printf("25Q64 Ready!\r\n");
	
	//SPI_Flash_Read((u8)&saveData, FLASH_SIZE-100, sizeof(SaveData));

	saveData.front = 0x5a;
	saveData.rear = 0xa5;
	saveData.plantName = 1;
	saveData.fanStatus = 0;
	saveData.pumpStatus = 0;
	saveData.lightStatus = 0;

	while(1)
	{
		temperature = DS18B20_Read();   //读取精确温度
		DHT11_Read_Data(&wd, &sd);		//读取温湿度值
		Soil_Moisture_Read(soilMoisture);  //读取湿度
		lightLux = HB1750_Read_Data();		//读取光照强度
		co2ppm = Co2_Read();

		saveData.temp = temperature;
		saveData.airHumidity = sd;
		//saveData.soilMoisture = (soilMoisture[0] + soilMoisture[0] + soilMoisture[0]) / 3;
		saveData.soilMoisture = (soilMoisture[0] + soilMoisture[1]) / 2;
		saveData.lightLux = lightLux;
		saveData.co2ppm = co2ppm;
		//sprintf(uartBuff, "%f %d %f %f %d", saveData.temp, saveData.airHumidity, saveData.soilMoisture, saveData.lightLux, saveData.co2ppm);
		//USART1_Puts(uartBuff);
		//USART3_PutData((char*)&saveData, 20);  

		if(Uart3Sta == 1 && Uart3Cnt == 6)
		{
			
			memset(Uart3Buff, 0, 64);
			memcpy(Uart3Buff, (char*)&Uart3Buffer[Uart3Front + 1], Uart3Cnt);
			
			receiveData = (ReceiveData *)9;
			if(receiveData->isChangePlant == 1){
				saveData.plantName = receiveData->plantName;
			}


			if(receiveData->fanControl == 0){
				saveData.fanStatus = 0;
				controlData.fanlock = 0;
				controlData.fancnt	= 0;
			}else if(receiveData->fanControl == 1 && controlData.fanlock == 0)
			{
				saveData.fanStatus = 1;
				controlData.fanlock = 1;
			}else if(receiveData->fanControl == 2)
			{}


			if(receiveData->pumpControl == 0){
				saveData.pumpStatus = 0;
				controlData.pumplock = 0;
				controlData.pumpcnt = 0;
			}else if(receiveData->pumpControl == 1 && controlData.pumplock == 0 && saveData.soilMoisture < 60)
			{
				saveData.pumpStatus = 1;
				controlData.pumplock = 1;
			}else if(receiveData->pumpControl == 2)
			{}


			if(receiveData->lightControl == 0){
				saveData.lightStatus = 0;
				controlData.lightlock = 0;
				controlData.lightcnt = 0;
			}else if(receiveData->lightControl == 1 && controlData.lightlock == 0)
			{
				saveData.lightStatus = 1;
				controlData.lightlock = 1;
			}else if(receiveData->lightControl == 2)
			{}

			if(receiveData->getStatus == 1){
				
				USART3_PutData(saveData, sizeof(SaveData));

				// sprintf(uartBuff, "Air temperature: %d ℃ (%.2f ℃ )\r\n", wd, temperature);
				// USART3_Puts(uartBuff);    
				// sprintf(uartBuff, "Air humidity: %d %%\r\n", sd);
				// USART3_Puts(uartBuff); 
				// sprintf(uartBuff, "Soil moisture: %.2f %%, %.2f %%\r\n", soilMoisture[0], soilMoisture[1]);
				// USART3_Puts(uartBuff);
				// sprintf(uartBuff, "Light intensity: %.2f lx\r\n", lightLux);
				// USART3_Puts(uartBuff);
				// sprintf(uartBuff, "CO2: %d ppm\r\n", co2ppm);
				// USART3_Puts(uartBuff);
				// sprintf(uartBuff, "F=%d, P=%d, L=%d", saveData.fanStatus, saveData.pumpStatus, saveData.lightStatus);
				// USART3_Puts(uartBuff);

				USART3_Puts("\r\n");
			}
			
			Uart3Front = 0;
			Uart3Rear = 0;
			Uart3Sta = 0;
		}
		if(controlData.fanlock)
		{
			controlData.fancnt++;
			if(controlData.fancnt > 30)
			{
				controlData.fancnt	= 0;
				controlData.fanlock = 0;
				saveData.fanStatus = 0;
			}
		}
		if(controlData.pumplock)
		{
			controlData.pumpcnt++;
			if(controlData.pumpcnt > 5)
			{
				controlData.pumpcnt = 0;
				controlData.pumplock = 0;
				saveData.pumpStatus = 0;
			}
		}
		if(controlData.lightlock)
		{
			controlData.lightcnt++;
			if(controlData.lightcnt > 250)
			{
				controlData.lightcnt = 0;
				controlData.lightlock = 0;
				saveData.lightStatus = 0;
			}
		}

		if(saveData.fanStatus == 1)
		{
			TIM_SetCompare2(TIM3, 2250);
		}else{
			TIM_SetCompare2(TIM3, 0);
		}
		if(saveData.pumpStatus == 1)
		{
			TIM_SetCompare4(TIM3, 2000);
		}else{
			TIM_SetCompare4(TIM3, 0);
		}

		SPI_Flash_Write((u8*)&saveData, FLASH_SIZE-100, sizeof(SaveData));

		Delay_ms(1000);
		Delay_ms(1000);
	}

// 	while(1){
// 		//Delay(1000);
// 		key = KEY_Scan(0);
// 		if(key == KEY2){
// 			printf("KEY2\r\n");
// 			SPI_Flash_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);
// 			printf("Write OK!\r\n");
// 		}
// 		if(key == KEY3){
// 			printf("KEY3\r\n");
// 			SPI_Flash_Read(datatemp,FLASH_SIZE-100,SIZE);
// 			printf("Read OK!\r\n");
// 			printf("%s\r\n", datatemp);
			
// 			p = (char*)datatemp;
// 			while(*p != '\0')
// 				printf("%c", *p++);
			
// 		}
		
// 		if(key == KEY1){
// 			printf("Erasing\r\n");
// 			//SPI_Flash_Erase_Chip();
// 			SPI_Flash_Erase_Sector((FLASH_SIZE - 100) / 4096);
// 			printf("Erase OK!\r\n");
// 		}
// //		if(key == KEY4){
// //			printf("KEY4\r\n");
// //		}
// 		i++;
// 		Delay_ms(10);
// 		if(i == 20){
// 			LED2_REV();
// 			//Delay_ms(1000);
// 			i = 0;
// 		}
// 	}
}

