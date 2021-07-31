# Interfacing DS1307
In this file there are two codes, the first one “DS1307.c” will interface the DS1307 RTC module with the STM32 Discovery board. The second on ”DS1307_I2C.c” wil is a continuation of the first one which will display the date and time on the LCD using the I2C module. It displays the time by default and if the push button is pressed a interupt is triggered which will display the date for two seconds and return back to displaying the time every second.


## Components:
[STM32 F407VG Discovery Board](https://www.st.com/en/evaluation-tools/stm32f4discovery.html), [DS1307](https://www.tomsonelectronics.com/products/ds1307-i2c-rtc-ds1307-24c32-real-time-clock-module), [16x2 LCD Module](https://www.tomsonelectronics.com/products/16x2-jhd-lcd-display), [I2C Adaptor Module for LCD](https://www.tomsonelectronics.com/products/iic-i2c-serial-interface-adapter-module-for-display), Breadboard, Jumper Wires


## Output:

### DS1307.c

For DS1307.c there is no output as such, by using the debug mode of the compiler you can see the changes in the variable.
![Screenshot (205)](https://user-images.githubusercontent.com/71063467/127688979-ceefb28b-bb44-4086-ae07-cbd56708c4bb.png)

### DS1307_I2C.c

For DS1307_I2C.c the set time will be displayed on the LCD, with a refresh rate of 1 second. If the user pushbutton is pressed then the set date is displayed for 2 seconds after which it will go back to displaying the time.
![IMG_20210730_224643](https://user-images.githubusercontent.com/71063467/127688960-55e95a91-0168-412b-b854-1451cafca65b.jpg)


## Hardware Connections:

### DS1307.c

* PB7 – SDA 
* PB6 – SCL
* VCC – 5V
* GND – GND

![image](https://user-images.githubusercontent.com/71063467/127061024-76e809cd-cad4-45ec-9b60-22d090c5fdc0.png)


### DS1307_I2C.c

* PB7 – SDA & SDA
* PB6 – SCL & SCL
* VCC – 5V 
* GND – GND

![image](https://user-images.githubusercontent.com/71063467/127061077-e8387f7c-4861-4870-8d57-c2ac3cd3f4c6.png)

Find more information about interfacing the I2C adaptor : [LCD_I2C]( https://github.com/kskumaree/STM32F407_Discovery-Board_VIT/tree/master/LCD_I2C )

Read more about this project: [Blog](https://danielalapat.hashnode.dev/date-and-time) 
