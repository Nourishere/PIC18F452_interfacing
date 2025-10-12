/* 
 * File:   ecu_dc_motor.c
 * Author: nour
 *
 * Created on February 13, 2025, 8:26 AM
 */

#include "ecu_dc_motor.h"

#if (DC_MOTOR_MODULE == STD_ON)
/*@brief: Initializes a DC motor. 
 *@param: A pointer to a dc_motor_t type specifying a DC motor.
 *@return: E_OK if initialization is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType dc_motor_initialize(const dc_motor_t * dc_motor_l){
	STD_ReturnType ret = E_OK;
	pin_config_t lpin1, lpin2;
	if( NULL == dc_motor_l)
		ret = E_NOT_OK;
	else{
		dc_motor_linit(dc_motor_l,&lpin1,&lpin2);
		if(E_NOT_OK == GPIO_check_access(&(lpin1)) || E_NOT_OK == GPIO_check_access(&(lpin2)))
			ret = E_NOT_OK;
			
		else{
			ret = GPIO_pin_initialize(&(lpin1));
			ret = GPIO_pin_initialize(&(lpin2));
		}
	}
	return ret;
}
/*@brief: Sends a signal to a DC motor turning it right. 
 *        (Note that the direction of rotation depends on the connection 
 *		  of the motor leads and can differ from motor to another. Use with caution.)
 *@param: A pointer to a dc_motor_t type specifying a DC motor.
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
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
/*@brief: Sends a signal to a DC motor turning it left. 
 *        (Note that the direction of rotation depends on the connection 
 *		  of the motor leads and can differ from motor to another. Use with caution.)
 *@param: A pointer to a dc_motor_t type specifying a DC motor.
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
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
/*@brief: Sends a stop signal to a DC motor. 
 *@param: A pointer to a dc_motor_t type specifying a DC motor.
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
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
/*@brief: (H) Initializes a pin_config_t type given a dc_motor_t type.
 *@param: A pointer to a dc_motor_t type and two pointers to two local pin_config_t type. 
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
 */
static STD_ReturnType dc_motor_linit(const dc_motor_t * dc_motor_l, pin_config_t * llpin1, pin_config_t * llpin2){
	STD_ReturnType ret = E_OK;
	if( NULL == dc_motor_l)
		ret = E_NOT_OK;
	else{
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
#endif
