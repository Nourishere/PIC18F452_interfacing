/* 
 * File:   ecu_btn.c
 * Author: nour
 *
 * Created on November 26, 2024, 2:11 PM
 */
#include "ecu_btn.h"

/*@brief: Initializes a push button.
 *@param: A pointer to a type btn_t.
 *@return: E_OK if initialization is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType btn_initialize(const btn_t* btn){
	STD_ReturnType ret = E_NOT_OK;
	// The button cannot be set for output for instace.
	if (btn == NULL || (E_NOT_OK == GPIO_check_access(&(btn -> btn_pin))) || ((btn -> btn_pin.direction) == GPIO_OUT) ){
		ret = E_NOT_OK;
	}
	else{
		ret = GPIO_pin_direction_initialize(&(btn -> btn_pin));
	}
	return ret; 
}
/*@brief: Reads the state of a push button.
 *@param: A pointer to a type btn_t defining a push button 
 *    	  and a pointer to a type btn_status containing the button state for return.
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType btn_read_state(const btn_t* btn, btn_status *btn_s){
	STD_ReturnType ret = E_NOT_OK;
		logic_t btn_logic = GPIO_LOW;
	if (btn == NULL || (btn_s == NULL) || (E_NOT_OK == GPIO_check_access(&(btn -> btn_pin))) ){
		ret = E_NOT_OK;
	}
	else{
		GPIO_pin_read_logic(&(btn -> btn_pin), &btn_logic);
		switch (btn -> btn_mode){	
			case(btn_AL):
				if (GPIO_HIGH == btn_logic){
					*btn_s = btn_free; 	
					break;
				}
				else{
					*btn_s = btn_pressed;	
					break;
				}	
			case(btn_AH):
				if (GPIO_HIGH == btn_logic){
					*btn_s = btn_pressed;	
					break;
				}
				else{
					*btn_s = btn_free;
					break;
				}
			default:
				ret = E_NOT_OK;
				break;		
		}
	}
	return ret;
}
