.include "m328PBdef.inc"

.equ FOSC_MHZ=16	;MHz
.equ DEL_mS=100	;mS
.equ F1=FOSC_MHZ*DEL_mS
	
 reset:
    ldi r24,low(RAMEND)
    out SPL,r24
    ldi r24,high(RAMEND)
    out SPH,r24  
    
    ser r24
    out DDRD,r24
    out PORTD,r24
    
    clr r26
    
main:
    ldi r24, low(F1-2)
    ldi r25, high(F1-2)
    rcall wait_x_msec
    inc r26
    out PORTD,r26
    rjmp main
    
    
wait_x_msec:
    ldi	r23, 249	    ; (1 cycle)	
    nop

delay_one:
    dec r23		    ; 1 cycle
    nop
    brne delay_one	    ; 1 or 2 cycles
    ;total group delay 996 cycles

    ldi	r23, 249	    ; (1 cycle)	

delay_two:
    dec r23		    ; 1 cycle
    nop
    brne delay_two	    ; 1 or 2 cycles
    ;total group delay 996 cycles
    
    ldi	r23, 249	    ; (1 cycle)	


delay_three:		    
    ldi	r23, 249	    ; (1 cycle)	
loop:
    dec r23		    ; 1 cycle
    nop			    ; 1 cycle
    brne loop	    ; 1 or 2 cycles
     
    sbiw r24 ,1		    ; 2 cycles
    brne delay_three	    ; 1 or 2 cycles
 
    ret			    ;4 cycles