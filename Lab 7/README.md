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
  <b>Lab 7</b>
</p>



**Project Specifications**

*DS18B20 Temperature Sensor Communication*: 
To implement code for the ATmega328PB microcontroller, in C language for the communication of the microcontroller with the DS18B20 temperature sensor on the NtuAboard_G1 card. The program should include only the elementary functions, taking into account that only one sensor is connected to the PD4 pin. These elementary functions are based on the commands received by the sensor and are presented in the DS18B20 technical manual.

The procedure includes the following steps to interact with the temperature sensor (DS18B20) connected to the PD4 pin:

1. Initialize and check device connection with the one_wire_reset routine.
2. Using the one_wire_transmit_byte routine to send a 0xCC command, bypassing device selection due to single connection.
3. Send command 0x44 to start temperature measurement.
4. Use one_wire_receive_bit for measurement completion check.
5. New device initialization with one_wire_reset.
6. Send 0xCC command.
7. Send 0xBE command to read 16-bit temperature.
8. Store value in registers r25:r24 by calling one_wire_receive_byte twice.
9. Return value 0x8000 if there is no device on the PD4 pin.

*Temperature LCD Display*: 
Write a program in C that, using the previous routine, displays the temperature in C on the LCD display as a three-digit signed decimal value (-55C to +125C). Also in the event that there is no device connected it will display the message “NO Device”.