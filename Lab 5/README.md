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
  <b>Lab 5</b>
</p>



**Project Specifications**

*Logic Function Implementation*: 
To implement the logic functions for the ATmega328PB microcontroller, in C language: 
F0 = (A'B + B'CD)' 
F1= (AC)(B+D) 
Input variables are given in PORTB bits [3:0] (A= PORTB.0, B= PORTB.1, C= PORTB.2, D= PORTB.3) of ntuAboard_G1. The values of F0- F1 are displayed respectively on the IO0_0 and IO0_1 pins of the PCA9555 integrated port expander, on ntuAboard_G1. The terminals IO0_0 and IO0_1 of the connector P18 should be connected, via cables, to the terminals LED_PD0 and LED_PD1 of the connector J18 respectively, so that the values of the functions F0 and F1 are displayed on the leds PD0 PD1.

*Visual Display of Logical State and LED Control*: 
In order to be able to have a visual display of the logical state of the pins IO0_0 to IO0_3, of expansion port 0, set them as outputs and connect, via cables, to the pins LED_PD0 to LED_PD3 of the connector J18 respectively. 
Pin IO1_0 of expansion port 1 should be set as output while pins IO1_4 to IO0_7 should be set as inputs. 
To implement a code for the ATmega328PB microcontroller, in C language, which when the "*" key is pressed, the PD0 led lights up, when the "0" key is pressed, the PD 1 led lights up, when the "#" key is pressed, it lights up the PD2 led and when the “D” key is pressed, the PD3 led lights up. If no key is pressed then all leds should remain off.

*LCD Display Connection and Name Display Implementation*: 
Connect the 2x16 character LCD screen, through the J19 connector, to the expansion port 1 of the integrated PCA9555 and to implement code for the ATmega328PB microcontroller, in C language, which will display your first and last name on the screen