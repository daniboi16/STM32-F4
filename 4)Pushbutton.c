// hardware connections and code explanation can be found at : https://danielalapat.hashnode.dev/rotary-encoder
// to impliment the push button of the rotary encoder
// SW pin connected to port E pin 0 

#include "stm32f4xx.h"
void configureLED(void);
void configurePB(void);
void msDelay(uint32_t msTime);
int main(void)
{
	configureLED();
	configurePB();
	unsigned int PB_state=0;                		//variable to store the state of the button
	msDelay(500);
	while(1)
	{
		//push button is low enabled
		PB_state=((GPIOE->IDR) & (0x1UL<<0));		//to read the value of the 0 bit of the input register
		if(PB_state==(0x1UL<<0))			//if the push button is not pressed
		{
			GPIOD->ODR = 0x0UL;			//turn off LED
		}
		else						//if push button is pressed
		{
			GPIOD->ODR = (0xFUL<<12);		//turn on the LED for 100ms
			msDelay(100);
		}
	}
}
void configureLED(void)				//to configure the LED pins as output
{
	RCC->AHB1ENR |=(1UL<<3);				//to enable the clock for port E
	GPIOD->MODER &= ~(0xFFUL<<12*2);			//to make sure all the pins 12,13,14,15 are "0"
	GPIOD->MODER |= (0x55UL<<12*2);				//to set pins 12,13,14,15 to output "01"
}

void configurePB(void)				//to configure the button as input
{
	RCC->AHB1ENR |=(0x1UL<<4);				//to enable the clock for port E
}

void msDelay(uint32_t msTime)			//to set up software delay
{
	//"For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	//16MHz/4=4MHz. If we want 1000ms delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	for(uint32_t i=0;i<(msTime*4000);i++);
}
