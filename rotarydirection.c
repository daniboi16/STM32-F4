// hardware connections and code explanation available at : https://danielalapat.hashnode.dev/rotary-encoder 
// to turn the green led on if the encoder is rotated clockwise and the red led if rotated anticlockwise
// CLK pin connected to port E pin 2 and DT to port E pin 1
#include "stm32f4xx.h"
void configureLED(void);
void configurePB(void);
void msDelay(uint32_t msTime);
int main(void)
{
	configureLED();
	configurePB();
	unsigned int currentCLK=0;                            //to store the current state of the CLK pin
	unsigned int lastCLK=0;                               //to store the previous state of the CLK pin
	unsigned int currentDT=0;                             //to store the current state of the DT pin  
	lastCLK = ((GPIOE->IDR) & (0x1UL<<2));                //to read the value of the CLK pin 
	msDelay(500);
	while(1)
	{
		currentCLK = ((GPIOE->IDR) & (0x1UL<<2));           //to read the value of the CLK pin
		if (currentCLK != lastCLK){                         //if there is a change in state
			currentDT = ((GPIOE->IDR) & (0x1UL<<1));          //to read the value of the DT pin
			if (currentDT != currentCLK) {                    //if it is rotated clockwise
				GPIOD->ODR = (0xCUL<<12);	                      //turn on Green LED
				msDelay(100);}
			
			else {                                            //if it is rotated anti clockwise
				GPIOD->ODR = (0x3UL<<12);	                      //turn of red LED
			msDelay(100);}
		}
		currentCLK=lastCLK;                                 //update the value of the CLK pin
	}
}
void configureLED(void)               //to configure the LED pins as output
{
	RCC->AHB1ENR |=(1UL<<3);                              //to enable the clock for port D
	GPIOD->MODER &= ~(0xFFUL<<12*2);                      //to make sure pins 12,13,14,15  are "0"
	GPIOD->MODER |= (0x55UL<<12*2);                       //to set pins 12,13,14,15 to output "01" 
}

void msDelay(uint32_t msTime)       //to set up the software delay
{
	//"For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	//16MHz/4=4MHz. If we want 1000ms delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	for(uint32_t i=0;i<msTime*4000;i++);
}
