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
  <b>Lab 6</b>
</p>



**Project Specifications**

*Keyboard to LED Mapping*: 
To implement code for the ATmega328PB microcontroller, in C language, which reads the ntuAboard_G1 keyboard, and maps four of the keyboard keys to four leds according to the following table:

  | 1 | 5 | 9 | D |
  | Led in PB0 | Led in PB1 | Led in PB2 | Led in PB3 |

*Keypress Display*: 
Implement code for the ATmega328PB microcontroller, in C language, which will use the functions of the above question and display on the 2x16 LCD screen the character corresponding to the last key pressed. It is assumed that only one key can be pressed at a time.

*Electronic Lock Program with Group Number Verification*: 
Write an "electronic lock" program that lights up the 6 leds PB0 to PB5 for 4 sec when your 2-digit group number (eg 09) is given by the 4x4 keypad. If a student does not have a group number, use the number 99. If the two digits of the group number are not given correctly, then the leds PB0 to PB5 should flash (250 mSec on, 250 mSec off) for 5 sec. Regardless of how long a key is held down, the program should consider it only pressed once. After pressing two numbers, the program should not accept another number for 5 seconds. The program must be in continuous operation. Give the flowchart and program in C language.