/* 
 * File:   ecu_keypad.c
 * Author: nour
 *
 * Created on February 24, 2025, 7:25 AM
 */
#include "ecu_keypad.h"
static const uint8 keypad_elements[KEY_ROWS][KEY_COLUMNS] =
{{'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}};
static STD_ReturnType keypad_linit(const keypad_t *keypad, pin_config_t lpin[], uint8 rc){
	uint8 i = 0;
	STD_ReturnType ret = E_OK;
	if(NULL == keypad || NULL == lpin || rc > 1)
		ret = E_NOT_OK;
	else{
		if(rc == ROW){
			for(;i<KEY_ROWS;i++){
				lpin[i]. port = keypad -> keypad_rows[i].port;
				lpin[i]. pin = keypad -> keypad_rows[i].pin;
				lpin[i]. direction = GPIO_OUT;
				if(keypad -> connection == pull_up)
					lpin[i].logic = GPIO_LOW;
				else
					lpin[i].logic = GPIO_HIGH;
			}
		}
		else{
            i = 0;
			for(;i<KEY_COLUMNS;i++){
				lpin[i].port = keypad -> keypad_columns[i].port;
				lpin[i].pin = keypad -> keypad_columns[i].pin;
				lpin[i].direction = GPIO_IN;
				if(keypad -> connection == pull_up)
					lpin[i].logic = GPIO_HIGH;
				else
					lpin[i].logic = GPIO_LOW;
			}
		} 
	}   

	return ret;
}
STD_ReturnType keypad_initialize(const keypad_t *keypad){
	uint8 i = 0;
	STD_ReturnType ret = E_OK;
	pin_config_t lpinr[KEY_ROWS];
	pin_config_t lpinc[KEY_COLUMNS];
	if(NULL == keypad)
		ret = E_NOT_OK;
	else{
		ret = keypad_linit(keypad, lpinr, ROW);  	
		ret = keypad_linit(keypad, lpinc, COL);  	

		for(;i<KEY_ROWS && ret != E_NOT_OK; i++)
			ret = GPIO_pin_initialize(&(lpinr[i]));
		for(;i<KEY_COLUMNS && ret != E_NOT_OK; i++)
			ret = GPIO_pin_direction_initialize(&(lpinc[i]));
	}
	return ret;
}
STD_ReturnType keypad_read_number(const keypad_t *keypad, uint8 * value){
	STD_ReturnType ret = E_OK;
	uint8 i = 0, j = 0,c = 0;
	logic_t col_logic, row_logic;
	pin_config_t lpinr[KEY_ROWS],lpinc[KEY_COLUMNS];	
	if(NULL == keypad)
		ret = E_NOT_OK;
	else{
		ret = keypad_linit(keypad, lpinr, ROW);
		ret = keypad_linit(keypad, lpinc, COL);
		if(keypad -> connection == pull_down){
			for(j=0;j<KEY_ROWS;j++){
				for(i=0;i<KEY_ROWS;i++)
					GPIO_pin_write_logic(&(lpinr[i]), GPIO_LOW);
				GPIO_pin_write_logic(&(lpinr[j]), GPIO_HIGH);	
				for(c=0;c<KEY_COLUMNS;c++){
					//ret = GPIO_pin_read_logic(&(keypad -> keypad_columns[c]), &col_logic); 	
					ret = GPIO_pin_read_logic(&(lpinc[c]), &col_logic); 	
					if (col_logic == GPIO_HIGH)
						*value = keypad_elements[j][c];
					
					else{}
				}
			}
		}
		else{
			for(j=0;j<KEY_ROWS;j++){
				for(i=0;i<KEY_ROWS;i++)
					GPIO_pin_write_logic(&(lpinr[i]), GPIO_HIGH);
				GPIO_pin_write_logic(&(lpinr[j]), GPIO_LOW);	
				for(c=0;c<KEY_COLUMNS;c++){
					//ret = GPIO_pin_read_logic(&(keypad -> keypad_columns[c]), &col_logic); 	
					ret = GPIO_pin_read_logic(&(lpinc[c]), &col_logic); 	
					if (col_logic == GPIO_LOW){
						*value = keypad_elements[j][c];
					}
					else{}
				}
			}
	}
	}
	return ret;
}
