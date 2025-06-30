/* 
 * File:   hal_tmr0.h
 * Author: nour
 *
 * Created on June 27, 2025, 10:19 AM
 * @Brief: This header file contains configurations for 
 * 		   the Timer 0 module.
 */

#ifndef HAL_TMR0_H
#define	HAL_TMR0_H

/******* includes ****************/
#include "../Interrupt/mcal_external_interrupt.h"
#include "../Interrupt/mcal_interrupt_config.h" //includes other crucial headers 

/******* macros ******************/
#define RISING 1
#define FALLING 0
#define _8BIT 0
#define _16BIT 1
#define EXT 1 // External clock source
#define INT 0 // Internal clock source
#define RANGE -2

/******* function like macros ****/
/* Turn on/off the module */
#define TMR0_ON() T0CONbits.TMR0ON=1
#define TMR0_OFF() T0CONbits.TMR0ON=0
/* Select 8-bit or 16-bit mode */
#define TMR0_8BIT() T0CONbits.T08BIT=1
#define TMR0_16BIT() T0CONbits.T08BIT=0
/* Select internal or external clock source */
/* @note: Configuring the timer to use the internal oscillator clock
 * means that it behaves as a timer. If it is to be used as a counter, 
 * external source should be connected to RA4 pin.
 */
#define TMR0_EXT() T0CONbits.T0CS=1
#define TMR0_INT() T0CONbits.T0CS=0
/* Edge control */
#define TMR0_RIS() T0CONbits.T0SE=0
#define TMR0_FALL() T0CONbits.T0SE=1
/* Turn on/off prescaler */
#define TMR0_PRESC_ON() T0CONbits.PSA=0
#define TMR0_PRESC_OFF() T0CONbits.PSA=1
/* Configure prescaler */
#define TMR0_PRESC_WR(x) T0CONbits.T0PS=(x & 0x07)

/******* data types **************/
typedef enum{
	TMR0_PRESC_NONE = -1, //No prescaler used
	TMR0_PRESC_1_2,
	TMR0_PRESC_1_4,
	TMR0_PRESC_1_8,
	TMR0_PRESC_1_16,
	TMR0_PRESC_1_32,
	TMR0_PRESC_1_64,
	TMR0_PRESC_1_128,
	TMR0_PRESC_1_256
}TMR0_PRESC_SEL;
typedef struct{
#if (INT_TMR0== INT_EN)
	void (*TMR0_interrupt_handler) (void); // use NULL if unwanted
#if (INT_PR == INT_EN)
	uint8 priority; /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	TMR0_PRESC_SEL prescaler; 
	uint16 preloaded_value; // Initial preloaded value   
	uint8 mode : 1; /* @ref: hal_tmr0.h -> _8BIT & _16BIT */ 
	uint8 edge : 1; /* @ref: hal_tmr0.h -> RISING & FALLING */  
	uint8 clk_source : 1; /* @ref: hal_tmr0.h -> INT & EXT */   
	uint8 resereved : 5;
}TMR0_t;
/*******  prototypes *************/
STD_ReturnType TMR0_initialize(const TMR0_t * tmr0);
STD_ReturnType TMR0_deinitialize(const TMR0_t * tmr0);
STD_ReturnType TMR0_start(const TMR0_t * tmr0);
STD_ReturnType TMR0_stop(const TMR0_t * tmr0);
STD_ReturnType TMR0_read(const TMR0_t * tmr0, uint16 * value); 
STD_ReturnType TMR0_write(const TMR0_t * tmr0, uint16 * value); 
STD_ReturnType TMR0_delayms(const TMR0_t * tmr0, uint16 delay, uint8* accuracy);
static STD_ReturnType TMR0_scale2ratio(TMR0_PRESC_SEL prescale, uint16 * val);

#endif	/* HAL_TMR0_H */

