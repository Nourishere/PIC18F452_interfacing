/* 
 * File:   mcal_eeprom.h
 * Author: nour
 *
 * Created on April 29, 2025, 2:28 PM
 */

#ifndef MCAL_EEPROM_H
#define	MCAL_EEPROM_H

/****** includes **********/
#include <xc.h>
#include "mcal_eeprom_cfg.h"
#include "../Interrupt/mcal_interrupt_config.h"
#include "../std_libs.h"
#include "../mcal_std_types.h"
/****** Macros ***********/
/* Setting this bit to 0 allows access to EEPROM data
 * While setting it to 1 allows flash data access */
#define EEPROM_DATA_ACCESS() EECON1bits.EEPGD=0
/* Setting this bit to 0 allows access to EEPROM or flash 
 * instead of accessing configuration bits */
#define EEPROM_FLAEEP_DATACC() EECON1bits.CFGS=0 
/* Setting this bits initializes writing to the EEPROM.
 * Clering this bit is done via hardware */
#define EEPROM_WR_EN() EECON1bits.WR=1
/* Allows writing to the EEPROM */
#define EEPROM_ALLOW_WR_CYCLES() EECON1bits.WREN=1
/* Denies writing to the EEPROM */
#define EEPROM_DENY_WR_CYCLES() EECON1bits.WREN=0
/* Allows reading from the EEPROM */
#define EEPROM_ALLOW_RD_CYLES() EECON1bits.RD=1
/* Denies reading from the EEPROM */
#define EEPROM_DENY_RD_CYLES() EECON1bits.RD=0
/* Writes addr to EEADD */
#define EEPROM_WR_ADDR(addr) EEADR=(uint8)addr
/* Writes data to EEDATA */
#define EEPROM_WR_DATA(addr) EEDATA=(uint8)addr
/* Write to the EECON1 register */
#define EECON2_WR(val) EECON2=(uint8)val
/****** Function declarations ******/

STD_ReturnType EEPROM_read(uint8 addr, uint8* data);
STD_ReturnType EEPROM_write(uint8 addr, uint8 data);
STD_ReturnType EEPROM_erase(void);

#endif	/* MCAL_EEPROM_H */

