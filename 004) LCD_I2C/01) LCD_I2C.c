#include "stm32f4xx.h"								// Include header file for the board

unsigned int temp;								// Temporary variable to clear the bit
uint8_t d=0;									// Initializing variable d

// Pre initializing the functions
void configure_GPIOB(void);
void msDelay(uint32_t msTime);
void configure_I2C(void);
void I2C_start(void);
void I2C_send(uint8_t data);
void I2C_address(void);
void I2C_stop(void);
void I2C_write4(uint8_t data);
void I2C_write8(uint8_t data);
void I2C_write4C(uint8_t data);
void I2C_write8C(uint8_t data);


int main ()
{
	configure_GPIOB();							// To configure the GPIO pins PB6 and PB7
	configure_I2C();							// To configure the I2C1 register
	msDelay(100);

	// Initial commands to set up the LCD in 4 bit mode
	I2C_write4(0x30);							//to clear the screen
	msDelay(4200);
	I2C_write4(0x30);							//to clear the screen
	msDelay(150);
	I2C_write4(0x30);							//to clear the screen
	msDelay(37);
	I2C_write4(0x20);							//to turn on the screen
	msDelay(37);

	// Set LCD to 4 bit mode, 2 lines, 5x8 font
	I2C_write4(0x28);							//upper nibble 								
	msDelay(37);	
	I2C_write8(0x28);							//lower nibble
	msDelay(37);


	// Turn the display on with blinking cursor
	I2C_write4(0x0C);							//upper nibble
	msDelay(37);  
	I2C_write8(0x0C);							//lower nibble
	msDelay(37); 

	// Clear the contents on the screen
	I2C_write4(0x01);							//upper nibble
	msDelay(37);  
	I2C_write8(0x01);							//lower nibble
	msDelay(1600);

	// Set LCD to write from left to right
	I2C_write4(0x06);							//upper nibble
	msDelay(37);  
	I2C_write8(0x06);							//lower nibble
	msDelay(37);

	// Display the character "C"
	I2C_write4C(0x43);							//upper nibble
	msDelay(37);
	I2C_write8C(0x43);							//lower nibble
	msDelay(41);

	// Display the character "A"
	I2C_write4C(0x41);							//upper nibble
	msDelay(37);
	I2C_write8C(0x41);							//lower nibble
	msDelay(41);

	// Display the character "R"
	I2C_write4C(0x52);							//upper nibble
	msDelay(37);
	I2C_write8C(0x52);							//lower nibble
	msDelay(41);

	while (1)
		{}
	}

void configure_GPIOB(void){							//to configure the GPIO pins PB6 and PB7
	//SDA - PB7, SCL - PB6
	RCC -> AHB1ENR |=(1UL<<1);						//Enable clock for port B
	GPIOB -> MODER |=(2UL<<(6*2));						//PB6 to alternate function pin "10"
	GPIOB -> MODER |=(2UL<<(7*2));						//PB7 to alternate function pin "10"
	GPIOB -> PUPDR |=(0x5UL<<(6*2));					//set PB6 and PB7 as pull up pins "01"
	GPIOB -> OTYPER |=(0x3UL<<6);						//Set PB6 and PB7 as open drain "1"
	GPIOB -> OSPEEDR |=(0xAUL<<(6*2));					//Set PB6 and PB7 as high speed "10"
	GPIOB -> AFR[0] |= (0x44<<(6*4));					//Set PB6 and PB7 to alternate function 4 "0100"
}

void msDelay(uint32_t msTime){							//function for software delay
	// "For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	// 16MHz/4=4MHz. If we want 1000ms (1second) delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	for (uint32_t i=0;i<msTime*4000;i++){
		__NOP();
	}
}
void configure_I2C(void){							// To configure the I2C registers
	RCC -> APB1ENR |=(1UL<<21);						// Enable I2C clock
	I2C1 -> CR1 |= (1UL<<15);						// Reset I2C
	I2C1 -> CR1 &= ~(1UL<<15);						// set I2C
	I2C1 -> CR2 |=(16UL<<0);						// Set peripheral clock at 16MHz
	I2C1 -> OAR1 |=(1UL<<14);						// Should be set high
	I2C1 -> CCR |=(0x50UL<<0);						// Set SCL as 100KHz
	I2C1 -> TRISE |=(17UL<<0);						// Configure maximum rise time
	I2C1 -> CR1 |= (1UL<<0);						// Enable I2C
}
void I2C_start(void){								// To send the I2C start bit
	I2C1 -> CR1 |= (1<<10);							// Enable the ACK Bit
	I2C1 -> CR1 |= (1<<8);							// Send the start bit
	while (!(I2C1->SR1 & (1<<0)));						// Wait for SB bit to set
}
void I2C_address(void){								// To send the I2C address
	I2C1 -> DR = 78;  							// Send the slave address      // change according to the device
	while (!(I2C1->SR1 & (1<<1))); 						// Wait for ADDR bit to set
	temp = ((I2C1->SR1) | (I2C1->SR2));					// Read SR1 and SR2 to clear the ADDR bit
}
void I2C_send(uint8_t data){							// To send data to the I2C device
	while (!(I2C1->SR1 & (1<<7)));						// Wait till TX buffer is empty
	I2C1 -> DR = data;							// Write data to I2C slave
	while (!(I2C1->SR1 & (1<<2)));						// Wait till Byte transfer is completed
}
void I2C_stop(void){								// To send the I2C stop bit
	I2C1 -> CR1 |= (1<<9);  						// Stop I2C
}
void I2C_write4(uint8_t data){							// To send upper four bit for commands
	d=0;									// Reset value for d
	d |= (data & 0xF0); 							// To select only the upper nibble
  	d |= 1<<2;								// To set the enable bit
	d |= 1<<3;								// To set the backlight
	I2C_start();								// Send the start bit
	I2C_address();								// Send the I2C address
	I2C_send(d);								// Send the data
	msDelay(1);								// Wait for 1ms 
	d &= ~(1<<2);								// Set the enable bit low
	I2C_send(d);								// Send the same data but without enable bit to latch the data
	I2C_stop();								// Send the stop bit
}

void I2C_write8(uint8_t data){							// To send lower four bit for commands
	d=0;
	d |= ((data & 0x0F)<<4); 						// To select only the lower nibble
	d |= 1<<2;
	d |= 1<<3;
	I2C_start();
	I2C_address();
	I2C_send(d);
	msDelay(1);
	d &= ~(1<<2);
	I2C_send(d);
	I2C_stop();
}

void I2C_write4C(uint8_t data){							// To send upper four bits for data
	d=0;
  	d |= (data & 0xF0); 							// To select only the upper nibble
  	d |= 1<<2;
	d |= 1<<0;								// To select the data register not the command
	d |= 1<<3;
	I2C_start();
	I2C_address();
	I2C_send(d);
	msDelay(1);
	d &= ~(1<<2);
	I2C_send(d);
	I2C_stop();
}

void I2C_write8C(uint8_t data){							// To send the lower four bits for data
	d=0;
 	d |= ((data & 0x0F)<<4); 						// To select only the lower nibble
  	d |= 1<<2;
	d |= 1<<0;								// To select data register not command register
	d |= 1<<3;
	I2C_start();
	I2C_address();
	I2C_send(d);
	msDelay(1);
	d &= ~(1<<2);
	I2C_send(d);
	I2C_stop();
}
