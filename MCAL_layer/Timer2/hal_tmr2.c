/* 
 * File:   hal_tmr2.c
 * Author: nour
 *
 * Created on July 9, 2025, 6:46 PM
 * @note: Timer2 can only operate as a timer.
 */

#include "hal_tmr2.h"

uint8 preloaded_tmr2;

/* @brief: Initialize the Timer2 module.
 * @param: A pointer to a struct of type TMR2_t.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType TMR2_initialize(const TMR2_t * tmr2){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr2)
		ret = E_NOT_OK;
	else{
		/* Turn off the module */
		TMR2_OFF();		
#if (INT_TMR2 == INT_EN)
		/* Configure the interrupt */
#if (INT_PR == INT_EN)
		INT_TMR2_init(tmr2 -> priority); 
#else
		INT_TMR2_init(1); // garbage value
#endif
		INT_TMR2_set_callback_routine(tmr2 -> TMR2_interrupt_handler); 
#endif
		/* Configure the prescaler */
		if(tmr2 -> prescaler == TMR_PRESC_1_1)
			T2CONbits.T2CKPS=0x00;
		else if(tmr2 -> prescaler == TMR_PRESC_1_4)
			T2CONbits.T2CKPS=0x01;
		else if(tmr2 -> prescaler == TMR_PRESC_1_16)
			T2CONbits.T2CKPS=0x02;
		else{	
			ret = E_NOT_OK;
		}
		/* Configure the postscaler */
		TMR2_POSTSC_WR(tmr2 -> postscaler);		
		/* Set up preloaded value */
		TMR2_write(tmr2, &(tmr2 -> preloaded_value));
		/* Save a copy of the preloaded value */
		preloaded_tmr2 = tmr2 -> preloaded_value;
	}
	return ret;
}

/* @brief: Deinitialize the Timer2 module.
 * @param: A pointer to a struct of type TMR2_t.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType TMR2_deinitialize(const TMR2_t * tmr2){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr2)
		ret = E_NOT_OK;
	else{
		/* Turn off the interrupt */
		ret = INT_TMR2_deinit();	
		/* Turn off the module */
		TMR2_OFF();	
	}
	return ret;
}

/* @brief: Start Timer2 module.
 * @param: A pointer to a TMR2_t struct.
 * @return: E_OK upcon success and E_NOT_OK otherwise.
 */
STD_ReturnType TMR2_start(const TMR2_t * tmr2){
	STD_ReturnType ret = E_OK;
	if (NULL == tmr2)
		ret = E_NOT_OK;
	else
		TMR2_ON();
	return ret;
}

/* @brief: Stop the Timer2 module.
 * @param: A pointer to a TMR2_t struct.
 * @return: E_OK upcon success and E_NOT_OK otherwise.
 */
STD_ReturnType TMR2_stop(const TMR2_t * tmr2){
	STD_ReturnType ret = E_OK;
	if (NULL == tmr2)
		ret = E_NOT_OK;
	else
		TMR2_OFF();
	return ret;
}

/* @brief: Read the contents of the TMR2 register.
 * @param: A pointer to a TMR2_t struct
 * 		   and a pointer to a uint8 to store the read value.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 * @note: This routine reads the content of the TMR2 register.
 */
STD_ReturnType TMR2_read(const TMR2_t * tmr2, uint8 * rd_value){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr2 || rd_value == NULL)
		ret = E_NOT_OK;
	else{
		*rd_value = TMR2;
	}
	return ret;
}

/* @brief: Write to the TMR2 register.
 * @param: A pointer to a TMR2_t struct
 * 		   and a pointer to a uint8 for the value to be written. 
 * @return: E_OK upon success and E_NOT_OK otherwise.
 * @note: The TMR2 register is compared to the PR2 register for the interrupt 
 * 	      flag to be asserted. The PR register is initialized with 0xFF upon reset.
 */
STD_ReturnType TMR2_write(const TMR2_t * tmr2, uint8 * wr_value){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr2 || wr_value == NULL)
		ret = E_NOT_OK;
	else{
		TMR2 = (uint8)(*wr_value & 0xFF); 
	}
	return ret;
}

/* @brief: Write to the PR2 register.
 * @param: A pointer to a TMR2_t struct
 * 		   and a pointer to a uint8 for the value to be written. 
 * @return: E_OK upon success and E_NOT_OK otherwise.
 * @note: The PR2 register is used to comparison with the TMR2 register
 * 		  for which the interrupt flag is to be set when they are equal. 
 */
STD_ReturnType TMR2_write_PR_reg(const TMR2_t * tmr2, uint8 * wr_value){
	STD_ReturnType ret = E_OK;
	if(NULL == tmr2 || wr_value == NULL)
		ret = E_NOT_OK;
	else{
		PR2 = (uint8)(*wr_value & 0xFF); 
	}
	return ret;
}
