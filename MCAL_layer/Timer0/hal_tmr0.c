/* 
 * File:   hal_tmr0.h
 * Author: nour
 *
 * Created on June 27, 2025, 10:20 AM
 * @Brief: This source file contains the Timer0 driver implementation.
 */
#include "hal_tmr0.h"

/* @Brief: A copy of the preloaded value defined in TMR0_t 
 * 		   used to preloaded the register on overflow.
 */
uint16 volatile preloaded;

/* @Brief: This routine initializes the Timer0 module.
 * @Param: A pointer to a stuct of type TMR0_t representing the Timer0 module.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes: If the priority feature is disabled, the struct element (priority)
 * 		   is redundant and not used in the interrupt initialization.
 */
STD_ReturnType TMR0_initialize(const TMR0_t * tmr0){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr0)
		ret = E_NOT_OK;
	else{
		/* Turn off the module */
		TMR0_OFF();		
#if (INT_TMR0 == INT_EN)
		/* Configure the interrupt */
#if (INT_PR == INT_EN)
		INT_TMR0_init(tmr0 -> priority); 
#else
		INT_TMR0_init(1); // garbage value
#endif
		INT_TMR0_set_callback_routine(tmr0 -> TMR0_interrupt_handler); 
#endif
		/* Configure the prescaler */
		if(tmr0 -> prescaler == TMR0_PRESC_NONE)
			TMR0_PRESC_OFF(); // Turn off the prescaler	
		else{	
			TMR0_PRESC_ON(); // Turn on the prescaler
			TMR0_PRESC_WR(tmr0 -> prescaler);
		}
		/* Set up mode */
		if(tmr0 -> mode == _8BIT)
			TMR0_8BIT();		
		else if(tmr0 -> mode == _16BIT)
			TMR0_16BIT();		
		else
			ret = E_NOT_OK;
		/* Set the edge */
		if(tmr0 -> edge == RISING)
			TMR0_RIS();	
		else if(tmr0 -> edge == FALLING)
			TMR0_FALL();	
		else
			ret = ret && E_NOT_OK;
		/* Set clock source*/  
		if(tmr0 -> clk_source == EXT)
			TMR0_EXT(); // Used as a counter
		else if(tmr0 -> clk_source == INT)
			TMR0_INT(); // Used as a timer
		else
			ret = ret && E_NOT_OK;
		/* Set up preloaded value */
		TMR0_write(&tmr0, &(tmr0 -> preloaded_value));
		/* Save a copy of the preloaded value */
		preloaded = tmr0 -> preloaded_value;
	}

	return ret;
}

/* @Brief: This routine deinitializes the Timer0 module.
 * @Param: A pointer to a struct of type TMR0_t representing a Timer0 module.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes:
 */
STD_ReturnType TMR0_deinitialize(const TMR0_t * tmr0){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr0)
		ret = E_NOT_OK;
	else{
		/* Turn off the interrupt */
		ret = INT_TMR0_deinit();	
		/* Turn off the module */
		TMR0_OFF();	
	}
	return ret;
}

/* @Brief: Read the contents of the timer.
 * @Param: A pointer to a struct of type TMR0_t representing a Timer0 module.
 * 		   A uint16 to store the read value.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes: The TMR0H register is updated when TMR0L is read. 
 * 		   In 8 bit mode, the result is stored in the lower 8 bits of hte uint16 variable.
 */
STD_ReturnType TMR0_read(const TMR0_t * tmr0, uint16 * value){ 
	STD_ReturnType ret = E_OK;
	if(tmr0 == NULL || value == NULL)
		ret = E_NOT_OK;
	else{
		if(tmr0 -> mode == _8BIT) /* 8 bit mode */
			*value = (uint16)(0x00FF & TMR0L);
		else if(tmr0 -> mode == _16BIT){
			uint8 low = TMR0L; /*TMR0H is updated after this read */
			uint8 high = TMR0H; 
			*value = ((uint16)(high << 8)) | low;	
		}
		else
			ret = E_NOT_OK;
	}
	return ret;
}

/* @Brief: Write a value to the timer. 
 * @Param: A pointer to a struct of type TMR0_t representing a Timer0 module.
 * 		   A uint16 to write from. 
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes: The value in the TMR0H register is updated after a write happens to the 
 * 		   TMR0L register.
 */
STD_ReturnType TMR0_write(const TMR0_t * tmr0, uint16 * value){ 
	STD_ReturnType ret = E_OK;
	if(tmr0 == NULL || value == NULL)
		ret = E_NOT_OK;
	else{
		if(tmr0 -> mode == _8BIT)
			TMR0L = (uint8)(*value & 0x00FF); 
			
		else if (tmr0 -> mode == _16BIT){
			TMR0H = (uint8)((*value >> 8) & 0x00FF);
			TMR0L = (uint8)(*value & 0x00FF); /* TMR0H will be updated after this write */ 
		}
		else
			ret = E_NOT_OK;
	}
	return ret;
}

/* @Brief: Stop the timer0 module.
 * @Param: A pointer to a struct of type TMR0_t.
 * @Return: E_OK upon success and E_NOT_OK if the module is 
 * 			not initialized correctly.
 */
STD_ReturnType TMR0_start(const TMR0_t * tmr0){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr0)
		ret = E_NOT_OK;
	else{
		TMR0_ON();	
	}
	return ret;
}

/* @Brief: Stop the timer0 module.
 * @Param: A pointer to a struct of type TMR0_t.
 * @Return: E_OK upon success and E_NOT_OK if the module is 
 * 			not initialized correctly.
 */
STD_ReturnType TMR0_stop(const TMR0_t * tmr0){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr0)
		ret = E_NOT_OK;
	else{
		TMR0_OFF();	
	}
	return ret;
}

/* @Brief: (Beta) Delay time in ms using the timer0 module.
 * @Param: A pointer to a TMR0_t struct representing a timer, a delay time in ms
 * 		   and a pointer to a uint8 to return the accuacy of the operation.
 * @Return: E_OK upon success and accuracy is bigger than 90%  and E_NOT_OK otherwise.
 */
/*
STD_ReturnType TMR0_delayms(const TMR0_t * tmr0, uint16 delay, uint8* accuracy){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr0)
		ret = E_NOT_OK;
	else{
		double val = 0;	
		uint16 ratio = 0;
		uint64 fosc = 8000000;
		ret = TMR0_scale2ratio(tmr0 -> prescaler, &ratio);
		if(tmr0 -> mode == _8BIT){
			val = (256 - ((delay * fosc) / (ratio * 4000)));	
			ret = ret & TMR0_write(&tmr0, &((uint16)val)); 	
			*accuracy = (uint8)((uint16) val / val)  * 100; 
		}
		else if(tmr0 -> mode == _16BIT){
			val = (65536 - ((delay * fosc) / (ratio * 4000)));	
			ret = ret & TMR0_write(&tmr0, &(val)); 	
			*accuracy = (uint8)((uint16) val / val)  * 100; 
		}
		else
			ret = E_NOT_OK;
	}
	if(*accuracy < 90)
		ret = E_NOT_OK;
	return ret;
}

/* @Brief: Convert a prescale type to a ratio.
 * @Param: A type TMR0_PRESC_SEL representing a precaler
 * 		   and a pointer to a uint16 to store the output.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 */
/*
static STD_ReturnType TMR0_scale2ratio(TMR0_PRESC_SEL prescale, uint16 * val){
	STD_ReturnType ret = E_OK;
	if(NULL == val)
		ret = E_NOT_OK;
	else{
	switch(prescale){
		case(TMR0_PRESC_NONE):
			*val = 1;
			break;
		case(TMR0_PRESC_1_2):
			*val = 2;
			break;
		case(TMR0_PRESC_1_4):
			*val = 4;
			break;
		case(TMR0_PRESC_1_8):
			*val = 8;
			break;
		case(TMR0_PRESC_1_16):
			*val = 16;
			break;
		case(TMR0_PRESC_1_32):
			*val = 32;
			break;
		case(TMR0_PRESC_1_64):
			*val = 64;
			break;
		case(TMR0_PRESC_1_128):
			*val = 128;
			break;
		case(TMR0_PRESC_1_256):
			*val = 256;
			break;
		default:	
			ret = E_NOT_OK;
			break;
		}
	}
	return ret;
} 
*/
