/* 
 * File:   mcal_external_interrupt.h
 * Author: nour
 *
 * Created on April 15, 2025, 2:26 PM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H

/************ includes *****************/
#include "mcal_interrupt_config.h"

/************ Macros *******************/
/********* function-like macros ********/
#if INT_INTx == INT_EN
/* Enable INT0 */
#define INT_INT0_EN() INTCONbits.INT0IE=1
/* Disable INT0 */
#define INT_INT0_DIS() INTCONbits.INT0IE=0
/* Clear INT0 flag bit */
#define INT_INT0_CLRF() INTCONbits.INT0IF=0
/* Set INT0 to be rising edge triggered */
#define INT_INT0_RIS() INTCON2bits.INTEDG0=1
/* Set INT0 to be falling edge triggered */
#define INT_INT0_FALL() INTCON2bits.INTEDG0=0

/* Enable INT1 */
#define INT_INT1_EN() INTCONbits.INT0IE=1
/* Disables INT1 */
#define INT_INT1_DIS() INTCON3bits.INT1IE=0
/* Clear INT1 flag bit */
#define INT_INT1_CLRF() INTCON3bits.INT1IF=0
/* Set INT1 to be rising edge triggered */
#define INT_INT1_RIS() INTCON2bits.INTEDG1=1
/* Set INT1 to be falling edge triggered */
#define INT_INT1_FALL() INTCON2bits.INTEDG1=0

/* Enable INT2 */
#define INT_INT2_EN() INTCON3bits.INT2IE=1
/* Disable INT2 */
#define INT_INT2_DIS() INTCON3bits.INT2IE=0
/* Clear INT2 flag bit */
#define INT_INT2_CLRF() INTCON3bits.INT2IF=0
/* Set INT2 to be rising edge triggered */
#define INT_INT2_RIS() INTCON2bits.INTEDG2=1
/* Set INT2 to be falling edge triggered */
#define INT_INT2_FALL() INTCON2bits.INTEDG2=0
/* Set INTx priority level (INT0 does not have a priority bit.) */
#if INT_PR == INT_EN
#define INT_INT1_HP() INTCON3bits.INT1IP=1 
#define INT_INT1_LP() INTCON3bits.INT1IP=0
#define INT_INT2_HP() INTCON3bits.INT2IP=1
#define INT_INT2_LP() INTCON3bits.INT2IP=0
#endif
#endif
#if INT_PORTB == INT_EN
/* Enables the PORTB (RB(4-7)) change interrupt */
#define INT_RB_EN() INTCONbits.RBIE=1
/* Disables the PORTB (RB(4-7)) change interrupt */
#define INT_RB_DIS() INTCONbits.RBIE=0
/* Clears the PORTB (RB(4-7)) change interrupt flag */
#define INT_RB_CLRF() INTCONbits.RBIF=0
#if INT_PR == INT_EN
/* Sets the PORTB (RB(4-7)) change interrupt priority level */
#define INT_RB_HP() INTCON2bits.RBIP=1
#define INT_RB_LP() INTCON2bits.RBIP=0
#endif
#endif
/******** Data types *******************/

typedef enum{
	falling,
	rising
}INTx_edge;
typedef enum{
	INT0_I,
	INT1_I,
	INT2_I
}INTx_index;

typedef struct{
	void (*ext_interrupt_handler) (void); /* use NULL if you do not want a callback */
	pin_config_t Ipin;
	INTx_index index;
	INTx_edge edge;
#if INT_PR == INT_EN
	uint8 priority; /* macro defined */	
#endif
}INT_INTx_t;

typedef struct{/* Index is given in Ipin */
	void (*ext_interrupt_handler) (void); /* use NULL if you do not want a callback */
	pin_config_t Ipin;
	uint8 priority; 
}INT_RBx_t;

/******** function declarations ********/
void INT0_ISR();
void INT1_ISR();
void INT2_ISR();
STD_ReturnType INT_INTx_initialize(const INT_INTx_t *lint);
STD_ReturnType INT_INTx_enable(const INT_INTx_t *lint);
STD_ReturnType INT_INTx_disable(const INT_INTx_t *lint);
static STD_ReturnType INT_INTx_priority_initialize(const INT_INTx_t *lint);
static STD_ReturnType INT_INTx_edge_initialize(const INT_INTx_t *lint);
static STD_ReturnType INT_INTx_pin_initialize(const INT_INTx_t *lint);
static STD_ReturnType INT_INTx_clear_flag(const INT_INTx_t *lint);
static STD_ReturnType INT_INTx_set_callback_routine(const INT_INTx_t *lint);
/* RB change interrupt pins are RB(4-7) */
STD_ReturnType INT_RBx_enable(const INT_RBx_t *lint);
STD_ReturnType INT_RBx_disable(const INT_RBx_t *lint);
STD_ReturnType INT_RBx_initialize(const INT_RBx_t *lint);
static STD_ReturnType INT_RBx_priority_init(const INT_RBx_t *lint);

static STD_ReturnType INT_INTx_check_access(const INT_INTx_t *lint);
static STD_ReturnType INT_RBx_check_access(const INT_RBx_t *lint);
#endif	/* MCAL_EXTERNAL_INTERRUPT_H */

