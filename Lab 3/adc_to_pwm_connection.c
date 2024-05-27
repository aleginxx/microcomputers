#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>


unsigned char duty[] = {0x05, 0x1A, 0x2E, 0x43, 0x57, 0x6C, 0x80, 0x94, 0xA7, 0xBD, 0xD2, 0xE6, 0xFB};      // Ascending order

int main(){

    TCCR1A = (1<<WGM10)|(1<<COM1A1);	// Non-inverting Mode, Fast PWM, 8-bit Mode
    TCCR1B = (1<<WGM12)|(1<<CS11);		// Prescaler is 1 (frequency = f.clk/8)
    //Set Vref = 5V
    ADMUX = (1 << REFS0) | (1 << MUX0) |(1 << ADLAR);

    //Enable ADC & set ADC prescaler
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
	
	
    DDRB=0b001111;      				// Set PB1 as output, PB4,5 as input
    DDRD=0xFF;							// Set PORTD as output

    unsigned int DC_VALUE = 6;
    unsigned char input , adcValue, output;
    OCR1AL = duty[DC_VALUE];			// Initial dc 50%



    while(1){
        input = PINB&0b010000;
        if((!(input))&&(DC_VALUE<12)){		// If PB4 is pressed
            DC_VALUE++ ;			// Increase dc by 8%
            _delay_ms(50);
        }
        input = PINB&0b100000;
        if((!(input))&&(DC_VALUE>0)){			// If PB5 is pressed
            DC_VALUE--;				// Decrease dc by 20%
            _delay_ms(50);
    }

    OCR1AL = duty[DC_VALUE];

    ADCSRA |= (1<<ADSC);
    
    output = ADCSRA;
    output &= 0b01000000;
    while(output){
        output = ADCSRA;
        output &= 0b01000000;
    }

    adcValue = ADCH;

    if(adcValue<32){
        output=0x01;
    }
    else if(adcValue<64){
        output=0x02;
    }
    else if(adcValue<96){
        output=0x04;
    }
    else if(adcValue<128){
        output=0x08;
    }
    else if(adcValue<160){
        output=0x10;
    }
    else if(adcValue<192){
        output=0x20;
    }
    else if(adcValue<224){
        output=0x40;
    }
    else{
        output=0x80;
    }

    PORTD = output;
    _delay_ms(100);

    }
    return 0;
}