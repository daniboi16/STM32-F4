//find the blog with hardware and code explanation at : https://danielalapat.hashnode.dev/stm32-f4-led-blink-and-setup
#include "stm32f4xx.h"
void configureLED(void);
void msDelay(uint32_t msTime);
int main(void)
{
	configureLED();
	msDelay(500);
	while (1)
	{
		GPIOD->ODR ^=(0xAUL<<12);
		msDelay(100);
		GPIOD->ODR ^=(0x5UL<<12);
		msDelay(100);
	}
}

void configureLED(void)
{
	RCC->AHB1ENR  |= (1UL<<3);
	GPIOD->MODER &= ~(0xFFUL<<12*2);
	GPIOD->MODER |= (0x55UL<<12*2);
}

void msDelay(uint32_t msTime)
{
	for (uint32_t i=0;i<msTime*4000;i++)
	{
		__NOP();
	}
}
