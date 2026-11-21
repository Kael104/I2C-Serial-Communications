/*
 * main.c
 * I2C Exercise Reference
 *
 * Maintainer: Allen Tan
 * For any bugs, please send an e-mail to allen.jason.tan@eee.upd.edu.ph
 * Updated: 23 November 2025
 */

// Common include for the XC32 compiler
#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "platform.h"

/////////////////////////////////////////////////////////////////////////////


int main(void)
    {
	// Initialization	
	SERCOM2_I2C_Initialize();
    
    uint8_t mx[2];
    uint8_t input_state;
    uint8_t previous_state = 0xFF;
    uint8_t reg;
    uint16_t count;
    // set up output LEDs
    mx[0] = 0;
    mx[1] = 0;
	SERCOM2_I2C_Write_Polled(0x25, mx, 2);
    
    mx[0] = 0xA; // 0x09
    // mx[1] = ~(0x9D);
    mx[1] = (uint8_t)~'K';
    SERCOM2_I2C_Write_Polled(0x25, mx, 2);
    
    // I/O Expander 2 (0x24): configure all pins as inputs.
    mx[0] = 0x00;       // IODIR
    mx[1] = 0xFF;       // 1 = input
    SERCOM2_I2C_Write_Polled(0x24, mx, 2);
	
    // Enable the internal pull-up resistors on all input pins.
    mx[0] = 0x06;       // GPPU
    mx[1] = 0xFF;
    SERCOM2_I2C_Write_Polled(0x24, mx, 2);
    while (1) {
        // Select the GPIO register on I/O Expander 2.
        reg = 0x09;
        count = SERCOM2_I2C_Write_Polled(0x24, &reg, 1);

        // Read the eight button/joystick input states.
        if (count == 1) {
            count = SERCOM2_I2C_Read_Polled(0x24, &input_state, 1);

            if (count == 1 && input_state != previous_state) {
                // Both the inputs and LEDs are active-low:
                // a pressed switch reads 0 and turns its corresponding LED on.
                mx[0] = 0x0A;       // OLAT on I/O Expander 1
                mx[1] = input_state;
                SERCOM2_I2C_Write_Polled(0x25, mx, 2);
                previous_state = input_state;
            }
        }
    }
    
    // This line must never be reached
    return 1;
}
