.include "m328PBdef.inc"

reset:
    ldi r24, low(RAMEND)
    out SPL, r24
    ldi r24, high(RAMEND)
    out SPH, r24

start:
    clr r16
    out DDRB, r16 ; PORTB as input
    ser r16
    out PORTB, r16 ; Pull-up PORTB
    ldi r16, 0x03
    out DDRC, r16 ; PORTC as output

main:
    clr r21
    clr r22
    in r16, PINB
    mov r17, r16
    andi r17, 0x01
    lsr r16
    mov r18, r16
    andi r18, 0x01
    lsr r16
    mov r19, r16	
    andi r19, 0x01
    lsr r16
    mov r20, r16
    andi r20, 0x01

    ; Compute F0 = A.B + B'.D
    mov r23, r17
    and r23, r18 ; A.B in r23
    mov r24, r18
    com r24 ; B' in r24
    and r24, r20 ; B'.D in r24
    mov r21, r23 ; A.B into F0
    or r21, r24 ; + B'.D into F0

    ; Compute F1 = (A'+C').(B+D')
    mov r23, r17
    com r23 ; A' in r23
    mov r24, r19
    com r24 ; C' in r24
    or r23, r24 ; A'+C' in r23
    mov r24, r18
    mov r22, r20
    com r22 ; D' in r22
    or r24, r22 ; B+D' in r24
    and r23, r24 ; (A'+C').(B+D') into r23
    mov r22, r23 ; F1 = (A'+C').(B+D')

    ; Output F0 and F1 to PORTC
    andi r21, 0x01 ; isolate LSB
    lsl r22 ; shift F1 to second LSB
    andi r22, 0x02 ; isolate second LSB
    or r21, r22 ; combine F0 and F1
    out PORTC, r21

    rjmp main