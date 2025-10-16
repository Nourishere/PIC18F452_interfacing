/* 
 * File:   mcal_tmr1.h
 * Author: nour
 *
 * Created on July 3, 2025, 2:08 PM
 * @Brief: This header file contains configurations for 
 * 		   the Timer 1 module.
 */

#ifndef HAL_TMR1_H
#define	HAL_TMR1_H

/************* includes ***************/
#include <xc.h>
#include "../mcal_tmr_cfg.h"
#include "../GPIO/mcal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../Interrupt/mcal_interrupt_config.h" //includes other crucial headers 

/******* function-like macros *********/
/* Turn on/off timer1 */
#define TMR1_ON() (T1CONbits.TMR1ON=1)
#define TMR1_OFF() (T1CONbits.TMR1ON=0)
/* Oscillator on/off */
#define TMR1_OSC_ON() (T1CONbits.T1OSCEN=1)
#define TMR1_OSC_OFF() (T1CONbits.T1OSCEN=0)
/* Write and read mode.
 * @note: These macros determine whether the read/write 
 * 		  operations on the timer1 register are done as 
 * 		  one 16 bit instruction or two 8 bit instructions
 * 		  (timer0 style).
 */
#define TMR1_REG_8() (T1CONbits.RD16=0)
#define TMR1_REG_16() (T1CONbits.RD16=1)
/* Clock select */
#define TMR1_EXT() (T1CONbits.TMR1CS=1)
#define TMR1_INT() (T1CONbits.TMR1CS=0) // Fosc / 4
/* External clock sync 
 * @note: The timer1 clock can be synchronized with
 * 		  the external clock on rising edge. 
 */
#define TMR1_ESYNC_ON() (T1CONbits.T1SYNC=0)
#define TMR1_ESYNC_OFF() (T1CONbits.T1SYNC=1)

/******* data types **************/
typedef struct{
#if (INT_TMR1== INT_EN)
	void (*TMR1_interrupt_handler) (void); // use NULL if unwanted
#if (INT_PR == INT_EN)
	uint8 priority; /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	TMR_PRESC_SEL prescaler; 
	uint16 preloaded_value; // Initial preloaded value   
	uint8 rdwr_mode : 1; /* @ref: hal_tmr_cfg.h -> _8BIT & _16BIT */ 
	uint8 clk_source : 1; /* @ref: hal_tmr.cfg.h -> INT & EXT */   
	uint8 clk_sync : 1; /* @ref: hal_tmr.cfg.h -> SYNC_ON & SYNC_OFF */
	uint8 resereved : 5;
}TMR1_t;
/******* function prototypes *****/
STD_ReturnType TMR1_initialize(const TMR1_t * tmr1);
STD_ReturnType TMR1_deinitialize(const TMR1_t * tmr1);
STD_ReturnType TMR1_start(const TMR1_t * tmr1);
STD_ReturnType TMR1_stop(const TMR1_t * tmr1);
STD_ReturnType TMR1_read(const TMR1_t * tmr1, uint16 * value); 
STD_ReturnType TMR1_write(const TMR1_t * tmr1, uint16 * value); 


#endif	/* HAL_TMR1_H */

