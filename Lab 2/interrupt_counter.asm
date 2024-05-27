.include "m328PBdef.inc"
.equ FOSC_MHZ=16
.equ delay=1000
.equ F1=FOSC_MHZ*delay

.org 0x00
    rjmp reset
.org 0x02
    rjmp ISR0

reset:
    ; Initialize the stack pointer
    ldi r24, low(RAMEND)
    out SPL, r24
    ldi r24, high(RAMEND)
    out SPH, r24
    
    ; Set all pins of Port C as outputs
    ser r26
    out DDRC, r26

    ; Set only the lower 5 bits of PORTB as inputs (PB0-PB4)
    clr r26
    out DDRB, r26	
    
    ; Configure external interrupt INT0 for rising edge trigger
    ldi r24, (1 << ISC01) | (1 << ISC00)
    sts EICRA, r24
    
    ; Enable external interrupt INT0
    ldi r24, (1 << INT0)
    out EIMSK, r24
    
    ; Enable global interrupts
    sei

loop1:
    clr r26
loop2:
    ; Set all pins of Port C to 0
    out PORTC, r26
    
    ; Load delay value
    ldi r24, low(F1)
    ldi r25, high(F1)
    nop
    rcall delay_mS
    
    ; Increment r26
    inc r26
    
    ; Compare r26 with 32
    cpi r26, 32
    brne loop2
    rjmp loop1

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

ISR0:
    ; Push registers onto the stack to preserve their values
    push r20
    push r21
    push r22
    push r23
    push r24
    push r25

    ; Save the state of the status register (SREG) and push it onto the stack
    in r20, SREG
    push r20

    ; Clear r20 (used for bit manipulation)
    clr r20

    ; Read the state of the PINB register (Port B input)
    in r20, PINB

    ; Invert all bits in r20 (complement)
    com r20

    ; Initialize r21 for loop control (5 iterations for PB4-PB0)
    ldi r21, 5

    ; Clear r22 (used for accumulating bits)
    clr r22

    loop3:
        ; Rotate right through carry (bit 0 becomes the carry flag)
        ror r20

        ; Check the carry flag
        brcc NEXT

        ; If the carry flag is set, rotate left through carry and accumulate in r22
        rol r22

        NEXT:
        ; Decrement the loop counter r21
        dec r21

        ; Repeat the loop while r21 is not zero
        brne loop3

    ; Output the accumulated value (r22) to Port C
    out PORTC, r22

    ; Load the delay values
    ldi r24, LOW(F1)
    ldi r25, HIGH(F1)

    ; Call the delay function
    rcall delay_mS

    ; Restore the registers and status register
    pop r20
    out SREG, r20
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20

    ; Enable global interrupts
    sei

    ; Return from the interrupt
    reti