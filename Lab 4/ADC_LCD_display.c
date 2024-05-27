#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

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

int main(){
    
    DDRB = 0xFF;                    //Set PORTB as output
    DDRD = 0xFF;                    //Set PORTD as output
    DDRC = 0x00;                    //Set PORTC as input
    
    // Initialize the ADC for analog input
    ADMUX = (1 << REFS0) | (1 << MUX1);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = 0x00;
    DIDR0 = ~(1 << ADC2D);
    
    while(1){
        
        // Initialize the LCD
        LCD_init();
        
        // Start ADC conversion
        ADCSRA |= (1 << ADSC);
        
        // Wait for ADC conversion to complete
        while ((ADCSRA & (1 << ADSC)) == (1 << ADSC));
        
        // Calculate voltage from ADC value
        float adc = ADC;
        float voltage = adc * 5 / 1024;
        
         // Extract individual digits of the voltage
        int voltage_msb = (uint8_t)(voltage);
        int voltage_first_dec = (voltage - voltage_msb)*10;
        voltage_first_dec = (uint8_t)(voltage_first_dec);
        int voltage_second_dec = (((voltage - voltage_msb)*10) - voltage_first_dec)*10;
        voltage_second_dec = (uint8_t)(voltage_second_dec);
        
        // Convert digits to ASCII characters
        voltage_msb |= 0x30;
        voltage_first_dec |= 0x30;
        voltage_second_dec |= 0x30;
        
        // Display voltage on the LCD
        LCD_data(voltage_msb);
        LCD_data('.');
        LCD_data(voltage_first_dec);
        LCD_data(voltage_second_dec);

    }
}