/* 
 * File:   delay.h
 * Author: nour
 *
 * Created on October 22, 2025, 3:34 PM
 * @Brief: This header contains configurations for the delay soruce.
 */

#ifndef DELAY_H
#define	DELAY_H

/****************** include **********************/
#include "../MCAL_layer/Timer0/mcal_tmr0.h"
#include "../MCAL_layer/Timer1/mcal_tmr1.h"
#include "../MCAL_layer/Timer2/mcal_tmr2.h"
#include "../MCAL_layer/Timer3/mcal_tmr3.h"

/***************** macros  **********************/
#define _TMR0 0
#define _TMR1 1
#define _TMR2 2
#define _TMR3 3

/**************** function macros **************/





/*************** data types *******************/





/************** function prototypes **********/
/* @Brief: These functions are used in case a TMRx_delayms() is called */
void __TMR0_delay(void); 
void __TMR1_delay(void);
void __TMR2_delay(void);
void __TMR3_delay(void);

STD_ReturnType TMR0_delayms(uint32 delay);
STD_ReturnType TMR1_delayms(uint32 delay);
STD_ReturnType TMR2_delayms(uint32 delay);
STD_ReturnType TMR3_delayms(uint32 delay);
#endif	/* DELAY_H */

