.include "m328PBdef.inc"  ; Include the predefined definitions for the ATmega328PB microcontroller.

.equ FOSC_MHZ=16 ;MHz  ; Define the frequency of the oscillator in megahertz.
.equ DEL_mS=1000 ;mS  ; Define the delay in milliseconds.
.equ F1=FOSC_MHZ*DEL_mS  ; Calculate the number of cycles required for the specified delay.

.DEF temp = r16  ; Define the register r16 as 'temp'.
.DEF leds = r17  ; Define the register r17 as 'leds'.

;Init Stack Pointer
ldi r24, LOW(RAMEND)  ; Load the lower byte of the RAMEND address into register r24.
out SPL, r24  ; Store the value of r24 into the Stack Pointer Low register.
ldi r24, HIGH(RAMEND)  ; Load the higher byte of the RAMEND address into register r24.
out SPH, r24  ; Store the value of r24 into the Stack Pointer High register.

start:
    ldi leds, 0x01  ; Load the value 0x01 (binary 00000001) into the 'leds' register.

    ;Init PORTD as output
    ser temp  ; Set all bits of the 'temp' register to 1.
    out DDRD, temp  ; Configure all pins of PORTD as output.

    ldi r24, low(F1)  ; Load the lower byte of F1 into register r24.
    ldi r25, high(F1)  ; Load the higher byte of F1 into register r25. Set delay.

left:
    out PORTD, leds  ; Output the value of 'leds' to PORTD, turning on the corresponding LEDs.
    sbrc leds,0  ; Skip the next instruction if the least significant bit (LSB) of 'leds' is clear (not set).
    rcall delay_outer  ; Call the delay_outer subroutine to create a delay.

    ; The following two rcall delay_outer instructions introduce an intentional delay to control the speed of the LED movement.

    rcall delay_outer  ; Call the delay_outer subroutine to create a delay.
    lsl leds  ; Logical shift left the value of 'leds', effectively shifting the LEDs to the left.
    sbrs leds,7  ; Skip the next instruction if the most significant bit (MSB) of 'leds' is set.
    rjmp left  ; Jump back to the 'left' label.

right:
    out PORTD, leds  ; Output the value of 'leds' to PORTD, turning on the corresponding LEDs.
    sbrc leds,7  ; Skip the next instruction if the MSB of 'leds' is clear.
    rcall delay_outer  ; Call the delay_outer subroutine to create a delay.

    ; The following two rcall delay_outer instructions introduce an intentional delay to control the speed of the LED movement.

    rcall delay_outer  ; Call the delay_outer subroutine to create a delay.
    lsr leds  ; Logical shift right the value of 'leds', effectively shifting the LEDs to the right.
    sbrs leds, 0  ; Skip the next instruction if the LSB of 'leds' is set.
    rjmp right  ; Jump back to the 'right' label.

    rjmp left  ; Unconditional jump to the 'left' label.

; This routine is used to produce a delay of 993 cycles.
delay_inner:
    ldi r23, 247  ; Load the value 247 into register r23.

    loop3:
    dec r23  ; Decrement the value of r23 by 1.
    nop  ; No operation (1 cycle).
    brne loop3  ; Branch to 'loop3' if the result of the decrement operation is not equal to zero.
    nop  ; No operation (1 cycle).
    ret  ; Return from the subroutine.

    ; This routine is used to produce a delay of (1000*F1+14) cycles.
delay_outer:
    push r24  ; Push the value of register r24 onto the stack.
    push r25  ; Push the value of register r25 onto the stack to save them.

loop4:
    rcall delay_inner  ; Call the delay_inner subroutine, which produces a delay of 993 cycles.
    sbiw r24, 1  ; Subtract 1 from the 16-bit value formed by r24 and r25.
    brne loop4  ; Branch to 'loop4' if the result of the subtraction is not equal to zero.

    pop r25  ; Pop the value from the stack into register r25.
    pop r24  ; Pop the value from the stack into register r24 to restore their original values.
    ret  ; Return from the subroutine.