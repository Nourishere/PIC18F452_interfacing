/* 
 * File:   ecu_relay.h
 * Author: nour
 *
 * Created on February 11, 2025, 8:38 AM
 */

#ifndef ECU_RELAY_H
#define	ECU_RELAY_H
/************ includes ****************/
#include "../../MCAL_layer/GPIO/hal_gpio.h" 
#include "ecu_relay_cfg.h"
/************ data types **************/
typedef enum {
	relay_off,
	relay_on
}relay_status;

typedef struct{
	uint8 port : 3;
	uint8 pin : 3;
	uint8 state : 1;	
}relay_t;

/******** function declarations *******/
STD_ReturnType relay_initialize(const relay_t * lrelay);
STD_ReturnType relay_turn_on(const relay_t * lrelay);
STD_ReturnType relay_turn_off(const relay_t * lrelay);




#endif	/* ECU_RELAY_H */

