#include "delay.h"
#ifdef DELAY_H

static __IO u32 TimingDelay;

void SysTick_Init(void)
{
	 // SystemFrequency / 1000     1ms
	 // SystemFrequency / 100000	 10us
	 // SystemFrequency / 1000000  1us
	 
	if (SysTick_Config(SystemCoreClock / 1000000))	// 1us
	{ 
		/* Capture error */ 
		while (1);
	}
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

#endif
