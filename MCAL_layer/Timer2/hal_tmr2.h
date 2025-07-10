/* 
 * File:   hal_tmr2.h
 * Author: nour
 *
 * Created on July 9, 2025, 6:45 PM
 * @Brief: This header contains all Timer2-related configurations.
 */

#ifndef HAL_TMR2_H
#define	HAL_TMR2_H

/******* includes ****************/
#include "../hal_tmr_cfg.h"
#include "../Interrupt/mcal_internal_interrupt.h"

/******* macros ******************/
/* Enable/Disable the module */
#define TMR2_ON() (T2CONbits.TMR2ON=1)
#define TMR2_OFF() (T2CONbits.TMR2ON=0)

/******* Function macros *********/
#define TMR2_POSTSC_WR(x) ( ((uint8) x) & 0x0F)

/******* Data types **************/
typedef enum{
	TMR2_POSTSC_1_1,	
	TMR2_POSTSC_1_2,	
	TMR2_POSTSC_1_3,	
	TMR2_POSTSC_1_4,	
	TMR2_POSTSC_1_5,	
	TMR2_POSTSC_1_6,	
	TMR2_POSTSC_1_7,	
	TMR2_POSTSC_1_8,	
	TMR2_POSTSC_1_9,	
	TMR2_POSTSC_1_10,	
	TMR2_POSTSC_1_11,	
	TMR2_POSTSC_1_12,	
	TMR2_POSTSC_1_13,	
	TMR2_POSTSC_1_14,	
	TMR2_POSTSC_1_15,	
	TMR2_POSTSC_1_16,	
}TMR2_POSTSC_SEL;

typedef struct{
#if (INT_TMR2== INT_EN)
	void (*TMR2_interrupt_handler) (void); // use NULL if unwanted
#if (INT_PR == INT_EN)
	uint8 priority; /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	uint8 preloaded_value;
	TMR_PRESC_SEL prescaler; /* @ref: hal_tmr_cfg.h */
	TMR2_POSTSC_SEL postscaler;
}TMR2_t;

/******* Function declarations ***/
STD_ReturnType TMR2_initialize(const TMR2_t * tmr2);
STD_ReturnType TMR2_deinitialize(const TMR2_t * tmr2);
STD_ReturnType TMR2_start(const TMR2_t * tmr2);
STD_ReturnType TMR2_stop(const TMR2_t * tmr2);
STD_ReturnType TMR2_read(const TMR2_t * tmr2, uint8 * rd_value);
STD_ReturnType TMR2_write(const TMR2_t * tmr2, uint8 * wr_value);
STD_ReturnType TMR2_write_PR_reg(const TMR2_t * tmr2, uint8 * wr_value);

#endif	/* HAL_TMR2_H */
