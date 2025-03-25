/* 
 * File:   ecu_7_segment.h
 * Author: nour
 *
 * Created on February 20, 2025, 9:28 AM
 */
#ifndef ECU_7_SEGMENT_H
#define	ECU_7_SEGMENT_H
#define _XTAL_FREQ 8000000
/***************** includes ***************************/
#include "../../MCAL_layer/GPIO/hal_gpio.h"
/******************** macros **************************/
#define OFF 0
#define ON 1
#define COM_cathode 0
#define COM_anode 1
/*** data types */
typedef struct{
	port_pin_t seven_seg_pins[4];
	uint8 start_status;		
}seven_segment_t;

/*************** function declarations *****************/
static STD_ReturnType seven_segment_linit(const seven_segment_t *seg, pin_config_t lpin[]);
STD_ReturnType seven_segment_initialize(const seven_segment_t *seg);
STD_ReturnType seven_segment_write_number(const seven_segment_t * seg, uint8 number);
STD_ReturnType seven_segment_write_2digit_number(const seven_segment_t * seg,const pin_config_t *seg_tenth_en,
												 const pin_config_t *seg_units_en, uint8 number);

#endif	/* ECU_7_SEGMENT_H */
