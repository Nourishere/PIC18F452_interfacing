/* 
 * File:   ecu_7_segment.h
 * Author: nour
 *
 * Created on February 20, 2025, 9:30 AM
 */
#include "ecu_7_segment.h"
/** Helper function **/
static STD_ReturnType seven_segment_linit(const seven_segment_t *seg, pin_config_t lpin[]){
	STD_ReturnType ret = E_OK;
	uint8 i = 0;
	if(seg == NULL || lpin == NULL)
		ret = E_NOT_OK;	
	else{
		for(;i<4;i++){
			lpin[i] . port = seg -> seven_seg_pins[i].port;
			lpin[i] . pin= seg -> seven_seg_pins[i].pin;
			lpin[i] . direction = GPIO_OUT; 
			lpin[i] . logic = seg -> start_status;
		}
	}
	return ret;
}
STD_ReturnType seven_segment_intialize(const seven_segment_t *seg){
	STD_ReturnType ret = E_OK;
	uint8 i = 0;
	pin_config_t lpin[4];
	if (NULL == seg)
		ret = E_NOT_OK;
	else{
		ret = seven_segment_linit(seg, lpin);
		for(;i<4;i++){
			if(E_NOT_OK == GPIO_check_access(&lpin[i])){
				return E_NOT_OK;
			}
		}
		for(i=0;i<4;i++)
			ret = GPIO_pin_intialize(&lpin[i]);
	}	
	return ret;
}
STD_ReturnType seven_segment_write_number(const seven_segment_t * seg, uint8 number){
	STD_ReturnType ret = E_OK;
	pin_config_t lpin[4];
	uint8 c=0;
	if (NULL == seg || number > 9)
		ret = E_NOT_OK;	
	else{
		ret = seven_segment_linit(seg,lpin);
		while(c < 4){
			if(( (number >> c) & 0x01) == 1)
				ret = GPIO_pin_write_logic(&lpin[c], GPIO_HIGH); 
			else
				ret = GPIO_pin_write_logic(&lpin[c], GPIO_LOW); 
			c++;
		}
	}
	return ret;
}
STD_ReturnType seven_segment_write_2digit_number(const seven_segment_t * seg, const pin_config_t *seg_tenth_en,
const pin_config_t *seg_units_en, uint8 number){
	STD_ReturnType ret = E_OK;
	if (NULL == seg|| NULL == seg_tenth_en || NULL == seg_units_en || number > 99)
		ret = E_NOT_OK;	
	else{
			ret = seven_segment_write_number(seg,(uint8)number%10);
			ret = GPIO_pin_write_logic(seg_units_en, GPIO_HIGH);
			__delay_ms(10);
			ret = GPIO_pin_write_logic(seg_units_en, GPIO_LOW);
			ret = seven_segment_write_number(seg, (uint8)number/10);
			ret = GPIO_pin_write_logic(seg_tenth_en, GPIO_HIGH);
    			__delay_ms(10);
			ret = GPIO_pin_write_logic(seg_tenth_en, GPIO_LOW);
	}
	return ret;
}
