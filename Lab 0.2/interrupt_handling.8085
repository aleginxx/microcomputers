.include "m328pbdef.inc"
.equ FOSC_MHZ=16
.equ DEL_MS=500
.equ DEL_NU=FOSC_MHZ*DEL_MS
    
.org 0x00
    rjmp reset  ; Reset Handler
    
.org 0x04
    rjmp ISR1   ; ISR1 Handler
   
reset:
    ldi r24, low(RAMEND)
    out SPL, r24
    ldi r24, high(RAMEND)
    out SPH, r24
    ser r26
    out DDRC,r26
    ldi r24,(1 << ISC11) | (1 << ISC10)
    sts EICRA, r24
    ldi r24, (1<<INT1)
    out EIMSK,r24
    sei
	
delay_mS:
    ldi r23, 249
    loop_inn:
	dec r23
	nop
	brne loop_inn
	sbiw r24,1
	brne delay_mS
ret
	
ISR1:
    push r21
    push r22
    push r23
    push r24
    push r25
    in r21,SREG
    push r21
    
    loop:
    ldi r24, low(5*16)
    ldi r25, high(5*16)
    ldi r22, (1 << INTF1)
    
    out EIFR, r22
    rcall delay_mS
    in r22,EIFR
    andi r22,2
    brne loop
    
    in r19,PIND
    andi r19,128
    breq END
    inc r20
    cpi r20,32
    brne NEXT
    clr r20
    NEXT:
    out PORTC,r20
    END:
    pop r21
    out SREG,r21
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    sei
    reti