#include "stm32f407xx.h"								// Include header file for the board

// Defining I2C addresses
#define DS1307_ADDR 0xd0

unsigned int temp=0;									// Temporary variable to clear the bit
uint8_t sec=0, min=0, hour=0, day=0, date=0, month=0, year=0;

// Pre initializing the functions
uint8_t bcd2decimal(uint8_t num);
uint8_t decimal2bcd(uint8_t num);
void DS_setval(void);
void DS_readval(void);
void configure_GPIOB(void);
void configure_I2C(void);
void msDelay(uint32_t msTime);
void I2C_start(void);
void I2C_write(uint8_t data);
void I2C_address(uint8_t Address);
void I2C_stop(void);
void I2C_read(uint8_t Address, uint8_t *buffer, uint8_t size);


int main()
{
	configure_GPIOB();								// To configure the GPIO pins PB6 and PB7
	configure_I2C();								// To configure the I2C1 register
	msDelay(100);
	//****************************************************************************************************************************************
	// This line is not require after the first time. After the time is set will remain set untill power is removed
	DS_setval();									// To set the time and date
	msDelay(100);
	while(1){
		DS_readval();								// To read the time and date
		msDelay(1000);								// Repeat every second
	}
}

uint8_t decimal2bcd(uint8_t num){							// Function to convert decimal to BCD
	return ((num/10 * 16) + (num % 10));
}

uint8_t bcd2decimal(uint8_t num){							// Function to convert BCD to decimal
	return ((num/16 * 10) + (num % 16));
}

void DS_setval(void){									// Function to set the time and date
	I2C_start();
	I2C_address(DS1307_ADDR);
	I2C_write(0x00); 								// Move pointer to 00h register
	
	//****************************************************************************************************************************************
	//****************************************************************************************************************************************
	// Change according to the time you want to set
	I2C_write(0x80); 								// Stop the clock don’t write seconds
	I2C_write(decimal2bcd(30)); 							// Setting minuite = 30
	I2C_write(decimal2bcd(11)); 							// Setting hour = 11
	I2C_write(decimal2bcd(3)); 							// Setting day of week = Tuesday
	I2C_write(decimal2bcd(4)); 							// Setting date = 4
	I2C_write(decimal2bcd(6)); 							// Setting month = 6
	I2C_write(decimal2bcd(21)); 							// Setting year = 2021
	I2C_stop();
	msDelay(1);
	I2C_start();
	I2C_address(DS1307_ADDR);
	I2C_write(0x00);								// Move pointer to 00h
	I2C_write(decimal2bcd(24)); 							// Setting seconds = 24
	I2C_stop();
}	

void DS_readval(void){									// Function to read the time and date
	uint8_t buf[7];
	I2C_start();
	I2C_address(DS1307_ADDR);
	I2C_write(0x00);
	I2C_start();
	I2C_read(DS1307_ADDR+0x01, buf, 7); 						// To read from I2C device, set LSB to 1
	I2C_stop();
	sec=bcd2decimal(buf[0] & 0x7F);
	min=bcd2decimal(buf[1]);
	hour=bcd2decimal(buf[2]);
	day=bcd2decimal(buf[3]);
	date=bcd2decimal(buf[4]);
	month=bcd2decimal(buf[5]);
	year=bcd2decimal(buf[6]);
}

void configure_GPIOB(void){								// Function to configure GPIO port B pin 6 and 7
	RCC->AHB1ENR |= (1UL<<1);							// Enable clock for port B
	GPIOB->MODER |= (2UL<<12);							// PB6 to alternate function pin
	GPIOB->MODER |= (2UL<<14);							// PB7 to alternate function pin
	GPIOB->PUPDR |= (0x5UL<<12);							// Set PB6 and 7 as pull up pins
	GPIOB->OTYPER |= (0x3UL<<6);							// Set PB6 and 7 as open drain
	GPIOB->OSPEEDR |= (0xAUL<<12);							// Set PB6 and 7 as high speed
	GPIOB->AFR[0] |= (0x44<<24);							// Set PB6 and 7 to alternate function 4
}

void configure_I2C(void){								// Function to configure I2C registers
	RCC->APB1ENR |=(1UL<<21);							// Enable I2C clock
	I2C1->CR1 |= (1UL<<15);								// Reset I2C
	I2C1->CR1 &= ~(1UL<<15);							// Set I2C
	I2C1->CR2 |=(16UL<<0);								// Set peripheral clock at 16MHz
	I2C1->OAR1 |=(1UL<<14);								// Should be set high
	I2C1->CCR |=(0x50UL<<0);							// Set SCL as 100KHz
	I2C1->TRISE |=(17UL<<0);							// Configure maximum rise time
	I2C1->CR1 |= (1UL<<0);								// Enable I2C
}

void msDelay(uint32_t msTime){								// Function for software delay
	//"For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	//16MHz/4=4MHz. If we want 1000ms (1second) delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	for (uint32_t i=0;i<msTime*4000;i++){
		__NOP();
	}
}

void I2C_start(void){									// Function to set I2C start bit
	I2C1->CR1 |= (1<<10);								// Enable the ACK Bit
	I2C1->CR1 |= (1<<8);								// Send the start bit
	while (!(I2C1->SR1 & (1<<0)));							// Wait for SB bit to set
}

void I2C_write(uint8_t data){								// Function to write data 
	while (!(I2C1->SR1 & (1<<7)));							// Wait till TX buffer is empty
	I2C1->DR = data;								// Write data to I2C slave
	while (!(I2C1->SR1 & (1<<2)));							// Wait till Byte transfer is completed
}

void I2C_address(uint8_t Address){							// Function to send I2C adress
	I2C1->DR = Address;  								//send the slave address
	while (!(I2C1->SR1 & (1<<1)));  						// wait for ADDR bit to set
	temp = I2C1->SR1 | I2C1->SR2;  							// Read SR1 and SR2 to clear the ADDR bit
}

void I2C_stop(void){									// Function to send the stop bit
	I2C1->CR1 |= (1<<9);  								// Stop I2C
}

void I2C_read (uint8_t Address, uint8_t *buffer, uint8_t size)				// Function to read from I2C device
{
	int remaining = size;
	if (size == 1)
	{
		I2C1->DR = Address; 							// Send the address
		while (!(I2C1->SR1 & (1<<1)));  					// Wait for ADDR bit to set
		I2C1->CR1 &= ~(1<<10);  						// Clear the ACK bit
		temp = I2C1->SR1 | I2C1->SR2;  						// Read SR1 and SR2 to clear the ADDR bit
		while (!(I2C1->SR1 & (1<<6)));  					// Wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  					// Read the data from the data register
	}
	else
	{
		I2C1->DR = Address;  							// Send the address
		while (!(I2C1->SR1 & (1<<1)));  					// Wait for ADDR bit to set
		temp = I2C1->SR1 | I2C1->SR2;  						// Read SR1 and SR2 to clear the ADDR bit
		while (remaining>2)
		{
			while (!(I2C1->SR1 & (1<<6)));  				// Wait for RxNE to set
			buffer[size-remaining] = I2C1->DR;  				// Copy the data into the buffer
			I2C1->CR1 |= 1<<10;  						//Set the ACK bit to Acknowledge the data received
			remaining--;
		}
		while (!(I2C1->SR1 & (1<<6)));  					// Wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;
		I2C1->CR1 &= ~(1<<10);  						// Clear the ACK bit
		I2C1->CR1 |= (1<<9);  							// Stop I2C
		remaining--;
		while (!(I2C1->SR1 & (1<<6)));  					// Wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  					// Copy the data into the buffer
	}
}
