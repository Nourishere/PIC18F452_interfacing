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
#if (INT_PR == INT_EN)
void __interrupt() InterruptManager(void){
	#if (INT_MSSP == INT_EN)
		if(INT_MSSP_STATUS == INT_EN && INT_MSSP_F == INT_HIGH){
			INT_MSSP_CLRF();
			MSSP_ISR();
		}
	#endif
	#if (INT_CCP1 == INT_EN)
		if(INT_CCP1_STATUS == INT_EN && INT_CCP1_F == INT_HIGH){
			INT_CCP1_CLRF();
			CCP1_ISR();
		}
	#endif
	#if (INT_CCP2 == INT_EN)
		if(INT_CCP2_STATUS == INT_EN && INT_CCP2_F == INT_HIGH){
			INT_CCP2_CLRF();
			CCP2_ISR();
		}
	#endif
	
	#if (INT_TMR0 == INT_EN)
		if(INT_TMR0_STATUS == INT_EN && INT_TMR0_F == INT_HIGH){
			INT_TMR0_CLRF();
			T0CONbits.TMR0ON=0; // Turn off the module
			TMR0_ISR();
		}
	#endif

	#if (INT_TMR1 == INT_EN)
		if(INT_TMR1_STATUS == INT_EN && INT_TMR1_F == INT_HIGH){
			INT_TMR1_CLRF();
			T1CONbits.TMR1ON=0; // Turn off the module
			TMR1_ISR();
		}
	#endif

	#if (INT_TMR2 == INT_EN)
		if(INT_TMR2_STATUS == INT_EN && INT_TMR2_F == INT_HIGH){
			INT_TMR2_CLRF();
			T2CONbits.TMR2ON=0; // Turn off the module
			TMR2_ISR();
		}
	#endif

	#if (INT_TMR3 == INT_EN)
		if(INT_TMR3_STATUS == INT_EN && INT_TMR3_F == INT_HIGH){
			INT_TMR3_CLRF();
			T3CONbits.TMR3ON=0; // Turn off the module
			TMR3_ISR();
		}
	#endif
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
	#if (INT_TMR0 == INT_EN)
		if(INT_TMR0_STATUS == INT_EN && INT_TMR0_F == INT_HIGH){
			INT_TMR0_CLRF();
			T0CONbits.TMR0ON=0; // Turn off the module
			TMR0_ISR();
		}
	
	#endif
	#if (INT_CCP1 == INT_EN)
		if(INT_CCP1_STATUS == INT_EN && INT_CCP1_F == INT_HIGH){
			INT_CCP1_CLRF();
			CCP1_ISR();
		}
	#endif
	#if (INT_CCP2 == INT_EN)
		if(INT_CCP2_STATUS == INT_EN && INT_CCP2_F == INT_HIGH){
			INT_CCP2_CLRF();
			CCP2_ISR();
		}
	#endif

	#if (INT_TMR1 == INT_EN)
		if(INT_TMR1_STATUS == INT_EN && INT_TMR1_F == INT_HIGH){
			INT_TMR1_CLRF();
			T1CONbits.TMR1ON=0; // Turn off the module
			TMR1_ISR();
		}
	#endif

	#if (INT_TMR2 == INT_EN)
		if(INT_TMR2_STATUS == INT_EN && INT_TMR2_F == INT_HIGH){
			INT_TMR2_CLRF();
			T2CONbits.TMR2ON=0; // Turn off the module
			TMR2_ISR();
		}
	#endif

	#if (INT_TMR3 == INT_EN)
		if(INT_TMR3_STATUS == INT_EN && INT_TMR3_F == INT_HIGH){
			INT_TMR3_CLRF();
			T3CONbits.TMR3ON=0; // Turn off the module
			TMR3_ISR();
		}
	#endif
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
	#if (INT_TMR0 == INT_EN)
		if(INT_TMR0_STATUS == INT_EN && INT_TMR0_F == INT_HIGH){
			INT_TMR0_CLRF();
			T0CONbits.TMR0ON=0; // Turn off the module
			TMR0_ISR();
		}
	#endif
	#if (INT_CCP1 == INT_EN)
		if(INT_CCP1_STATUS == INT_EN && INT_CCP1_F == INT_HIGH){
			INT_CCP1_CLRF();
			CCP1_ISR();
		}
	#endif
	#if (INT_CCP2 == INT_EN)
		if(INT_CCP2_STATUS == INT_EN && INT_CCP2_F == INT_HIGH){
			INT_CCP2_CLRF();
			CCP2_ISR();
		}
	#endif
	
	#if (INT_TMR1 == INT_EN)
		if(INT_TMR1_STATUS == INT_EN && INT_TMR1_F == INT_HIGH){
			INT_TMR1_CLRF();
			T1CONbits.TMR1ON=0; // Turn off the module
			TMR1_ISR();
		}
	#endif
	
	#if (INT_TMR2 == INT_EN)
		if(INT_TMR2_STATUS == INT_EN && INT_TMR2_F == INT_HIGH){
			INT_TMR2_CLRF();
			T2CONbits.TMR2ON=0; // Turn off the module
			TMR2_ISR();
		}
	#endif

	#if (INT_TMR3 == INT_EN)
		if(INT_TMR3_STATUS == INT_EN && INT_TMR3_F == INT_HIGH){
			INT_TMR3_CLRF();
			T3CONbits.TMR3ON=0; // Turn off the module
			TMR3_ISR();
		}
	#endif
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
}

#endif
