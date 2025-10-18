/* 
 * File:   mcal_tmr3.c
 * Author: nour
 *
 * Created on July 11, 2025, 11:11 PM
 */

#include "mcal_tmr3.h"

uint16 preloaded_tmr3;

#if (TMR3_MODULE == STD_ON)
extern pin_config_t T1CLKI_pin; // Defined in hal_tmr3.c

/* @Brief: This routine initializes the Timer3 module.
 * @Param: A pointer to a stuct of type TMR3_t representing the Timer0 module.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes: If the priority feature is disabled, the struct element (priority)
 * 		   is redundant and not used in the interrupt initialization.
 * 		   Timer3 is very similar to Tiemr1.
 */
STD_ReturnType TMR3_initialize(const TMR3_t * tmr3){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr3 || tmr3 -> prescaler > TMR_PRESC_1_8)
		ret = E_NOT_OK;
	else{
		/* Turn off the module */
		TMR3_OFF();		
		/* Set 16 bit rdwr mode */
		TMR3_REG_16();
#if (INT_TMR1 == INT_EN)
		/* Configure the interrupt */
#if (INT_PR == INT_EN)
		INT_TMR3_init(tmr3 -> priority); 
#else
		INT_TMR3_init(1); // garbage value
#endif
		INT_TMR3_set_callback_routine(tmr3 -> TMR3_interrupt_handler); 
#endif
		/* Configure the prescaler */
		TMR3_PRESC_WR(tmr3 -> prescaler);
		/* Set clock source*/  
		if(tmr3 -> clk_source == EXT){
			TMR3_EXT(); // Used as a counter
			/* Initialize the pin for counter mode */
			GPIO_pin_direction_initialize(&T1CLKI_pin); 
		}
		else if(tmr3 -> clk_source == INT)
			TMR3_INT(); // Used as a timer
		else
			ret = ret && E_NOT_OK;
	    /* Set up oscillator */
			
		/* Set the clock synchronization */
		/* @note: only available in counter mode */
		if(tmr3 -> clk_source == EXT){ 	
			if(tmr3 -> clk_sync == SYNC_ON)
				TMR3_ESYNC_ON();	
			else if(tmr3 -> clk_sync == SYNC_OFF)
				TMR3_ESYNC_OFF();	
		}
		else if(tmr3 -> clk_source == INT){
		}
		else
			ret = E_NOT_OK; 
		/* Set up preloaded value */
		TMR3_write(tmr3, &(tmr3 -> preloaded_value));
		/* Save a copy of the preloaded value */
		preloaded_tmr3 = tmr3 -> preloaded_value;
	}
	return ret;
}

/* @Brief: This routine deinitializes the Timer1 module.
 * @Param: A pointer to a struct of type TMR1_t representing a Timer1 module.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType TMR3_deinitialize(const TMR3_t * tmr3){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr3)
		ret = E_NOT_OK;
	else{
		/* Turn off the interrupt */
		ret = INT_TMR3_deinit();	
		/* Turn off the module */
		TMR3_OFF();	
	}
	return ret;
}

/* @Brief: Turn on the Timer 3 module. 
 * @Param: Pointer to a struct of type TMR3_t. 
 * @Return: E_OK upcon success and E_NOT_OK otherwise.
 */ 		   
STD_ReturnType TMR3_start(const TMR3_t * tmr3){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr3)
		ret = E_NOT_OK;
	else{
		TMR3_ON();
	}
	return ret;
}

/* @Brief: Stop the Timer3 module.
 * @Param: Pointer to a struct of type TMR3_t.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 */ 		   
STD_ReturnType TMR3_stop(const TMR3_t * tmr3){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr3)
		ret = E_NOT_OK;
	else{
		TMR3_OFF();
	}
	return ret;
}

/* @Brief: Read the contents of the Timer3 module.
 * @Param: A pointer to a struct of type TMR3_t representing a Timer3 module.
 * 		   A uint16 to store the read value.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes: The TMR3H register is updated when TMR3L is read. 
 */
STD_ReturnType TMR3_read(const TMR3_t * tmr3, uint16 * value){ 
	STD_ReturnType ret = E_OK;
	if(tmr3 == NULL || value == NULL)
		ret = E_NOT_OK;
	else{
		uint8 low = TMR3L; /*TMR3H is updated after this read */
		uint8 high = TMR3H; 
		*value = ((uint16)(high << 8)) | low;	
	}
	return ret;
}

/* @Brief: Write a value to the Timer3 module. 
 * @Param: A pointer to a struct of type TMR3_t representing a Timer3 module.
 * 		   A uint16 to write from. 
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes: The value in the TMR3H register is updated after a write happens to the 
 * 		   TMR3L register.
 */
STD_ReturnType TMR3_write(const TMR3_t * tmr3, uint16 * value){ 
	STD_ReturnType ret = E_OK;
	if(tmr3 == NULL || value == NULL)
		ret = E_NOT_OK;
	else{
		TMR3H = (uint8)((*value >> 8) & 0x00FF);
		TMR3L = (uint8)(*value & 0x00FF); /* TMR3H will be updated after this write */ 
	}
	return ret;
}
#endif
