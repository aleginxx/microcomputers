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
  <b>Lab 1</b>
</p>



**Project Specifications**

*Delay Handling*: 
Realize an assembly code for microcontroller ATmega328, in the form that is displayed below, that produces regulated delays with total acuracy.
```
…
rcall wait_x_msec
…
wait_x_msec:
….
ret
```

*Logic Gates*: 
Realize an assembly code for microcontroller ATmega328 that calculates the following logic functions:
F0 = (A'∙B' + C'∙D)'
F1 = (A'+C)∙(B'+D')
The function calculation should be inserted in a loop that will be executed 5 times. With each repetition, variable A will be augmented by 0x01, variable B by 0x02, variable C by 0x04 and variable D by 0x05.
Logic variables A, B, C and D have a size of 1 Bytes and are initialized as follows:
A = 0x45, B = 0x23, C = 0x21 and D = 0x01

*Shift Wagon*: 
Realize an assembly code for microcontroller ATmega328 that controls an automated wagon that is constantly moving, starting from the right to the left and then in the opposite direction.
The wagon should be simulated with a bit in the output PORTD that is constantly moving from the LSB to the MSB (and opposite). 
- Each move of  the wagon by one spot (one led in PORTD) happens every 1.5 sec.
- The direction of the wagon is stored in T flag of SREG.
- The wagon makes an extra 2-second stop when it changes direction, a.k.a. stays in the farmost right and farmost left for 3.5 seconds. 
- Realize the delays with the code realized in file delay_handling.8085.