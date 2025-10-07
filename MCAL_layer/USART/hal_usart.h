/* 
 * File:   hal_usart.h
 * Author: nour
 *
 * Created on August 16, 2025, 11:41 AM
 * @Brief: This header contains driver configurations 
 * 		   for the Universal Synchronous Asynchronous 
 * 		   Receiver Transmitter (USART) module.
 * 		   The USART can work in asynchronous mode, for 
 * 		   connections with outside devices and in synchronous mode
 * 		   for connections with peripherals
 */

#ifndef HAL_USART_H
#define	HAL_USART_H

/****************** include **********************/
#include "hal_usart_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_interrupt_config.h"
#include "../Interrupt/mcal_internal_interrupt.h"

/***************** macros  **********************/
#define TSR_EMPTY 1
#define TSR_STATUS (TXSTAbits.TRMT)
#define TX9D_STATUS (TXSTAbits.TX9D)
#define FRAME_BIT (RCSTAbits.FERR)
#define FRAME_ERR 1
#define FRAME_OK 0
#define OVERRUN_BIT (RCSTAbits.OERR)
#define OVERRUN_ERR 1
#define OVERRUN_OK 0
#define _RX9D (RCSTAbits.RX9D)

/**************** function macros **************/
/* Configure the USART synchronous mode clock source.
 * Master mode -> clock generated from BRG.
 * Slave mode -> clock uses external source.
 * Meaningless in asynchronous mode
 */
#define USART_CLKS_MASTER() (TXSTAbits.CSRC=1)
#define USART_CLKS_SLAVE() (TXSTAbits.CSRC=0)
/* 9-bit transmission */
#define USART_TX9() (TXSTAbits.TX9=1)
#define USART_TX8() (TXSTAbits.TX9=0)
/* Enable/Disable transmission */
#define USART_TX_EN() (TXSTAbits.TXEN=1)
#define USART_TX_DIS() (TXSTAbits.TXEN=0)
/* Synchronization mode */
#define USART_SYNC() (TXSTAbits.SYNC=1)
#define USART_ASYNC() (TXSTAbits.SYNC=0)
/* High baud selection 
 * @note: Better use the high speed mode for
 * 	      lower error rates.
 */
#define USART_HS() (TXSTAbits.BRGH=1)
#define USART_LS() (TXSTAbits.BRGH=0)

/* Set Rx and Tx pins settings */
#define USART_PIN_SERIAL() (RCSTAbits.SPEN=1)
#define USART_PIN_IO() (RCSTAbits.SPEN=0)
/* 9-bit receiving */
#define USART_RX9() (RCSTAbits.RX9=1)
#define USART_RX8() (RCSTAbits.RX9=0)
/* Single receive configuration (synchronous mode only) */
#define USART_1RX_ON() (RCSTAbits.SREN=1)
#define USART_1RX_OFF() (RCSTAbits.SREN=0)
/* Enable/Disable the receiver
 * @note: these should be used only in async mode.
 */
#define USART_RX_EN() (RCSTAbits.CREN=1)
#define USART_RX_DIS() (RCSTAbits.CREN=0)
/* Continuous receive setting
 * @note: these should be used only in sync mode.
 * 		  Note that reception is ongoing till CREN is 0.
 */
#define USART_RX_CONTINOUS_ON() (RCSTAbits.CREN=1)
#define USART_RX_CONTINOUS_OFF() (RCSTAbits.CREN=0)
/* Enable/Disable address detecting */
#define USART_RX_ADDRDET_ON() (RCSTAbits.ADDEN=1)
#define USART_RX_ADDRDET_OFF() (RCSTAbits.ADDEN=0)

/*************** data types *******************/
typedef enum{
	USART_even,
	USART_odd
}USART_parity;
typedef enum{
	USART_single,
	USART_continous
}USART_reception_mode;
typedef enum{
	USART_low_speed,
	USART_high_speed
}USART_baud_mode;
typedef enum{
	_8bits,
	_9bits	
}USART_stream;
/* @note: The receiver and transmitter units can both run asynchronously
 * 	      with each one running a different baud rate. Synchronous mode, 
 * 	      however, applies to both units at the same time. If synchronous 
 * 	      mode is used, the baud rate and baud mode used are these given 
 * 	      in the latest initialized USART_Rx/Tx unit. For the sake of 
 * 	      consistensy, identical values are adviced for each unit. 
 */ 
typedef struct{
#if(INT_USART_TX == INT_EN)
	void (*USART_Tx_interrupt_handler) (void); // Use NULL if not needed
#if(INT_PR == INT_EN)
	uint8 priority; /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	USART_baud_mode baud_mode; /* @note: High speed / Low speed */
	/* @note: Common baud rates are 4800, 9600, 19200, 
     * 		  57600, and 115200. */
	uint32 baud_rate; 
#if (_9BIT == EN)
	uint8 _9th:1;
#if (USART_PARITY == _ON)
	 USART_parity parity; /* USART_even, USART_odd */
#endif
#endif
}USART_Tx_t;
typedef struct{
#if(INT_USART_RX == INT_EN)
	void (*USART_Rx_interrupt_handler) (void); // Use NULL if not needed
#if(INT_PR == INT_EN)
	uint8 priority; /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	USART_baud_mode baud_mode; /* @note: High speed / Low speed */
	uint32 baud_rate;
	/* @note: Common values for the baud rate are 4800, 9600, 19200, 
     * 		  57600, and 115200. */
	USART_reception_mode reception;
#if (_9BIT == EN)
	uint8 _9th:1;
#if (USART_PARITY == _ON)
	 USART_parity parity; /* USART_even, USART_odd */
#endif
#endif
}USART_Rx_t;

/************** function prototypes **********/
STD_ReturnType USART_Tx_initialize(const USART_Tx_t* usart_tx);
STD_ReturnType USART_Rx_initialize(const USART_Rx_t* usart_rx);
STD_ReturnType USART_send_raw(const USART_Tx_t* usart_tx, uint16 data);
STD_ReturnType USART_send_string(const USART_Tx_t* usart_tx, char* str);
STD_ReturnType USART_send_char(const USART_Tx_t* usart_tx, char character);
STD_ReturnType USART_receive_string(USART_Rx_t* usart_rx, char* str);
STD_ReturnType USART_receive_char(USART_Rx_t* usart_rx, char* character);

#endif	/* HAL_USART_H */

