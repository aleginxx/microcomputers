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
  <b>Lab 2</b>
</p>



**Project Specifications**

*Interrupt Handling*: 
In the code of the counter of Figure 2.1, add a service routine for the external interrupt INT1 (PD3), in Assembly language, which enumerates the number of interrupts INT1 from 0 to 31. When it reaches 31 it starts again from zero. As long as the PD6 button is pressed (logic 0) the interrupt count freezes. When the PD6 button is released again, the measurement continues from where it was left. The number of external interruptions should be displayed, in binary form, on the PC4-PC0 leds.
Since there is a possibility that the flickering phenomenon may occur, by pressing the PD3 button of the previous example, add the appropriate instructions to the service routine of the external interrupt INT1 to avoid this phenomenon.

*Interrupt Counter*: 
To implement Assembly code for the ATmega328PB microcontroller, corresponding to the code of the counter in figure 2.1, with the difference that the measurement will be 0 to 31, the display will be done on the leds PC4-PC0 and the delay time between the exchanges will ranges at 1000 mS.
The service routine of the external interrupt INT0 (PD2) should be modified so that when activated it lights up as many LEDs of the PORTC port starting from the LSBs as the number of 5 buttons PB4 - PB0 that are pressed.

*Lamp Automation*: 
To implement automation that controls the switching on and off of a light fixture. When we press the push button PD3 (that is, by activating INT1), the PB0 led of the PORTB port (which we assume represents the light body) lights up. The led will turn off after 4 seconds, unless there is a new press of PD3 in between, in which case the time of 4 seconds will be renewed. Every time a refresh is made, all the LEDs of the PORTΒ port (PB7-PB0) light up for 0.5 sec, then turn off except for the PB0 led which stays on for a total of 4 sec unless it is refreshed. The code should be given in assembly and C.