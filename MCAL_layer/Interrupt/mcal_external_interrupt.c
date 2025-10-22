/* 
 * File:   mcal_external_interrupt.c
 * Author: nour
 *
 * Created on April 15, 2025, 2:27 PM
 */
#include "mcal_external_interrupt.h"

#if (EXT_INTERRUPT_MODULE == STD_ON)
/* Locally defined interrupt handling functions */
void (*INT0_handler) (void) = NULL;
void (*INT1_handler) (void) = NULL;
void (*INT2_handler) (void) = NULL;
/* @note: High handlers are for rising edges while
 * low are for falling edges.
 *
 * If you wish to use the RB interrupts as
 * change interrupts, use NULL for both callback.
 */
void (*RB4_handler_low)  (void) = NULL;
void (*RB4_handler_high) (void) = NULL;
void (*RB5_handler_high) (void) = NULL;
void (*RB5_handler_low)  (void) = NULL;
void (*RB6_handler_high) (void) = NULL;
void (*RB6_handler_low)  (void) = NULL;
void (*RB7_handler_high) (void) = NULL;
void (*RB7_handler_low)  (void) = NULL;

#if (INT_TMR0== INT_EN)
void (*TMR0_callback) (void) = NULL;
extern volatile uint16 preloaded;
void TMR0_ISR(void){
	/* Flag is already clear */
	/* Preload the registers */
	TMR0H = (uint8)(preloaded >> 8);
	TMR0L = (uint8)(preloaded & 0x00FF);
	if(TMR0_callback){
		TMR0_callback();
	}
	T0CONbits.TMR0ON=1; // Turn on the module 
}
#endif
#if (INT_INTx == INT_EN)
void INT0_ISR(){
	INT_INT0_CLRF();
	if(INT0_handler)
		INT0_handler(); /* calls back a function from application.c */
}
void INT1_ISR(){
	INT_INT1_CLRF();
	if(INT1_handler)
		INT1_handler(); 
}
void INT2_ISR(){
	INT_INT2_CLRF();
	if(INT2_handler)
		INT2_handler(); 
}
#endif
#if (INT_PORTB == INT_EN)
void RB4_ISR(uint8 fl){
	INT_RB_CLRF();
	if(fl){
		if(RB4_handler_high)
			RB4_handler_high();
    }
	else{
		if(RB4_handler_low)
			RB4_handler_low();
	}	
}
void RB5_ISR(uint8 fl){
	INT_RB_CLRF();
	if(fl){
		if(RB5_handler_high)
			RB5_handler_high();
    }
	else{
		if(RB5_handler_low)
			RB5_handler_low();
	}	
}
void RB6_ISR(uint8 fl){
	INT_RB_CLRF();
	if(fl){
		if(RB6_handler_high)
			RB6_handler_high();
    }
	else{
		if(RB6_handler_low)
			RB6_handler_low();
	}	
}
void RB7_ISR(uint8 fl){
	INT_RB_CLRF();
	if(fl){
		if(RB7_handler_high)
			RB7_handler_high();
	}	
	else{
		if(RB7_handler_low)
			RB7_handler_low();
    }
}
#endif

/*@brief: Initialize a INTx interrupt pin.
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
#if (INT_INTx == INT_EN)
STD_ReturnType INT_INTx_initialize(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint || E_NOT_OK == INT_INTx_check_access(lint))
		ret = E_NOT_OK;
	else{
		/* disables INTx */
		ret = INT_INTx_disable(lint);
		/* clear flag */
		ret &= INT_INTx_clear_flag(lint);
		/* initialize interrupt pin */
		ret &= GPIO_pin_direction_initialize(&(lint -> Ipin));
		/* initialize interrupt callback routine */
		ret &= INT_INTx_set_callback_routine(lint);
		/* initialize priority */
#if (INT_PR == INT_EN)
		ret &= INT_INTx_priority_initialize(lint);
#endif
		/* initialize edge */
		ret &= INT_INTx_edge_initialize(lint);
		/* enable INTx */
		ret &= INT_INTx_enable(lint);
	}
	return ret;
}

/*@brief: Enable an INTx interrupt pin. 
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
STD_ReturnType INT_INTx_enable(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	INTx_index ind = NA;
	if(NULL == lint)
		ret = E_NOT_OK;
#if (INT_PR == INT_EN)
	/* Check for invalid priority */
	else if((lint -> priority != INT_PHIGH) && (lint -> priority != INT_PLOW))
		ret = E_NOT_OK;
#endif
	else{
		ind = INT_INTx_get_index(lint);
		switch(ind) {
			case(INT0_I):
				INT_INT0_EN();	
#if (INT_PR == INT_EN)
                INT_GHPEN();
#else
				INT_GEN();
#endif
				break;	
			case(INT1_I):
				INT_INT1_EN();	
#if (INT_PR == INT_EN)
				if (lint -> priority == INT_PHIGH)
                                    INT_GHPEN();
                else{
                    INT_GLPEN();
					INT_GEN();
				}
				
#else
				INT_GEN();
#endif
				break;	
			case(INT2_I):
				INT_INT2_EN();
#if (INT_PR == INT_EN)
				if (lint -> priority == INT_PHIGH)
                                    INT_GHPEN();
                else{
                    INT_GLPEN();
					INT_GEN();
				}
#else
				INT_GEN();
#endif
				break;	
			default:
				ret=E_NOT_OK;	
				break;
			}
		} 
	return ret;
}

/*@brief: Disable an INTx interrupt pin. 
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
STD_ReturnType INT_INTx_disable(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	INTx_index ind = NA;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
		ind = INT_INTx_get_index(lint);
		switch(ind){
			case(INT0_I):
				INT_INT0_DIS();	
				break;	
			case(INT1_I):
				INT_INT1_DIS();	
				break;	
			case(INT2_I):
				INT_INT2_DIS();	
				break;	
			default:
				ret=E_NOT_OK;	
				break;
			}
		} 
	return ret;
}
#endif

/*@brief: Enable RBx interrupt pins. 
 *@param: A pointer to a struct of type INT_RBx_t which describes the RBx interrupt pins.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
#if (INT_PORTB == INT_EN)
STD_ReturnType INT_RBx_enable(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
#if (INT_PR == INT_EN)
	else if(lint -> priority != INT_PHIGH || lint -> priority != INT_PLOW)
		ret = E_NOT_OK;
#endif
	else{
		INT_RB_EN();
#if (INT_PR == INT_EN)
	if (lint -> priority == INT_PHIGH)
            INT_GHPEN();
    else{
			INT_GEN();
            INT_GLPEN();
		}
#else
		INT_GEN();
#endif
		} 
	return ret;
}

/*@brief: Disable RBx interrupt pins. 
 *@param: A pointer to a struct of type INT_RBx_t which describes the RBx interrupt pins.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
STD_ReturnType INT_RBx_disable(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
		INT_RB_DIS();	
	}
	return ret;
}
/*@brief: Initialize the RBx interrupt pins.
 *@param: A pointer to a struct of type INT_RBx_t which describes an RBx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
 *@note: The selected pin for the RB interrupt must be configured as input.
*/
STD_ReturnType INT_RBx_initialize(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint || E_NOT_OK == INT_RBx_check_access(lint))
		ret = E_NOT_OK;
	else{
		/* disables RBx */
		INT_RB_DIS();
		/* clear flag */
		INT_RB_CLRF();
		/* initialize interrupt pin */
		ret = GPIO_pin_direction_initialize(&(lint -> Ipin));
		/* initialize interrupt callback routine */
		ret = ret && INT_RBx_set_callback_routine(lint);
		/* initialize priority */
#if (INT_PR == INT_EN)
		ret = ret && INT_RBx_priority_initialize(lint);
#endif
		/* enable RBx */
		ret = ret && INT_RBx_enable(lint);
	}
	return ret;
}
#endif

/** Helper functions **/
#if (INT_PR == INT_EN)
/*@brief:(H) Initialize the priority bits of a INTx interrupt pin.
 *		 (Priority preconfiguration should be enabled)
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
#if (INT_INTx == INT_EN)
static STD_ReturnType INT_INTx_priority_initialize(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	INTx_index ind = NA;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
            INT_PREN(); /* enable the priority feature */
			ind = INT_INTx_get_index(lint);
		switch(ind){  
            case(INT0_I):/* INT0 is always high priority */ 
                    break;       /* Do change nothing */ 
			case(INT1_I):
				switch(lint -> priority){
					case(INT_PHIGH):
						INT_INT1_HP();
						break;
					case(INT_PLOW):
						INT_INT1_LP();
						break;
					default:
						ret = E_NOT_OK;
						break;
					}
				break;
			case(INT2_I):
				switch(lint -> priority){
					case(INT_PHIGH):
						INT_INT2_HP();
						break;
					case(INT_PLOW):
						INT_INT2_LP();
						break;
					default:
						ret = E_NOT_OK;
						break;
					}
				break;
			default:
				ret = E_NOT_OK;
				break;
		}
	}
	return ret;
}
#endif

/*@brief:(H) Initialize the priority bits of a RBx interrupt pins.
 *		 (Priority preconfiguration should be enabled)
 *@param: A pointer to a struct of type INT_RBx_t which describes an RBx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
#if (INT_PORTB == INT_EN)
static STD_ReturnType INT_RBx_priority_initialize(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
        INT_PREN(); /* enable the priority feature */
		switch(lint -> priority){
				case(INT_PHIGH):
					INT_RB_HP();
					break;
				case(INT_PLOW):
					INT_RB_LP();
					break;
				default:
					ret = E_NOT_OK;
					break;
				}
		}
	return ret;
}
#endif
#endif
/*@brief:(H) Initialize the edge bits of a INTx interrupt pin.
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
#if (INT_INTx == INT_EN)
static STD_ReturnType INT_INTx_edge_initialize(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	INTx_index ind = NA;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
		ind = INT_INTx_get_index(lint);
		switch(ind){
			case(INT0_I): 
				switch(lint -> edge){
					case(falling):
						INT_INT0_FALL();
						break;
					case(rising):
						INT_INT0_RIS();
						break;
					default:
						ret = E_NOT_OK;
						break;
					}
				break;
			case(INT1_I):
				switch(lint -> edge){
					case(falling):
						INT_INT1_FALL();
						break;
					case(rising):
						INT_INT1_RIS();
						break;
					default:
						ret = E_NOT_OK;
						break;
					}
				break;
			case(INT2_I):
				switch(lint -> edge){
					case(falling):
						INT_INT2_FALL();
						break;
					case(rising):
						INT_INT2_RIS();
						break;
					default:
						ret = E_NOT_OK;
						break;
					}
				break;
			default:
				ret = E_NOT_OK;
				break;
		}
	}
	return ret;
}

/*@brief:(H) Clear the flag of an INTx interrupt pin.
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
static STD_ReturnType INT_INTx_clear_flag(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	INTx_index ind = NA;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
		ind = INT_INTx_get_index(lint);
		switch(ind){
			case(INT0_I):
				INT_INT0_CLRF();
				break;
			case(INT1_I):
				INT_INT1_CLRF();
				break;
			case(INT2_I):
				INT_INT2_CLRF();
				break;
			default:
				ret = E_NOT_OK;		
				break;
		}
	}	
	return ret;
}

/*@brief:(H) Checks on the pin_config_t struct for undefined pin configuration.
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
static STD_ReturnType INT_INTx_check_access(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint || (lint -> Ipin.port != PORTB_I) || ( (lint -> Ipin.pin != PIN0) 
		&& (lint -> Ipin.pin != PIN1) && (lint -> Ipin.pin != PIN2)) ){
		ret = E_NOT_OK;	
	}
	return ret;	
}
#endif

/*@brief:(H) Checks on the pin_config_t struct for undefined pin configuration.
 *@param: A pointer to a struct of type INT_RBx_t which describes an RBx change interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
#if (INT_PORTB == INT_EN)
static STD_ReturnType INT_RBx_check_access(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint || (lint -> Ipin.port != PORTB_I) || (lint -> Ipin.pin < PIN3)
	 || (lint -> Ipin.direction == GPIO_OUT)){ /* For this interrupt to work, the pin must be input */
		ret = E_NOT_OK;
	}
	return ret;
}
#endif

/*@brief:(H) Checks on the callback function within the INT_INTx_t struct. If there is a function
 * 		  given, it is aliased with one of the three predefined functions in this source file's header.
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
#if (INT_INTx == INT_EN)
static STD_ReturnType INT_INTx_set_callback_routine(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	INTx_index ind = NA;
	if(NULL == lint) 
		ret = E_NOT_OK;
	else if (NULL == lint -> ext_interrupt_handler){/* Do nothing and skip the else, return E_OK */
		}
	else{
		ind = INT_INTx_get_index(lint);
		switch(ind){
			case(INT0_I):
				INT0_handler = lint -> ext_interrupt_handler;		
				break;
			case(INT1_I):
				INT1_handler = lint -> ext_interrupt_handler;		
				break;
			case(INT2_I):
				INT2_handler = lint -> ext_interrupt_handler;		
				break;
			default:
				ret = E_NOT_OK;
				break;
			}
	}
	return ret;
}
#endif

/*@brief:(H) Checks on the callback function within the INT_RBx_t struct. If there is a function
 * 		  given, it is aliased with one of the three predefined functions in this source file's header.
 *@param: A pointer to a struct of type INT_RBx_t which describes an RBx change interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
#if (INT_PORTB == INT_EN)
static STD_ReturnType INT_RBx_set_callback_routine(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{ /* Initialize both callbacks */
		switch(lint -> Ipin.pin){
			case(PIN4):
				RB4_handler_high = (lint -> ext_interrupt_handler_high);
				RB4_handler_low = (lint -> ext_interrupt_handler_low);
				break;
			case(PIN5):
				RB5_handler_high = (lint -> ext_interrupt_handler_high);
				RB5_handler_low = (lint -> ext_interrupt_handler_low);
				break;
			case(PIN6):
				RB6_handler_high = (lint -> ext_interrupt_handler_high);
				RB6_handler_low = (lint -> ext_interrupt_handler_low);
				break;
			case(PIN7):
				RB7_handler_high = (lint -> ext_interrupt_handler_high);
				RB7_handler_low = (lint -> ext_interrupt_handler_low);
				break;
			default:
				ret = E_NOT_OK;
				break;
		}
	}
	return ret;
}
#endif

/*@brief:(H) Find the index of an INTx interrupt from its pin configuration. 
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: Returns the index of the INTx interrupt upon success and NA (-1) otherwise. 
*/
#if (INT_INTx == INT_EN)
static INTx_index INT_INTx_get_index(const INT_INTx_t *lint){
	INTx_index ind = NA;	
	if(NULL == lint){
		ind = NA;
	}	
	else{
		switch(lint -> Ipin.pin){
			case(PIN0):
				ind = INT0_I;	
				break;
			case(PIN1):
				ind = INT1_I;	
				break;
			case(PIN2):
				ind = INT2_I;	
				break;
			default:
				ind = NA;
				break;
		}
	}
	return ind;
}
#endif

#if (INT_TMR0 == INT_EN)
/* @Brief: This routine initializes the interrupt feature for the Timer0 module.
 * @Param: A uint8 representing the priority of the Timer0 module.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 * @Notes: 
 */
STD_ReturnType INT_TMR0_init(uint8 priority){
	STD_ReturnType ret = E_OK;
	/* Disable the interrupt */
	INT_TMR0_DIS();	
	/* Clear the flag */
	INT_TMR0_CLRF();
	/* Enable global interrupt */	
	INT_GEN();
	/* Set up priority */
#if (INT_PR == INT_EN)
    INT_PREN(); /* enable the priority feature */
	if(priority == INT_PHIGH){
		INT_TMR0_HP();	
		INT_GHPEN();
	}
	else if(priority == INT_PLOW){
		INT_TMR0_LP();	
		INT_GLPEN();
	}
	else{
		ret = E_NOT_OK;
	}
#else 
	INT_PRDIS(); /* disable the priority feature */
#endif
	INT_TMR0_EN(); /* Enable the TMR0 peripheral */
	return ret;
}

/* @Brief: Deinitialize the Timer0 interrupt feature.
 * @Param: None
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType INT_TMR0_deinit(void){
	STD_ReturnType ret = E_OK;
	/* Disable the TMR0 interrupt */
	INT_TMR0_DIS();
	/* Clear the TMR0 interrupt flag */
	INT_TMR0_CLRF();
	return ret;
}

/* @Brief: Set a callback function.
 * @Param: A pointer to a function with no input and no return.
 * @Return: E_OK upcon success and E_NOT_OK otherwise
 * @Notes:
 */
STD_ReturnType INT_TMR0_set_callback_routine(void (*callback) (void)){
	STD_ReturnType ret = E_OK;
		TMR0_callback = callback;	
	return ret;
}
#endif
#endif
