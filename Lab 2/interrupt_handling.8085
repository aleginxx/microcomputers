.include "m328pbdef.inc"
.equ FOSC_MHZ=16
.equ delay=500
.equ F1=FOSC_MHZ*delay

.org 0x00
    rjmp reset  ; Reset Handler

.org 0x04
    rjmp ISR1   ; ISR1 Handler

reset:
    ; Initialize the stack pointer
    ldi r24, low(RAMEND)
    out SPL, r24
    ldi r24, high(RAMEND)
    out SPH, r24
    
    ; Init PORTC as output
    ser r26
    out DDRC, r26
    
    ; Init PORTB as output
    clr r26
    ser r26
    out DDRB, r26
    
    ; Init PORTD as input
    clr r26
    out DDRD,r26
    
    ; Configure external interrupt INT1 (PD3) for falling edge trigger
    ldi r24, (1 << ISC11) | (1 << ISC10)
    sts EICRA, r24
    
    ; Enable external interrupt INT1
    ldi r24, (1 << INT1)
    out EIMSK, r24
    
    ; Enable global interrupts
    sei

loop1:
    clr r27

loop2:
    ; Set all pins of Port B to 0
    out PORTB, r27
    
    ; Load delay value
    ldi r24, LOW(F1)
    ldi r25, HIGH(F1)
    nop
    rcall delay_mS

    ; Increment r27
    inc r27

    ; Compare r27 with 32
    cpi r27, 32
    brne loop2  ; Branch if r27 is equal to 32
    rjmp loop1  ; Repeat loop2

delay_mS:
    ; Delay function using nested loops
    ldi r23, 249
    loop_inn:
        dec r23
        nop
        brne loop_inn
        sbiw r24, 1
        brne delay_mS
    ret

ISR1:
    push r21

    push r22
    push r23
    push r24
    push r25
    in r21, SREG
    push r21

    loop:
    ; Load delay value
    ldi r24, low(F1)
    ldi r25, high(F1)
    ldi r22, (1 << INTF1)

    ; Clear the interrupt flag for INT1
    out EIFR, r22

    ; Call delay function
    rcall delay_mS

    ; Read the EIFR register to check INTF1 flag
    in r22, EIFR
    andi r22, 2

    ; Branch if INTF1 flag is set
    brne loop

    ; Read PIND to check a specific bit (bit 6)
    in r19, PIND
    andi r19, 64

    ; Branch if the bit is not set
    breq END

    ; Increment r20
    inc r20

    ; Compare r20 with 32
    cpi r20, 32

    ; Branch if r20 is not equal to 32
    brne NEXT

    ; Clear r20
    clr r20

    NEXT:
    ; Output r20 to Port C
    out PORTC, r20

    END:
    ; Restore the SREG
    pop r21
    out SREG, r21

    ; Pop registers
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21

    ; Enable global interrupts
    sei

    reti