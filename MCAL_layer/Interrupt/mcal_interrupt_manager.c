/* 
 * File:   mcal_interrupt_manager.c
 * Author: nour
 *
 * Created on April 15, 2025, 2:28 PM
 */
#include "mcal_interrupt_manager.h"
#if INT_PR == INT_EN
void __interrupt() InterruptManager(void){
	if(INTCONbits.INT0IF == INT_EN && INTCONbits.INT0IE == INT_EN)
        INT0_ISR();
	if(INTCON3bits.INT1IF == INT_EN && INTCON3bits.INT1IE == INT_EN)
        INT1_ISR();
	if(INTCON3bits.INT2IF == INT_EN && INTCON3bits.INT2IE == INT_EN)
        INT2_ISR();
	if(INTCONbits.RBIF == INT_EN && INTCONbits.RBIE == INT_EN)
		RB_ISR();
}
void __interrupt(low_priority) InterruptManagerLow(void){
	if(INTCONbits.INT0IF == INT_EN && INTCONbits.INT0IE == INT_EN)
        INT0_ISR();
	if(INTCON3bits.INT1IF == INT_EN && INTCON3bits.INT1IE == INT_EN)
        INT1_ISR();
	if(INTCON3bits.INT2IF == INT_EN && INTCON3bits.INT2IE == INT_EN)
        INT2_ISR();
	if(INTCONbits.RBIF == INT_EN && INTCONbits.RBIE == INT_EN)
		RB_ISR();
}
#else
void __interrupt() InterruptManager(void){
	if(INTCONbits.INT0IF == INT_EN && INTCONbits.INT0IE == INT_EN)
        INT0_ISR();
	if(INTCON3bits.INT1IF == INT_EN && INTCON3bits.INT1IE == INT_EN)
        INT1_ISR();
	if(INTCON3bits.INT2IF == INT_EN && INTCON3bits.INT2IE == INT_EN)
        INT2_ISR();
	if(INTCONbits.RBIF == INT_EN && INTCONbits.RBIE == INT_EN)
		RB_ISR();
}
#endif
