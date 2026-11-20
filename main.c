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
    // set up output LEDs
    mx[0] = 0;
    mx[1] = 0;
	SERCOM2_I2C_Write_Polled(0x25, mx, 2);
    mx[0] = 0xA;
    // mx[1] = ~(0x9D);
    mx[1] = (uint8_t)~'K';
    
    SERCOM2_I2C_Write_Polled(0x25, mx, 2);
	
    while(1) {
        // infinite loop; does nothing for now       
    }
    
    // This line must never be reached
    return 1;
}
