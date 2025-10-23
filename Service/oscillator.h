/* 
 * File:   oscillator.h
 * Author: nour
 *
 * Created on October 23, 2025, 4:00 PM
 */

#ifndef OSCILLATOR_H
#define	OSCILLATOR_H

/****************** include **********************/
#include <xc.h>
#include "../MCAL_layer/Timer1/mcal_tmr1.h"

/***************** macros  **********************/
#define _XTAL
#define _XTAL_FREQ _LP_FREQ /* The defined crystal clock frequency */
#define _LP_FREQ 32768

/************** function prototypes **********/
STD_ReturnType LP_XTAL_initialize(void);
STD_ReturnType LP_XTAL_select(void);
STD_ReturnType Primary_OSC_select(void); 





#endif	/* OSCILLATOR_H */

