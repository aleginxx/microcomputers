.include "m328PBdef.inc"
.equ FOSC_MHZ=16	;MHz
.equ DEL_mS=100	;mS
.equ F2=FOSC_MHZ*DEL_mS

.def A = r16  ; Variable A
.def B = r17  ; Variable B
.def C = r18  ; Variable C
.def D = r19  ; Variable D
.def F0 = r20 ; Result for F0
.def F1 = r21 ; Result for F1
.def i = r22   ; Loop counter
    
    
reset:
    ldi r24,LOW(RAMEND)	    ; Initialize stack pointer
    out SPL,r24
    ldi r24,HIGH(RAMEND)
    out SPH,r24

    ldi A, 0x45		    ; Initialize variables
    nop
    ldi B, 0x23
    nop
    ldi C, 0x21
    nop
    ldi D, 0x01
    nop
    ldi i, 0x05  
    
main:
    clr F0
    clr F1
    
    rcall F_0
    rcall F_1
    
    ldi r24, low(F2)
    ldi r25, high(F2)
    rcall delay_mS
    
    ; Increment variables
    ldi r23, 0x01    ; Increment value for A
    add A, r23
    ldi r23, 0x02    ; Increment value for B 
    add B, r23
    ldi r23, 0x04    ; Increment value for C
    add C, r23
    ldi r23, 0x05    ; Increment value for D
    add D, r23
    
    ; Decrement counter and check if the loop should continue
    dec i
    brne main
    breq end
    
     
F_0:
    mov r23, A
    com r23	    ;A'
    mov r26, B
    com r26	    ;B'
    and r23, r26    ;A'*B' in r23
    mov r27, C
    com r27	    ;C'
    and r23, r27    ;A'*B'*C' in r23
    or r23, D	    ;A'*B'*C'+D in r23
    com r23	    ;(A'*B'*C'+D)'
    mov F0, r23	    ;to F0
    ret
    
F_1:
   clr r23
   clr r26
   mov r23, A 
   com r23	    ;A'
   or r23, C	    ;A'+C sto r23
   mov r26, B
   com r26	    ;B'
   mov r28, D    
   com r28	    ;D'
   or r26, r28	    ;B'+D' sto r26
   mov F1, r23	    ;A'+C sto F1
   and F1, r26	    ;(A'+C)*(B'+D')
   ret

delay_mS:
     
    ;total group delay 996 cycles
delay_inner:		    
    ldi	r23, 249	    ; (1 cycle)	
loop_inn:
    dec r23		    ; 1 cycle
    nop			    ; 1 cycle
    brne loop_inn	    ; 1 or 2 cycles
     
    sbiw r24 ,1		    ; 2 cycles
    brne delay_inner	    ; 1 or 2 cycles
 
    ret			    ;4 cycles
    
end: