# LCD with I2C module


I have interfaced the STM32 discovery board with the LCD module using the I2C module. The module uses the I2C Serial protocol to send data to the LCD. By using the I2C module we can use the number of connections between the board and the LCD. I2C is a 2-wire connection.


## Components:
[STM32 F407VG Discovery Board](https://www.st.com/en/evaluation-tools/stm32f4discovery.html), [16x2 LCD Module](https://www.tomsonelectronics.com/products/16x2-jhd-lcd-display), [I2C Adaptor Module for LCD](https://www.tomsonelectronics.com/products/iic-i2c-serial-interface-adapter-module-for-display), Breadboard, Jumper Wires


## Output: 
“CAR”


## Hardware Connections:
Discovery board and I2C module:
* PB7 – SDA 
* PB6 – SCL
* VCC – 5V
* GND – GND

![image](https://user-images.githubusercontent.com/71063467/126905743-d4b63fdc-336b-4db9-b9d7-6deda31a80db.png)
![image](https://user-images.githubusercontent.com/71063467/126905749-995a6add-d882-4f30-8cf0-ee8f72d44eb5.png)


### Internal connections:
The connections of the I2C module are a bit odd, it is as follows: 
* Pins I2C : Pins LCD
* 1 : VSS
* 2 : VCC
* 3 : VEE
* 4 : RS
* 5 : R/W
* 6 : E
* 7,8,9,10 : NC
* 11 : DB4
* 12 : DB5
* 13 : DB6
* 14 : DB7
* 15 : LED+
* 16 : LED-

### The 8 bits of the I2C bus:
* D0 : RS
* D1 : R/W
* D2 : E
* D3 : BACKLIGHT
* D4 : DB4
* D5 : DB5
* D6 : DB6
* D7 : DB7


I have used the I2C1 peripheral which is connected to PB6 and PB7. We have to use the LCD in 4-bit mode as the lower 4 data pins are not connected to the I2C bus, hence we need to send 4 bits at a time. I use “I2C_write4” function to send the upper four bits and the “I2C_write8” function to send the lower four bits.

Reference: [Arduino library](https://github.com/blackhack/LCD_I2C/tree/master/src)


Read more about this project: [Blog](https://danielalapat.hashnode.dev/interfacing-lcd-with-i2c)
