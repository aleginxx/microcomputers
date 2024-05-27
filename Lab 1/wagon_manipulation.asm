.include "m328PBdef.inc"

.equ FOSC_MHZ = 16          
.equ inner_delay = 1500     ; Inner loop delay 
.equ outer_delay = 2000     ; Outer loop delay 
.equ F1 = FOSC_MHZ * inner_delay
.equ F2 = FOSC_MHZ * outer_delay

.def temp = r16             ; Temporary register
.def leds = r17             ; LEDs state register

ldi r24, low(RAMEND)        ; Initialize stack pointer
out SPL, r24
ldi r24, high(RAMEND)
out SPH, r24

start:
ldi leds, 0x01              ; Initialize LED state

ser temp                    
out DDRD, temp              ; Set PORTD as output

ldi r24, low(F1-2)          ; Load F1 delay
ldi r25, high(F1-2)         

left:
out PORTD, leds		    ; Output LED state to PORTD
clt			    ; Clear the T flag (left direction)
ldi r24, low(F1-2)          ; Load F1 delay 
ldi r25, high(F1-2)         
rcall wait_x_msec           ; Call delay subroutine
lsl leds		    ; Left shift LEDs state
rcall test_direction1       ; Call direction test subroutine
brts right		    ; If T flag is set, go right
rjmp left		    ; Otherwise, continue left

right:
out PORTD, leds		    ; Output LED state to PORTD
set			    ; Set the T flag (right direction)
ldi r24, low(F1-2)          ; Load F1 delay
ldi r25, high(F1-2)         
rcall wait_x_msec           ; Call delay subroutine
lsr leds		    ; Right shift LEDs state
rcall test_direction2       ; Call direction test subroutine
brtc left		    ; If T flag is cleared, go left
rjmp right		    ; Otherwise, continue right

test_direction1:
    cpi leds, 0b10000000    ; Test if LEDs are at the leftmost position
    brne left               ; If not, continue left
    breq change_to_right    ; If yes, change direction to right

test_direction2:
    cpi leds, 0b00000001    ; Test if LEDs are at the rightmost position
    brne right              ; If not, continue right
    breq change_to_left     ; If yes, change direction to left

change_to_right:
    set			    ; Set the T flag for right direction
    ldi r24, low(F2-2)      ; Load F2 delay 
    ldi r25, high(F2-2)     
    rcall wait_x_msec       ; Call delay subroutine
    rjmp right

change_to_left:
    clt			    ; Clear the T flag for left direction
    ldi r24, low(F2-2)      ; Load F2 delay 
    ldi r25, high(F2-2)     
    rcall wait_x_msec       ; Call delay subroutine
    rjmp left

wait_x_msec:
    ldi r23, 249           
    nop                     

delay_one:
    dec r23                  
    nop                    
    brne delay_one           
    
    ldi r23, 249             

delay_two:
    dec r23                  
    nop                     
    brne delay_two           

    ldi r23, 249             

delay_three:
    ldi r23, 249             
loop:
    dec r23                  
    nop                     
    brne loop                

    sbiw r24, 1              
    brne delay_three         

    ret