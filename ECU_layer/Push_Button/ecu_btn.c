/* 
 * File:   ecu_btn.c
 * Author: nour
 *
 * Created on November 26, 2024, 2:11 PM
 */
#include "ecu_btn.h"
//STD_ReturnType GPIO_check_access(const pin_config_t * _pin_config);
STD_ReturnType btn_intialize(const btn_t* btn){
	STD_ReturnType ret = E_NOT_OK;
	if (btn == NULL || (E_NOT_OK == GPIO_check_access(&(btn -> btn_pin))) || ((btn -> btn_pin.direction) == GPIO_OUT) ){
		ret = E_NOT_OK;
	}
	else{
		ret = GPIO_pin_direction_intialize(&(btn -> btn_pin));
	}
	return ret; 
}
STD_ReturnType btn_read_state(const btn_t* btn, btn_status *btn_s){
	STD_ReturnType ret = E_NOT_OK;
		logic_t btn_logic = GPIO_LOW;
	if (btn == NULL || (btn_s == NULL) || (E_NOT_OK == GPIO_check_access(&(btn -> btn_pin))) ){
		ret = E_NOT_OK;
	}
	else{
		GPIO_pin_read_logic(&(btn -> btn_pin), &btn_logic);
		if (btn -> btn_mode == btn_AL){	
			if (GPIO_HIGH == btn_logic){
				*btn_s = btn_free; 	
			}
			else{
				*btn_s = btn_pressed;	
			}	
		}
		else if (btn -> btn_mode == btn_AH){
			if (GPIO_HIGH == btn_logic){
				*btn_s = btn_pressed;	
			}
			else{
				*btn_s = btn_free;
			}	

		}
		else{}
		ret = E_OK;
	}
	return ret;
}
