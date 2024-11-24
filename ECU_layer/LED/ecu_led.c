/* 
 * File:   ecu_led.c
 * Author: nour
 *
 * Created on October 23, 2024, 7:25 AM
 */

#include "ecu_led.h"

STD_ReturnType LED_intialize(LED_t *led){
	STD_ReturnType ret = E_OK;
	if ((NULL == led) || ( (led -> port == PORTA_I) && ( (led -> pin) > PORTA_PIN_MAX_NUMBER - 1) ) || 
		( ((led -> port == PORTB_I) || (led -> port == PORTC_I) || (led -> port == PORTD_I) )
		 && ( (led -> pin) > PORTBCD_PIN_MAX_NUMBER - 1) ) ||( (led -> port == PORTE_I) &&
		 ((led -> pin) > PORTE_PIN_MAX_NUMBER - 1))	){
		ret = E_NOT_OK;	
	}
	else{
		pin_config_t pin_local = {.port = led -> port, .pin = led -> pin, .direction = GPIO_OUT, .logic = led -> LED_init_status};
		GPIO_pin_intialize(&pin_local);
	}

    return ret;
}
STD_ReturnType LED_on(LED_t *led){
	STD_ReturnType ret = E_OK;
	if ((NULL == led) || ( (led -> port == PORTA_I) && ( (led -> pin) > PORTA_PIN_MAX_NUMBER - 1) ) || 
		( ((led -> port == PORTB_I) || (led -> port == PORTC_I) || (led -> port == PORTD_I) )
		 && ( (led -> pin) > PORTBCD_PIN_MAX_NUMBER - 1) ) ||( (led -> port == PORTE_I) &&
		 ((led -> pin) > PORTE_PIN_MAX_NUMBER - 1))	){
		ret = E_NOT_OK;	
	}
	else{
		pin_config_t pin_local = {.port = led -> port, .pin = led -> pin, .direction = GPIO_OUT, .logic = led -> LED_init_status};
		GPIO_pin_write_logic(&pin_local, GPIO_HIGH);

	}


}
STD_ReturnType LED_off(LED_t *led){
	STD_ReturnType ret = E_OK;
	if ((NULL == led) || ( (led -> port == PORTA_I) && ( (led -> pin) > PORTA_PIN_MAX_NUMBER - 1) ) || 
		( ((led -> port == PORTB_I) || (led -> port == PORTC_I) || (led -> port == PORTD_I) )
		 && ( (led -> pin) > PORTBCD_PIN_MAX_NUMBER - 1) ) ||( (led -> port == PORTE_I) &&
		 ((led -> pin) > PORTE_PIN_MAX_NUMBER - 1))	){
		ret = E_NOT_OK;	
	}
	else{
		pin_config_t pin_local = {.port = led -> port, .pin = led -> pin, .direction = GPIO_OUT, .logic = led -> LED_init_status};
		GPIO_pin_write_logic(&pin_local, GPIO_LOW);
	}


}
STD_ReturnType LED_toggle(LED_t *led){
	STD_ReturnType ret = E_OK;
	if ((NULL == led) || ( (led -> port == PORTA_I) && ( (led -> pin) > PORTA_PIN_MAX_NUMBER - 1) ) || 
		( ((led -> port == PORTB_I) || (led -> port == PORTC_I) || (led -> port == PORTD_I) )
		 && ( (led -> pin) > PORTBCD_PIN_MAX_NUMBER - 1) ) ||( (led -> port == PORTE_I) &&
		 ((led -> pin) > PORTE_PIN_MAX_NUMBER - 1))	){
		ret = E_NOT_OK;	
	}
	else{
		pin_config_t pin_local = {.port = led -> port, .pin = led -> pin, .direction = GPIO_OUT, .logic = led -> LED_init_status};
		GPIO_pin_toggle_logic(&pin_local);
	}



}
