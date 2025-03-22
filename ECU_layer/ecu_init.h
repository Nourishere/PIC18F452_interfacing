/* 
 * File:   ecu_init.h
 * Author: nour
 *
 * Created on February 26, 2025, 7:34 AM
 */

#ifndef ECU_INIT_H
#define	ECU_INIT_H
/***** includes ********/
#include "LED/ecu_led.h"
#include "Push_Button/ecu_btn.h"
#include "Relay/ecu_relay.h"
#include "DC_Motor/ecu_dc_motor.h"
#include "7_Segment/ecu_7_segment.h"
#include "Keypad/ecu_keypad.h"
#include "LCD/ecu_LCD.h"
#include "../MCAL_layer/std_libs.h"

/****** function declarations ******/
STD_ReturnType ecu_init(void);
#endif	/* ECU_INIT_H */

