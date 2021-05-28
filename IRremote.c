//find the hardware schematic and code explanatino here: https://danielalapat.hashnode.dev/ir-remote-3

#include "stm32f4xx.h"  									// included header file for the board

//function initializations
void configureGPIO1(void);								
void configureGPIO2(void);
void configureGPIO3(void);
void msDelay(uint32_t msTime);
void usDelay(uint32_t msTime);
void carrier(uint32_t t_micro);
void data(uint32_t code);

//hex codes for the different buttons ( make sure to use long )
unsigned long input_code=0x20DFD02F;	
unsigned long power_code=0x20DF10EF;
unsigned long mute_code=0x20DF906F;
unsigned long vol_down_code=0x20DFC03F;
unsigned long vol_up_code=0x20DF40BF;

//variable used to monitor the input buttons
unsigned int powerbut=0;			//for the first push button - power button 
unsigned int tvbut=0;					//for the second push button - HDMI3 to AV2
unsigned int hdbut=0;					//for the third push button - AV2 to HDMI3
unsigned int mute=1;					//for the rotary encoder button - mute
unsigned int currentCLK=0;		//to store the current CLK pin value
unsigned int lastCLK=0;				//to store the previous CLK pin value
unsigned int currentDT=0;			//to store the current DT pin value
unsigned int vol=0;						//to keep track of the volume
	
int main(void)
{
	configureGPIO1();						//to configure the IR LED on port A pin 0
	configureGPIO2();						//to configure the 3 push butoons on port D pin 0,1,2
	configureGPIO3();						//to configure the rotary encoder pin on port B pin 0,1,2
	lastCLK = ((GPIOD->IDR) & (0x1UL<<2));		// to read the value of the CLK pin
	msDelay(500);

	
	while (1)
	{
		vol=0;																		//initialize the volume is 0
		powerbut = ((GPIOB->IDR) & (0x1UL<<0));		//to read the first push button	( port B pin 0 )
		tvbut = ((GPIOB->IDR) & (0x1UL<<1));			//to read the second push button ( port B pin 1 )
		hdbut = ((GPIOB->IDR) & (0x1UL<<2));			//to read the third push button ( port B pin 2 )
		mute =	((GPIOD->IDR) & (0x1UL<<0)); 			//to read the rotary encoder button ( port D pin 0 )
		
		//code for rotary encoder
		currentCLK = ((GPIOD->IDR) & (0x1UL<<2));	//reading the current value of the CLK pin
		if (currentCLK != lastCLK){								//seeing if there is a change in the current and previous value, i.e if the knob is moved
			currentDT = ((GPIOD->IDR) & (0x1UL<<1));//reading the current value of the DT pin
			if (currentDT != currentCLK) {					//if it is rotated clockwise
				vol=vol+1;}														//increment the volume
			else{																		//if it is rotated anticlockwise
				vol=vol-1;}}													//decrement the volume
		currentCLK=lastCLK;												//update the CLK pin value
		
		if(!mute){																//if the mute button is pressed ( it is normally high )
			data(mute_code);												//send the code for the mute button
			mute=1;																	//reset the value
			msDelay(100);
		}
		
		if (powerbut){														//if the first button is pressed 	
			data(power_code);												//send the code for the power button
			powerbut=0;															//reset the value
			msDelay(100);
		}
		
		if (tvbut){																//if the second button is pressed
			for (unsigned int i=0;i<7;i++){					//send the code 6 times
				data(input_code);											//send the code for the input button
				msDelay(1000);												//wait 1 second between presses 
			}
		tvbut=0;																	//reset the value
		}
		
		if (hdbut){																//if the third button is pressed
			for (unsigned int i=0;i<5;i++){					//send the code 4 time
				data(input_code);											//send the code for the input button
				msDelay(1000);												//wait 1 second between presses
			}
		hdbut=0;																	//reset the value
		}
		
		if (vol!=0){															//if the volume has been changed
			if (vol>0){															//if volume increases
				for (unsigned int j=0;j<(vol+1);j++){	//send the code as many times as pressed
					data(vol_up_code);									//sednt the code for volume up
					msDelay(1000);}											//wait 1 second between presses	
				}
			else{																		//if volume decreases
				for (unsigned int j=0;j>(vol-1);j--){	//send the code as many times as pressed
					data(vol_down_code);								//send the code for volume down
					msDelay(1000);}											//wait 1 second between presses
				}
		}
	}
}

void configureGPIO1(void)						//configure ir led as output
{
	RCC->AHB1ENR  |= (0x1UL<<0);			//port A clock enabled
	GPIOA->MODER  |= (0x1UL<<0);			//setting bit 0 and 1 to "01" coreesponding to output
}

void configureGPIO2(void)						//configure the 3 push buttons as input
{
	RCC->AHB1ENR  |= (0x1UL<<3);			//port D clock enabled
}

void configureGPIO3(void)						//configure the rotary encoder pins as input
{
	RCC->AHB1ENR  |= (0x1UL<<1);			//port B clock enabled
}

void usDelay(uint32_t msTime)				//delay micro second
{
	//"For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	//16MHz/4=4MHz. If we want 1000000us (1second) delay, 4MHz/1000000=4, so we have to multiply by 4 to get a delay of 1s
	for (uint32_t i=0;i<msTime*4;i++)	
	{
		__NOP();
	}
}

void msDelay(uint32_t msTime)				//delay milli seconds
{
	//"For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	//16MHz/4=4MHz. If we want 1000ms (1second) delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	for (uint32_t i=0;i<msTime*4000;i++)
	{
		__NOP();
	}
}

void carrier(uint32_t t_micro)		//function to add the carrier wave
{
	for (unsigned int i=0; i<(t_micro/26); i++)
	{
		GPIOA->ODR |= (0x1UL<<0);			//turn IR LED on
		usDelay(13);
		GPIOA->ODR &= ~(0x1UL<<0);		//turn IR LED off
		usDelay(13);
	}
}

void data(uint32_t code)					//function to send the IR hex code
{
	carrier(9000);									//NEC protocol first send the carrier frequency for 9us
	usDelay(4500);									//NEC protocol then requires a 4.5ms delay
	unsigned long c=code;						//storing the value of the IR code
	unsigned int time_period=562;		//setting the time period
	for (unsigned int j=0;j<32;j++)	//itterating through each bit of the code
	{
		carrier(time_period);
		if (c & 0x80000000)						//mask to select the MSB
		{
			usDelay((3*time_period));		//if the bit is "1" we need to send it for 3* timeperiod
		}
		else{													//if the bit is "0" we need to send it for the timeperiod
			usDelay(time_period);
		}
		c<<=1;												//to interate thorugh each bit
	}
	carrier(time_period);						//to send the stop bit
}
