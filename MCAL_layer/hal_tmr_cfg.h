/* 
 * File:   hal_tmr_cfg.h
 * Author: nour
 *
 * Created on July 3, 2025, 2:09 PM
 * @Brief: This header contains common configurations used
 * 		   in all timer modules.
 */

#ifndef TMR_CFG_H
#define	TMR_CFG_H

/************** Macros ***************/
#define RISING 1
#define FALLING 0
#define _8BIT 0
#define _16BIT 1
#define EXT 1 // External clock source
#define INT 0 // Internal clock source
#define SYNC_ON 1
#define SYNC_OFF 0

/********* function-like macros ******/
/* Configure prescaler */
#define TMR_PRESC_WR(x) T0CONbits.T0PS=(x & 0x07)

/********* Data types ***********/
typedef enum{
	TMR_PRESC_1_1 = -1, //No prescaler used
	TMR_PRESC_1_2,
	TMR_PRESC_1_4,
	TMR_PRESC_1_8,
	TMR_PRESC_1_16,
	TMR_PRESC_1_32,
	TMR_PRESC_1_64,
	TMR_PRESC_1_128,
	TMR_PRESC_1_256
}TMR_PRESC_SEL;

#endif	/* TMR_CFG_H */

