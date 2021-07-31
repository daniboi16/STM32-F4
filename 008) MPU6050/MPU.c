#include "stm32f407xx.h"								                  // Include header file for the board

// Defining I2C addresses
#define MPU6050_ADDR 0xD0									
#define SMPLRT_DIV_REG 0x19							
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

// Pre initializing the functions
void configure_MPU (void);
void read_ACCEL(void);
void MPU_Write(uint8_t address, uint8_t reg, uint8_t data);
void MPU_Read(uint8_t address, uint8_t reg, uint8_t *buffer, uint8_t size);
void configure_GPIOB(void);
void configure_I2C(void);
void I2C_start(void);
void I2C_write(uint8_t data);
void I2C_address(uint8_t Address);
void I2C_stop(void);
void I2C_read(uint8_t Address, uint8_t *buffer, uint8_t size);


// Variables for accelerometer  
int16_t x_read=0,y_read=0,z_read=0;
float x_accel,y_accel,z_accel;
uint8_t check,temp;

int main ()
{
	configure_GPIOB();
	configure_I2C();
	configure_MPU();
	while (1)
	{
		read_ACCEL ();
	}
}

void configure_MPU (void){														                                       // Function of configure the MPU
	uint8_t check;
	uint8_t data;
	MPU_Read(MPU6050_ADDR,WHO_AM_I_REG, &check, 1);			                                // Check device ID

	if (check == 104)                                                                          	// 0x68 will be returned by the sensor
	{
		data = 0;
		MPU_Write (MPU6050_ADDR, PWR_MGMT_1_REG, data);                                         // Power up the sensor
		data = 0x07;
		MPU_Write(MPU6050_ADDR, SMPLRT_DIV_REG, data);                                          // Sampling rate of 1KHz
		data = 0x00;
		MPU_Write(MPU6050_ADDR, ACCEL_CONFIG_REG, data);                                        //accelerometer range=+/- 2g
	}
}

void read_ACCEL(void){															                                        // Function to read the acceleration
	uint8_t Buf[6];
	// Read 6 BYTES of data starting from ACCEL_XOUT_H register
	MPU_Read(MPU6050_ADDR, ACCEL_XOUT_H_REG, Buf, 6);
	x_read = (int16_t)(Buf[0] << 8 | Buf[1]);
	y_read = (int16_t)(Buf[2] << 8 | Buf[3]);
	z_read = (int16_t)(Buf[4] << 8 | Buf[5]);
	x_accel = x_read/16384.0;
	y_accel = y_read/16384.0;
	z_accel = z_read/16384.0;
}

void MPU_Write(uint8_t address, uint8_t reg, uint8_t data){                                		// Function to write to the MPU
	I2C_start();
	I2C_address(address);
	I2C_write(reg);
	I2C_write(data);
	I2C_stop ();
}

void MPU_Read(uint8_t address, uint8_t reg, uint8_t *buffer, uint8_t size){			        // Function to read from the MPU
	I2C_start();
	I2C_address(address);
	I2C_write(reg);
	I2C_start();
	I2C_read(address+0x01, buffer, size);
	I2C_stop();
}

void configure_GPIOB(void){								                    // Function to configure GPIO port B pin 6 and 7
	RCC->AHB1ENR |= (1UL<<1);							                    // Enable clock for port B
	GPIOB->MODER |= (2UL<<12);							                    // PB6 to alternate function pin
	GPIOB->MODER |= (2UL<<14);							                    // PB7 to alternate function pin
	GPIOB->PUPDR |= (0x5UL<<12);							                    // Set PB6 and 7 as pull up pins
	GPIOB->OTYPER |= (0x3UL<<6);							                    // Set PB6 and 7 as open drain
	GPIOB->OSPEEDR |= (0xAUL<<12);							                    // Set PB6 and 7 as high speed
	GPIOB->AFR[0] |= (0x44<<24);							                    // Set PB6 and 7 to alternate function 4
}

void configure_I2C(void){								                    // Function to configure I2C registers
	RCC->APB1ENR |=(1UL<<21);							                    // Enable I2C clock
	I2C1->CR1 |= (1UL<<15);								                    // Reset I2C
	I2C1->CR1 &= ~(1UL<<15);							                    // Set I2C
	I2C1->CR2 |=(16UL<<0);								                    // Set peripheral clock at 16MHz
	I2C1->OAR1 |=(1UL<<14);								                    // Should be set high
	I2C1->CCR |=(0x50UL<<0);							                    // Set SCL as 100KHz
	I2C1->TRISE |=(17UL<<0);							                    // Configure maximum rise time
	I2C1->CR1 |= (1UL<<0);								                    // Enable I2C
}

void I2C_start(void){									                    // Function to set I2C start bit
	I2C1->CR1 |= (1<<10);								                    // Enable the ACK Bit
	I2C1->CR1 |= (1<<8);								                    // Send the start bit
	while (!(I2C1->SR1 & (1<<0)));							                    // Wait for SB bit to set
}

void I2C_write(uint8_t data){								                    // Function to write data 
	while (!(I2C1->SR1 & (1<<7)));							                    // Wait till TX buffer is empty
	I2C1->DR = data;								                    // Write data to I2C slave
	while (!(I2C1->SR1 & (1<<2)));							                    // Wait till Byte transfer is completed
}

void I2C_address(uint8_t Address){							                    // Function to send I2C address
	I2C1->DR = Address;  								                    // Send the slave address
	while (!(I2C1->SR1 & (1<<1)));  						                    // wait for ADDR bit to set
	temp = I2C1->SR1 | I2C1->SR2;  							                    // Read SR1 and SR2 to clear the ADDR bit
}

void I2C_stop(void){									                    // Function to send the stop bit
	I2C1->CR1 |= (1<<9);  								                    // Stop I2C
}

void I2C_read(uint8_t Address, uint8_t *buffer, uint8_t size)				                    // Function to read from I2C device
{
	int remaining = size;
	if (size == 1)
	{
		I2C1->DR = Address; 							                   // Send the address
		while (!(I2C1->SR1 & (1<<1)));  					                   // Wait for ADDR bit to set
		I2C1->CR1 &= ~(1<<10);  						                   // Clear the ACK bit
		temp = I2C1->SR1 | I2C1->SR2;  						                   // Read SR1 and SR2 to clear the ADDR bit
		while (!(I2C1->SR1 & (1<<6)));  					                   // Wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  					                   // Read the data from the data register
	}
	else
	{
		I2C1->DR = Address;  							                   // Send the address
		while (!(I2C1->SR1 & (1<<1)));  					                   // Wait for ADDR bit to set
		temp = I2C1->SR1 | I2C1->SR2;  						                   // Read SR1 and SR2 to clear the ADDR bit
		while (remaining>2)
		{
			while (!(I2C1->SR1 & (1<<6)));  				                   // Wait for RxNE to set
			buffer[size-remaining] = I2C1->DR;  				                   // Copy the data into the buffer
			I2C1->CR1 |= 1<<10;  						                   // Set the ACK bit to Acknowledge the data received
			remaining--;
		}
		while (!(I2C1->SR1 & (1<<6)));  					                   // Wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;
		I2C1->CR1 &= ~(1<<10);  						                   // Clear the ACK bit
		I2C1->CR1 |= (1<<9);  							                   // Stop I2C
		remaining--;
		while (!(I2C1->SR1 & (1<<6)));  					                   // Wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  					                   // Copy the data into the buffer
	}
}
