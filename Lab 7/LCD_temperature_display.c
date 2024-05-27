#include <avr/io.h>
#define F_CPU   16000000UL 
#include <util/delay.h>

#define cbi(reg,bit) (reg &= ~(1 << bit))
#define sbi(reg,bit) (reg |= (1 << bit))

#define PCA9555_0_ADDRESS 0x40 //A0=A1=A2=0 by hardware
#define TWI_READ 1 // reading from twi device
#define TWI_WRITE 0 // writing to twi device
#define SCL_CLOCK 100000L // twi clock in Hz
//Fscl=Fcpu/(16+2*TWBR0_VALUE*PRESCALER_VALUE)
#define TWBR0_VALUE ((F_CPU/SCL_CLOCK)-16)/2

// PCA9555 REGISTERS
typedef enum {
    REG_INPUT_0 = 0,
            
    REG_INPUT_1 = 1,
            
    REG_OUTPUT_0 = 2,
            
    REG_OUTPUT_1 = 3,
            
    REG_POLARITY_INV_0 = 4,
            
    REG_POLARITY_INV_1 = 5,
            
    REG_CONFIGURATION_0 = 6,
            
    REG_CONFIGURATION_1 = 7,
            
} PCA9555_REGISTERS;

//----------- Master Transmitter/Receiver -------------------
#define TW_START 0x08
#define TW_REP_START 0x10

//---------------- Master Transmitter ----------------------
#define TW_MT_SLA_ACK 0x18
#define TW_MT_SLA_NACK 0x20
#define TW_MT_DATA_ACK 0x28

//---------------- Master Receiver ----------------
#define TW_MR_SLA_ACK 0x40
#define TW_MR_SLA_NACK 0x48
#define TW_MR_DATA_NACK 0x58

#define TW_STATUS_MASK 0b11111000
#define TW_STATUS (TWSR0 & TW_STATUS_MASK)

uint8_t buffer;

//initialize TWI clock
void twi_init(void)
{
    TWSR0 = 0; // PRESCALER_VALUE=1
    TWBR0 = TWBR0_VALUE; // SCL_CLOCK 100KHz
}


// Read one byte from the twi device ( request more data from device)
unsigned char twi_readAck(void)
{
    TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    while(!(TWCR0 & (1<<TWINT)));
    return TWDR0;
}


// Issues a start condition and sends address and transfer direction.
// return 0 = device accessible, 1= failed to access device
unsigned char twi_start(unsigned char address)
{
    uint8_t twi_status;
    // send START condition
    TWCR0 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    // wait until transmission completed
    while(!(TWCR0 & (1<<TWINT)));
    // check value of TWI Status Register.
    twi_status = TW_STATUS & 0xF8;
    if ( (twi_status != TW_START) && (twi_status != TW_REP_START)) return 1;
    // send device address
    TWDR0 = address;
    TWCR0 = (1<<TWINT) | (1<<TWEN);
    // wail until transmission completed and ACK/NACK has been received
    while(!(TWCR0 & (1<<TWINT)));
    // check value of TWI Status Register.
    twi_status = TW_STATUS & 0xF8;
    if ( (twi_status != TW_MT_SLA_ACK) && (twi_status != TW_MR_SLA_ACK) )
    {
        return 1;
    }
    return 0;
}

// Send start condition, address, transfer direction.
// Use ack polling to wait until device is ready
void twi_start_wait(unsigned char address)
{
    uint8_t twi_status;
    while ( 1 )
    {
        // send START condition
        TWCR0 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
        
        // wait until transmission completed
        while(!(TWCR0 & (1<<TWINT)));
        
        // check value of TWI Status Register.
        twi_status = TW_STATUS & 0xF8;
        if ( (twi_status != TW_START) && (twi_status != TW_REP_START)) continue;
        
        // send device address
        TWDR0 = address;
        TWCR0 = (1<<TWINT) | (1<<TWEN);
        
        // wail until transmission completed
        while(!(TWCR0 & (1<<TWINT)));
        
        // check value of TWI Status Register.
        twi_status = TW_STATUS & 0xF8;
        if ( (twi_status == TW_MT_SLA_NACK )||(twi_status ==TW_MR_DATA_NACK) )
        {
            /* device busy, send stop condition to terminate write operation */
            TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
            
            // wait until stop condition is executed and bus released
            while(TWCR0 & (1<<TWSTO));
            
            continue;
        }
        break;
    }
}

// Send one byte to twi device, Return 0 if write successful or 1 if write failed
unsigned char twi_write( unsigned char data )
{
    // send data to the previously addressed device
    TWDR0 = data;
    TWCR0 = (1<<TWINT) | (1<<TWEN);
    // wait until transmission completed
    
    while(!(TWCR0 & (1<<TWINT)));
    if( (TW_STATUS & 0xF8) != TW_MT_DATA_ACK) return 1;
    return 0;
}

// Send repeated start condition, address, transfer direction
//Return: 0 device accessible
// 1 failed to access device

unsigned char twi_rep_start(unsigned char address)
{
    return twi_start( address );
}

// Terminates the data transfer and releases the twi bus
void twi_stop(void)
{
    // send stop condition
    TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
    // wait until stop condition is executed and bus released
    while(TWCR0 & (1<<TWSTO));
}

unsigned char twi_readNak(void)
{
    TWCR0 = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR0 & (1<<TWINT)));
    
    return TWDR0;
}

void PCA9555_0_write(PCA9555_REGISTERS reg, uint8_t value)
{
    twi_start_wait(PCA9555_0_ADDRESS + TWI_WRITE);
    twi_write(reg);
    twi_write(value);
    twi_stop();
}

uint8_t PCA9555_0_read(PCA9555_REGISTERS reg)
{
    uint8_t ret_val;
    
    twi_start_wait(PCA9555_0_ADDRESS + TWI_WRITE);
    twi_write(reg);
    twi_rep_start(PCA9555_0_ADDRESS + TWI_READ);
    ret_val = twi_readNak();
    twi_stop();
    
    return ret_val;
}

void write_2_nibbles(uint8_t input){
    
    uint8_t temp = input;
    uint8_t pin = buffer;
    pin &= 0x0F;
    input &=0xF0;
    input |= pin;
    buffer = input;
    buffer |= 0x08;
    PCA9555_0_write(REG_OUTPUT_0, buffer);

    buffer &= 0xF7;
    PCA9555_0_write(REG_OUTPUT_0, buffer);
    
    input = temp;
    input &= 0x0F;
    input = input << 4;
    input |= pin;
    buffer = input;
    buffer |= 0x08;
    PCA9555_0_write(REG_OUTPUT_0, buffer);

    buffer &= 0xF7;
    PCA9555_0_write(REG_OUTPUT_0, buffer);

    return;
}

void LCD_data(uint8_t x){
    
    buffer |= 0x04;
    PCA9555_0_write(REG_OUTPUT_0, buffer);

    write_2_nibbles(x);
    _delay_us(250);
    return;
    
}

void LCD_command(uint8_t x){
    
    buffer &= 0xFB;
    PCA9555_0_write(REG_OUTPUT_0, buffer);

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
    
    buffer = 0x30;
    buffer |= 0x08;
    PCA9555_0_write(REG_OUTPUT_0, buffer);

    buffer &= 0xF7;
    PCA9555_0_write(REG_OUTPUT_0, buffer);
    _delay_us(250);
    
    buffer = 0x30;
    buffer |= 0x08;
    PCA9555_0_write(REG_OUTPUT_0, buffer);

    buffer &= 0xF7;
    PCA9555_0_write(REG_OUTPUT_0, buffer);
    _delay_us(250);
    
    buffer = 0x20;
    buffer |= 0x08;
    PCA9555_0_write(REG_OUTPUT_0, buffer);

    buffer &= 0xF7;
    PCA9555_0_write(REG_OUTPUT_0, buffer);
    _delay_us(250);
    
    LCD_command(0x28);
    LCD_command(0x0C);
    
    LCD_clear_display();
    
    LCD_command(0x06);
    
}

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

void displayNoDeviceMessage() {
    LCD_clear_display();
    _delay_ms(2);
    LCD_data('N');
    LCD_data('O');
    LCD_data(' ');
    LCD_data('D');
    LCD_data('E');
    LCD_data('V');
    LCD_data('I');
    LCD_data('C');
    LCD_data('E');
    _delay_ms(500);
}


int main(){
    
    twi_init();
    
    LCD_init(); // Initialize the LCD
    DDRB = 0xFF; // Set all pins of PORTB as output
    DDRD = 0xFF; // Set all pins of PORTD as output
    
     // Declare variables for temperature processing
    uint8_t temperatureLow, temperatureHigh, temperatureSign, temperatureDec;
    uint16_t temperatureFinal, temperatureHigh16, temperatureFinalOutput;
    
     while (1)
    {
         // Check if device is connected
        if (!one_wire_reset()) {
            displayNoDeviceMessage();
            continue;
        }
        
        // Send commands to request temperature reading
        one_wire_transmit_byte(0xCC);       // Send command 0xCC
        one_wire_transmit_byte(0x44);       // Send command 0x44
        
        // Wait for temperature conversion to complete
        while (one_wire_receive_bit() != 0x01);
        
        // Recheck if device is connected
        if (!one_wire_reset()) {
            displayNoDeviceMessage();
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
        //temperatureDec = temperatureLow & 0x0F;
        
        // Check if temperature is negative or positive
        if (temperatureSign == 0xF8)
            temperatureFinalOutput = ~(temperatureFinal) + 1;   // Two's compliment
        else
            temperatureFinalOutput = temperatureFinal;
        
        /* DECIMAL PART CALCULATE */

        temperatureDec = temperatureFinalOutput & 0x0F;

        int decDigit1 = 0;
        int decDigit2 = 0;
        int decDigit3 = 0;
        int decDigit4 = 0;
        int decSum = 0;
        uint8_t bitOne;

        bitOne = temperatureDec & 0x08;
        if (bitOne == 8)
            decSum += 5000;

        bitOne = temperatureDec & 0x04;
        if (bitOne == 4)
            decSum += 2500;

        bitOne = temperatureDec & 0x02;
        if (bitOne == 2)
            decSum += 1250;

        bitOne = temperatureDec & 0x01;
        if (bitOne == 1)
            decSum += 625;

        while (decSum >= 1000) {
            decDigit1++;
            decSum -= 1000;
        }

        while (decSum >= 100) {
            decDigit2++;
            decSum -= 100;
        }

        while (decSum >= 10) {
            decDigit3++;
            decSum -= 10;
        }

        while (decSum >= 1) {
            decDigit4++;
            decSum -= 1;
        }

        /* INTEGER PART CALCULATE */

        temperatureFinalOutput = temperatureFinalOutput >> 4;

        int intDigit100 = 0;
        int intDigit10 = 0;
        int intDigit1 = 0;

        while (temperatureFinalOutput >= 100) {
            intDigit100++;
            temperatureFinalOutput -= 100;
        }

        while (temperatureFinalOutput >= 10) {
            intDigit10++;
            temperatureFinalOutput -= 10;
        }

        while (temperatureFinalOutput >= 1) {
            intDigit1++;
            temperatureFinalOutput -= 1;
        }

        /* PRINT ON LCD */

        intDigit100 |= 0x30;
        intDigit10 |= 0x30;
        intDigit1 |= 0x30;
        decDigit1 |= 0x30;
        decDigit2 |= 0x30;
        decDigit3 |= 0x30;
        decDigit4 |= 0x30;

        LCD_command(0x01);
        _delay_ms(2);

        if (temperatureSign == 0xF8)
            LCD_data('-');
        else
            LCD_data('+');

        if (intDigit100 != 0x30)
            LCD_data(intDigit100);
        if (intDigit10 != 0x30)
            LCD_data(intDigit10);
        
        LCD_data(intDigit1);
        LCD_data('.');
        LCD_data(decDigit1);
        LCD_data(decDigit2);
        LCD_data(decDigit3);
        LCD_data(decDigit4);
        LCD_data('\xdf');
        LCD_data('C');
        _delay_ms(500);

    }
}