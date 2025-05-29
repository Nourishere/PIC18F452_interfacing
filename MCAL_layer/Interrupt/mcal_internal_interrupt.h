/* 
 * File:   mcal_internal_interrupt.h
 * Author: nour
 * Brief: This header file contains the configuration of
 * 		  all internal and peripheral interrupt features.
 * Created on April 15, 2025, 2:25 PM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H
/***** includes ********/
#include "mcal_interrupt_config.h"
/***** macros *********/
/************ ADC interrupt *****************/
/* Status of the ADC (EN/DIS) */
#define INT_ADC_STATUS (PIE1bits.ADIE)
/* The value of the ADC flag */
#define INT_ADC_F (PIR1bits.ADIF)
/****** function-like macros ******/
/************ ADC interrupt *****************/
#if (INT_ADC == INT_EN)
/* Enable/Disable the ADC interrupt */
#define INT_ADC_EN() (PIE1bits.ADIE=1)
#define INT_ADC_DIS() (PIE1bits.ADIE=0)
/* Clear the ADC interrupt flag */
#define INT_ADC_CLRF() (PIR1bits.ADIF=0)
/* Set ADC low/high priority */
#if (INT_PR == INT_EN)
#define INT_ADC_HP() (IPR1bits.ADIP=1)
#define INT_ADC_LP() (IPR1bits.ADIP=0)
#endif
#endif

/***** Function prototypes *****/
void ADC_ISR(void);
#if (INT_ADC == INT_EN)
STD_ReturnType INT_ADC_init(uint8 priority);
STD_ReturnType INT_ADC_deinit(void);
STD_ReturnType INT_ADC_set_callback_routine(void (*callback) (uint16 * result));
#endif

#endif	/* MCAL_INTERNAL_INTERRUPT_H */
