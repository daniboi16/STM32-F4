#include "stm32f407xx.h"								// Include header file for the board

// Pre initializing the functions
void configureLED(void);
void configurePB(void);
void msDelay(uint32_t msTime);

int main(void)
{
	configureLED();
	configurePB();
	unsigned int currentCLK=0;							// To store the current state of the CLK pin
	unsigned int lastCLK=0;								// To store the previous state of the CLK pin
	unsigned int currentDT=0;							// To store the current state of the DT pin
	lastCLK = ((GPIOE->IDR) & (0x1UL<<2));						// Reading the initial state of the CLK pin 
	msDelay(500);
	while(1)
	{
		currentCLK = ((GPIOE->IDR) & (0x1UL<<2));				// Reading the current state of CLK pin
		if (currentCLK != lastCLK){						// If there is a chnage in the rotary posission
			currentDT = ((GPIOE->IDR) & (0x1UL<<1));			// Reading the cuurent state of the DT pin
			if (currentDT != currentCLK) {					// If rotation is clockwise
				GPIOD->ODR = (0xCUL<<12);	
				msDelay(100);}
			
			else {								// If rotation is anticlockwise
				GPIOD->ODR = (0x3UL<<12);	
			msDelay(100);}
		}
		currentCLK=lastCLK;							// Updating the CLK pin
	}
}
void configureLED(void)									// Function to set the LED as output
{
	RCC->AHB1ENR |=(1UL<<3);							// Enable clock for port D
	GPIOD->MODER &= ~(0xFFUL<<12*2);
	GPIOD->MODER |= (0x55UL<<12*2);							// Configure as output
}

void configurePB(void)									// Function to set the CLK, DT pins as input
{
	RCC->AHB1ENR |=(0x1UL<<4);							// Enable clock for port E
}

void msDelay(uint32_t msTime)								// Function for software delay
{
	//"For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	//16MHz/4=4MHz. If we want 1000ms delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	for(uint32_t i=0;i<msTime*4000;i++){
		__NOP();
	}
}
