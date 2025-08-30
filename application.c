/* 
 * File:   application.c
 * Author: nour
 *
 * Created on October 23, 2024, 7:29 AM
 */
#include "application.h"
#include "/home/nour/programs/microchip/xc8/v2.50/pic/include/builtins.h"   
#include "MCAL_layer/CCP/hal_ccp.h"
#include "MCAL_layer/ADC/hal_adc.h"
#include "MCAL_layer/Timer0/hal_tmr0.h"
#include "MCAL_layer/Interrupt/mcal_internal_interrupt.h"
#include "MCAL_layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_layer/USART/hal_usart.h"
volatile uint8 Iflag;
#define MYREG (*(volatile unsigned char*)0xF81)

/* We are operating in the HS configuration of the internal oscillator which has
  an clock speed of 8MHz/16MHz */
//#define _XTAL_FREQ 8000000
//This macro is used in the __delay_ms() function

CCP1_t ccp1 = {
	__CCP1,
	INT_PLOW,	
	CCP_PWM,
	{
		.pwm={
				40000,
				50,
				{/* Timer */
                                        NULL,
					INT_PLOW, /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
					TMR_PRESC_1_1, 
					0xAA, // Initial preloaded value   
					TMR2_POSTSC_1_1
				}
			}
	}	
};
STD_ReturnType ret;
uint16 value=0xAA;
USART_Tx_t usar;
int main(void){
    //MYREG = 0xAA;
    *(long*)0xF82 = 0x11;
    ret = application_initialize();
	if(E_NOT_OK == ret){
		return -1; 
	}
    while(1){
    //    USART_baud_initialize(& usar);
	}
    
}

STD_ReturnType application_initialize(){
    STD_ReturnType ret = E_OK;
	//CCP1_initialize(&ccp1);
//	ret = ecu_init();
    /* If you may initialize types that are locally defined */

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
void __TMR0(void){
	Iflag++;
}
void __CCP1(void){
	Iflag++;
}
