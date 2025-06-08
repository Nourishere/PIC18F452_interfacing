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
#include "MCAL_layer/Interrupt/mcal_external_interrupt.h"
volatile uint8 Iflag;
/* We are operating in the HS configuration of the internal oscillator which has
  an clock speed of 8MHz/16MHz */
 //#define _XTAL_FREQ 8000000
//This macro is used in the __delay_ms() function
ADC_t ADC1 = {
	__ADC,
	//INT_PLOW, 	
	ADC_ch7,
	ADC_CCLOCK_FOSC_DIV_32,
	1
};
INT_INTx_t INT_1 = {
    __INT1,
    {PORTB_I,PIN1,GPIO_IN,GPIO_LOW},
    rising,
   // INT_PLOW
};

STD_ReturnType ret;
int main(void){
        ret = application_initialize();
	if(E_NOT_OK == ret){
		return -1; 
	}
        
    while(1){
    	ADC_convert(&ADC1,ADC_default); 
        __delay_us(10); /* Delay for acquisition */
	}
    
}

STD_ReturnType application_initialize(){
    STD_ReturnType ret = E_OK;
    /* If you may initialize types that are locally defined */
	ret = ecu_init();
       ret = ret && ADC_init(&ADC1);
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
	uint8 i = 0;	
	uint16 lresult = *result; /* The result is stored here */

	for(i=0;i<10;i++){
		if(*result >> i & 1)
			LED_on(&LED_arr[i]);
          	else
            		LED_off(&LED_arr[i]); 
	}

}
