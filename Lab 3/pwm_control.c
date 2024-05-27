#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char duty[] = {0x05, 0x1A, 0x2E, 0x43, 0x57, 0x6C, 0x80, 0x94, 0xA7, 0xBD, 0xD2, 0xE6, 0xFB};

int main() {
    
    DDRB |= 0b00111111; //Output
    DDRC |= 0b00111111;
    DDRD |= 0b00000000; //Input
    
    TCCR1A = (1 << WGM10) | (1 << COM1A1);
    TCCR1B = (1 << WGM12) | (1 << CS11);
    
    //Set Vref = 5V
    ADMUX = (1<<REFS0) | (1<<ADLAR);
    
    //Enable ADC & set ADC prescaler to 128
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) |(1 << ADPS0);
    
    unsigned char DC_VALUE = 6;
    unsigned char MODE = 1;
    OCR1AL = duty[DC_VALUE];
    
    while(1) {
        
        if (PIND == 0b10111111) {    //PD6
            MODE = 1;
        }
        if (PIND == 0b01111111) {  //PD7
            MODE = 2;
        }
        
        while (MODE == 1) {
                PORTC = DC_VALUE;
                if (PIND == 0b11111101) {
                    _delay_ms(200);
                    if (DC_VALUE >= 12) ;
                    else if (DC_VALUE < 12) {
                        DC_VALUE++;
                        OCR1AL = duty[DC_VALUE];
                    }
                }
                if (PIND == 0b11111011) {
                        _delay_ms(200) ;
                        if (DC_VALUE > 0) {
                            DC_VALUE--;
                            OCR1AL = duty[DC_VALUE];
                    }
                }
            if (PIND == 0b01111111) {  //PD7
                MODE = 2;
            }
            //return 0;
        }
        while (MODE == 2) {
            
            ADCSRA |= (1 <<ADSC);
            
            //Read the potentiometer value
            while (ADCSRA & (1 << ADSC)) ;
            uint16_t potentiometerValue = ADC;  //Store the ADC result
                
            DC_VALUE = (uint8_t)((potentiometerValue*12)/1023);
                
            OCR1AL = duty[DC_VALUE];
            
            if (PIND == 0b10111111) {    //PD6
            MODE = 1;
            }
        }
    }
    return 0;
}