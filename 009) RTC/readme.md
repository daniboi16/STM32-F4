# RTC
In my most recent project, which you can read [Here.](https://danielalapat.hashnode.dev/date-and-time ) I interfaced my DS1307 real time clock with my STM 32 discovery board 
and displayed the date and time on LCD display using an I2C adaptor. While doing that project I realized that my STM32 F407VG has an inbuild RTC. The RTC is inbuild into the 
processor, so I don’t need to use I2C or anything, however this would be a lot harder as I have to deal with the internal registers. But today I am ready to take up the 
challenge. The internal RTC means that there are no hardware connections as the RTC is part of the chip. 

Some of the features available in the inbuild RTC. It has the time and data with automatic leap year correction like the DS1307. It does have additional feature which is absent
in the DS1307 like: wake up, alarm, timestamp, daylight saving correction and tamper detection as well as a more precise oscillator. 

![Screenshot (213)](https://user-images.githubusercontent.com/71063467/129457356-ecd0c872-1632-444b-ab8b-116548824cea.png)

BLOG: [Internal RTC](https://danielalapat.hashnode.dev/internal-rtc)
