/* 
 * File:   mcal_internal_interrupt.c
 * Author: nour
 *
 * Created on April 15, 2025, 2:24 PM
 */
#include "mcal_internal_interrupt.h"

#if (INT_ADC == INT_EN)
void (*ADC_callback) (uint16 * result) = NULL;
uint16 ADC_output;

void ADC_ISR(void){
	INT_ADC_CLRF();
	if(ADC_callback){
		ADC_output = (uint16) ( (((uint16)(ADRESH)) << 8) | ((uint16) (ADRESL)) );
		ADC_callback(&ADC_output);
	}
}

/* @brief: Initialize the ADC interrupt.
 * @param: A pointer to an ADC_t type.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType INT_ADC_init(uint8 priority){
	STD_ReturnType ret = E_OK;
		INT_ADC_CLRF();	
		INT_ADC_DIS();
#if (INT_PR == INT_EN)
		if(priority != INT_PHIGH && priority != INT_PLOW)
			ret = E_NOT_OK;
		else{
            INT_PREN(); /* enable the priority feature */
	 		/* @ref: hal_interrupt_config.h -> INT_PHIGH & INT_PLOW */
			if(priority == INT_PHIGH){
				INT_ADC_HP(); /* Enable ADC as a high priority */
            	INT_GHPEN(); /* Enable global high interrupts */
			}
			else if(priority == INT_PLOW){
				INT_ADC_LP();
            	INT_GLPEN();
				INT_GEN();	
			}
			else
				ret = E_NOT_OK;
           }
		INT_ADC_EN(); /* Enable the ADC peripheral */
#elif (INT_PR == INT_DIS)
        INT_PRDIS(); /* disable the priority feature */
		INT_GEN(); /* General interrupt enable */
		INT_PEEN(); /* Peripheral interrupt enable */
		INT_ADC_EN(); /* Enable the ADC peripheral */
#else 
		ret = E_NOT_OK;
#endif
	return ret;
}

/* @brief: De-initialize the ADC interrupt. 
 * @param: This function does not take any input.
 * @return: E_OK.
 */
STD_ReturnType INT_ADC_deinit(void){
	STD_ReturnType ret = E_OK;
	INT_ADC_CLRF();
	INT_ADC_DIS();
	return ret;
}

/* @brief: Set up the callback function for the ADC interrupt.
 * @param: A pointer to a ADC_t type.
 * @return: E_OK.
 */
STD_ReturnType INT_ADC_set_callback_routine(void (*callback) (uint16 * result)){
	STD_ReturnType ret = E_OK;
		ADC_callback = callback;	
	return ret;
}
#endif
#if (INT_TMR1 == INT_EN)
void (*TMR1_callback) (void) = NULL;
extern uint16 preloaded_tmr1;
void TMR1_ISR(void){
	/* Flag is already clear */
	/* Preload the registers */
	TMR1H = (uint8)(preloaded_tmr1 >> 8);
	TMR1L = (uint8)(preloaded_tmr1 & 0x00FF);
	if(TMR1_callback){
		TMR1_callback();
	}
	T1CONbits.TMR1ON=1; // Turn on the module
}

/* @brief: Initialize the interrupt feature for the Timer1 module.
 * @param: A uint8 specifying interrupt priority.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType INT_TMR1_init(uint8 priority){
	STD_ReturnType ret = E_OK;
	INT_GEN(); // Enable the interupt feature
	/* Disable the interrupt */
	INT_TMR1_DIS();
	#if (INT_PR == INT_EN)
	INT_PREN(); // Enable priority feature
	if(priority == INT_PHIGH){
		INT_GHPEN();
		INT_TMR1_HP();
	}
	else if (priority == INT_PLOW){
		INT_GLPEN();
		INT_TMR1_LP();
	}
	#elif (INT_PR == INT_DIS)
	INT_PRDIS();
	INT_PEEN();  // Enable peripheral interupts
	#else
	ret = E_NOT_OK;
	#endif
	INT_TMR1_EN();
	return ret;
}

/* @brief: Deinitialize the Timer1 module.
 * @param: None.
 * @return: E_OK.
 */
STD_ReturnType INT_TMR1_deinit(void){
	STD_ReturnType ret = E_OK;
	INT_TMR1_CLRF();
	INT_TMR1_DIS();
	return ret;
}

/* @brief: Set a callback routine for the Timer1 module.
 * @param: A pointer to a function with void parameters and void return.
 * @return: E_OK;
 */
STD_ReturnType INT_TMR1_set_callback_routine(void (*callback) (void)){
	STD_ReturnType ret = E_OK;
	TMR1_callback = callback;
	return ret;
}
#endif

#if (INT_TMR2 == INT_EN)
void (*TMR2_callback) (void) = NULL;
extern uint8 preloaded_tmr2;
void TMR2_ISR(void){
	/* Flag is already clear */
	/* Preload the registers */
	TMR2 = (preloaded_tmr2);
	if(TMR2_callback){
		TMR2_callback();
	}
	T2CONbits.TMR2ON=1; // Turn on the module
}

/* @brief: Initialize the interrupt feature for the Timer2 module.
 * @param: A uint8 specifying interrupt priority.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType INT_TMR2_init(uint8 priority){
	STD_ReturnType ret = E_OK;
	INT_GEN(); // Enable the interupt feature
	/* Disable the interrupt */
	INT_TMR2_DIS();
	#if (INT_PR == INT_EN)
	INT_PREN(); // Enable priority feature
	if(priority == INT_PHIGH){
		INT_GHPEN();
		INT_TMR2_HP();
	}
	else if (priority == INT_PLOW){
		INT_GLPEN();
		INT_TMR2_LP();
	}
	#elif (INT_PR == INT_DIS)
	INT_PRDIS();
	INT_PEEN();  // Enable peripheral interupts
	#else
	ret = E_NOT_OK;
	#endif
	INT_TMR2_EN();
	return ret;
}

/* @brief: Deinitialize the Timer2 module.
 * @param: None.
 * @return: E_OK.
 */
STD_ReturnType INT_TMR2_deinit(void){
	STD_ReturnType ret = E_OK;
	INT_TMR2_CLRF();
	INT_TMR2_DIS();
	return ret;
}

/* @brief: Set a callback routine for the Timer2 module.
 * @param: A pointer to a function with void parameters and void return.
 * @return: E_OK;
 */
STD_ReturnType INT_TMR2_set_callback_routine(void (*callback) (void)){
	STD_ReturnType ret = E_OK;
	TMR2_callback = callback;
	return ret;
}
#endif

#if (INT_TMR3 == INT_EN)
void (*TMR3_callback) (void) = NULL;
extern uint16 preloaded_tmr3;
void TMR3_ISR(void){
	/* Flag is already clear */
	/* Preload the registers */
	TMR3 = (preloaded_tmr3);
	if(TMR3_callback){
		TMR3_callback();
	}
	T3CONbits.TMR3ON=1; // Turn on the module
}

STD_ReturnType INT_TMR3_init(uint8 priority){
	STD_ReturnType ret = E_OK;
	INT_GEN(); // Enable the interupt feature
	/* Disable the interrupt */
	INT_TMR3_DIS();
	#if (INT_PR == INT_EN)
	INT_PREN(); // Enable priority feature
	if(priority == INT_PHIGH){
		INT_GHPEN();
		INT_TMR3_HP();
	}
	else if (priority == INT_PLOW){
		INT_GLPEN();
		INT_TMR3_LP();
	}
	#elif (INT_PR == INT_DIS)
	INT_PRDIS();
	INT_PEEN();  // Enable peripheral interupts
	#else
	ret = E_NOT_OK;
	#endif
	INT_TMR3_EN();
	return ret;
}
STD_ReturnType INT_TMR3_deinit(void){
	STD_ReturnType ret = E_OK;
	INT_TMR2_CLRF();
	INT_TMR2_DIS();
	return ret;
}
STD_ReturnType INT_TMR3_set_callback_routine(void (*callback) (void)){
	STD_ReturnType ret = E_OK;
	TMR3_callback = callback;
	return ret;
}
#endif
