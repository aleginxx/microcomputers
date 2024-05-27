.include "m328PBdef.inc"
.equ FOSC_MHZ=16
.equ basic_delay=3000
.equ double_delay=500
.equ F1=FOSC_MHZ*basic_delay
.equ F2=FOSC_MHZ*double_delay

.org 0x00
	rjmp reset
.org 0x04
	rjmp ISR1

reset:
	ldi r24, low(RAMEND)
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24
	ser r26
	out DDRC, r26
	out DDRB, r26
	ldi r24, (1 << ISC11) | (1 << ISC10)
	sts EICRA, r24
	ldi r24, (1 << INT1)
	out EIMSK, r24
	sei
	loop1:
	clr r20
	rjmp loop1

delay_mS:
  
    ldi	r23, 249	    ; (1 cycle)	
loop_inn:
    dec r23		    ; 1 cycle
    nop			    ; 1 cycle
    brne loop_inn	    ; 1 or 2 cycles
     
    sbiw r24 ,1		    ; 2 cycles
    brne delay_mS	    ; 1 or 2 cycles
 
    ret			    ;4 cycles


ISR1:

cpi r20,0
breq first_interrupt

more_interrupts:
ser r22
out PORTB,r22
ldi r24, low(F2)
ldi r25, high(F2)
rcall delay_mS
ldi r22,1
out PORTB,r22
ldi r24, low(F1)
ldi r25, high(F1)
ldi r23, 249
sei
reti


first_interrupt:
ldi r21,1
out PORTB,r21
ser r20
sei
ldi r24, low(F1)
ldi r25, high(F1)
rcall delay_mS
ldi r21,0
out PORTB,r21
reti