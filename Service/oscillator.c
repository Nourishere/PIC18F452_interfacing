/* 
 * File:   oscillator.c
 * Author: nour
 *
 * Created on October 23, 2025, 4:00 PM
 */

#include "oscillator.h"
char osc_initialized = 0;
const TMR1_t tmr1 = {
    .osc = STD_ON
};

/* @brief: Initialize the timer1 module to operate in oscillator mode.
 * @param: A pointer to a struct of type TMR1_t.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType LP_XTAL_initialize(void){
	STD_ReturnType ret = E_OK;
	ret = TMR1_initialize(&tmr1);
	osc_initialized = 1;
	return ret;
}

/* @brief: Select the Timer3 oscillator as the main oscillator.
 * @param: Nothing
 * @return: E_OK upon success and E_NOT_OK if the osc-timer struct is 
 * 		    not initialized.
 */
STD_ReturnType LP_XTAL_select(void){
	STD_ReturnType ret = E_OK;
	if (osc_initialized == 0)
		return E_NOT_OK;
	OSCCONbits.SCS = 1;
	return ret;
}

/* @brief: Selects the main oscillator as the main oscillator.
 * @param: Nothing.
 * @return: E_OK.
 */
STD_ReturnType Primary_OSC_select(void){
	STD_ReturnType ret = E_OK;
	OSCCONbits.SCS = 0;
	return ret;
}

