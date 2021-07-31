#include "stm32f4xx.h"
void configureLED(void);
void configurePB(void);
void msDelay(uint32_t msTime);
int main(void)
{
	configureLED();
	configurePB();
	
	lastCLK = ((GPIOE->IDR) & (0x1UL<<2));
	


	unsigned int currentCLK=0;                            //to store the current state of the CLK pin
	unsigned int lastCLK=0;                               //to store the previous state of the CLK pin
	unsigned int currentDT=0;                             //to store the current state of the DT pin  
	unsigned int state=50;				      //to store state which will be incremented and decremented	
	unsigned int butt=1;				      //to store button state
	lastCLK = ((GPIOE->IDR) & (0x1UL<<2));                //to read the value of the CLK pin 
	msDelay(100);
	while(1)
	{
		currentCLK = ((GPIOE->IDR) & (0x1UL<<2));           //to read the value of the CLK pin
		if (currentCLK != lastCLK){                         //if there is a change in state
			currentDT = ((GPIOE->IDR) & (0x1UL<<1));    //to read the value of the DT pin
			if (currentDT != currentCLK) {              //if it is rotated clockwise
				GPIOD->ODR = (0xCUL<<12);	    //turn on Green LED
				msDelay(100);
				state=state+1;			    //increment the state	
			}
			else {					    //if it is rotated anti clockwise
				GPIOD->ODR = (0x4UL<<12);	    //turn of red LED
				msDelay(100);
				state=state-1;			    //decrement the state
			}
		}
		currentCLK=lastCLK;				    //update the value of the CLK pin
		butt=((GPIOE->IDR) & (0x1UL<<0));		    //to read the value of the button
		if (butt !=0x1UL){				    //if button is pressed
			
			GPIOD->ODR = (0xFUL<<12);		    //turn on all 4 LED
			msDelay(100);
		}
		if(state==50){					    //if button is not pressed
			GPIOD->ODR = (0xAUL<<12);		    //turn off all 4 leds
			msDelay(100);}	
	}
}
	
void configureLED(void)               //to configure the LED pins as output
{
	RCC->AHB1ENR |=(1UL<<3);                              //to enable the clock for port D
	GPIOD->MODER &= ~(0xFFUL<<12*2);                      //to make sure pins 12,13,14,15  are "0"
	GPIOD->MODER |= (0x55UL<<12*2);                       //to set pins 12,13,14,15 to output "01" 
}

void configurePB(void)		     //to configure the GPIO pins as input
{
	RCC->AHB1ENR |=(0x1UL<<4);			     //to enable the clock for port E
}	

void msDelay(uint32_t msTime)       //to set up the software delay
{
	//"For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	//16MHz/4=4MHz. If we want 1000ms delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	for(uint32_t i=0;i<msTime*4000;i++)
	{
		__NOP();
	}
}
