/* 
 * File:   mcal_adc.h
 * Brief: This is the header file for the ADC module.
 * 		  The PIC18F452 device contains only one ADC.
 * Author: nour
 *
 * Created on May 16, 2025, 2:50 PM
 */

#ifndef HAL_ACD_H
#define	HAL_ACD_H
/*** includes *****/
#include <xc.h>
#include "mcal_adc_cfg.h"
#include "../GPIO/mcal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../Interrupt/mcal_interrupt_config.h"  
/*** macros *****/
/* Turn on/off the ADC module */
#define ADC_ON() (ADCON0bits.ADON=1)
#define ADC_OFF() (ADCON0bits.ADON=0)
/* Return the status of the ADC 
 * 1 -> in progress 
 * 0 -> finished
 */
#define ADC_STATUS (ADCON0bits.GO_nDONE)
/* Start the conversion process */
#define ADC_START() (ADCON0bits.GO_nDONE=1)
/* Choose result justification */
/* Result of the AD conversion is either 
 * right justified   (000000xx xxxxxxxx)
 * or left justified (xxxxxxxx xx000000)
 */
#define __RIGHT 1
#define __LEFT 0
#define ADC_JUST __RIGHT
#define ADC_JUSTIFY_RIGHT() (ADCON1bits.ADFM=1)
#define ADC_JUSTIFY_LEFT() (ADCON1bits.ADFM=0)
/* ADC conversion clock select */
#define ADC_CCLK_SET(n) \
	(ADCON1bits.ADCS2 = ((uint8)(n >> 2) & 0x01)); \
	(ADCON0bits.ADCS1 = ((uint8)(n >> 1) & 0x01)); \
	(ADCON0bits.ADCS0 = ((uint8)(n) & 0x01)); 
/* ADC channel select */
#define ADC_CH_SEL(CH) (ADCON0bits.CHS=(uint8)CH & 0x07)
/* A/D port configuration */
/* @Note: The configuration bits determine whether certain A/D pins are configured
 * as digital I/O or as analog inputs.
 * The bits that control A/D port configuration are the same that control the choice
 * of Vref+ and Vref- (the positive and negative reference voltages for the ADC.)
 * There are 16 (4-bit) available combinations for A/D pin configuration & Vref choice.
 * Follow the following documentation for correct choice.
 */
/* 					AN7	AN6	AN5	AN4	AN3	AN2	AN1	AN0	Vref+ Vref-
 * Combination 1:	 A   A   A   A   A   A   A   A   Vdd   Vss 
 * Combination 2:    A   A	 A	 A   Vr+ A   A   A   AN3   Vss 
 * Combination 3:	 D   D   D   A   A   A   A   A   Vdd   Vss
 * Combination 4:	 D   D   D   A   Vr+ A   A   A   AN3   Vss     
 * Combination 5:    D   D   D   D   A   D   A   A   Vdd   Vss 
 * Combination 6:    D   D   D   D   Vr+ A   A   A   AN3   Vss 
 * Combination 7:    D   D   D   D   D   D   D   D    -     - 
 * Combination 8:    A   A   A   A   Vr+ Vr- A   A   AN3   AN2 
 * Combination 9:    D   D   A   A   A   A   A   A   Vdd   Vss 
 * Combination 10:   D   D   A   A   Vr+ A   A   A   AN3   Vss 
 * Combination 11:   D   D   D   A   Vr+ Vr- A   A   AN3   AN2 
 * Combination 12:   D   D   D   D   Vr+ Vr- A   A   AN3   AN2 
 * Combination 13:   D   D   D   D   Vr+ Vr- A   A   AN3   AN2 
 * Combination 14:   D   D   D   D   D   D   D   A   Vdd   Vss
 * Combination 15:   D   D   D   D   Vr+ Vr- D   A   AN3   AN2 
 */
/* AN0 -> RA0, AN1 -> RA1, AN2 -> RA2, AN3 -> RA3
 * AN4 -> RA5, AN5 -> RE0, AN6 -> RE1, AN7 -> RE2 */

/* Choose 8,11,12,13, or 15 if you want a Vref defined by AN3 & AN2 */
/* Choose 2,4,6, or 10 if you want only the a positve Vref (Vref+) 	*/
/********************************************************************/
/* Choose a combination from 1 to 15 */
#define ADC_CHOOSE_COMB(n) \
    do{ \
        if (n >= 7) \
        (ADCON1bits.PCFG = ((uint8)(n) & 0x0F)); \
         else \
        (ADCON1bits.PCFG = ((uint8)(n-1) & 0x0F)); \
    }while (0)
/*** data types ***/
typedef enum{
	ADC_default=-1,
	ADC_ch0,
	ADC_ch1,
	ADC_ch2,
	ADC_ch3,
	ADC_ch4,
	ADC_ch5,
	ADC_ch6,
	ADC_ch7
}ADC_CH_t;
typedef enum{
	ADC_CCLOCK_FOSC_DIV_2,
	ADC_CCLOCK_FOSC_DIV_8,
	ADC_CCLOCK_FOSC_DIV_32,
	ADC_CCLOCK_FRC,
	ADC_CCLOCK_FOSC_DIV_4,
	ADC_CCLOCK_FOSC_DIV_16,
	ADC_CCLOCK_FOSC_DIV_64
}ADC_CCLK_t;
typedef enum{
	nDone = -1,
	Done,
}ADC_status_t;
typedef struct{
#if (INT_ADC == INT_EN)
	void (*ADC_interrupt_handler) (uint16 * result);
#if (INT_PR == INT_EN)
	uint8 priority; /*@ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	ADC_CH_t channel;
	ADC_CCLK_t conversion_clock;/* @ref: ADC_CCLK_t */
	uint8 combination; /* @ref: A/D port configuration */
}ADC_t;

typedef uint16 ADC_result_size;
/*** prototypes ***/
STD_ReturnType ADC_initialize(const ADC_t * lADC);
STD_ReturnType ADC_deinit(const ADC_t * lADC);
STD_ReturnType ADC_select_channel(ADC_CH_t lCH);
STD_ReturnType ADC_convert(const ADC_t * lADC, ADC_CH_t lCH);
STD_ReturnType ADC_result(const ADC_t * lADC, ADC_result_size * result);
STD_ReturnType ADC_status(const ADC_t * lADC, ADC_status_t * status);

#endif	/* HAL_ACD_H */

