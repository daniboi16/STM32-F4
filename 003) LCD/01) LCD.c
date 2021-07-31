#include "stm32f4xx.h"						// Include header file for the board

// Pre initializing the functions
void configureGPIO(void);
void msDelay(uint32_t msTime);
void command(void);
void data(void);

int main(void)
{
	configureGPIO();
	msDelay(500);
	
	GPIOD->ODR &= ~(0xFFUL<<0); 				// To make bits 0-7 = 0
	GPIOD->ODR |= (0x38UL<<0); 				// Command to set 2x16 mode
	command();
	
	GPIOD->ODR &= ~(0xFFUL<<0); 				// To make bits 0-7 = 0
	GPIOD->ODR |= (0xFUL<<0); 				// Command to set screen on and cursor blinking
	command();
	
	GPIOD->ODR &= ~(0xFFUL<<0); 				// To make bits 0-7 = 0
	GPIOD->ODR |= (0x6UL<<0); 				// Command to increment cursor
	command();
	
	GPIOD->ODR &= ~(0xFFUL<<0); 				// To make bits 0-7 = 0
	GPIOD->ODR |= (0x1UL<<0); 				// Command to clear screen
	command();
	
	GPIOD->ODR &= ~(0xFFUL<<0); 				// To make bits 0-7 = 0
	GPIOD->ODR |= (0x80UL<<0); 				// Command to force cursor to start of first line
	command();
	
	GPIOD->ODR &= ~(0xFFUL<<0);				// To make bits 0-7 = 0"
	GPIOD->ODR |= (0x43UL<<0); 				// To print "C"
	data();
	
	GPIOD->ODR &= ~(0xFFUL<<0); 				// To make bits 0-7 = 0
	GPIOD->ODR |= (0x41UL<<0); 				// To print "A"
	data();
	
	GPIOD->ODR &= ~(0xFFUL<<0);				// To make bits 0-7 = 0
	GPIOD->ODR |= (0x52UL<<0); 				// To print "R"
	data();
		
	while (1)
	{}
}

void configureGPIO(void)					// Function to configure GPIO pins
{
	RCC->AHB1ENR  |= (1UL<<3); 				// Enable clock for port d
	GPIOD->MODER &= ~(0x3FFFFFUL<<0); 			// Making sure pins 0-10 = 0 
	GPIOD->MODER |= (0x155555UL<<0); 			// Making pins 0-10 = 01 (output mode)
}

void msDelay(uint32_t msTime)					//Function for delay
{
	for (uint32_t i=0;i<msTime*4000;i++)
	{
		__NOP();
	}
}

void command(void)						// Function to send commands
{
	GPIOD->ODR &= ~(0x7UL<<8); 				// To make pin 8,9,10 = 000
	GPIOD->ODR |= (0x4UL<<8); 				// To make pin 8,9,10 = 001
	msDelay(1);
	GPIOD->ODR &= ~(0X7UL<<8); 				// To make pin 8,9,10 = 000
	msDelay(1);
}

void data(void)							// Function to send data
{
	GPIOD->ODR &= ~(0X7UL<<8); 				// To make pin 8,9,10 = 000
	GPIOD->ODR |= (0x5UL<<8);  				// To make pin 8,9,10 = 101
	msDelay(1);
	GPIOD->ODR &= ~(0x6UL<<8); 				// To make pin 8,9,0 = 100
	msDelay(1);
}
