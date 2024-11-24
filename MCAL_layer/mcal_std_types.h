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
#include "compiler.h"
/*************** macros ********************/
#define STD_HIGH 0x01;
#define STD_LOW  0x00;
#define STD_ON  0x01;
#define STD_OFF  0x00;
#define STD_ACTIVE 0x01;
#define STD_IDLE 0x00;
#define E_OK (STD_ReturnType)(0x01)
#define E_NOT_OK (STD_ReturnType)(0x00)
/*************** function macros ***********/
/*************** data types ****************/
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed char sint8;
typedef signed int sint32;
typedef signed short sint16;
typedef uint8 STD_ReturnType;
/*************** function declarations *****/

#endif	/* MCAL_STD_TYPES_H */

