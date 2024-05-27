.include "m328PBdef.inc"          ; Include ATmega328P microcontroller definitions

.equ FOSC_MHZ = 16                ; Microcontroller operating frequency in MHz

; Define DC_VALUE at the beginning
.def DC_VALUE = r18

ldi DC_VALUE, 0x80                ; Set the initial duty cycle to 50%

reset:
    ldi r24, (1<<WGM10) | (1<<COM1A1)
    sts TCCR1A, r24                   ; Set Timer/Counter1 Control Register A

    ldi r24, (1<<WGM12) | (1<<CS11)
    sts TCCR1B, r24                   ; Set Timer/Counter1 Control Register B

    ; Initialize Stack Pointer
    ldi r24, low(RAMEND)
    out SPL, r24
    ldi r24, high(RAMEND)
    out SPH, r24

    ; Initialize PORTB as output
    ser r26
    out DDRB, r26

    ; Initialize PORTD as input
    clr r27
    out DDRD, r27

    clr r17                          ; Clear r17 for use as a temporary register
    ldi r21, 0x06                    ; Initialize array position at the middle, 50% DC

    ldi DC_VALUE, 0x80               ; Set the initial duty cycle to 50%
    sts OCR1AL, DC_VALUE             ; Set the output compare register for 50% duty cycle

main:
    ; Load the address of the 'duty' array into Z register
    ldi zh, high(duty*2)
    ldi zl, low(duty*2)

    ; Read the input from PORTD (PD1 and PD2)
    in r20, PIND
    cpi r20, 0b11111101               ; Check if PD1 is pressed
    breq butt_1
    cpi r20, 0b11111011               ; Check if PD2 is pressed
    breq butt_2
    rjmp main

button_1:
    in r20, PIND
    cpi r20, 0b11111101
    breq button_1                        ; Button 1 debounce, stay in this state

    cpi r21, 0x0C                        ; Check if r21 is at the maximum value (12)
    breq main
    inc r21                              ; Increment r21

    ; Calculate the new array index and load the corresponding value
    mov r22, r21
    lsl r22                              ; r22 = r21 * 2
    add zl, r22
    adc zh, r17                          ; r17 = 0
    lpm                                  ; Load value from 'duty' into r0
    mov r19, r0                          ; Store the value in r19

    sts OCR1AL, r19                      ; Set the duty cycle based on the array value
    rjmp main

button_2:
    in r20, PIND
    cpi r20, 0b11111011
    breq button_2                      ; Button 2 debounce, stay in this state

    cpi r21, 0x00                      ; Check if r21 is at the minimum value (0)
    breq main
    dec r21                            ; Decrement r21

    ; Calculate the new array index and load the corresponding value
    mov r22, r21
    lsl r22                            ; r22 = r21 * 2
    add zl, r22
    adc zh, r17                        ; r17 = 0
    lpm                                ; Load value from 'duty' into r0
    mov r19, r0                        ; Store the value in r19

    sts OCR1AL, r19                    ; Set the duty cycle based on the array value
    rjmp main

duty:
    .DW 0x0005, 0x001A, 0x002E, 0x0043, 0x0057, 0x006C, 0x0080, 0x0094, 0x00A7, 0x00BD, 0x00D2, 0x00E6, 0x00FB