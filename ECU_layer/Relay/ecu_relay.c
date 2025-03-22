#include "ecu_relay.h"
/*
 *@brief: initializes a relay object 
 *@param: a pointer to a relay_t struct
 *@return: returns status of operation 
 */

STD_ReturnType relay_initialize(const relay_t * lrelay){
	STD_ReturnType ret = E_OK;
	if(NULL == lrelay)
		ret = E_NOT_OK;
	else{
		pin_config_t lpin = {lrelay -> port , lrelay -> pin, GPIO_OUT, lrelay -> state};  
		if (GPIO_check_access(&lpin) == E_NOT_OK)
			ret = E_NOT_OK;
		else
			GPIO_pin_initialize(&lpin);
	}
	return ret;
}
/*
 *@brief: turns a relay on (applies high logic)
 *@param: a pointer to a relay_t struct
 *@return: returns status of operation 
 */
STD_ReturnType relay_turn_on(const relay_t * lrelay){
	STD_ReturnType ret = E_OK;
	if(NULL == lrelay)
		ret = E_NOT_OK;
	else{
		pin_config_t lpin = {lrelay -> port , lrelay -> pin, GPIO_OUT, lrelay -> state};  
		if (GPIO_check_access(&lpin) == E_NOT_OK)
			ret = E_NOT_OK;
		else
			GPIO_pin_write_logic(&lpin, GPIO_HIGH);
	}
	
	return ret;
}
/*
 *@brief: turns a relay off (applies low logic) 
 *@param: a pointer to a relay_t struct
 *@return: returns status of operation 
 */
STD_ReturnType relay_turn_off(const relay_t * lrelay){
	STD_ReturnType ret = E_OK;
	if(NULL == lrelay)
		ret = E_NOT_OK;
	else{
		pin_config_t lpin = {lrelay -> port , lrelay -> pin, GPIO_OUT, lrelay -> state};  
		if (GPIO_check_access(&lpin) == E_NOT_OK)
			ret = E_NOT_OK;
		else
			GPIO_pin_write_logic(&lpin, GPIO_LOW);
	}

	return ret;
}
