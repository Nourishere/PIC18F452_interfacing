/* 
 * File:   ecu_keypad.h
 * Author: nour
 *
 * Created on February 24, 2025, 7:24 AM
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H

#include "../../MCAL_layer/GPIO/mcal_gpio.h"

/****** Macros *****************/
#define KEY_ROWS 4
#define KEY_COLUMNS 4
#define ROW 0
#define COL 1
/****** Data types *************/
typedef enum{
	pull_down,	
	pull_up
}column_connection;
/* The keypad type has two arrays, one for the rows and one for 
   the columns. The rows are the ones that ouput speedy pulses to 
   test for any pressing. The columns are connected to GND or Vcc 
   based on the value of connection. */
typedef struct{
	port_pin_t keypad_rows[KEY_ROWS];		
	port_pin_t keypad_columns[KEY_COLUMNS];		
	column_connection connection;
}keypad_t;
/***** Function declarations ****/
static STD_ReturnType keypad_linit(const keypad_t *keypad, pin_config_t lpin[], uint8 rc); 
STD_ReturnType keypad_initialize(const keypad_t *keypad);
STD_ReturnType keypad_read_number(const keypad_t *keypad, uint8 * value);

#endif	/* ECU_KEYPAD_H */

