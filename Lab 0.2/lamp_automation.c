#define F_CPU 16000000UL 
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/delay.h>

volatile int pointer = 0; // Variable to track the state of the LED
int counter;

ISR(INT1_vect){ // Interrupt Service Routine for External Interrupt 1
    
    if(pointer == 0){ // If the LED is currently off
        pointer = 1; // Set the flag to indicate LED is on
        PORTB = 0x01; // Turn on the LED connected to PORTB pin 0
        sei(); // Enable global interrupts
        for(counter=1; counter<=4000; counter++){
                _delay_ms(1); // Delay for 4000 milliseconds (4 seconds)
        }
        PORTB = 0x00; // Turn off the LED
    }
    else{ // If the LED is currently on
        PORTB = 0xFF; // Turn on all the LEDs connected to PORTB
        for(counter=1; counter<=500; counter++){
                _delay_ms(1); // Delay for 4000 milliseconds (4 seconds)
        }
        PORTB = 0x01; // Turn on the LED connected to PORTB pin 0
        counter=1;
    }

}

int main(){

    EICRA = (1 << ISC11) | (1 << ISC10); // Configure External Interrupt 1 to trigger on rising edge
    EIMSK = (1 << INT1); // Enable External Interrupt 1
    
    sei(); // Enable global interrupts
    
    DDRB = 0xFF; // Configure PORTB as output
    
    while(1){
        PORTB = 0x00; // Turn off the LED (assuming all other PORTB pins are also off)
        pointer=0;
    }
}
