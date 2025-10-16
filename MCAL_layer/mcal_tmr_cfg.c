/* 
 * File:   mcal_tmr_cfg.c
 * Author: nour
 *
 * Created on August 15, 2025, 8:17 PM
 * @Brief: This source contains implementation of common
 * 		   routines used by all timer modules.
 */
#include "mcal_tmr_cfg.h"

/* @brief: Convert prescaler enum value to integer multiple value.
 * @param: A timer TMR_PRESC_SEL specifying the enum value and a pointer 
 * 		   to a uint16 to store the return value.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType TMR_PRESC2VAL(TMR_PRESC_SEL prescaler, uint16* val){
	STD_ReturnType ret = E_OK;
	if(val == NULL)
		ret = E_NOT_OK;
	else{
		switch(prescaler){
			case(TMR_PRESC_1_1):
				*val = 1;	
				break;
			case(TMR_PRESC_1_2):
				*val = 2;
				break;
			case(TMR_PRESC_1_4):
				*val = 4;
				break;
			case(TMR_PRESC_1_16):
				*val = 16;
				break;
			case(TMR_PRESC_1_32):
				*val = 32;
				break;
			case(TMR_PRESC_1_64):
				*val = 64;
				break;
			case(TMR_PRESC_1_128):
				*val = 128;
				break;
			case(TMR_PRESC_1_256):
				*val = 256;
				break;
			default:
				ret = E_NOT_OK;
				break;
		}
	}
	return ret;
}
