/* 
 * File:   hal_ccp.h
 * Author: nour
 *
 * Created on July 23, 2025, 4:18 PM
 * @Brief: This header contains configuration for the CCPx modules.
 */

#ifndef HAL_CCP_H
#define	HAL_CCP_H

/****************** include **********************/
#include <xc.h>
#include "mcal_ccp_cfg.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../Interrupt/mcal_interrupt_config.h"
#include "../GPIO/hal_gpio.h"
#include "../Timer1/hal_tmr1.h"
#include "../Timer2/hal_tmr2.h"
#include "../Timer3/hal_tmr3.h"

/**************** function macros **************/
/* Disable the CCP1 module */
#define CCP1_DIS() (CCP1CONbits.CCP1M=0x00)
/** CCP1 compare mode **/
/* Toggle output on match */
#define CCP1_COMPARE_TOGG() (CCP1CONbits.CCP1M=0x02)
/* Set CCP1 pin to low */
#define CCP1_COMPARE_FORCE_HIGH() (CCP1CONbits.CCP1M=0x08)
/* Set CCP1 pin to high */
#define CCP1_COMPARE_FORCE_LOW() (CCP1CONbits.CCP1M=0x09)
/* CCP1 pin unaffected */
#define CCP1_COMPARE_NOP() (CCP1CONbits.CCP1M=0x0A)
/* Trigger special event */
#define CCP1_COMPARE_SPECIAL() (CCP1CONbits.CCP1M=0x0B)
/** CCP1 Capture mode **/
/* Set Capture on every rising edge */
#define CCP1_CAPTURE_RIS() (CCP1CONbits.CCP1M=0x05)
/* Set Capture on every fourth rising edge */
#define CCP1_CAPTURE_4RIS() (CCP1CONbits.CCP1M=0x06)
/* Set Capture on every sixteenth rising edge */
#define CCP1_CAPTURE_16RIS() (CCP1CONbits.CCP1M=0x07)
/* Set Capture on every falling edge */
#define CCP1_CAPTURE_FALL() (CCP1CONbits.CCP1M=0x04)
/** CCP1 PWM mode **/
#define CCP1_PWM() (CCP1CONbits.CCP1M=0x0C)
/* Write the CCP1 PWM duty cycle register */
/* @param: x is a uint16 type that holds 10 bits of data
 */
#define CCP1_PWM_DS_WR(x) \
	do { \
		(CCP1CONbits.DC1B = (uint8) (x & 0x03)); \
		(CCPR1L = (uint8) (x & 0xFF)); \
	} while(0) \

/* Disable the CCP2 module */
#define CCP2_DIS() (CCP2CONbits.CCP2M=0000)
/** CCP2 compare mode **/
/* Toggle output on match */
#define CCP2_COMPARE_TOGG() (CCP2CONbits.CCP2M=0010)
/* Set CCP2 pin to low */
#define CCP2_COMPARE_FORCE_HIGH() (CCP2CONbits.CCP2M=1000)
/* Set CCP2 pin to high */
#define CCP2_COMPARE_FORCE_LOW() (CCP2CONbits.CCP2M=1001)
/* CCP2 pin unaffected */
#define CCP2_COMPARE_NOP() (CCP2CONbits.CCP2M=1010)
/* Trigger special event */
#define CCP2_COMPARE_SPECIAL() (CCP2CONbits.CCP2M=1011)
/** CCP2 Capture mode **/
/* Set Capture on every rising edge */
#define CCP2_CAPTURE_RIS() (CCP2CONbits.CCP2M=0101)
/* Set Capture on every fourth rising edge */
#define CCP2_CAPTURE_4RIS() (CCP2CONbits.CCP2M=0110)
/* Set Capture on every sixteenth rising edge */
#define CCP2_CAPTURE_16RIS() (CCP2CONbits.CCP2M=0111)
/* Set Capture on every falling edge */
#define CCP2_CAPTURE_FALL() (CCP2CONbits.CCP2M=0100)
/** CCP2 PWM mode **/
#define CCP2_PWM() (CCP2CONbits.CCP2M=1100)
/* Write the CCP2 PWM duty cycle register */
/* @param: x is a uint16 type that holds 10 bits of data
 */
#define CCP2_PWM_DS_WR(x) \
	do { \
		(CCP2CONbits.DC2B = (uint8) (x & 0x03)); \
		(CCPR2L = (uint8) (x & 0xFF)); \
	} while(0) \

/* Choose CCPx clock source */
 /* Select TMR3 as source for CCPx */
#define CCP_TMR3() do { \
    T3CONbits.T3CCP1 = 0; \
    T3CONbits.T3CCP2 = 1; \
} while (0)
/* Select TMR1 as source for CCPx */
#define CCP_TMR1() do { \
	(T3CONbits.T3CCP1=0); \
    (T3CONbits.T3CCP2=0); \
} while (0) 
 /* Select TMR3 as source for CCP2
  * and TMR1 as source for CCP1
  */
#define CCP2_TMR3() do { \
	(T3CONbits.T3CCP1=1); \
    (T3CONbits.T3CCP2=0); \
} while (0)
#define CCP1_TMR1 CCP2_TMR3 
                    
 /* Select TMR1 as source for CCPx */
/*************** data types *******************/
typedef union{
	TMR1_t tmr1;
	TMR1_t tmr2;
	TMR3_t tmr3;
}CCP1_tmrs;
typedef enum{
	CCP_Toggle,
	CCP_High,
	CCP_Low,
	CCP_Unchanged,
	CCP_Special_event
}CCP_comp_event;
typedef enum{
	CCP_TMR1,
	CCP_TMR2,
	CCP_TMR3
}CCP_TMR;
typedef enum{
	CCP_Capture,
	CCP_Compare,
	CCP_PWM
}CCP_mode;
typedef enum{
	CCP_falling,
	CCP_rising,
	CCP_rising_4th,
	CCP_rising_16th
}CCP_capture_event;
typedef struct{
	CCP_capture_event event_trg;
	CCP_TMR	timer;
	CCP1_tmrs u_timer;
}CCP_capture_t;
typedef struct{
	CCP_comp_event event_trg;
	CCP_TMR timer;
	CCP1_tmrs u_timer;
}CCP_compare_t;
typedef struct{
	uint32 PWM_freq;
	/*@note: The duty cycle should be a percent (e.g. 50) */
	uint8 PWM_duty_cycle;
	/* Timer2 is used by default */
	TMR2_t timer2;
}CCP_PWM_t;
typedef union{
	CCP_capture_t cap;
	CCP_compare_t comp;
	CCP_PWM_t pwm;
}CCP_op;

/* Only this following type should be used for initialization */
typedef struct{
#if (INT_CCP1== INT_EN)
	void (*CCP1_interrupt_handler) (void); // use NULL if unwanted
#if (INT_PR == INT_EN)
	uint8 priority; /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	CCP_mode mode; 
	CCP_op op_mode; 
}CCP1_t;

typedef struct{
#if (INT_CCP2== INT_EN)
	void (*CCP2_interrupt_handler) (void); // use NULL if unwanted
#if (INT_PR == INT_EN)
	uint8 priority; /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	CCP_mode mode; 
	CCP_op op_mode; 
}CCP2_t;
/************** function prototypes **********/
/*@note: The CCPx initialization routines initialize
 * 		 the specified Timer module.
 */
STD_ReturnType CCP1_initialize(const CCP1_t * ccp1);
STD_ReturnType CCP2_initialize(const CCP2_t * ccp2);
STD_ReturnType CCP1_deinitialize(const CCP1_t * ccp1);
STD_ReturnType CCP2_deinitialize(const CCP2_t * ccp2);

STD_ReturnType CCP1_PWM_set_duty_cycle(const CCP1_t * ccp1, uint8 duty_cycle);
STD_ReturnType CCP2_PWM_set_duty_cycle(const CCP2_t * ccp2, uint8 duty_cycle);
STD_ReturnType CCP1_PWM_set_frequency(const CCP1_t * ccp1, uint32 freq);
STD_ReturnType CCP2_PWM_set_frequency(const CCP2_t * ccp2, uint32 freq);

STD_ReturnType CCP1_start(const CCP1_t * ccp1);
STD_ReturnType CCP2_start(const CCP2_t * ccp2);
STD_ReturnType CCP1_stop(const CCP1_t * ccp1);
STD_ReturnType CCP2_stop(const CCP2_t * ccp2);

#endif	/* HAL_CCP_H */

