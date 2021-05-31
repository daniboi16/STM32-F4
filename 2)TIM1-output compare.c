// find the blog along with hardware conections and code explanation at :https://danielalapat.hashnode.dev/stm32-f4-timers
// using timers to blink the on board LEDs

#include "stm32f4xx.h"
void configure_LED(void);
void configure_TIM(void);
int main(void)
{
	configure_LED();
	configure_TIM();
	TIM4->CR1 |= (1UL<<0); 			//to enable the timer
	while (1)
	{
	}
}
void configure_LED(void)			//to set the GPIO pins as alternate function
{
	RCC->AHB1ENR |= (1UL<<3);		//to enable clock for port d
	GPIOD-> MODER &= ~(0xFFUL<<12*2);	//to make sure pins 12,13,14,15 are "0"
	GPIOD-> MODER |= (0xAAUL<<12*2);	//to set pins 12,13,14,15 to alterate function "10"
	GPIOD->AFR[1] |= (0x2222UL<<4*4);	//to set the alterate function to correspond to the timer		
	GPIOD->OSPEEDR |= (0x55UL<<12*2);	//to set the speed of the pins 
	
}

void configure_TIM(void)
{
	// Basic configuration
	RCC->APB1ENR |= (0x1UL<<2);  		//Enable TIM4 clock
	TIM4->CR1 &= ~(0x1UL<<4);		//Set the mode to Count up    		
	TIM4->PSC = 16000-1;			//Set the Prescalar output to 1khz
	TIM4->ARR = 1000; 			//Set period to 1000
	TIM4->SR &= ~(0x1UL<<0);		//Clear Update interrupt flag
	
	// TIM Clock source config 
	TIM4->SMCR &= ~(0x7UL<<0);    		//Clock source to internal
	TIM4->SMCR &= ~(0x7UL<<4);    		//Reset register parameters
	TIM4->SMCR &= ~(0x1UL<<7); 		//DISABLE MasterSlave mode
	TIM4->CR2 &= ~(0x7UL<<4);		//TRGO RESET
	
	//Output Compare configuration
	TIM4->CCER &= ~(0x111UL<<0);		//turing off all 4 channels
	//above line is equivalent to the following 4 lines
	//TIM4->CCER &= ~(1UL);			//Disable channel 1
	//TIM4->CCER &= ~(1UL << 4);		//Disable channel 2
	//TIM4->CCER &= ~(1UL << 8);		//Disable channel 3
	//TIM4->CCER &= ~(1UL << 12);		//Disable channel 4
	
	TIM4->CCER &= ~(0x2222UL<<0); 		//setting the polarity to active high
	//above line is equivalent to the following 4 lines
	//TIM4->CCER &= ~(0x1UL<<1);   		//Output Compare polarity to active high
	//TIM4->CCER &= ~(0x1UL<<5);  		//Output Compare polarity to active high
	//TIM4->CCER &= ~(0x1UL<<9);   		//Output Compare polarity to active high
	//TIM4->CCER &= ~(0x1UL<<13);   	//Output Compare polarity to active high
	
	
	//Channel 1 config
	TIM4->CCMR1 &= ~(0x3UL<<0); 		//to configure as output
	TIM4->CCMR1 |= (0x3UL<<4);    		//Set OutputCompare mode to Toggle
	TIM4->CCER |= (0x1UL<<0);   		//Capture compare output enable
	//Set the CC register value (i.e. Pulse)
	TIM4->CCR1 = 250;
	
	//Channel 2 config
	TIM4->CCMR1 &= ~(0x3UL<<8);   		//to configure as output
	TIM4->CCMR1 |= (0x3UL<<12);    		//Set OutputCompare mode to Toggle
	TIM4->CCER |= (0x1UL<<4);   		//Capture compare output enable
	//Set the CC register value (i.e. Pulse)
	TIM4->CCR2 = 500;
	
	
	//Channel 3 config
	TIM4->CCMR2 &= ~(0x3UL<<0);   		//to configure as output
	TIM4->CCMR2 |= (0x3UL<<4);    		//Set OutputCompare mode to Toggle
	TIM4->CCER |= (0x1UL<<8);   		//Capture compare output enable
	//Set the CC register value (i.e. Pulse)
	TIM4->CCR3 = 750;
	
	//Channel 4 config
	TIM4->CCMR2 &= ~(0x3UL<<8);   //to configure as output
	TIM4->CCMR2 |= (0x3UL<<12);    //Set OutputCompare mode to Toggle
	TIM4->CCER |= (0x1UL<<12);   	//Capture compare output enable
	//Set the CC register value (i.e. Pulse)
	TIM4->CCR4 = 1000;
}
