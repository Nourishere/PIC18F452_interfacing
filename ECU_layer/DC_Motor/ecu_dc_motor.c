/* 
 * File:   ecu_dc_motor.c
 * Author: nour
 *
 * Created on February 13, 2025, 8:26 AM
 */
#include "ecu_dc_motor.h"

/*
 *@brief: Helper fuction that converts a struct used by the user
 *        to a struct of type pin_config_t to be processed.
 *		  The idea is to isolate the application layer better
 * 		  from things like direction that the user has not interset in.
 *@param: (dc_motor_l) which contains an array of two 
 *		  dc_motor_simple_t each containing port,pin, and init state.
		  (llpin1)&(llpin2) a pointer to a pin_config_t that may be intilized in
		  other functions.
 *@return: returns status for the operation.
 */
static STD_ReturnType dc_motor_linit(const dc_motor_t * dc_motor_l, pin_config_t * llpin1, pin_config_t * llpin2){
	STD_ReturnType ret = E_OK;
	if( NULL == dc_motor_l)
		ret = E_NOT_OK;
	else{
//		llpin1 = &(pin_config_t){dc_motor_l -> dc_motor_arr[0].port, dc_motor_l -> dc_motor_arr[0].pin, GPIO_OUT,
//							dc_motor_l -> dc_motor_arr[0].logic};
//		llpin2 = &(pin_config_t){dc_motor_l -> dc_motor_arr[1].port,dc_motor_l -> dc_motor_arr[1].pin,GPIO_OUT,
//							dc_motor_l -> dc_motor_arr[1].logic};
		llpin1->port = dc_motor_l -> dc_motor_arr[0].port;
		llpin1->pin= dc_motor_l -> dc_motor_arr[0].pin;
		llpin1->direction= GPIO_OUT;
		llpin1->logic= dc_motor_l -> dc_motor_arr[1].logic;
		llpin2->port = dc_motor_l -> dc_motor_arr[1].port;
		llpin2->pin= dc_motor_l -> dc_motor_arr[1].pin;
		llpin2->direction= GPIO_OUT;
		llpin2->logic= dc_motor_l -> dc_motor_arr[1].logic;
	}
	return ret;
}

/*
 *@brief: intialize a two pin motor
 *@param: (dc_motor_l) which contains an array of two 
 *		  dc_motor_simple_t each containing port,pin, and init state.
 *@return: returns status for the operation
 */
STD_ReturnType dc_motor_intialize(const dc_motor_t * dc_motor_l){
	STD_ReturnType ret = E_OK;
	pin_config_t lpin1, lpin2;
	if( NULL == dc_motor_l)
		ret = E_NOT_OK;
	else{
		dc_motor_linit(dc_motor_l,&lpin1,&lpin2);
		if(E_NOT_OK == GPIO_check_access(&(lpin1)) || E_NOT_OK == GPIO_check_access(&(lpin2)))
			ret = E_NOT_OK;
			
		else{
			ret = GPIO_pin_intialize(&(lpin1));
			ret = GPIO_pin_intialize(&(lpin2));
		}
	}
	return ret;
}
/*
 *@brief:turn a motor right 
 *@param: (dc_motor_l) which contains an array of two 
 *		  dc_motor_simple_t each containing port,pin, and init state.
 *@return: returns status for the operation
 */
STD_ReturnType dc_motor_turn_right(const dc_motor_t * dc_motor_l){
	STD_ReturnType ret = E_OK;
	pin_config_t lpin1,lpin2;
	if(NULL == dc_motor_l)
		ret = E_NOT_OK;
	else{
		dc_motor_linit(dc_motor_l,&lpin1,&lpin2);
		ret = GPIO_pin_write_logic(&(lpin1), GPIO_HIGH);
		ret = GPIO_pin_write_logic(&(lpin2), GPIO_LOW);
	}

	return ret;
}
/*
 *@brief: turn at motor left
 *@param: (dc_motor_l) which contains an array of two 
 *		  dc_motor_simple_t each containing port,pin, and init state.
 *@return: returns status for the operation
 */
STD_ReturnType dc_motor_turn_left(const dc_motor_t * dc_motor_l){
	STD_ReturnType ret = E_OK;
	pin_config_t lpin1,lpin2;
	if(NULL == dc_motor_l)
		ret = E_NOT_OK;
	else{
		dc_motor_linit(dc_motor_l,&lpin1,&lpin2);
		ret = GPIO_pin_write_logic(&(lpin1), GPIO_LOW);
		ret = GPIO_pin_write_logic(&(lpin2), GPIO_HIGH);
	}

	return ret;
}
/*
 *@brief: stops a motor 
 *@param: (dc_motor_l) which contains an array of two 
 *		  dc_motor_simple_t each containing port,pin, and init state.
 *@return: returns status for the operation
 */
STD_ReturnType dc_motor_stop(const dc_motor_t * dc_motor_l){
	STD_ReturnType ret = E_OK;
	pin_config_t lpin1,lpin2;
	if(NULL == dc_motor_l)
		ret = E_NOT_OK;
	else{
		dc_motor_linit(dc_motor_l,&lpin1,&lpin2);
		ret = GPIO_pin_write_logic(&(lpin1), GPIO_LOW);
		ret = GPIO_pin_write_logic(&(lpin2), GPIO_LOW);
	}

	return ret;
}
