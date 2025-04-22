/* 
 * File:   mcal_external_interrupt.c
 * Author: nour
 *
 * Created on April 15, 2025, 2:27 PM
 */
#include "mcal_external_interrupt.h"

/* Locally defined interrupt handling functions */
void (*INT0_handler) (void) = NULL;
void (*INT1_handler) (void) = NULL;
void (*INT2_handler) (void) = NULL;

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
/*@brief: Initialize a INTx interrupt pin.
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
STD_ReturnType INT_INTx_initialize(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint || E_NOT_OK ==INT_INTx_check_access(lint))
		ret = E_NOT_OK;
	else{
		/* disables INTx */
		ret = INT_INTx_disable(lint)&&
		/* clear flag */
		INT_INTx_clear_flag(lint)&&
		/* initialize interrupt pin */
		INT_INTx_pin_initialize(lint)&&
		/* initialize interrupt callback routine */
		INT_INTx_set_callback_routine(lint)&& 
		/* initialize priority */
#if (INT_PR == INT_EN)
		INT_INTx_priority_initialize(lint)&&
#endif
		/* initialize edge */
		INT_INTx_edge_initialize(lint)&&
		/* enable INTx */
		INT_INTx_enable(lint);
	}
	return ret;
}

/*@brief: Enable an INTx interrupt pin. 
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
STD_ReturnType INT_INTx_enable(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
		switch(lint -> index){
			case(INT0_I):
				INT_INT0_EN();	
#if INT_PR == INT_EN
				INT_GHPEN();
				INT_GLPEN();
#else
				INT_GEN();
#endif
				break;	
			case(INT1_I):
				INT_INT1_EN();	
#if INT_PR == INT_EN
				INT_GHPEN();
				INT_GLPEN();
#else
				INT_GEN();
#endif
				break;	
			case(INT2_I):
				INT_INT2_EN();
#if INT_PR == INT_EN
				INT_GHPEN();
				INT_GLPEN();
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
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
		switch(lint -> index){
			case(INT0_I):
				INT_INT0_DIS();	
				break;	
			case(INT1_I):
				INT_INT0_DIS();	
				break;	
			case(INT2_I):
				INT_INT0_DIS();	
				break;	
			default:
				ret=E_NOT_OK;	
				break;
			}
		} 
	return ret;
}
/*@brief
 *@param
 *@return
*/
/*
STD_ReturnType INT_RBx_enable(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
		switch(lint -> Ipin -> pin){
			case(INT0_I):
				INT_INT0_DIS();	
				break;	
			case(INT1_I):
				INT_INT0_DIS();	
				break;	
			case(INT2_I):
				INT_INT0_DIS();	
				break;	
			default:
				ret=E_NOT_OK;	
				break;
			}
		} 
	return ret;
}
*/
/*@brief
 *@param
 *@return
*/
STD_ReturnType INT_RBx_disable(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{

	
	}
	return ret;
}
/*@brief
 *@param
 *@return
*/
STD_ReturnType INT_RBx_initialize(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{

	
	}
	return ret;
}
/** Helper functions **/
#if INT_PR == INT_EN
/*@brief:(H) Initialize the priority bits of a INTx interrupt pin.
 *		 (Priority preconfiguration should be enabled)
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
static STD_ReturnType INT_INTx_priority_initialize(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
            INT_PREN();
		switch(lint -> index){  
                        case(INT0_I):/* INT0 does not support priority */ 
                           /* Do change nothing */ 
			case(INT1_I):
				switch(lint -> priority){
					case(INT_EN):
						INT_INT1_HP();
						break;
					case(INT_DIS):
						INT_INT1_LP();
						break;
					default:
						ret = E_NOT_OK;
						break;
					}
				break;
			case(INT2_I):
				switch(lint -> priority){
					case(INT_EN):
						INT_INT2_HP();
						break;
					case(INT_DIS):
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

/*@brief
 *@param
 *@return
*/
/*
static STD_ReturnType INT_RBx_priority_initialize(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
		switch(lint -> index){
			switch(lint -> priority){
				case(INT_EN):
					INT_RBx_HP();
					break;
				case(INT_DIS):
					INT_RBx_LP();
					break;
				default:
					ret = E_NOT_OK;
					break;
				}
			}
		}
	return ret;
}
*/
#endif

/*@brief:(H) Initialize the edge bits of a INTx interrupt pin.
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
static STD_ReturnType INT_INTx_edge_initialize(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
		switch(lint -> index){
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
/*@brief:(H) Initialize the GPIO pin of the interrupt given the
 * 		  pin_config_t struct embedded in the interrupt struct.
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
static STD_ReturnType INT_INTx_pin_initialize(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint || (E_NOT_OK == INT_INTx_check_access(lint)) )
		ret = E_NOT_OK;
	else{
		ret = GPIO_pin_direction_initialize(&(lint -> Ipin));
	}	
	return ret;
}
/*@brief:(H) Clear the flag of an INTx interrupt pin.
 *@param: A pointer to a struct of type INT_INTx_t which describes an INTx interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
static STD_ReturnType INT_INTx_clear_flag(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint)
		ret = E_NOT_OK;
	else{
		switch(lint -> index){
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
/*@brief:(H) Checks on the pin_config_t struct for undefined pin configuration.
 *@param: A pointer to a struct of type INT_RBx_t which describes an RBx change interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
static STD_ReturnType INT_RBx_check_access(const INT_RBx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint || (lint -> Ipin.port != PORTB_I) || (lint -> Ipin.pin < PIN3)){
		ret = E_NOT_OK;
	}
	return ret;
}
/*@brief:(H) Checks on the callback function within the INT_INTx_t struct. If there is a function
 * 		  given, it is aliased with one of the three predefined functions in this source file's header.
 *@param: A pointer to a struct of type INT_RBx_t which describes an RBx change interrupt pin.
 *@return: E_OK upon success and E_NOT_OK otherwise.
*/
static STD_ReturnType INT_INTx_set_callback_routine(const INT_INTx_t *lint){
	STD_ReturnType ret = E_OK;
	if(NULL == lint || NULL == lint -> ext_interrupt_handler) /* no callback function given */
		ret = E_NOT_OK;
	else{
		switch(lint -> index){
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
