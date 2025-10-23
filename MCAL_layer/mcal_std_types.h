/* 
 * File:   mcal_std_types.h
 * Author: nour
 *
 * Created on October 23, 2024, 7:15 AM
 */

#ifndef MCAL_STD_TYPES_H
#define	MCAL_STD_TYPES_H
/**************** includes ******************/
#include "std_libs.h"

/*************** macros ********************/
#define STD_HIGH 0x01
#define STD_LOW  0x00
#define STD_ON  0x01
#define STD_OFF  0x00
#define STD_ACTIVE 0x01
#define STD_IDLE 0x00
/* @note: The E_OK and E_NOT_OK are assigned as 1 and 0 so that
 * 		  it is easier to AND results from routines with a 
 * 		  STD_ReturnType type. (ret = ret && routine()). 
 */
#define E_OK (STD_ReturnType)(0x01)
#define E_NOT_OK (STD_ReturnType)(0x00)
#define E_OVR (STD_ReturnType)(0xFF) /* Overrun error (USART) */
#define E_FERR (STD_ReturnType)(0xFE) /* Framming error (USART) */
#define E_NOT_ADDRD (STD_ReturnType)(0xFD) /* USART not addressed */
#define E_RX_PARITY_ERR (STD_ReturnType)(0xFC) /* Parity error at the receiver */
#define E_BUS_COLL (STD_ReturnType)(0xFB) /* Bus collision (I2C) */
#define E_WR_COLL (STD_ReturnType)(0xFA) /* Write collision (I2C) */
/*************** function macros ***********/
/*************** data types ****************/
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef signed char sint8;
typedef signed long sint32;
typedef signed short sint16;
typedef uint8 STD_ReturnType;
/*************** function declarations *****/

#endif	/* MCAL_STD_TYPES_H */

