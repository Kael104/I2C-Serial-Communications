#include "platform.h"

#include <xc.h>

/*
    Initialization function based off code generated through Microchip Code Configurator. (c) Microchip Inc.
    Read/Write functions written by Allen.
*/

void SERCOM2_I2C_Clock_Init() {
    // All APB C bus peripheral clocks are enabled by default, so this isn't necessary
	// MCLK_REGS->MCLK_APBCMASK |= (1 << 3);
	
	// Set clock to Default GEN0 (4MHz)
	GCLK_REGS->GCLK_PCHCTRL[19] = 0x00000040;
	while ((GCLK_REGS->GCLK_PCHCTRL[19] & 0x00000040) == 0)
		asm("nop");
}

void SERCOM2_I2C_Abort();

void SERCOM2_I2C_Initialize(void)
{
    SERCOM2_I2C_Clock_Init();
    
    /* Reset the module */
    SERCOM2_REGS->I2CM.SERCOM_CTRLA = SERCOM_I2CM_CTRLA_SWRST_Msk ;

    /* Wait for synchronization */
    while((SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }

    /* Enable smart mode, set smart mode response to ACK */
    SERCOM2_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_SMEN_Msk;

    /* Wait for synchronization */
    while((SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }

    /* Baud rate - Master Baud Rate*/
    SERCOM2_REGS->I2CM.SERCOM_BAUD = 0xE8;
    
    // enable interrupts for ERROR, SB, MB (but not their NVIC interrupt lines)
    SERCOM2_REGS->I2CM.SERCOM_INTENSET |= SERCOM_I2CM_INTENSET_MB(1) | SERCOM_I2CM_INTENSET_SB(1) | SERCOM_I2CM_INTENSET_ERROR(1);
    
    // configure pins here
    // PA12 (SDA) and PA13 (SCL)
    PORT_SEC_REGS->GROUP[0].PORT_PMUX[6] = 0x22;
	PORT_SEC_REGS->GROUP[0].PORT_PINCFG[13] = 0x03;
	PORT_SEC_REGS->GROUP[0].PORT_PINCFG[12] = 0x03;

    /* Set Operation Mode (Master), SDA Hold time, run in stand by and i2c master enable */
    SERCOM2_REGS->I2CM.SERCOM_CTRLA = SERCOM_I2CM_CTRLA_INACTOUT(0x2) | SERCOM_I2CM_CTRLA_MODE_I2C_MASTER | SERCOM_I2CM_CTRLA_SDAHOLD_75NS | SERCOM_I2CM_CTRLA_SPEED_SM | SERCOM_I2CM_CTRLA_SCLSM(0UL) | SERCOM_I2CM_CTRLA_ENABLE_Msk;

    /* Wait for synchronization */
    while((SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }

    /* Initial Bus State: IDLE */
    SERCOM2_REGS->I2CM.SERCOM_STATUS = (uint16_t)SERCOM_I2CM_STATUS_BUSSTATE(0x01UL);

    /* Wait for synchronization */
    while((SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }
}

uint16_t SERCOM2_I2C_Write_Polled(uint8_t addr, uint8_t *buf, uint16_t len) {
    uint8_t flags = 0;
    uint16_t pos = 0;
    uint32_t temp = 0;
    // Step 0: Wait for the bus to become idle
    // Either wait for the bus to become idle or break if not idle

    // Step 1: Initialize transfer by writing to ADDR (ADDR[0] indicates direction)

    // Step 2: Wait for and process interrupt flags
    // if error, abort and exit
       
    // Step 3: Write data, wait for ACKs
    //for (pos; pos < len; pos++) {
        // write data

        // check for ack

        // if error, abort and exit
    //}
    
    // Step 4: Issue stop condition and return state back to idle

    //return pos;

    // Maximum polling time to prevent infinite loops
    const uint32_t TIMEOUT = 1000000UL;

    // Validate input
    if ((addr > 0x7F) || (buf == NULL && len > 0))
        return 0;

    // Step 0: Wait for the bus to become idle
    temp = 0;

    while ((SERCOM2_REGS->I2CM.SERCOM_STATUS &
            SERCOM_I2CM_STATUS_BUSSTATE_Msk) !=
            SERCOM_I2CM_STATUS_BUSSTATE(0x01UL))
    {
        if (++temp >= TIMEOUT)
        {
            SERCOM2_I2C_Abort();
            return 0;
        }
    }

    // Step 1: Initialize transfer by writing to ADDR
    // Bit 0 = 0 means WRITE
    // Bits 7:1 contain the 7-bit I2C address

    SERCOM2_REGS->I2CM.SERCOM_ADDR = (uint32_t)(addr << 1);

    // Step 2: Wait for and process interrupt flags
    temp = 0;

    while (1)
    {
        flags = SERCOM2_REGS->I2CM.SERCOM_INTFLAG;

        if (flags & SERCOM_I2CM_INTFLAG_ERROR_Msk)
        {
            SERCOM2_I2C_Abort();
            return pos;
        }

        if (flags & SERCOM_I2CM_INTFLAG_MB_Msk)
            break;

        if (++temp >= TIMEOUT)
        {
            SERCOM2_I2C_Abort();
            return pos;
        }
    }

    // Check for arbitration loss or bus error
    if (SERCOM2_REGS->I2CM.SERCOM_STATUS &
        (SERCOM_I2CM_STATUS_ARBLOST_Msk |
         SERCOM_I2CM_STATUS_BUSERR_Msk))
    {
        SERCOM2_I2C_Abort();
        return pos;
    }

    // Check whether the address was acknowledged
    if (SERCOM2_REGS->I2CM.SERCOM_STATUS &
        SERCOM_I2CM_STATUS_RXNACK_Msk)
    {
        // Address was not acknowledged
        // Issue STOP condition

        SERCOM2_REGS->I2CM.SERCOM_CTRLB |=
            SERCOM_I2CM_CTRLB_CMD(0x3UL);

        while (SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY &
               SERCOM_I2CM_SYNCBUSY_SYSOP_Msk)
        {
        }

        return pos;
    }

    // Step 3: Write data and wait for ACKs
    for (pos = 0; pos < len; )
    {
        // Write one byte to the DATA register
        SERCOM2_REGS->I2CM.SERCOM_DATA = buf[pos];

        temp = 0;

        // Wait for the host-on-bus flag
        while (1)
        {
            flags = SERCOM2_REGS->I2CM.SERCOM_INTFLAG;

            if (flags & SERCOM_I2CM_INTFLAG_ERROR_Msk)
            {
                SERCOM2_I2C_Abort();
                return pos;
            }

            if (flags & SERCOM_I2CM_INTFLAG_MB_Msk)
                break;

            if (++temp >= TIMEOUT)
            {
                SERCOM2_I2C_Abort();
                return pos;
            }
        }

        // Check for arbitration loss or bus error
        if (SERCOM2_REGS->I2CM.SERCOM_STATUS &
            (SERCOM_I2CM_STATUS_ARBLOST_Msk |
             SERCOM_I2CM_STATUS_BUSERR_Msk))
        {
            SERCOM2_I2C_Abort();
            return pos;
        }

        // Check whether the data byte was acknowledged
        if (SERCOM2_REGS->I2CM.SERCOM_STATUS &
            SERCOM_I2CM_STATUS_RXNACK_Msk)
        {
            // Client did not acknowledge the data
            break;
        }

        // Byte successfully transmitted and acknowledged
        pos++;
    }

    // Step 4: Issue STOP condition
    SERCOM2_REGS->I2CM.SERCOM_CTRLB |=
        SERCOM_I2CM_CTRLB_CMD(0x3UL);

    // Wait for STOP command synchronization
    temp = 0;

    while (SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY &
           SERCOM_I2CM_SYNCBUSY_SYSOP_Msk)
    {
        if (++temp >= TIMEOUT)
        {
            SERCOM2_I2C_Abort();
            return pos;
        }
    }

    // Wait until the bus returns to IDLE
    temp = 0;

    while ((SERCOM2_REGS->I2CM.SERCOM_STATUS &
            SERCOM_I2CM_STATUS_BUSSTATE_Msk) !=
            SERCOM_I2CM_STATUS_BUSSTATE(0x01UL))
    {
        if (++temp >= TIMEOUT)
        {
            SERCOM2_I2C_Abort();
            return pos;
        }
    }

    return pos;
}

}

uint16_t SERCOM2_I2C_Read_Polled(uint8_t addr, uint8_t *buf, uint16_t len) {
    uint8_t flags = 0;
    uint16_t pos = 0;
    uint32_t temp = 0;
    // Step 0: Wait for the bus to become idle
    // Either wait for the bus to become idle or break if not idle

    // Step 1: Initialize transfer by writing to ADDR (ADDR[0] indicates direction)

    // Step 2: Wait for and process interrupt flags
    // if error, abort and exit
    
    // Step 3: Read data, respond with ACKs (I2C smart mode automatically sends an ACK when DATA is read)
    for (pos; pos < len; pos++) {
        // wait for data
        // first iteration of the loop will skip waiting because data's already there

        // read data
        // Send ack or nack+stop depending on how many bytes are left

        // if error, abort and exit
    }
    
    // Step 4: Issue stop condition and return state back to idle

    return pos;
}

void SERCOM2_I2C_Abort() {
    // turn it off and on again
    SERCOM2_REGS->I2CM.SERCOM_CTRLA &= ~SERCOM_I2CM_CTRLA_ENABLE_Msk;
    while((SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY) != 0);

    SERCOM2_REGS->I2CM.SERCOM_CTRLA |= SERCOM_I2CM_CTRLA_ENABLE_Msk;
    while((SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY) != 0);

    // reinitialize state to idle
    SERCOM2_REGS->I2CM.SERCOM_STATUS = (uint16_t)SERCOM_I2CM_STATUS_BUSSTATE(0x01UL);
    while((SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY) != 0);    
}