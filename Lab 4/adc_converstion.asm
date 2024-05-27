include "m328PBdef.inc"    ; ATmega328P microcontroller definitions

.equ PD3 = 3
.equ PD2 = 2
    
.def temp = r16 
.def counter = r17
.def ADC_L = r21 
.def ADC_H = r22
    
.org 0x00
rjmp reset
.org 0x2A	                ; ADC Conversion Complete Interrupt
rjmp adc_interrupt  


reset:
    
;Initialize Stack Pointer
    ldi r24, low(RAMEND)	
    out SPL, r24
    ldi r24, high(RAMEND)
    out SPH, r24		
    
    ser r24
    out DDRD, r24		    ;Set PORTD as output 
    out DDRB, r24		    ;Set PORTD as output 
    
    clr r24
    out DDRC, r24		    ;Set PORTC as input
    
    sei				    ;Enable global interrupts
    
    ;REFSn[1:0]=01 => select Vref=5V, MUXn[4:0]=0010 => select ADC2(pin PC2)
    ;ADLAR=0 => RIGHT adjust the ADC result
    
    ldi r16, 0b01000010		    
    sts ADMUX, r16		    
    
    ;ADEN=1 => ADC Enable
    
    ldi r16, 0b11101111
    sts ADCSRA, r16
    
    ldi r16, 0b00000000
    sts ADCSRB, r16
    

main: 
    jmp main    
    
    
adc_interrupt:
    
    ; Initialize LCD and wait for 200 ms
    rcall lcd_init
    ldi r24, low(200)
    ldi r25, high(200)
    rcall wait_msec
    
    lds ADC_L, ADCL		    ;Load the low byte of ADC result into ADC_L
    lds ADC_H, ADCH		    ;Load the high byte of ADC result into ADC_H

    
    ; Copy the values of ADC_L and ADC_H to r17 and r18, respectively
    mov r17, ADC_L
    mov r18, ADC_H
    
    lsl r17
    rol r18
    lsl r17
    rol r18
    add r17, ADC_L
    adc r18, ADC_H
    
    ;Isolation of the MSB
    mov ADC_H, r18
    andi r18, 0b00011100
    sub	ADC_H, r18
    lsr r18
    lsr r18
    
    ; Add 0x30 for the display output
    ori r18, 0b00110000
    mov r26, r18		    ;Store the MSB in r26
    
    ;deftero psifio
    mov ADC_L, r17
    mov r18, ADC_H
    
    lsl r17
    rol r18
    lsl r17
    rol r18
    lsl r17
    rol r18
    add r17, ADC_L
    adc r18, ADC_H
    add r17, ADC_L
    adc r18, ADC_H
    
    ;Isolation of the first decimal
    mov ADC_H, r18
    andi r18,0b00111100
    sub ADC_H, r18
    lsr r18
    lsr r18
    
    ; Add 0x30 for the display output
    ori r18,0b00110000
    mov r27, r18	            ;Store the 1st decimal in r27
    
    
    ;trito  psifio
    mov ADC_L, r17
    mov r18, ADC_H
    lsl r17
    rol r18
    lsl r17
    rol r18
    lsl r17
    rol r18
    add r17, ADC_L
    adc r18, ADC_H
    add r17, ADC_L
    adc r18, ADC_H
    
    ;Isolation of the second decimal
    mov ADC_H, r18
    andi r18, 0b00111100
    sub ADC_H, r18
    lsr r18
    lsr r18
    
    ; Add 0x30 for the display output
    ori r18, 0b00110000
    
    mov r24,r26
    rcall lcd_data

    ldi r24,'.'
    rcall lcd_data

    mov r24,r27
    rcall lcd_data

    mov r24,r18
    rcall lcd_data
    ldi r24, low(10)
    ldi r25, high(10)
    rcall wait_msec

    reti
    
write_2_nibbles:
    
    push r24			    ; save r24(LCD_Data)

    in r25 ,PIND		    ; read PIND

    andi r25 ,0x0f		
    andi r24 ,0xf0		    ; r24[3:0] Holds previus PORTD[3:0]
    add r24 ,r25		    ; r24[7:4] <-- LCD_Data_High_Byte
    out PORTD ,r24		

    sbi PORTD ,PD3		    ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3

    pop r24			    ; Recover r24(LCD_Data)
    swap r24 ;
    andi r24 ,0xf0		    ; r24[3:0] Holds previus PORTD[3:0]
    add r24 ,r25		    ; r24[7:4] <-- LCD_Data_Low_Byte
    out PORTD ,r24

    sbi PORTD ,PD3		    ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3

    ret

 lcd_data:
    
    sbi PORTD ,PD2		    ; LCD_RS=1(PD2=1), Data
    rcall write_2_nibbles	    ; send data
    ldi r24 ,250	
    ldi r25 ,0			    ; Wait 250uSec
    rcall wait_usec
    ret
 
 lcd_command:
    
    cbi PORTD ,PD2		    ; LCD_RS=0(PD2=0), Instruction
    rcall write_2_nibbles	    ; send Instruction
    ldi r24 ,250 
    ldi r25 ,0			    ; Wait 250uSec
    rcall wait_usec
    ret
 
 lcd_clear_display:

    ldi r24 ,0x01		    ; clear display command
    rcall lcd_command

    ldi r24 ,low(5) 
    ldi r25 ,high(5)		    ; Wait 5 mSec
    rcall wait_msec 

    ret
 
 lcd_init:
    ldi r24 ,low(200) 
    ldi r25 ,high(200)		    ; Wait 200 mSec
    rcall wait_msec 

    ldi r24 ,0x30		    ; command to switch to 8 bit mode
    out PORTD ,r24 
    sbi PORTD ,PD3		    ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    ldi r24 ,250 
    ldi r25 ,0			    ; Wait 250uSec
    rcall wait_usec 

    ldi r24 ,0x30		    ; command to switch to 8 bit mode
    out PORTD ,r24 
    sbi PORTD ,PD3		    ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    ldi r24 ,250 
    ldi r25 ,0			    ; Wait 250uSec
    rcall wait_usec 

    ldi r24 ,0x30		    ; command to switch to 8 bit mode
    out PORTD ,r24 
    sbi PORTD ,PD3		    ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    ldi r24 ,250 
    ldi r25 ,0			    ; Wait 250uSec
    rcall wait_usec

    ldi r24 ,0x20		    ; command to switch to 4 bit mode
    out PORTD ,r24
    sbi PORTD ,PD3		    ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    ldi r24 ,250 
    ldi r25 ,0			    ; Wait 250uSec
    rcall wait_usec

    ldi r24 ,0x28		    ; 5x8 dots, 2 lines
    rcall lcd_command
    ldi r24 ,0x0c		    ; dislay on, cursor off
    rcall lcd_command

    rcall lcd_clear_display
    ldi r24 ,0x06		    ; Increase address, no display shift
    rcall lcd_command ;
    ret
 
 wait_usec:
    sbiw r24 ,1			    ; 2 cycles (2/16 usec)
    call delay_8cycles		    ; 4+8=12 cycles
    brne wait_usec		    ; 1 or 2 cycles
    ret

 delay_8cycles:
    nop
    nop
    nop

wait_msec:
    ; Delay function using nested loops
    ldi r23, 249
    loop_inn:
        dec r23
        nop
        brne loop_inn
        sbiw r24, 1
        brne wait_msec
    ret