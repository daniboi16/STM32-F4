# Interfacing LCD

I have interfaced the common 16x2 LCD display with the STM 32 Discovery Board. The mode of interfacing is parallel mode, hence there is a lot of connections between the board and the LCD. 


## Components:
[STM32 f407VG Discovery Board](https://www.st.com/en/evaluation-tools/stm32f4discovery.html), [16x2 LCD Module](https://www.tomsonelectronics.com/products/16x2-jhd-lcd-display), Breadboard, Jumper Wires

## Output:
“CAR”

## Hardware Connections:
* LED- – GND
* LED+ - 5V
* DB7 – PD7
* DB6 – PD6
* DB5 – PD5
* DB4 – PD4
* DB3 – PD3
* DB2 – PD2
* DB1 – PD1
* DB0 – PD0
* E – PD10
* R/W – PD9
* RS – PD8
* VEE – Resistor
* VCC – 5V
* VSS - GND

![image](https://user-images.githubusercontent.com/71063467/127050987-f118df0b-67db-4445-b7f7-ed37095f4fde.png)
![image](https://user-images.githubusercontent.com/71063467/127051021-a239e0ac-afa9-47ed-af4b-bc5828a69f7a.png)

I have used two functions `void command(void)` which is used to set the RS to command, R/W to read and Enable to a high to low pulse and `void data(void)` which is used to set the RS to data, R/W to read and Enable to a high to low pulse.

Read more about this project: [Blog](https://danielalapat.hashnode.dev/stm32-f4-interfacing-lcd) 
 
