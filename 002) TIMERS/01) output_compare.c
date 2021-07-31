#include "stm32f407xx.h"			// Include header file for the board

// Pre defining functions
void configure_LED(void);
void configure_TIM(void);

int main(void)
{
	configure_LED();
	configure_TIM();
	TIM4 -> CR1 |= (1UL<<0);
	while (1)
	{}
}
void configure_LED(void)
{
	RCC -> AHB1ENR |= (1UL<<3);		// Enable clock for port D
	GPIOD -> MODER &= ~(0xFFUL<<12*2);	// Set all bits = 0
	GPIOD -> MODER |= (0xAAUL<<12*2);	// Set pins to output
	GPIOD -> AFR[1] |= (0x2222UL<<4*4);	// Select alternate function
	GPIOD -> OSPEEDR |= (0x55UL<<12*2);	// Select output speed
}

void configure_TIM(void)
{
	// Timer configuration
	RCC -> APB1ENR |= (0x1UL<<2);  	    // Enable TIM4 clock
	TIM4 -> CR1 &= ~(0x1UL<<4);	    // Set  mode = Count up
	TIM4 -> PSC = 16000-1;		    // Set the prescalar  = 1khz
	TIM4 -> ARR = 1000; 		    // Set period = 1000
	TIM4 -> SR &= ~(0x1UL<<0);	    // Clear interrupt flag

	// TIM Clock 
	TIM4 -> SMCR &= ~(0x7UL<<0);        // Clock source = internal
	TIM4 -> SMCR &= ~(0x7UL<<4);        // Reset register parameters
	TIM4 -> SMCR &= ~(0x1UL<<7); 	    // Disable MasterSlave mode
	TIM4 -> CR2 &= ~(0x7UL<<4);	    // TRGO RESET

	// Output Compare
	TIM4 -> CCER &= ~(0x111UL<<0);	    // Turing off all 4 channels
	TIM4 -> CCER &= ~(0x2222UL<<0);     // Setting the polarity

	// Channel 1
	TIM4 -> CCMR1 &= ~(0x3UL<<0); 	    // To configure as output
	TIM4 -> CCMR1 |= (0x3UL<<4);        // Set OutputCompare mode = Toggle
	TIM4 -> CCER |= (0x1UL<<0);   	    // Capture compare output enable
	// Set the Pulse
	TIM4 -> CCR1 = 250;

	//Channel 2
	TIM4 -> CCMR1 &= ~(0x3UL<<8);       // To configure as output
	TIM4 -> CCMR1 |= (0x3UL<<12);       // Set OutputCompare mode = Toggle
	TIM4 -> CCER |= (0x1UL<<4);   	    // Capture compare output enable
	//Set the Pulse
	TIM4 -> CCR2 = 500;

	//Channel 3
	TIM4 -> CCMR2 &= ~(0x3UL<<0);       //to configure as output
	TIM4 -> CCMR2 |= (0x3UL<<4);        //Set OutputCompare mode = Toggle
	TIM4 -> CCER |= (0x1UL<<8);   	    //Capture compare output enable
	//Set the Pulse
	TIM4 -> CCR3 = 750;

	//Channel 4
	TIM4 -> CCMR2 &= ~(0x3UL<<8);       //to configure as output
	TIM4 -> CCMR2 |= (0x3UL<<12);       //Set OutputCompare mode = Toggle
	TIM4 -> CCER |= (0x1UL<<12);   	    //Capture compare output enable
	//Set the Pulse
	TIM4 -> CCR4 = 1000;
}
