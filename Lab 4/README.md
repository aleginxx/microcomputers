<img align="center" width="100" height="100" src="https://lh5.googleusercontent.com/proxy/MRBDx8ZGLT3hSY5t3q2KhUkOG_Gzt5I7GlafOJ8LYyeep_qBNeylB6YoIZasv3_iTLDBCqOXg9Co3vtRMeDpDQAlV7wftJTaEOPXEjBocWE">
<p align="center">
  NATIONAL TECHNICAL UNIVERSITY OF ATHENS 
</p>
<p align="center">
  SCHOOL OF ELECTRICAL AND COMPUTER ENGINEER
</p>
<p align="center">
  MICROLAB
</p>
<p align="center">
  AC. YEAR 2023-2024
</p>
<p align="center">
  <b>Microcomputers Lab</b>
</p>
<p align="center">
  <b>Lab 4</b>
</p>



**Project Specifications**

*ADC Conversion and Voltage Display*: 
Write an assembly program for the ATmega328PB which will start an ADC conversion every 1Sec. Reading the ADC data must be done within the ADC conversion completion interrupt service routine. This interrupt (ADC) transfers control to address 0x02A, if the corresponding interrupt is enabled (from the ADIE bit of ADCSRA) as well as the all interrupts enable flag. The measured value of the ADC is converted into a voltage and printed on the LCD screen, each time starting from the first character of the first line, with an accuracy of two decimal places (no rounding needed).
The voltage at analog input A2 is calculated using the following formula:

\[ V_{in} = \left( \frac{ADC}{1024} \right) \times V_{ref} \]

Where:
- **V<sub>in</sub>** is the voltage at analog input A2 of the microcontroller.
- **ADC** is the value read by the ADC (10-bit, ranging from 0 to 1023).
- **V<sub>ref</sub>** is the reference voltage, set to 5V.

*ADC Polling LCD Display*: 
Write in C language a program for the ATmega328PB that will initiate an ADC conversion, as in Problem 4.1. It will not use the ADC conversion completion interrupt and will wait until the ADC conversion is complete by checking the ADSC bit of ADCSRA which becomes 0 once the conversion is complete (Polling). It will print the voltage on the LCD screen with two decimal places.

*CO Monitoring System with LED and LCD Alerts*: 
Create code in C to monitor a space where there is an increased risk of carbon monoxide (CO). The CO sensor is connected to analog input A3 of the microcontroller.
Throughout, the sensor value should be read every 100 ms (small deviations are acceptable) and an indication of the gas level should be displayed on LEDs PB0-PB5. If at any time the concentration of CO exceeds 70ppm, the message GAS DETECTED will be printed on the LCD and the corresponding LEDs in PB0-PB5 will flash depending on the level of the gas. The level should continue to be read (and displayed on the LEDs) and if it returns to a level below 70ppm the LEDs will stop flashing and the LCD will print CLEAR.