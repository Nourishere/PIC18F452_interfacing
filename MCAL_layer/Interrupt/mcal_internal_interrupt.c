/* 
 * File:   mcal_internal_interrupt.c
 * Author: nour
 *
 * Created on April 15, 2025, 2:24 PM
 */
#include "mcal_internal_interrupt.h"

#if (INT_ADC == INT_EN)
void (*ADC_callback) (uint16 * result) = NULL;
uint16 ADC_output;
#endif
void ADC_ISR(void){
	INT_ADC_CLRF();
	if(ADC_callback){
		ADC_output = (uint16) ( (((uint16)(ADRESH)) << 8) | ((uint16) (ADRESL)) );
		ADC_callback(&ADC_output);
	}
}
#if (INT_ADC == INT_EN)
/* @brief: Initialize the ADC interrupt.
 * @param: A pointer to an ADC_t type.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType INT_ADC_init(uint8 priority){
	STD_ReturnType ret = E_OK;
		INT_ADC_CLRF();	
		INT_ADC_DIS();
#if (INT_PR == INT_EN)
		if(priority != INT_PHIGH && priority != INT_PLOW)
			ret = E_NOT_OK;
		else{
            INT_PREN(); /* enable the priority feature */
	 		/* @ref: hal_interrupt_config.h -> INT_PHIGH & INT_PLOW */
			if(priority == INT_PHIGH){
				INT_ADC_HP(); /* Enable ADC as a high priority */
            	INT_GHPEN(); /* Enable global high interrupts */
			}
			else if(priority == INT_PLOW){
				INT_ADC_LP();
            	INT_GLPEN();
				INT_GEN();	
			}
			else
				ret = E_NOT_OK;
           }
		INT_ADC_EN(); /* Enable the ADC peripheral */
#elif (INT_PR == INT_DIS)
        INT_PRDIS(); /* disable the priority feature */
		INT_GEN(); /* General interrupt enable */
		INT_PEEN(); /* Peripheral interrupt enable */
		INT_ADC_EN(); /* Enable the ADC peripheral */
#else 
		ret = E_NOT_OK;
#endif
	return ret;
}

/* @brief: De-initialize the ADC interrupt. 
 * @param: This function does not take any input.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType INT_ADC_deinit(void){
	STD_ReturnType ret = E_OK;
	INT_ADC_CLRF();
	INT_ADC_DIS();
	return ret;
}

/* @brief: Set up the callback function for the ADC interrupt.
 * @param: A pointer to a ADC_t type.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType INT_ADC_set_callback_routine(void (*callback) (uint16 * result)){
	STD_ReturnType ret = E_OK;
		ADC_callback = callback;	
	return ret;
}
#endif
