# ROTARY ENCODER

I have interfaced a rotary encoder with the STMM 32 Discovery board. The rotary encoder is a knob similar to a potentiometer but digital in function. It has 3 pins, the CLK, 
DT, SW. the CLK and DT pins are used to figure out the direction of rotation while the SW pin is to detect the pushbutton feature which can be activated by pushing down on the 
knob. 

![image](https://user-images.githubusercontent.com/71063467/127699653-c02a1033-897d-44f5-a329-450c52275b7a.png)


### Components

[STM32 F407VG Discovery Board](https://www.st.com/en/evaluation-tools/stm32f4discovery.html), 
[Rotary Encoder](https://www.tomsonelectronics.com/products/360-degree-rotary-encoder-module), Jumper wires.

### Hardware Connection

* CLK – PE2
* DT – PE1
* SW - PE0
* +ve - 5V
* GND – GND

![image](https://user-images.githubusercontent.com/71063467/127699692-4aa80941-b66a-4b18-913b-347ac990a668.png)

# 01) pushbutton.c
To implement the push button of the rotary encoder. SW pin connected to port E pin 0

# 02) rotaryencoder1.c

To implement the direction of the rotary encoder. 

### Output
* When rotated clockwise = Red and blue led turn on 
* When rotated anticlockwise = green and orange led turn on

# 03) rotaryencoder2.c
To implement the volume knob with a rotary encoder, however without a LCD. 

### OUTPUT
* turns the green led on if the encoder is rotated clockwise and the red led if rotated anticlockwise
* if state =50, then turn of the blue and yellow LED
* if button is pressed then turn on all four LED


Read more about this project: [Blog](https://danielalapat.hashnode.dev/rotary-encoder) 
