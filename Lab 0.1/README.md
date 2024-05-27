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
  AC. YEAR 2022-2023
</p>
<p align="center">
  <b>Introduction to Microcomputers</b>
</p>
<p align="center">
  <b>Lab 1</b>
</p>



**Project Specifications**

*Logic_Gates*: 
Realize an assembly code for microcontroller ATmega328 that calculates the following logic functions:
F0 = (A∙B + B'∙D)
F1 = (A'+C')∙(B+D')
The logic variables A, B, C and D are connected to the first 4 LSBs of PORTB. 
F0 and F1 are provided by 2 PORTC LSBs.

*Shift_Wagon*: 
Realize an assembly code for microcontroller ATmega328 that controls an automated wagon that is constantly moving, starting from the right to the left and then in the opposite direction.
The wagon should be simulated with a bit in the output PORTD that is constantly moving from the LSB to the MSB (and opposite). Each move of  the wagon by one spot (one led in PORTD) happens every 1 sec.
The wagon makes an extra 1-second stop when it changes direction, a.k.a. stays in the farmost right and farmost left for 2 seconds. 