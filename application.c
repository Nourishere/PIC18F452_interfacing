/* 
 * File:   application.c
 * Author: nour
 *
 * Created on October 23, 2024, 7:29 AM
 */
#include "application.h"
#include "/home/nour/programs/microchip/xc8/v2.50/pic/include/builtins.h"   
uint8 Iflag;
/* We are operating in the HS configuration of the internal oscillator which has
  an clock speed of 8MHz/16MHz */
 //#define _XTAL_FREQ 8000000
//This macro is used in the __delay_ms() function

INT_RBx_t first_int = {
	NULL,
        NULL,
	{PORTB_I, PIN3, GPIO_IN, GPIO_LOW},
	INT_PLOW
};
INT_INTx_t second_int = {
	NULL,
	{PORTB_I, PIN1, GPIO_IN, GPIO_LOW},
        rising,
	INT_PLOW
};

pin_config_t seg_units_en = { 
	PORTD_I,
	PIN1,
	GPIO_OUT,
	GPIO_HIGH
};

uint8 i=90, j=0, knum=0, prev;

int main(void){
    STD_ReturnType ret = E_NOT_OK;
    if(INT_INTx_initialize(&second_int))
        LED_on(&LED_OK);
    else
        LED_on(&LED_NOK);
   
 //   ret = lcd_send_char_data_position(&LCD1,1,4,'A');
//    ret = lcd_send_command(&LCD1,LCD_8BIT_1LINE_5x10);
//	ret = lcd_send_string(&LCD1,"Nour");
        while(1){
          
		}
    
}

STD_ReturnType application_initialize(){
    STD_ReturnType ret = E_OK;
    /* If you may initialize functions that are locally defined */
	ret = ecu_init();
       
    return ret;
}

void __INT0(void){
	Iflag++;
}
void __INT1(void){
    Iflag++;
}
void __INT2(void){
    Iflag++;
}
void __RB(void){
    Iflag++;
}

