/* 
 * File:   mcal_interrupt_config.h
 * Author: nour
 * Brief: This header file contains the common configurations
 * 		  required for any interrupt module on the device.
 * Created on April 15, 2025, 2:24 PM
 */

#ifndef MCAL_INTERRUPT_CONFIG_H
#define	MCAL_INTERRUPT_CONFIG_H
/************ includes *****************/
#include <xc.h>
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"
/************ Macros *******************/
#define INT_EN 1
#define INT_DIS 0
#define INT_ACTIVE 1
#define INT_DEAD 0
#define INT_LOW 0
#define INT_HIGH 1
#define INT_PEN 1
#define INT_PDIS 0
#define INT_PLOW 0
#define INT_PHIGH 1
/* Priority feature control */
#define INT_PR INT_EN
/******** Interrupt configuration macros *******/
// External interrupt config
#define INT_INTx INT_EN
#define INT_PORTB INT_EN
// Internal interrupt config
/* ADC interrupt config */
#define INT_ADC INT_EN


/********* function-like macros ********/

/* Macro to check on RBx flags and choose ISR */
#define CHECK_PIN_CHANGE(pin, flag, handler)  \
    do { \
        if (PORTBbits.pin == GPIO_HIGH && flag == 1) { \
            flag = 0; \
            handler(1); \
        } else if (PORTBbits.pin == GPIO_LOW && flag == 0) { \
            flag = 1; \
            handler(0); \
        } \
    } while (0)

/* The bits GIEH and GIE are the same. GIEH is used when the priority feature is on (IPEN=1).
 * The bits GIEL and PEIE are the same. GIEL is used when the priority feature is on (IPEN=1).
 * If you wish to use the priority feature, set the macro INT_PR and use the macro function INT_PREN().
*/
/* priority feature enable */
#define INT_PREN() RCONbits.IPEN=1
/* priority feature disable */
#define INT_PRDIS() RCONbits.IPEN=0
#if (INT_PR == INT_EN)
/* global high priority enable */
#define INT_GHPEN() INTCONbits.GIEH=1
/* global high priority disable */
#define INT_GHPDIS() INTCONbits.GIEH=0
/* global low priority enable */
#define INT_GLPEN() INTCONbits.GIEL=1
/* global low priority disable */
#define INT_GLPDIS() INTCONbits.GIEL=0
#else
/* global enable */
#define INT_GEN() INTCONbits.GIE=1
/* global disable */
#define INT_GDIS() INTCONbits.GIE=0
/* peripheral interrupt enable */
#define INT_PEEN() INTCONbits.PEIE=1
/* peripheral interrupt disable */
#define INT_PEDIS() INTCONbits.PEIE=0
#endif

/******** function declarations ********/


#endif	/* MCAL_INTERRUPT_CONFIG_H */
