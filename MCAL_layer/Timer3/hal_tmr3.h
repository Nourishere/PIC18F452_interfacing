/* 
 * File:   hal_tmr3.h
 * Author: nour
 *
 * Created on July 11, 2025, 11:10 PM
 */

#ifndef HAL_TMR3_H
#define	HAL_TMR3_H
/*********** includes ***************/
#include "../hal_tmr_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../Interrupt/mcal_interrupt_config.h" //includes other crucial headers 

/*********** function macros *******/
/* Turn on/off the timer */
#define TMR3_ON() (T3CONbits.TMR3ON=1)
#define TMR3_OFF() (T3CONbits.TMR3ON=0)
/* 16 or 8 bit rdwr modes */
#define TMR3_REG_16() (T3CONbits.RD16=1)
#define TMR3_REG_8() (T3CONbits.RD16=0)
/* External clock synchronization
* @note: This works when the EXT mode is set
*/
#define TMR3_ESYNC_OFF() (T3CONbits.T3SYNC=1)
#define TMR3_ESYNC_ON() (T3CONbits.T3SYNC=0)
/* Clock source
 * @note: In the EXT mode, the counting starts
 * 		  on the second rising edge.
 */
#define TMR3_EXT() (T3CONbits.TMR3CS=1)
#define TMR3_INT() (T3CONbits.TMR3CS=0)
/* Write precaler value */
#define TMR3_PRESC_WR(x) (x & 0x03)
/*********** data types ************/
typedef struct{
#if (INT_TMR3== INT_EN)
	void (*TMR3_interrupt_handler) (void); // use NULL if unwanted
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
}TMR3_t;

/*********** func dec **************/
STD_ReturnType TMR3_initialize(const TMR3_t * tmr3);
STD_ReturnType TMR3_deinitialize(const TMR3_t * tmr3);
STD_ReturnType TMR3_start(const TMR3_t * tmr3);
STD_ReturnType TMR3_stop(const TMR3_t * tmr3);
STD_ReturnType TMR3_read(const TMR3_t * tmr3, uint16 * value); 
STD_ReturnType TMR3_write(const TMR3_t * tmr3, uint16 * value); 


#endif	/* HAL_TMR3_H */

