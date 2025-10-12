/* 
 * File:   ecu_init.c
 * Author: nour
 *
 * Created on February 26, 2025, 7:34 AM
 */
#include "ecu_init.h"

#if (__ECU == STD_ON)
LED_t LED_arr[10] = {
	{PORTD_I,PIN7,GPIO_LOW},	
	{PORTD_I,PIN6,GPIO_LOW},	
	{PORTD_I,PIN5,GPIO_LOW},	
	{PORTD_I,PIN4,GPIO_LOW},	
	{PORTC_I,PIN7,GPIO_LOW},	
	{PORTC_I,PIN6,GPIO_LOW},	
	{PORTC_I,PIN5,GPIO_LOW},	
	{PORTC_I,PIN4,GPIO_LOW},	
	{PORTD_I,PIN3,GPIO_LOW},	
	{PORTD_I,PIN2,GPIO_LOW}
};
uint8 i;
STD_ReturnType ecu_init(void){
    STD_ReturnType ret = E_OK;
	for(i=0;i<10;i++)
		ret = ret && LED_initialize(&(LED_arr[i]));
    return ret;
}
#endif

