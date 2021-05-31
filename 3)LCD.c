// find the blog along with hardware conections and code explanation at : https://danielalapat.hashnode.dev/stm32-f4-interfacing-lcd
// parallel interfacing of LCD
#include "stm32f4xx.h"
void configureGPIO(void);
void msDelay(uint32_t msTime);
void command(void);
void data(void);

int main(void)
{
	configureGPIO();  
	msDelay(500);
	
	GPIOD->ODR &= ~(0xFFUL<<0); //to make bit 0 to 7 "0"
	GPIOD->ODR |= (0x38UL<<0); //to set 2x16
	command();
	
	GPIOD->ODR &= ~(0xFFUL<<0); //to make bit 0 to 7 "0"
	GPIOD->ODR |= (0xFUL<<0); //to set screen on cursor blinking
	command();
	
	GPIOD->ODR &= ~(0xFFUL<<0); //to make bit 0 to 7 "0"
	GPIOD->ODR |= (0x6UL<<0); //to increment cursor
	command();
	
	GPIOD->ODR &= ~(0xFFUL<<0); //to make bit 0 to 7 "0"
	GPIOD->ODR |= (0x1UL<<0); //to clear screen
	command();
	
	GPIOD->ODR &= ~(0xFFUL<<0); //to make bit 0 to 7 "0"
	GPIOD->ODR |= (0x80UL<<0); //to force cursor to biging of first line
	command();
	
	GPIOD->ODR &= ~(0xFFUL<<0); //to make bit 0 to 7 "0"
	GPIOD->ODR |= (0x43UL<<0); //to print "C"
	data();
	
	GPIOD->ODR &= ~(0xFFUL<<0); //to make bit 0 to 7 "0"
	GPIOD->ODR |= (0x41UL<<0); //to print "A"
	data();
	
	GPIOD->ODR &= ~(0xFFUL<<0); //to make bit 0 to 7 "0"
	GPIOD->ODR |= (0x52UL<<0); //to print "R"
	data();
		
	while (1)
	{
	}
}

void configureGPIO(void)		//to set GPIO pins as output
{
	RCC->AHB1ENR  |= (1UL<<3);		 //clock for port d
	GPIOD->MODER &= ~(0x3FFFFFUL<<0); 	//making sure pins 0 to 10 are "0" 
	GPIOD->MODER |= (0x155555UL<<0); 	//making pins 0 to 10 to "01" which is output
}

void msDelay(uint32_t msTime)		//to impliment software delay
{
	//"For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	//16MHz/4=4MHz. If we want 1000ms delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	for (uint32_t i=0;i<msTime*4000;i++)
	{
		__NOP();
	}
}

void command(void)			//handshake to send commands to LCD
{
	GPIOD->ODR &= ~(0x7UL<<8); //to make pin 8,9,10 to "000"
	GPIOD->ODR |= (0x4UL<<8); //to make pin 8,9,10 to "001"
	msDelay(1);
	GPIOD->ODR &= ~(0X7UL<<8); //to make pin 8,9,10 to "000"
	msDelay(1);
}

void data(void)				//handshake to send data to LCD
{
	GPIOD->ODR &= ~(0X7UL<<8); //to make pin 8,9,10 to "000"
	GPIOD->ODR |= (0x5UL<<8);  //to make pin 8,9,10 to "101"
	msDelay(1);
	GPIOD->ODR &= ~(0x6UL<<8); //to make pin 8,9,0 to "100"
	msDelay(1);
}
