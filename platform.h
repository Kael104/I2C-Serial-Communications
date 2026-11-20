/**
 * @file  platform.h
 * @brief Declarations for platform-support routines
 *
 * @author Alberto de Villa <alberto.de.villa@eee.upd.edu.ph>
 * @date   28 Oct 2024
 * 
 * ("Updated" by Allen)
 */

#if !defined(EEE158_EX05_PLATFORM_H_) 
#define EEE158_EX05_PLATFORM_H_

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>

// C linkage should be maintained
#ifdef __cplusplus
extern "C" {
#endif

/*
 *      I2C Master functions
 *      Initialization --  initialize SERCOM2 to I2C Master, smart mode enabled
 *      Read -- polled read from addr into buffer of length len
 * 			    return: number of characters sent
 *      Write -- polled write to addr using buffer of length len
 * 				return: number of characters read
 *      Simple API!
 */

void SERCOM2_I2C_Initialize(void);
uint16_t SERCOM2_I2C_Write_Polled(uint8_t, uint8_t *, uint16_t);
uint16_t SERCOM2_I2C_Read_Polled(uint8_t, uint8_t *, uint16_t);

//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	//
