/* 
 * File:   mcal_interrupt_manager.c
 * Brief: This source file contains the ISR that handle
 * 		  all interrupts on the deivce.
 * Author: nour
 *
 * Created on April 15, 2025, 2:28 PM
 */
#include "mcal_interrupt_manager.h"

static volatile uint8 RB4f=1,RB5f=1,RB6f=1,RB7f=1;
#if INT_PR == INT_EN
void __interrupt() InterruptManager(void){
	/* INTx checking */
	#if INT_INTx == INT_EN
	if(INTCONbits.INT0IF == INT_HIGH && INTCONbits.INT0IE == INT_EN)
        INT0_ISR();
	if(INTCON3bits.INT1IF == INT_HIGH && INTCON3bits.INT1IE == INT_EN)
        INT1_ISR();
	if(INTCON3bits.INT2IF == INT_HIGH && INTCON3bits.INT2IE == INT_EN)
        INT2_ISR();
	#endif
	/* RB checking */
	#if INT_PORTB == INT_EN
	if(INTCONbits.RBIF == INT_HIGH && INTCONbits.RBIE == INT_EN){
		CHECK_PIN_CHANGE(RB4,RB4f,RB4_ISR);
		CHECK_PIN_CHANGE(RB5,RB5f,RB5_ISR);
		CHECK_PIN_CHANGE(RB6,RB6f,RB6_ISR);
		CHECK_PIN_CHANGE(RB7,RB7f,RB7_ISR);
	}
	#endif
	/* ADC internal interrupt checking */
#if (INT_ADC == INT_EN)
	if(INT_ADC_STATUS == INT_EN && INT_ADC_F == INT_HIGH)
		ADC_ISR();
#endif
}
void __interrupt(low_priority) InterruptManagerLow(void){
	/* INTx checking */
	#if INT_INTx == INT_EN
	if(INTCONbits.INT0IF == INT_HIGH && INTCONbits.INT0IE == INT_EN)
        INT0_ISR();
	if(INTCON3bits.INT1IF == INT_HIGH && INTCON3bits.INT1IE == INT_EN)
        INT1_ISR();
	if(INTCON3bits.INT2IF == INT_HIGH && INTCON3bits.INT2IE == INT_EN)
        INT2_ISR();
	#endif
	/* RB checking */
	#if INT_PORTB == INT_EN
	if(INTCONbits.RBIF == INT_HIGH && INTCONbits.RBIE == INT_EN){
		CHECK_PIN_CHANGE(RB4,RB4f,RB4_ISR);
		CHECK_PIN_CHANGE(RB5,RB5f,RB5_ISR);
		CHECK_PIN_CHANGE(RB6,RB6f,RB6_ISR);
		CHECK_PIN_CHANGE(RB7,RB7f,RB7_ISR);
	}
	#endif
	/* ADC internal interrupt checking */
#if (INT_ADC == INT_EN)
	if(INT_ADC_STATUS == INT_EN && INT_ADC_F == INT_HIGH)
		ADC_ISR();
#endif
}
#else
/* No priority */
void __interrupt() InterruptManager(void){
	/* INTx checking */
	#if INT_INTx == INT_EN
	if(INTCONbits.INT0IF == INT_HIGH && INTCONbits.INT0IE == INT_EN)
        INT0_ISR();
	if(INTCON3bits.INT1IF == INT_HIGH && INTCON3bits.INT1IE == INT_EN)
        INT1_ISR();
	if(INTCON3bits.INT2IF == INT_HIGH && INTCON3bits.INT2IE == INT_EN)
        INT2_ISR();
	#endif
	/* RB checking */
	#if INT_PORTB == INT_EN
	if(INTCONbits.RBIF == INT_HIGH && INTCONbits.RBIE == INT_EN){
		CHECK_PIN_CHANGE(RB4,RB4f,RB4_ISR);
		CHECK_PIN_CHANGE(RB5,RB5f,RB5_ISR);
		CHECK_PIN_CHANGE(RB6,RB6f,RB6_ISR);
		CHECK_PIN_CHANGE(RB7,RB7f,RB7_ISR);
	}
	#endif
	/* ADC internal interrupt checking */
#if (INT_ADC == INT_EN)
	if(INT_ADC_STATUS == INT_EN && INT_ADC_F == INT_HIGH)
		ADC_ISR();
}
#endif
#endif
