/* 
 * File:   hal_acd.c
 * Brief: This is the device driver implementation for the ADC module.
 * ref: Check the header file hal_adc.h
 * Author: nour
 *
 * Created on May 16, 2025, 2:51 PM
 */
#include "hal_adc.h"

/* @brief: Initialize the ADC module.
 * @param: A pointer to a struct of type ADC_t.
 * @return: E_OK if initialization is successful and
 * 			E_NOT_OK otherwise.
 */
STD_ReturnType ADC_init(const ADC_t * lADC){
	STD_ReturnType ret = E_OK;
	if(NULL == lADC || lADC -> channel == ADC_default)
		ret = E_NOT_OK;
	else{
		/* Disable the ADC */
		ADC_OFF();
		/* Select the default channel */
		ret = ADC_select_channel(lADC -> channel) &&
		/* Configure default channel pin configuration */
		ADC_set_pin_config(lADC -> channel); 
		/* Set the result justification */
#if (ADC_JUST == __RIGHT)
		ADC_JUSTIFY_RIGHT();
#elif (ADC_JUST == __LEFT)
		ADC_JUSTIFY_LEFT();
#else
		ret = E_NOT_OK;
#endif
		/* Set the conversion clock */
		ADC_CCLK_SET(lADC -> conversion_clock); 
#if (INT_ADC == INT_EN)
		/* Set the ADC interrupt */
		/* @ref: mcal_internal_interrupt.h */
#if (INT_PR == INT_EN)
		ret = ret && INT_ADC_init(lADC -> priority);
#elif (INT_PR == INT_DIS)
                ret = ret && INT_ADC_init(1); /* garbage value */
#else
		ret = E_NOT_OK;
#endif
		/* Set the callback routine */	
		/* @ref: mcal_internal_interrupt.h */
		ret = ret && INT_ADC_set_callback_routine(lADC-> ADC_interrupt_handler);
#endif
		/* Select A/D port combination
		 * @ref: hal_adc.h -> A/D port configuration 
		 */ 
		ADC_CHOOSE_COMB(lADC -> combination);
		/* Turn the ADC back on */
		ADC_ON();
	}
	return ret;
}
/* @brief: De-initializes the ADC module.
 * @param: A pointer to a type ADC_t.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType ADC_deinit(const ADC_t * lADC){
	STD_ReturnType ret = E_OK;
	if(NULL == lADC)
		ret = E_NOT_OK;
	else{
		/* Disable the ADC */
		ADC_OFF();
		/* Disable interrupts */
#if (INT_ADC == INT_EN)
		ret = INT_ADC_deinit();
#endif
	}
	return ret;
}
/* @brief:(H) Select the analog channel for the ADC.
 * @note: This routine takes care of the pin configuration.
 * @param: A variable from the type ADC_CH_t specifying a channel.
 * @return: Returns E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType ADC_select_channel(ADC_CH_t lCH){
	STD_ReturnType ret = E_OK;
		ADC_CH_SEL(lCH);
		ret = ADC_set_pin_config(lCH);
	return ret;
}
/* @brief: Start the ADC conversion process.
 * @param: A pointer to a ADC_t struct specifying an ADC.
 * 		   An ADC_CH_t type for channel selection. 
 * 		   In case lCH=ADC_DEFAULT, the default channel set in the 
 * 		   ADC_t struct is used.
 * @return: Returns E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType ADC_convert(const ADC_t * lADC, ADC_CH_t lCH){
	STD_ReturnType ret = E_OK;
	if((NULL == lADC) || (lADC -> channel == ADC_default) )
		ret = E_NOT_OK;
	else{
		/* Select the channel */
		if(lCH == ADC_default)
			ret = ADC_select_channel(lADC -> channel);
		else
			ret = ADC_select_channel(lCH);
		/* Start the conversion */
			ADC_START();	
	}
	return ret;
}
/* @brief: Stores the result of the conversion.
 * @param: A pointer to a struct of type ADC_t specifying an ADC.
 * @return: E_OK upon success and E_NOT_OK otherwise. The result is a 
 * 		    pointer to a ADC_result_size type (uint16) for which the 
 * 		    result of the conversion is stored.
 * @note: In case an ISR is used, this routine is redundant.
 */
STD_ReturnType ADC_result(const ADC_t * lADC, ADC_result_size * result){
	STD_ReturnType ret = E_OK;
	if(NULL == lADC || NULL == result)
		ret = E_NOT_OK;
	else{
		/* Wait for conversion */
		while(ADC_STATUS);
		/* configure justification */
#if (ADC_JUST == __RIGHT || ADC_JUST == __LEFT)
		*result = (ADC_result_size) ( (((ADC_result_size)(ADRESH)) << 8) | ((ADC_result_size) (ADRESL)) );
#else
		ret = E_NOT_OK;
#endif
	}
	return ret;
}	
/* @brief: Returns the status of the ADC.
 * @param: A pointer to a struct of type lADC specifying an ADC and a 
 * 		   pointer to an ADC_status_t to return the status.
 * @return: Returns E_OK upon success and E_NOT_OK otherwise. Returns 
 * 			Done (0) in pointer status if the conversion is done (idle)
 * 			and nDone (-1) if conversion is still in progress.
 */
STD_ReturnType ADC_status (const ADC_t * lADC, ADC_status_t * status){
	STD_ReturnType ret = E_OK;
	if(NULL == lADC || NULL == status)
		ret = E_NOT_OK;
	else{
		if(ADC_STATUS == 1)
			*status = Done;
		else
			*status = nDone;
	}
	return ret;
}
/* Helper functions */
/* @brief:(H) Set the pin for the conversion as input.
 * @param: A pointer to a struct of type ADC_t.
 * @return: Returns E_OK upon success and E_NOT_OK otherwise.
 */
static STD_ReturnType ADC_set_pin_config(ADC_CH_t channel){
	STD_ReturnType ret = E_OK;
	switch(channel){
		case(ADC_default):
			ret = E_NOT_OK;
			break;
		case(ADC_ch0):
			TRISAbits.RA0=1;
			break;
		case(ADC_ch1):
			TRISAbits.RA1=1;
			break;
		case(ADC_ch2):
			TRISAbits.RA2=1;
			break;
		case(ADC_ch3):
			TRISAbits.RA3=1;
			break;
		case(ADC_ch4):
			TRISAbits.RA5=1;
			break;
		case(ADC_ch5):
			TRISEbits.RE0=1;
			break;
		case(ADC_ch6):
			TRISEbits.RE1=1;
			break;
		case(ADC_ch7):
			TRISEbits.RE2=1;
			break;
		default:
			ret = E_NOT_OK;
			break;
	}
        return ret;
}
