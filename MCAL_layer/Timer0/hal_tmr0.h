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
#include "../hal_tmr_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_external_interrupt.h"
#include "../Interrupt/mcal_interrupt_config.h" //includes other crucial headers 

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

/******* data types **************/
typedef struct{
#if (INT_TMR0== INT_EN)
	void (*TMR0_interrupt_handler) (void); // use NULL if unwanted
#if (INT_PR == INT_EN)
	uint8 priority; /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	TMR_PRESC_SEL prescaler; 
	uint16 preloaded_value; // Initial preloaded value   
	uint8 mode :1; /* @ref: hal_tmr_cfg.h -> _8BIT & _16BIT */ 
	uint8 edge : 1; /* @ref: hal_tmr_cfg.h -> RISING & FALLING */  
	uint8 clk_source : 1; /* @ref: hal_tmr_cfg.h -> INT & EXT */   
	uint8 resereved : 5;
}TMR0_t;
/******* prototypes *************/
STD_ReturnType TMR0_initialize(const TMR0_t * tmr0);
STD_ReturnType TMR0_deinitialize(const TMR0_t * tmr0);
STD_ReturnType TMR0_start(const TMR0_t * tmr0);
STD_ReturnType TMR0_stop(const TMR0_t * tmr0);
STD_ReturnType TMR0_read(const TMR0_t * tmr0, uint16 * value); 
STD_ReturnType TMR0_write(const TMR0_t * tmr0, uint16 * value); 
STD_ReturnType TMR0_delayms(const TMR0_t * tmr0, uint16 delay, uint8* accuracy);
static STD_ReturnType TMR0_scale2ratio(TMR_PRESC_SEL prescale, uint16 * val);

#endif	/* HAL_TMR0_H */

