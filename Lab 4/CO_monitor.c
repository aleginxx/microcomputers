#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char level, flag;

void write_2_nibbles(uint8_t input){
    
    uint8_t temp = input;
    uint8_t pin = PIND;
    pin &= 0x0F;
    input &=0xF0;
    input |= pin;
    PORTD = input;
    PORTD |= 0x08;
    PORTD &= 0xF7;
    
    input = temp;
    input &= 0x0F;
    input = input << 4;
    input |= pin;
    PORTD = input;
    PORTD |= 0x08;
    PORTD &= 0xF7;
    
    return;
}

void LCD_data(uint8_t x){
    
    PORTD |= 0x04;
    write_2_nibbles(x);
    _delay_us(250);
    return;
    
}

void LCD_command(uint8_t x){
    
    PORTD &= 0xFB;
    write_2_nibbles(x);
    _delay_us(250);
    return;
    
}

void LCD_clear_display(){
    
    uint8_t x = 0x01;
    LCD_command(x);
    _delay_ms(5);
    
}


void LCD_init(void){
    
    _delay_ms(200);
    
    PORTD = 0x30;
    PORTD |= 0x08;
    PORTD &= 0xF7;
    _delay_us(250);
    
    PORTD = 0x30;
    PORTD |= 0x08;
    PORTD &= 0xF7;
    _delay_us(250);
    
    PORTD = 0x20;
    PORTD |= 0x08;
    PORTD &= 0xF7;
    _delay_us(250);
    
    LCD_command(0x28);
    LCD_command(0x0C);
    
    LCD_clear_display();
    
    LCD_command(0x06);
    
}

ISR(TIMER1_OVF_vect) {
			
	ADCSRA =0xEF;
}

ISR(ADC_vect) {	// check the CO level 
	
	if(21<=ADC && ADC<=45.2) {	//Cx = [0,14] ppm
		level = 0x01;
		PORTB = level;

		TCNT1H = 0xfc;		
		TCNT1L = 0xf3;		//Timer1 overflows in 100 msec
	}

    if(45.2<ADC && ADC<=90.4) { //Cx = [15,38] ppm
        level = 0x03;
        PORTB = level;
        
        TCNT1H = 0xfc;		
		TCNT1L = 0xf3;		//Timer1 overflows in 100 msec
    }
    
    if(90.4<ADC && ADC<=135.6) { //Cx = [29,42]
        level = 0x07;
        PORTB = level;
        
        TCNT1H = 0xfc;		
		TCNT1L = 0xf3;		//Timer1 overflows in 100 msec
    }
    
    if(135.6<ADC && ADC<=180) { //Cx = [29,42]
        level = 0x0F;
        PORTB = level;
        
        TCNT1H = 0xfc;		
		TCNT1L = 0xf3;		//Timer1 overflows in 100 msec
    } 

	if(180<ADC && ADC<=205) {	// Cx = [36,70] ppm
		level = 0x1F;
		PORTB = level;
		
		TCNT1H = 0xfc;		
		TCNT1L = 0xf3;		//Timer1 overflows in 100 msec
	}

	 if(205<ADC && ADC<=298 ) { // Cx = [71,105] ppm
		 level = 0xc;

		 TCNT1H = 0xfc;		
		 TCNT1L = 0xf3;		
		 	 
	}
	 
	if(298<ADC && ADC<=614) { 	// Cx = [105, ] ppm
		level = 0x30;

		TCNT1H = 0xfc;		
		TCNT1L = 0xf3;		//Timer1 overflows in 100 msec
	}
}

int main(){
    
    DDRB = 0xFF;
    DDRD = 0xFF;
    DDRC = 0x00;
    
    ADMUX = (1 << REFS0) | (1 << MUX1); //| (1 << MUX0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = 0x00;
    DIDR0 = ~(1 << ADC2D);
    
    TIMSK1 = (1 << TOIE1);		//Timer1 ,interrupt enable
    TCCR1B =(1<<CS12) | (0<<CS11) | (1<<CS10); //frequency of Timer1 8MHz/1024
		
	sei();	
    
    while(1){
        
        LCD_init();
        ADCSRA |= (1 << ADSC);
        
        if(flag==0x01) {
            LCD_data('C');
            LCD_data('L');
            LCD_data('E');
            LCD_data('A');
            LCD_data('R');
            _delay_ms(5000);
            LCD_clear_display();
            flag=0x00;
        }
        
        if((level==0x01) || (level==0x03) || (level==0x07) || (level==0x0F) || (level==0x1F)){
			PORTB = level;
		}
		
		if ((level==0xc) || (level==0x30)){
            LCD_data('G');
            LCD_data('A');
            LCD_data('S');
            LCD_data(' ');
            LCD_data('D');
            LCD_data('E');
            LCD_data('T');
            LCD_data('E');
            LCD_data('C');
            LCD_data('T');
            LCD_data('E');
            LCD_data('D');
	
            flag = 0x01;
            
            while ((level==0xc) || (level==0x30)){ // Cx = [71,105] ppm ALARM
				PORTB = 0xff;
				_delay_ms(500);
				PORTB = 0x00;
				_delay_ms(500);	
                
			}
		}
    }
}