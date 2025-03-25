/* 
 * File:   ecu_relay.c
 * Author: nour
 *
 * Created on February 11, 2025, 8:38 AM
 */

#include "ecu_relay.h"

/*@brief: Initializes a relay. 
 *@param: A pointer to a relay of type relay_t.
 *@return: E_OK if initialization is successful.
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
/*@brief: Turns on a relay. 
 *@param: A pointer to a relay of type relay_t.
 *@return: E_OK if initialization is successful.
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
/*@brief: Turns off a relay. 
 *@param: A pointer to a relay of type relay_t.
 *@return: E_OK if initialization is successful.
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
