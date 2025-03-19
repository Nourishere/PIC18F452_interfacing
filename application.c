/* 
 * File:   application.c
 * Author: nour
 *
 * Created on October 23, 2024, 7:29 AM
 */
#include "application.h"
#include "/home/nour/programs/microchip/xc8/v2.50/pic/include/builtins.h"   

/*We are operating in the HS configuration of the internal oscillator which has
  an clock speed of 8MHz/16MHz*/
 //#define _XTAL_FREQ 8000000
//This macro is used in the __delay_ms() function
//#define _XTAL_FREQ 16000000
pin_config_t seg_units_en = { 
	PORTC_I,
	PIN2,
	GPIO_OUT,
	GPIO_LOW
};
pin_config_t seg_tenth_en = {
	PORTC_I,	
	PIN1,
	GPIO_OUT,
	GPIO_LOW
};
uint8 i=90, j=0, knum=0, prev;
int main(void){
    STD_ReturnType ret = E_OK;
    application_intialize();
        while(1){
            
			}
    
}

void application_intialize(){
    STD_ReturnType ret = E_OK;
	ecu_init();
	/* If you may intialize functions that are locally defined */
//   ret = GPIO_pin_intialize(&seg_units_en);
//   ret = GPIO_pin_intialize(&seg_tenth_en);
}
