#ifndef CO2_H
#define CO2_H

#include "stm32f10x.h"
#include "uart.h"
#include "string.h"

#define COMMAND_DATA_SUM   9
#define MCOMMAND_DATA_SUM  9
#define ZCOMMAND_DATA_SUM  7

void Co2_Init(void);
u16 Co2_Read(void);

#endif
