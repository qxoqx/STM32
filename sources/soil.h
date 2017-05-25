#ifndef SOIL_H
#define SOIL_H

#include "stm32f10x.h"
#include "adc.h"

void Soil_Moisture_Init(void);
//float Soil_Moisture_Read(void);
void Soil_Moisture_Read(float *);

#endif
