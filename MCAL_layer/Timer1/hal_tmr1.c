/* 
 * File:   hal_tmr1.c
 * Author: nour
 *
 * Created on July 3, 2025, 2:09 PM
 */
#include "hal_tmr1.h"

uint16  preloaded_tmr1;

/* @Brief: A pin_config_t struct to be used to 
 * 		   initialize the counter mode pin 
 */
pin_config_t T1CLKI_pin = {
	PORTC_I,
	PIN0,
	GPIO_IN,
	GPIO_LOW, // garbage
};

/* @Brief: This routine initializes the Timer1 module.
 * @Param: A pointer to a stuct of type TMR1_t representing the Timer0 module.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes: If the priority feature is disabled, the struct element (priority)
 * 		   is redundant and not used in the interrupt initialization.
 */
STD_ReturnType TMR1_initialize(const TMR1_t * tmr1){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr1 || (tmr1 -> prescaler > TMR_PRESC_1_8))
		ret = E_NOT_OK;
	else{
		/* Turn off the module */
		TMR1_OFF();		
		/* Set 16 bit rdwr mode */
		TMR1_REG_16();
#if (INT_TMR1 == INT_EN)
		/* Configure the interrupt */
#if (INT_PR == INT_EN)
		INT_TMR1_init(tmr1 -> priority); 
#else
		INT_TMR1_init(1); // garbage value
#endif
		INT_TMR1_set_callback_routine(tmr1 -> TMR1_interrupt_handler); 
#endif
		/* Configure the prescaler */
		TMR_PRESC_WR(tmr1 -> prescaler);
		/* Set clock source*/  
		if(tmr1 -> clk_source == EXT){
			TMR1_EXT(); // Used as a counter
			/* Initialize the pin for counter mode */
			GPIO_pin_direction_initialize(&T1CLKI_pin); 
		}
		else if(tmr1 -> clk_source == INT)
			TMR1_INT(); // Used as a timer
		else
			ret = ret && E_NOT_OK;
	    /* Set up oscillator */
			
		/* Set the clock synchronization */
		/* @note: only available in counter mode */
		if(tmr1 -> clk_source == EXT){ 	
			if(tmr1 -> clk_sync == SYNC_ON)
				TMR1_ESYNC_ON();	
			else if(tmr1 -> clk_sync == SYNC_OFF)
				TMR1_ESYNC_OFF();	
		}
		else if(tmr1 -> clk_source == INT){
		}
		else
			ret = E_NOT_OK; 
		/* Set up preloaded value */
		TMR1_write(&tmr1, &(tmr1 -> preloaded_value));
		/* Save a copy of the preloaded value */
		preloaded_tmr1 = tmr1 -> preloaded_value;
	}
	return ret;
}

/* @Brief: This routine deinitializes the Timer1 module.
 * @Param: A pointer to a struct of type TMR1_t representing a Timer1 module.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType TMR1_deinitialize(const TMR1_t * tmr1){
	STD_ReturnType ret = E_OK;
	if (NULL == tmr1)
		ret = E_NOT_OK;
	else{
		/* Turn off the interrupt */
		ret = INT_TMR1_deinit();	
		/* Turn off the module */
		TMR1_OFF();	
	}
	return ret;
}

/* @Brief: Stop the timer1 module.
 * @Param: A pointer to a struct of type TMR1_t.
 * @Return: E_OK upon success and E_NOT_OK if the module is 
 * 			not initialized correctly.
 */
STD_ReturnType TMR1_start(const TMR1_t * tmr1){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr1)
		ret = E_NOT_OK;
	else{
		TMR1_ON();	
	}
	return ret;
}

/* @Brief: Stop the timer1 module.
 * @Param: A pointer to a struct of type TMR1_t.
 * @Return: E_OK upon success and E_NOT_OK if the module is 
 * 			not initialized correctly.
 */
STD_ReturnType TMR1_stop(const TMR1_t * tmr1){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr1)
		ret = E_NOT_OK;
	else{
		TMR1_OFF();	
	}
	return ret;
}

/* @Brief: Read the contents of the Timer1 module.
 * @Param: A pointer to a struct of type TMR1_t representing a Timer1 module.
 * 		   A uint16 to store the read value.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes: The TMR1H register is updated when TMR1L is read. 
 */
STD_ReturnType TMR1_read(const TMR1_t * tmr1, uint16 * value){ 
	STD_ReturnType ret = E_OK;
	if(tmr1 == NULL || value == NULL)
		ret = E_NOT_OK;
	else{
		uint8 low = TMR1L; /*TMR1H is updated after this read */
		uint8 high = TMR1H; 
		*value = ((uint16)(high << 8)) | low;	
	}
	return ret;
}

/* @Brief: Write a value to the Timer1 module. 
 * @Param: A pointer to a struct of type TMR1_t representing a Timer1 module.
 * 		   A uint16 to write from. 
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes: The value in the TMR1H register is updated after a write happens to the 
 * 		   TMR1L register.
 */
STD_ReturnType TMR1_write(const TMR1_t * tmr1, uint16 * value){ 
	STD_ReturnType ret = E_OK;
	if(tmr1 == NULL || value == NULL)
		ret = E_NOT_OK;
	else{
		TMR1H = (uint8)((*value >> 8) & 0x00FF);
		TMR1L = (uint8)(*value & 0x00FF); /* TMR1H will be updated after this write */ 
	}
	return ret;
}

