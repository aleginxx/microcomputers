#include <avr/io.h>
#define F_CPU   16000000UL 
#include <util/delay.h>
#define cbi(reg,bit) (reg &= ~(1 << bit))
#define sbi(reg,bit) (reg |= (1 << bit))


uint8_t one_wire_reset() { 
    
    sbi(DDRD,PD4);
    
    cbi(PORTD,PD4);
    _delay_us(480);
    
    cbi(DDRD,PD4);
    cbi(PORTD,PD4);
    
    _delay_us(100);
    
    uint8_t temp = PIND;
    
    _delay_us(380);
    
    temp = (temp & 0x10);
    uint8_t res = 0x00;
    
    if (temp == 0x00)
        res = 0x01;
    
    return res;
}

void one_wire_transmit_bit(uint8_t bit) {
    
    sbi(DDRD,PD4);
    
    cbi(PORTD,PD4);
    _delay_us(2);
    
    if (bit == 0x01) sbi(PORTD,PD4);
    if (bit == 0x00) cbi(PORTD,PD4);
    
    _delay_us(58);
    
    cbi(DDRD,PD4);
    cbi(PORTD,PD4);
    
    _delay_us(1);
    return;
}

void one_wire_transmit_byte(uint8_t byte) {
    
    uint8_t bit;
    uint8_t i = 0x08;
    
    while(i != 0){
        bit = (byte & 0x01);
        one_wire_transmit_bit(bit);
        byte = (byte >> 1);
        i--;
    }
    
    return;
}

uint8_t one_wire_receive_bit() {
    uint8_t bit,temp;
    
    sbi(DDRD,PD4);
    cbi(PORTD,PD4);
    
    _delay_us(2);
    
    cbi(DDRD,PD4);
    cbi(PORTD,PD4);
    
    _delay_us(10);
    
    temp = (PIND & 0x10);
    bit = 0x00;
    if (temp == 0x10) bit = 0x01;
    _delay_us(49);
    
    return bit;
}

uint8_t one_wire_receive_byte() {    
    uint8_t bit;
    uint8_t byte = 0x00;
    uint8_t i = 0x08;
    
    while(i != 0){
        bit = one_wire_receive_bit();
        byte = (byte >> 1);
        if (bit == 0x01) bit = 0x80;
        byte = (byte | bit);
        i--;
    }
    
    return byte;
}

int main(void)
{
    // Set DDRB and DDRD registers for output
    DDRB = 0xFF;
    DDRD = 0xFF;
    
    // Declare variables for temperature processing
    uint8_t temperatureLow, temperatureHigh, temperatureSign;
    uint16_t temperatureFinal, temperatureHigh16, temperatureFinalOutput;
    
    while (1)
    {
        // Check if device is connected
        if (!one_wire_reset()) {
            temperatureFinalOutput = 0x8000;
            continue;
        }
        
        // Send commands to request temperature reading
        one_wire_transmit_byte(0xCC);       // Send command 0xCC
        one_wire_transmit_byte(0x44);       // Send command 0x44
        
        // Wait for temperature conversion to complete
        while(one_wire_receive_bit() != 0x01);
        
        // Recheck if device is connected
        if (!one_wire_reset()) {
            temperatureFinalOutput = 0x8000;
            continue;
        }
        
        // Send commands to read temperature value
        one_wire_transmit_byte(0xCC);       // Send command 0xCC
        one_wire_transmit_byte(0xBE);       // Send command 0xBE
        
        // Receive temperature bytes
        temperatureLow = one_wire_receive_byte();
        temperatureHigh = one_wire_receive_byte();
        temperatureSign = temperatureHigh & 0xF8;
        temperatureHigh16 = temperatureHigh << 8;
        temperatureFinal = temperatureHigh16 + temperatureLow;
        
        // Check if temperature is negative or positive
        if (temperatureSign == 0xF8)
            temperatureFinalOutput = ~(temperatureFinal) + 1;   // Two's complement
        else
            temperatureFinalOutput = temperatureFinal;
    }
}