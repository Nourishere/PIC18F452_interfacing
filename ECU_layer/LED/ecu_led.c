/* 
 * File:   ecu_led.c
 * Author: nour
 *
 * Created on October 23, 2024, 7:25 AM
 */

#include "ecu_led.h"

/*@brief: Initializes an LED.
 *@param: A pointer to a LED of type LED_t.
 *@return: E_OK if initialization is successful and E_NOT_Ok otherwise.
 */       
STD_ReturnType LED_initialize(LED_t *led){
	STD_ReturnType ret = E_OK;
	if ( (NULL == led) ){
		ret = E_NOT_OK;	
	}
	else{
		pin_config_t pin_local = {.port = led -> port, .pin = led -> pin, .direction = GPIO_OUT, .logic = led -> LED_init_status};
		if(E_OK == GPIO_check_access(&pin_local))
			GPIO_pin_initialize(&pin_local);
		else
			ret = E_NOT_OK;
	}

    return ret;
}
/*@brief: Initializes an LED.
 *@param: A pointer to a LED of type LED_t.
 *@return: E_OK if operation is successful and E_NOT_Ok otherwise.
 */       
STD_ReturnType LED_on(LED_t *led){
	STD_ReturnType ret = E_OK;
	if ( (NULL == led) ){
		ret = E_NOT_OK;	
	}
	else{
		pin_config_t pin_local = {.port = led -> port, .pin = led -> pin, .direction = GPIO_OUT, .logic = led -> LED_init_status};
		if(E_OK == GPIO_check_access(&pin_local))
			GPIO_pin_write_logic(&pin_local, GPIO_HIGH);
		else
			ret = E_NOT_OK;
	}
	return ret;
}
/*@brief: Initializes an LED.
 *@param: A pointer to a LED of type LED_t.
 *@return: E_OK if operation is successful and E_NOT_Ok otherwise.
 */       
STD_ReturnType LED_off(LED_t *led){
	STD_ReturnType ret = E_OK;
	if ( (NULL == led) ){
		ret = E_NOT_OK;	
	}
	else{
		pin_config_t pin_local = {.port = led -> port, .pin = led -> pin, .direction = GPIO_OUT, .logic = led -> LED_init_status};
		if(E_OK == GPIO_check_access(&pin_local))
			GPIO_pin_write_logic(&pin_local, GPIO_LOW);
		else
			ret = E_NOT_OK;
	}
	return ret;
}
/*@brief: Initializes an LED.
 *@param: A pointer to a LED of type LED_t.
 *@return: E_OK if operation is successful and E_NOT_Ok otherwise.
 */       
STD_ReturnType LED_toggle(LED_t *led){
	STD_ReturnType ret = E_OK;
	if ( (NULL == led) ){
		ret = E_NOT_OK;	
	}
	else{
		pin_config_t pin_local = {.port = led -> port, .pin = led -> pin, .direction = GPIO_OUT, .logic = led -> LED_init_status};
		if(E_OK == GPIO_check_access(&pin_local))
			GPIO_pin_toggle_logic(&pin_local);
		else
			ret = E_NOT_OK;
	}
	return ret;
}
