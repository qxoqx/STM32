#include "soil.h"
#ifdef SOIL_H

void Soil_Moisture_Init(void)
{
	ADC1_Init();
}

// float Soil_Moisture_Read(void)
// {
// 	return (float)((4096 - ADC_ConvertedValue)* 100 / 4096);  //读取湿度
// }

void Soil_Moisture_Read(float *soilBuff)
{
	soilBuff[0] = ((4096 - ADC_ConvertedValue[0])* 100 / 4096);  //读取湿度
	soilBuff[1] = ((4096 - ADC_ConvertedValue[1])* 100 / 4096);  //读取湿度
	soilBuff[2] = ((4096 - ADC_ConvertedValue[2])* 100 / 4096);  //读取湿度
}

#endif
