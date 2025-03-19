/* 
 * File:   ecu_dc_motor.h
 * Author: nour
 *
 * Created on February 13, 2025, 8:25 AM
 */

#ifndef ECU_DC_MOTOR_H
#define	ECU_DC_MOTOR_H
/******** includes *********/
#include "../../MCAL_layer/GPIO/hal_gpio.h"
/******** data types ******/
typedef enum{
	dc_motor_off,
	dc_motor_on
}dc_motor_logic;

/*typedef struct{
	pin_config_t dc_motor_arr[2];
}dc_motor_t; */
typedef struct{
	pin_config_simple_t dc_motor_arr[2];
}dc_motor_t;

/*** function declaration **/
static STD_ReturnType dc_motor_linit(const dc_motor_t * dc_motor_l,pin_config_t * llpin1,pin_config_t * llpin2);

STD_ReturnType dc_motor_intialize(const dc_motor_t * dc_motor_l);
STD_ReturnType dc_motor_turn_right(const dc_motor_t * dc_motor_l);
STD_ReturnType dc_motor_turn_left(const dc_motor_t * dc_motor_l);
STD_ReturnType dc_motor_stop(const dc_motor_t * dc_motor_l);






#endif	/* ECU_DC_MOTOR_H */

