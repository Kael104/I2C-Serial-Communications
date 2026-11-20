
#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "platform.h"

// Debug variables: inspect these using MPLAB X Watch
volatile uint16_t iodir_result = 0;
volatile uint16_t olat_result = 0;

volatile uint16_t i2c_status_debug = 0;
volatile uint8_t i2c_flags_debug = 0;
volatile uint16_t i2c_baud_debug = 0;
volatile uint32_t i2c_ctrl_debug = 0;

int main(void)
{
    // Initialization
    SERCOM2_I2C_Initialize();

    uint8_t mx[2];

    // Configure all eight I/O pins as outputs
    mx[0] = 0x00;   // IODIR register
    mx[1] = 0x00;   // All pins are outputs

    iodir_result = SERCOM2_I2C_Write_Polled(0x25, mx, 2);

    // Display ASCII character 'K'
    mx[0] = 0x0A;   // OLAT register
    mx[1] = 0xB4;   // Active-low representation of ASCII K

    olat_result = SERCOM2_I2C_Write_Polled(0x25, mx, 2);

    // Capture peripheral registers
    i2c_status_debug =
        SERCOM2_REGS->I2CM.SERCOM_STATUS;

    i2c_flags_debug =
        SERCOM2_REGS->I2CM.SERCOM_INTFLAG;

    i2c_baud_debug =
        SERCOM2_REGS->I2CM.SERCOM_BAUD;

    i2c_ctrl_debug =
        SERCOM2_REGS->I2CM.SERCOM_CTRLA;
    while (1)
    {
        // Application idle loop
    }

    return 1;
}
