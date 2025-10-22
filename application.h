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
#include "Service/delay.h"

/******** Data types ****************/

/******* Function declarations ******/ 
//I want to modify this for error checking later
STD_ReturnType application_initialize(void);
#endif	/* APPLICATION_H */
void __RB_HIGH(void);
void __RB_LOW(void);
void __INT0(void);
void __INT1(void);
void __INT2(void);
void __ADC(uint16 * result);
void __TMR0(void);
void __CCP1(void);
