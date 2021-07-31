#include "stm32f407xx.h"									// Include header file for the board

// Defining I2C addresses for different registers
#define DS1307_ADDR 0xd0									// I2C address of the RTC
#define LCD_ADDR 0x4E										// I2C address of LCD			

unsigned int temp=0;										// Temporary variable to clear the bit
uint8_t sec=0,min=0,hour=0,day=0,date=0,month=0,year=0; 					// Global variables for all the registers 
uint8_t d=0;											// Initializing variable d
int flag=0;											// Flag for interrupts

// Pre initializing the functions
uint8_t bcd2decimal(uint8_t num);
uint8_t decimal2bcd(uint8_t num);
void interupt_init(void);
void EXTI0_IRQHandler( );
void DS_setval(void);
void DS_readval(void);
void LCD_display_time(void);
void LCD_display_date(void);
void configure_GPIOB(void);
void configure_I2C(void);
void msDelay(uint32_t msTime);
void I2C_start(void);
void I2C_write(uint8_t data);
void I2C_address(uint8_t Address);
void I2C_stop(void);
void I2C_read(uint8_t Address, uint8_t *buffer, uint8_t size);
void I2C_write4(uint8_t data);
void I2C_write8(uint8_t data);
void I2C_write4C(uint8_t data);
void I2C_write8C(uint8_t data);
void LCD_init(void);
void LCD_code(uint8_t num);

int main()
{
	configure_GPIOB();									// To configure the GPIO pins PB6 and PB7
	configure_I2C();									// To configure the I2C1 register
	interupt_init();									// To configure the interrupts
	msDelay(100);
	//****************************************************************************************************************************************
	// This line is not require after the first time. After the time is set will remain set untill power is removed
	DS_setval();										// To set the time and date
	msDelay(100);
	LCD_init();										// To set up the LCD, sending commands to the LCD (this would take a few seconds)
	while(1){
		DS_readval();									// Read values from the RTC
		if (flag==1){									// Condition if the push button is pressed
			
			// Clear the contents on the screen
			I2C_write4(0x01);							// Upper nibble
			msDelay(37);  
			I2C_write8(0x01);							// Lower nibble
			msDelay(37);
			
			LCD_display_date();							// To display the date on the LCD
			msDelay(1900);								// To display the date for 2 seconds
			flag=0;}								// Reset the flag value
		
		else{										// If the push button is not pressed (default condition)
			
			// Clear the contents on the screen
			I2C_write4(0x01);							// Upper nibble
			msDelay(37);  
			I2C_write8(0x01);							// Lower nibble
			msDelay(37);
			
			LCD_display_time();							// To display the time on the LCD
			msDelay(900);
		}
	}
}

uint8_t decimal2bcd(uint8_t num){								// Function to convert decimal to BCD
	return ((num/10 * 16) + (num % 10));
}

uint8_t bcd2decimal(uint8_t num){								// Function to convert BCD to decimal
	return ((num/16 * 10) + (num % 16));
}

void interupt_init(void)									// Function to configure the interrupt
{
	RCC->APB1ENR |= (1<<0);  								// Enable PORTA clock
	EXTI->RTSR |=(1<<0);
	EXTI->IMR |=(1<<0);
	NVIC->ISER[0] |= 1<<6;
}


void EXTI0_IRQHandler( ){									// This function occurs whenever interrupt occurs
	EXTI->PR |= (1<<0);
	flag=1;											// set flag
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


void LCD_display_time(void){								// Function to display the time on the LCD
	uint8_t hour_temp0, min_temp0, sec_temp0;
	uint8_t hour_temp1, min_temp1, sec_temp1;
	hour_temp0 = hour / 10;
	hour_temp1 = hour % 10;
	min_temp0 = min / 10;
	min_temp1 = min % 10;
	sec_temp0 = sec / 10;
	sec_temp1 = sec % 10;
	
	LCD_code(hour_temp0);
	LCD_code(hour_temp1);
	
	// To display the character ":"
	I2C_write4C(0x3A);								//upper nibble
	msDelay(37);
	I2C_write8C(0x3A);								//lower nibble
	msDelay(41);
	
	LCD_code(min_temp0);
	LCD_code(min_temp1);
	
	// To display the character ":"
	I2C_write4C(0x3A);								//upper nibble
	msDelay(37);
	I2C_write8C(0x3A);								//lower nibble
	msDelay(41);
	
	LCD_code(sec_temp0);
	LCD_code(sec_temp1);
}

void LCD_display_date(void){								// Function to display the date on the LCD
	uint8_t date_temp0, month_temp0, year_temp0;
	uint8_t date_temp1, month_temp1, year_temp1;
	date_temp0 = date / 10;
	date_temp1 = date % 10;
	month_temp0 = month / 10;
	month_temp1 = month % 10;
	year_temp0 = year / 10;
	year_temp1 = year % 10;
	
	LCD_code(date_temp0);
	LCD_code(date_temp1);
	
	// To display the character "-"
	I2C_write4C(0x2D);								//upper nibble
	msDelay(37);
	I2C_write8C(0x2D);								//lower nibble
	msDelay(41);
	
	LCD_code(month_temp0);
	LCD_code(month_temp1);
	
	// To display the character "-"
	I2C_write4C(0x2D);								//upper nibble
	msDelay(37);
	I2C_write8C(0x2D);								//lower nibble
	msDelay(41);
	
	LCD_code(year_temp0);
	LCD_code(year_temp1);
}


void configure_GPIOB(void){								// Function to configure GPIO port b pin 6 and 7					
	RCC->AHB1ENR |=(1UL<<1);							// Enable clock for port B
	GPIOB->MODER |=(2UL<<12);							// PB6 to alternate function pin
	GPIOB->MODER |=(2UL<<14);							// PB7 to alternate function pin
	GPIOB->PUPDR |=(0x5UL<<12);							// Set PB6 and 7 as pull up pins
	GPIOB->OTYPER |=(0x3UL<<6);							// Set PB6 and 7 as open drain
	GPIOB->OSPEEDR |=(0xAUL<<12);							// Set PB6 and 7 as high speed
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

void I2C_write4(uint8_t data){								// To send upper four bit for commands
	d=0;										// Reset value for d
	d |= (data & 0xF0); 								// To select only the upper nibble
  	d |= 1<<2;									// To set the enable bit
	d |= 1<<3;									// To set the backlight
	I2C_start();									// Send the start bit
	I2C_address(LCD_ADDR);								// Send the I2C address
	I2C_write(d);									// Send the data
	msDelay(1);									// Wait for 1ms 
	d &= ~(1<<2);									// Set the enable bit low
	I2C_write(d);									// Send the same data but without enable bit to latch the data
	I2C_stop();									// Send the stop bit
}

void I2C_write8(uint8_t data){								// To send lower four bit for commands
	d=0;
	d |= ((data & 0x0F)<<4); 							// To select only the lower nibble
	d |= 1<<2;
	d |= 1<<3;
	I2C_start();
	I2C_address(LCD_ADDR);
	I2C_write(d);
	msDelay(1);
	d &= ~(1<<2);
	I2C_write(d);
	I2C_stop();
}

void I2C_write4C(uint8_t data){								// To send upper four bits for data
	d=0;
  	d |= (data & 0xF0); 								// To select only the upper nibble
  	d |= 1<<2;
	d |= 1<<0;									// To select the data register not the command
	d |= 1<<3;
	I2C_start();
	I2C_address(LCD_ADDR);
	I2C_write(d);
	msDelay(1);
	d &= ~(1<<2);
	I2C_write(d);
	I2C_stop();
}

void I2C_write8C(uint8_t data){								// To send the lower four bits for data
	d=0;
 	d |= ((data & 0x0F)<<4); 							// To select only the lower nibble
  	d |= 1<<2;
	d |= 1<<0;									// To select data register not command register
	d |= 1<<3;
	I2C_start();
	I2C_address(LCD_ADDR);
	I2C_write(d);
	msDelay(1);
	d &= ~(1<<2);
	I2C_write(d);
	I2C_stop();
}

void LCD_init(void){								// Function to send the initial commands to the LCD
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
}

void LCD_code(uint8_t num){							// Function to match the number and display it
	if (num==0){
		// To display the charecter "0"
		I2C_write4C(0x30);								//upper nibble
		msDelay(37);
		I2C_write8C(0x30);								//lower nibble
		msDelay(41);}
	
	else if (num==1){
		//to display the character "1"
		I2C_write4C(0x31);								//upper nibble
		msDelay(37);
		I2C_write8C(0x31);								//lower nibble
		msDelay(41);}
	
	else if (num==2){
		//to display the character "2"
		I2C_write4C(0x32);								//upper nibble
		msDelay(37);
		I2C_write8C(0x32);								//lower nibble
		msDelay(41);}
	
	else if (num==3){
		//to display the character "3"
		I2C_write4C(0x33);								//upper nibble
		msDelay(37);
		I2C_write8C(0x33);								//lower nibble
		msDelay(41);}
	
	else if (num==4){
		//to display the character "4"
		I2C_write4C(0x34);								//upper nibble
		msDelay(37);
		I2C_write8C(0x34);								//lower nibble
		msDelay(41);}
	
	else if (num==5){
		//to display the character "5"
		I2C_write4C(0x35);								//upper nibble
		msDelay(37);
		I2C_write8C(0x35);								//lower nibble
		msDelay(41);}
	
	else if (num==6){
		//to display the character "6"
		I2C_write4C(0x36);								//upper nibble
		msDelay(37);
		I2C_write8C(0x36);								//lower nibble
		msDelay(41);}
	
	else if (num==7){
		//to display the character "7"
		I2C_write4C(0x37);								//upper nibble
		msDelay(37);
		I2C_write8C(0x37);								//lower nibble
		msDelay(41);}
		
	else if (num==8){
		//to display the character "8"
		I2C_write4C(0x38);								//upper nibble
		msDelay(37);
		I2C_write8C(0x38);								//lower nibble
		msDelay(41);}
	
	else if (num==9){
		//to display the character "9"
		I2C_write4C(0x39);								//upper nibble
		msDelay(37);
		I2C_write8C(0x39);								//lower nibble
		msDelay(41);}
	
	else{											//if unrecognised character display "#"
		//to display the character "#"
		I2C_write4C(0x23);								//upper nibble
		msDelay(37);
		I2C_write8C(0x23);								//lower nibble
		msDelay(41);}
}
