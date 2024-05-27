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
  <b>Lab 3</b>
</p>



**Project Specifications**

*PWM Control with Button-Adjusted Duty Cycle*: 
Create assembly language code that initializes the TMR1A in 8-bit mode and generates a PWM waveform at pin PB1. Assume that BOTTOM=0. An LED is connected to pin PB1 and its brightness changes if the Duty Cycle of the PWM waveform changes. Initially the Duty Cycle is set to 50% and stored in a variable named DC_VALUE. Then each time the PD1 button is pressed the Duty Cycle is increased by 8%. When the Duty Cycle reaches the maximum value of 98% pressing the PD1 button does not increase it further. Each time the PD2 button is pressed the Duty Cycle is reduced by 8%. When the Duty Cycle reaches the minimum value of 2% pressing the PD2 button does not reduce it further. The values that the OCR1A register should take, for the various Duty Cycle values, have been calculated in advance and placed in an array in the microcontroller's memory so that they do not need to be calculated during code execution.
Re-write the code in C.
In addition, for changing the brightness of the led there will be two operating modes, mode1 and mode2. In mode1 the brightness(Duty Cycle) will be stored in a variable named DC_VALUE and pressing the PD1 button will increase the brightness while pressing the PD2 button will decrease the brightness. In mode2 the brightness (Duty Cycle) will be controlled by the potentiometer POT1. If the PD6 button is pressed then mode1 will be selected while if the PD7 button is pressed then mode2 will be selected.

*ADC to PWM Connection and Voltage-Based LED Control*: 
Connect the A1 input of the ADC to the PB1_PWM analog filter. Disconnect led PB1 using dip switches SW1. Rewrite the code of problem 3.1 in C, which will produce a PWM output at pin PB1. In addition the ADC reads the varying DC voltage produced at the output of the analog filter PB1_PWM every 100 mSec (small deviations are acceptable). The measured value of the ADC (ADCH: ADCL) will be converted into a voltage value (Vadc) one of the leds connected to the PORTD port will light up according to the table below:

  | V_adc | LED ON |
  | 0 Volt ≤ V_adc ≤ 0.625 Volt | PD0 |
  | 0.625 Volt < V_adc ≤ 1.25 Volt | PD1 |
  | 1.25 Volt < V_adc ≤ 1.875 Volt | PD2 |
  | 1.875 Volt < V_adc ≤ 2.5 Volt | PD3 |
  | 2.5 Volt < V_adc ≤ 3.125 Volt | PD4 |
  | 3.125 Volt < V_adc ≤ 3.75 Volt | PD5 |
  | 3.75 Volt < V_adc ≤ 4.375 Volt | PD6 |
  | 4.375 Volt < V_adc ≤ 5 Volt | PD7 |