#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "keypad.h"
#include "twi-pca.h"
#include "LCD.h"
#include "potentiometers.h"
#include "temperature.h"
#include "usart.h"

char status[20] = "OK"; 
int nurse_status = 0;

char payload[1000] = "";

void status(long pressure, long temperature) {
    char pressedKey;
    if (scan_keypad_rising_edge()) {
       pressedKey = keypad_to_ascii();
    }
    
    if (pressedKey == '0' || nurse_status == 1) {
        strcpy(status, "NURSECALL");
        nurse_status = 1;
    }
    if (pressedKey == '#') {
        strcpy(status, "OK");
        nurse_status = 0;

    } else if ((pressure >= 12 || pressure < 4) && nurse_status == 0) {
        strcpy(status, "CHECKPRESSURE");
    } else if ((temperature < 34 || temperature >= 37) && nurse_status == 0) {
        strcpy(status, "CHECKTEMP");
    } else if (nurse_status == 0) {
        strcpy(status, "OK");
    }
}

int main(void) {

    usart_init(103); 
    twi_init();
    PCA9555_0_write(REG_CONFIGURATION_0, 0x00); //Set EXT_PORT0 as output
    PCA9555_0_write(REG_OUTPUT_0, 0x00);

    LCD_init();
    _delay_ms(100);

    usartcheck1("ESP:restart");
    
    _delay_ms(1500);
    LCD_clear_display();
    
    char text[15];

    if (usartcheck1("ESP:connect")) {
        strcpy(text, "1.Success\0");
    } else {
        strcpy(text, "1.Fail\0");
    }

    LCD_string(text);

    _delay_ms(1500);
    LCD_clear_display();

    if (usartcheck4("ESP:url:\"http://192.168.1.250:5000/data\"")) {
        strcpy(text, "2.Success\0");
    } else {
        strcpy(text, "2.Fail\0");
    }

    LCD_string(text);

    _delay_ms(1000); 

    LCD_init();
    _delay_ms(100);

    while (1) {
        LCD_init();
        _delay_ms(1000);
        LCD_clear_display();
        long temperature = 0;
        long IntTemp = 0;
        long DecTemp = 0;

        uint16_t pressure = get_pressure();
        int16_t rawTemp = get_temperature();
        long final_pressure = pressure * 20;
        final_pressure /= 1023;
        char buffer[32];
        
        if (rawTemp != 0x8000) {
            temperature = ((long) rawTemp * 625) / 10; // Convert to 3 decimal places

            // Extract integer and decimal parts
            IntTemp = (temperature / 1000) + 12; // Integer part
            DecTemp = abs(temperature % 1000); // Decimal part

            status(final_pressure, temperature / 1000 + 12);

            sprintf(buffer, "T=%02ld.%03ld P=%ld", IntTemp, DecTemp, final_pressure);
            LCD_string(buffer);
            LCD_command(0xC0);
            _delay_ms(2);
            LCD_string(status);
            _delay_ms(1500);
            LCD_clear_display();

        } else {
            // If no device is found, display "NO DEVICE"
            status(final_pressure, temperature / 1000 + 12);

            sprintf(buffer, "NO DEVICE P=%ld", final_pressure);
            LCD_string(buffer);
            LCD_command(0xC0);
            _delay_ms(2);
            LCD_string(status);
            _delay_ms(1500);
            LCD_clear_display();
        }

        sprintf(payload, "ESP:payload:[{\"name\" : \"temperature\", \"value\" : \"%03ld.%03ld\"},"
                 "{\"name\" : \"pressure\", \"value\" : \"%ld\"},"
                 "{\"name\" : \"team\", \"value\": \"30\"},"
                 "{\"name\": \"status\",\"value\": \"%s\"}]",
                 IntTemp, DecTemp, final_pressure, status);

        usart_init(103);    
        char *pay;
        pay = payload;

        if (usartcheck2(pay)) {
            strcpy(text, "3.Success");
        } else {
            strcpy(text, "3.Fail");
        }

        LCD_string(text);
        
        _delay_ms(1000);
        LCD_clear_display();
        _delay_ms(100);
        
        usartcheck3("ESP:transmit");
        _delay_ms(1000);


    }
}