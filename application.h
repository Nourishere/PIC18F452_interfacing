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
/******** Data types ****************/
extern seven_segment_t segment1;
extern keypad_t keypad1;
extern chr_LCD_t LCD1;
extern LED_t LED1;
/******* Function declarations ******/ 
//I want to modify this for error checking later
void application_initialize();
#endif	/* APPLICATION_H */

