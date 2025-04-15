/* 
 * File:   mcal_interrupt_config.h
 * Author: nour
 *
 * Created on April 15, 2025, 2:24 PM
 */

#ifndef MCAL_INTERRUPT_CONFIG_H
#define	MCAL_INTERRUPT_CONFIG_H
/************ includes *****************/
#include <xc.h>
#include "../mcal_std_types.h"

/************ Macros *******************/
#define INT_PRI_EN 1
#define INT_PRI_DIS 0
#define INT_PR INT_PRI_EN
#define INT_EN 1
#define INT_DIS 0
#define INT_ACTIVE 1
#define INT_DEAD 0
#define INT_PEN 1
#define INT_PDIS 0

/********* function-like macros ********/
/* The bits GIEH and GIE are the same. GIEH is used when the priority feature is on (IPEN=1).
 * The bits GIEL and PEIE are the same. GIEL is used when the priority feature is on (IPEN=1).
 * If you wish to use the priority feature, set the macro INT_PR and use the macro function INT_PREN().
*/

/* priority feature enable */
#define INT_PREN() RCON.IPEN=1
/* priority feature disable */
#define INT_PRDIS() RCON.IPEN=0
#if INT_PR
/* global high priority enable */
#define INT_GHPEN() INTCON.GIEH=1
/* global high priority disable */
#define INT_GHPDIS() INTCON.GIEH=0
/* global low priority enable */
#define INT_GLPEN() INTCON.GIEL=1
/* global low priority disable */
#define INT_GLPDIS() INTCON.GIEL=0
#else
/* global enable */
#define INT_GEN() INTCON.GIE=1
/* global disable */
#define INT_GDIS() INTCON.GIE=0
/* peripheral interrupt enable */
#define INT_PEEN() INTCON.PEIE=1
/* peripheral interrupt disable */
#define INT_PEDIS() INTCON.PEIE=0
#endif

/******** function declarations ********/


#endif	/* MCAL_INTERRUPT_CONFIG_H */

