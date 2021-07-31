# Interfacing MPU6050 

In this project I have interfaced a MPU6050 accelerometer with STM 32 Discovery board. In the first code I calculated the acceleration in the x, y and z directions.
In the second code I have displayed a “D” or an ”U” depending on the acceleration in the x direction. 

## Components

[STM32 F407VG Discovery Board](https://www.st.com/en/evaluation-tools/stm32f4discovery.html), [MPU6050](https://www.tomsonelectronics.com/products/10dof-gy-87-mpu6050-hmc5883l-bmp180-3-axis-gyro-3-axis-acceleration-3-axis-magnetic-field-air-pressure-module?variant=12516697669680), 
[16x2 LCD Module](https://www.tomsonelectronics.com/products/16x2-jhd-lcd-display), [I2C Adaptor Module for LCD](https://www.tomsonelectronics.com/products/iic-i2c-serial-interface-adapter-module-for-display), Breadboard, Jumper Wires

## Output
### MPU.c
There is no such output as such, you can use the debug mode of your compiler to see the real time acceleration values.
![image](https://user-images.githubusercontent.com/71063467/127749105-780d2cab-ba21-45b1-a279-95259f0054f1.png)

### MPU_LCD.c

When the module is moved downwards the letter “D” is displayed on the LCD and when the module is moved upwards the letter “U” is displayed on LCD. 

## Hardware Connections

### MPU.c

* PB6 – SCL
* PB7 – SDA
* GND – GND
* 5V - VCC

![image](https://user-images.githubusercontent.com/71063467/127749096-1004f8ba-7605-4727-a2e5-317464b64569.png)

### MPU_LCD.c
* PB6 – SCL - SCL
* PB7 – SDA - SDA
* GND – GND - GND
* 5V – VCC - +

![image](https://user-images.githubusercontent.com/71063467/127749093-7b6b3771-588d-4cda-9ae5-0071acbda37a.png)

Find more about this project: [BLOG](https://danielalapat.hashnode.dev/multiple-i2c-devices)  
