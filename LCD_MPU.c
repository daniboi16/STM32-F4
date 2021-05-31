//hardware schematic and code explantion can be found at : https://danielalapat.hashnode.dev/multiple-i2c-devices
#include "stm32f4xx.h"								//include headder file for the board

//defining I2C adresses for different registers 
#define LCD_ADDR 0x4E									//I2C address of LCD			
#define MPU6050_ADDR 0xD0							//I2C address of MPU
#define SMPLRT_DIV_REG 0x19							
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

//pre initializing the functino
void msDelay(uint32_t msTime);
void configure_GPIOB(void);
void configure_I2C(void);
void I2C_start(void);
void I2C_write(uint8_t data);
void I2C_address(uint8_t Address);
void I2C_read (uint8_t Address, uint8_t *buffer, uint8_t size);
void I2C_stop (void);
void I2C_write4(uint8_t data);
void I2C_write8(uint8_t data);
void I2C_write4C(uint8_t data);
void I2C_write8C(uint8_t data);
void I2C_send(uint8_t data);
void MPU6050_Read_Accel (void);
void MPU6050_Init (void);
void MPU_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size);
void MPU_Write (uint8_t Address, uint8_t Reg, uint8_t Data);

//initalzing variables for accelorometer calculations
int16_t x_read=0,y_read=0,z_read=0;
float x_accel,y_accel,z_accel;
unsigned int temp;
uint8_t check;
uint8_t d=0;													

int main ()
{
	configure_GPIOB();								//to configure the GPIO pins PB6 and PB7
	configure_I2C();									//to configure the I2C1 register
	msDelay(100);
	
	//following commands reffered for libray
	I2C_write4(0x30);									//to clear the screen
  msDelay(4200);
  I2C_write4(0x30);									//to clear the screen
  msDelay(150);
  I2C_write4(0x30);									//to clear the screen
  msDelay(37);
  I2C_write4(0x20);									//to turn on the screen
  msDelay(37);

	//to set LCD to 4 bit mode and 2 lines, 5x8 font
  I2C_write4(0x28);									//upper nibble 								
  msDelay(37);	
  I2C_write8(0x28);									//lower nibble
  msDelay(37);


  //to turn the display on with blinking curesor
  I2C_write4(0x0C);									//upper nibble
  msDelay(37);  
  I2C_write8(0x0C);									//lower nibble
  msDelay(37); 

  //to clear the contents on the screen
  I2C_write4(0x01);									//upper nibble
  msDelay(37);  
  I2C_write8(0x01);									//lower nibble
  msDelay(1600);

  //to set LCD to write from left to right
  I2C_write4(0x06);									//upper nibble
  msDelay(37);  
  I2C_write8(0x06);									//lower nibble
  msDelay(37);

	MPU6050_Init ();


	while(1){
		MPU6050_Read_Accel ();					//read the accelration
		msDelay(100);
		
		//to clear the contents on the screen
		I2C_write4(0x01);									//upper nibble
		msDelay(37);  
		I2C_write8(0x01);									//lower nibble
		msDelay(100);
		
		if(x_accel>0){
			//to display the charecter "U"
			I2C_write4C(0x55);								//upper nibble
			msDelay(37);
			I2C_write8C(0x55);								//lower nibble
			msDelay(41);			
		}
		else{
			//to display the charecter "D"
			I2C_write4C(0x44);								//upper nibble
			msDelay(37);
			I2C_write8C(0x44);								//lower nibble
			msDelay(41);	
		}
		msDelay(1000);	
	}
}	

void msDelay(uint32_t msTime){				//function for sotware delay
	//"For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	//16MHz/4=4MHz. If we want 1000ms (1second) delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	for (uint32_t i=0;i<msTime*4000;i++){
		__NOP();
	}
}

void configure_GPIOB(void){					//to configure the GPIO pins PB6 and PB7
	//SDA - PB7, SCL - PB6
	RCC->AHB1ENR |=(1UL<<1);					//Enable clock for port B
	GPIOB->MODER |=(2UL<<(6*2));			//PB6 to alternate function pin "10"
	GPIOB->MODER |=(2UL<<(7*2));			//PB7 to alternate function pin "10"
	GPIOB->PUPDR |=(0x5UL<<(6*2));		//set PB6 and PB7 as pull up pins "01"
	GPIOB->OTYPER |=(0x3UL<<6);				//Set PB6 and PB7 as open drain "1"
	GPIOB->OSPEEDR |=(0xAUL<<(6*2));	//Set PB6 and PB7 as high speed "10"
	GPIOB->AFR[0] |= (0x44<<(6*4));		//Set PB6 and PB7 to alternate function 4 "0100"
}

void configure_I2C(void){						//To configure the I2C registers
	RCC->APB1ENR |=(1UL<<21);					//Enable I2C clock
	I2C1->CR1 |= (1UL<<15);						//Reset I2C
	I2C1->CR1 &= ~(1UL<<15);					//set I2C
	I2C1->CR2 |=(16UL<<0);						//Set peripheral clock at 16MHz
	I2C1->OAR1 |=(1UL<<14);						//Should be set high
	I2C1->CCR |=(0x50UL<<0);					//Set SCL as 100KHz
	I2C1->TRISE |=(17UL<<0);					//Configure maximum rise time
	I2C1->CR1 |= (1UL<<0);						//Enable I2C
}

void I2C_start(void){								//to send the I2C start bit
	I2C1->CR1 |= (1<<10);							//Enable the ACK Bit
	I2C1->CR1 |= (1<<8);							//Send the start bit
	while (!(I2C1->SR1 & (1<<0)));		//Wait for SB bit to set
}

void I2C_write(uint8_t data){				//function to write data to MPU
	while (!(I2C1->SR1 & (1<<7)));		//Wait till TX buffer is empty
	I2C1->DR = data;									//Write data to I2C slave
	while (!(I2C1->SR1 & (1<<2)));		//Wait till Byte transfer is completed
}

void I2C_address(uint8_t Address){	//function to send I2C adress
	I2C1->DR = Address;  							//send the slave address
	while (!(I2C1->SR1 & (1<<1)));  	// wait for ADDR bit to set
	temp = I2C1->SR1 | I2C1->SR2;  		//read SR1 and SR2 to clear the ADDR bit
}

void I2C_send(uint8_t data){				//to send data to the I2C device
	while (!(I2C1->SR1 & (1<<7)));		//Wait till TX buffer is empty
	I2C1->DR = data;									//Write data to I2C slave
	while (!(I2C1->SR1 & (1<<2)));		//Wait till Byte transfer is completed
}

void I2C_read (uint8_t Address, uint8_t *buffer, uint8_t size){//function to read from I2C device
	int remaining = size;
	if (size == 1)
	{
		I2C1->DR = Address; 									//send the address
		while (!(I2C1->SR1 & (1<<1)));  			//wait for ADDR bit to set
		I2C1->CR1 &= ~(1<<10);  							//clear the ACK bit
		temp = I2C1->SR1 | I2C1->SR2;  				//read SR1 and SR2 to clear the ADDR bit.... EV6 condition
		while (!(I2C1->SR1 & (1<<6)));  			//wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  	//Read the data from the DATA REGISTER
	}
	else
	{
		I2C1->DR = Address;  									//send the address
		while (!(I2C1->SR1 & (1<<1)));  			//wait for ADDR bit to set
		temp = I2C1->SR1 | I2C1->SR2;  				//read SR1 and SR2 to clear the ADDR bit
		while (remaining>2)
		{
			while (!(I2C1->SR1 & (1<<6)));  		//wait for RxNE to set
			buffer[size-remaining] = I2C1->DR;  //copy the data into the buffer
			I2C1->CR1 |= 1<<10;  								//Set the ACK bit to Acknowledge the data received
			remaining--;
		}
		while (!(I2C1->SR1 & (1<<6)));  			//wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;
		I2C1->CR1 &= ~(1<<10);  							//clear the ACK bit
		I2C1->CR1 |= (1<<9);  								//stop I2C
		remaining--;
		while (!(I2C1->SR1 & (1<<6)));  			//wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  	//copy the data into the buffer
	}
}

void I2C_stop(void){											//function to send the stop bit
	I2C1->CR1 |= (1<<9);  									//Stop I2C
}

void I2C_write4(uint8_t data){			//to send upper four bit for commands
	d=0;															//reset value for d
	d |= (data & 0xF0); 							//fto select only the upper nibble
  d |= 1<<2;												//to set the enable bit
	d |= 1<<3;												//to set the backlight
	I2C_start();											//send the start bit
	I2C_address(LCD_ADDR);										//send the I2C address
	I2C_send(d);											//send the data
  msDelay(1);												//wait for 1ms 
  d &= ~(1<<2);											//set the enable bit low
  I2C_send(d);											//send the same data but without enable bit to latch the data
  I2C_stop();												//send the stop bit
}

void I2C_write8(uint8_t data){			//to send lower four bit for commands
	d=0;
  d |= ((data & 0x0F)<<4); 					//to select only the lower nibble
  d |= 1<<2;
	d |= 1<<3;
	I2C_start();
	I2C_address(LCD_ADDR);
	I2C_send(d);
  msDelay(1);
  d &= ~(1<<2);
  I2C_send(d);
  I2C_stop();
}

void I2C_write4C(uint8_t data){			//to send upper four bits for data
	d=0;
  d |= (data & 0xF0); 							//to select only the upper nibble
  d |= 1<<2;
	d |= 1<<0;												//to select the data register not the command
	d |= 1<<3;
	I2C_start();
	I2C_address(LCD_ADDR);
	I2C_send(d);
  msDelay(1);
  d &= ~(1<<2);
  I2C_send(d);
  I2C_stop();
}

void I2C_write8C(uint8_t data){			//to send the lower four bits for data
	d=0;
  d |= ((data & 0x0F)<<4); 					//to select only the lower nibble
  d |= 1<<2;
	d |= 1<<0;												//to select data register not command register
	d |= 1<<3;
	I2C_start();
	I2C_address(LCD_ADDR);
	I2C_send(d);
  msDelay(1);
  d &= ~(1<<2);
  I2C_send(d);
  I2C_stop();
}
void MPU_Write (uint8_t Address, uint8_t Reg, uint8_t Data)
{
	I2C_start ();
	I2C_address (Address);
	I2C_write (Reg);
	I2C_write (Data);
	I2C_stop ();
}
void MPU_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
	I2C_start ();
	I2C_address (Address);
	I2C_write (Reg);
	I2C_start ();
	I2C_read (Address+0x01, buffer, size);//To read, set LSB to 1
	I2C_stop ();
}
void MPU6050_Init (void)
{
	uint8_t check;
	uint8_t Data;
	MPU_Read (MPU6050_ADDR,WHO_AM_I_REG, &check, 1);//Check device ID

	if (check == 104)  // 0x68 will be returned by the sensor
	{
		Data = 0;
		MPU_Write (MPU6050_ADDR, PWR_MGMT_1_REG, Data);//Power up the sensor
		Data = 0x07;
		MPU_Write(MPU6050_ADDR, SMPLRT_DIV_REG, Data);//Sampling rate of 1KHz
		Data = 0x00;
		MPU_Write(MPU6050_ADDR, ACCEL_CONFIG_REG, Data);//accelerometer range=+/- 2g
	}
}
void MPU6050_Read_Accel (void)
{
	uint8_t Buf[6];
	// Read 6 BYTES of data starting from ACCEL_XOUT_H register
	MPU_Read (MPU6050_ADDR, ACCEL_XOUT_H_REG, Buf, 6);
	x_read = (int16_t)(Buf[0] << 8 | Buf[1]);
	y_read = (int16_t)(Buf[2] << 8 | Buf[3]);
	z_read = (int16_t)(Buf[4] << 8 | Buf[5]);
	x_accel = x_read/16384.0;
	y_accel = y_read/16384.0;
	z_accel = z_read/16384.0;
}
