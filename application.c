/* 
 * File:   application.c
 * Author: nour
 *
 * Created on October 23, 2024, 7:29 AM
 */
#include "application.h"
#include "/home/nour/programs/microchip/xc8/v2.50/pic/include/builtins.h"   
#include "MCAL_layer/ADC/hal_adc.h"
#include "MCAL_layer/Interrupt/mcal_internal_interrupt.h"
uint8 Iflag;
/* We are operating in the HS configuration of the internal oscillator which has
  an clock speed of 8MHz/16MHz */
 //#define _XTAL_FREQ 8000000
//This macro is used in the __delay_ms() function
ADC_t ADC1 = {
	__ADC,
	INT_PLOW, 	
	ADC_ch1,
	ADC_CCLOCK_FOSC_DIV_32,
	1
};

STD_ReturnType ret;
int main(void){
        ret = application_initialize();
	if(E_NOT_OK == ret){
		return -1; 
	}
    while(1){
		LED_toggle(&LED_main);      
		__delay_ms(1000);
	}
    
}

STD_ReturnType application_initialize(){
    STD_ReturnType ret = E_OK;
    /* If you may initialize types that are locally defined */
	ret = ecu_init() &&
		  ADC_init(&ADC1);
    return ret;
}

void __INT0(void){
}
void __INT1(void){
}
void __INT2(void){
}
void __ADC(uint16 * result){
	/* do something when the ADC 
  	conversion is done */
	uint16 lresult = *result; /* The result is stored */
        

}
