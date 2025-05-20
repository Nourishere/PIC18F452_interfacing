/* 
 * File:   application.h
 * Author: nour
 *
 * Created on October 23, 2024, 7:29 AM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H
/******** Includes ******************/
#include "ECU_layer/ecu_init.h"
#include "MCAL_layer/std_libs.h"
#include "MCAL_layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_layer/EEPROM/mcal_eeprom.h"
/******** Data types ****************/
extern seven_segment_t segment1;
extern keypad_t keypad1;
extern chr_LCD_t LCD1;
extern LED_t LED_OK;
extern LED_t LED_NOK;
extern LED_t LED_main;
extern LED_t LED_INT_HIGHPR;
extern LED_t LED_INT_LOWPR;
/******* Function declarations ******/ 
//I want to modify this for error checking later
STD_ReturnType application_initialize();
#endif	/* APPLICATION_H */
void __RB_HIGH(void);
void __RB_LOW(void);
void __INT0(void);
void __INT1(void);
void __INT2(void);
