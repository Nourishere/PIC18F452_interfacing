/* 
 * File:   mcal_internal_interrupt.h
 * Author: nour
 * Brief: This header file contains the configuration of
 * 		  all internal and peripheral interrupt features.
 * Created on April 15, 2025, 2:25 PM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H
/***** includes ********/
#include "mcal_interrupt_config.h"

/***** macros *********/
/* Status of the ADC (EN/DIS) */
#define INT_ADC_STATUS (PIE1bits.ADIE)
/* The value of the ADC flag */
#define INT_ADC_F (PIR1bits.ADIF)
/* status of the TMR1 (EN/DIS) */
#define INT_TMR1_STATUS (PIE1bits.TMR1IE)
/* The value of the TMR1 flag */
#define INT_TMR1_F (PIR1bits.TMR1IF)
/* status of the TMR2 (EN/DIS) */
#define INT_TMR2_STATUS (PIE1bits.TMR2IE)
/* The value of the TMR2 flag */
#define INT_TMR2_F (PIR1bits.TMR2IF)
/* status of the TMR3 (EN/DIS) */
#define INT_TMR3_STATUS (PIE2bits.TMR3IE)
/* The value of the TMR3 flag */
#define INT_TMR3_F (PIR2bits.TMR3IF)
/* status of the CCP1 (EN/DIS) */
#define INT_CCP1_STATUS (PIE1bits.CCP1IE)
/* The value of the CCP1 flag */
#define INT_CCP1_F (IPR1bits.CCP1IP)
/* status of the CCP2 (EN/DIS) */
#define INT_CCP2_STATUS (PIE2bits.CCP2IE)
/* The value of the CCP2 flag */
#define INT_CCP2_F (IPR2bits.CCP2IP)
/* status of the MSSP (EN/DIS) */
#define INT_MSSP_STATUS (PIE1bits.SSPIE)
/* The value of the MSSP flag */
#define INT_MSSP_F (PIR1bits.SSPIF)
/* status of the USART_Tx (EN/DIS) */
#define INT_USART_TX_STATUS (PIE1bits.TXIE)
/* The value of the USART_Tx flag */
#define INT_USART_TX_F (PIR1bits.TXIF)
/* status of the USART_Rx (EN/DIS) */
#define INT_USART_RX_STATUS (PIE1bits.RCIE)
/* The value of the USART_Rx flag */
#define INT_USART_RX_F (PIR1bits.RCIF)

/****** function-like macros ******/
#if (INT_ADC == INT_EN)
/* Enable/Disable the ADC interrupt */
#define INT_ADC_EN() (PIE1bits.ADIE=1)
#define INT_ADC_DIS() (PIE1bits.ADIE=0)
/* Clear the ADC interrupt flag */
#define INT_ADC_CLRF() (PIR1bits.ADIF=0)
/* Set ADC low/high priority */
#if (INT_PR == INT_EN)
#define INT_ADC_HP() (IPR1bits.ADIP=1)
#define INT_ADC_LP() (IPR1bits.ADIP=0)
#endif
#endif

#if (INT_TMR1 == INT_EN)
/* Enable/Disable the TMR1 interrupt */
#define INT_TMR1_EN() (PIE1bits.TMR1IE=1)
#define INT_TMR1_DIS() (PIE1bits.TMR1IE=0)
/* Clear the TMR1 interrupt flag */
#define INT_TMR1_CLRF() (PIR1bits.TMR1IF=0)
/* Set the TMR1 priority */
#if (INT_PR == INT_EN)
#define INT_TMR1_HP() (IPR1bits.TMR1IP=1)
#define INT_TMR1_LP() (IPR1bits.TMR1IP=0)
#endif
#endif

#if (INT_TMR2 == INT_EN)
/* Enable/Disable the TMR2 interrupt */
#define INT_TMR2_EN() (PIE1bits.TMR2IE=1)
#define INT_TMR2_DIS() (PIE1bits.TMR2IE=0)
/* Clear the TMR2 interrupt flag */
#define INT_TMR2_CLRF() (PIR1bits.TMR2IF=0)
/* Set the TMR2 priority */
#if (INT_PR == INT_EN)
#define INT_TMR2_HP() (IPR1bits.TMR2IP=1)
#define INT_TMR2_LP() (IPR1bits.TMR2IP=0)
#endif
#endif

#if (INT_TMR3 == INT_EN)
/* Enable/Disable the TMR3 interrupt */
#define INT_TMR3_EN() (PIE2bits.TMR3IE=1)
#define INT_TMR3_DIS() (PIE2bits.TMR3IE=0)
/* Clear the TMR3 interrupt flag */
#define INT_TMR3_CLRF() (PIR2bits.TMR3IF=0)
/* Set the TMR3 priority */
#if (INT_PR == INT_EN)
#define INT_TMR3_HP() (IPR2bits.TMR3IP=1)
#define INT_TMR3_LP() (IPR2bits.TMR3IP=0)
#endif
#endif

#if (INT_CCP1 == INT_EN)
/* Enable/Disable the CCP1 interrupt */
#define INT_CCP1_EN() (PIE1bits.CCP1IE=1)
#define INT_CCP1_DIS() (PIE1bits.CCP1IE=0)
/* Clear the CCP1 interrupt flag */
#define INT_CCP1_CLRF() (PIR1bits.CCP1IF=0)
/* Set the CCP1 interrupt priority */
#if (INT_PR == INT_EN)
#define INT_CCP1_HP() (IPR1bits.CCP1IP=1)
#define INT_CCP1_LP() (IPR1bits.CCP1IP=0)
#endif
#endif

#if (INT_CCP2 == INT_EN)
/* Enable/Disable the CCP2 interrupt */
#define INT_CCP2_EN() (PIE2bits.CCP2IE=1)
#define INT_CCP2_DIS() (PIE2bits.CCP2IE=0)
/* Clear the CCP2 interrupt flag */
#define INT_CCP2_CLRF() (PIR2bits.CCP2IF=0)
/* Set the CCP2 interrupt priority */
#if (INT_PR == INT_EN)
#define INT_CCP2_HP() (IPR2bits.CCP2IP=1)
#define INT_CCP2_LP() (IPR2bits.CCP2IP=0)
#endif
#endif

#if (INT_MSSP == INT_EN)
/* Enable/Disable the MSSP interrupt */
#define INT_MSSP_EN() (PIE1bits.SSPIE=1)
#define INT_MSSP_DIS() (PIE1bits.SSPIE=0)
/* Clear the MSSP interrupt flag */
#define INT_MSSP_CLRF() (PIR1bits.SSPIF=0)
/* Set the MSSP interrupt priority */
#if (INT_PR == INT_EN)
#define INT_MSSP_HP() (IPR1bits.SSPIP=1)
#define INT_MSSP_LP() (IPR1bits.SSPIP=0)
#endif
#endif

#if (INT_USART_TX == INT_EN)
/* Enable/Disable the USART Tx interrupt */
#define INT_USART_TX_EN() (PIE1bits.TXIE=1)
#define INT_USART_TX_DIS() (PIE1bits.TXIE=0)
/* Clear the USART Tx interrupt flag */
#define INT_USART_TX_CLRF() (PIR1bits.TXIF=0)
/* Set the USART Tx interrupt priority */
#if (INT_PR == INT_EN)
#define INT_USART_TX_HP() (IPR1bits.TXIP=1)
#define INT_USART_TX_LP() (IPR1bits.TXIP=0)
#endif
#endif

#if (INT_USART_RX == INT_EN)
/* Enable/Disable the USART Rx interrupt */
#define INT_USART_RX_EN() (PIE1bits.RCIE=1)
#define INT_USART_RX_DIS() (PIE1bits.RCIE=0)
/* Clear the USART Rx interrupt flag */
#define INT_USART_RX_CLRF() (PIR1bits.RCIF=0)
/* Set the USART Rx interrupt priority */
#if (INT_PR == INT_EN)
#define INT_USART_RX_HP() (IPR1bits.RCIP=1)
#define INT_USART_RX_LP() (IPR1bits.RCIP=0)
#endif
#endif

/***** Function prototypes *****/

#if (INT_ADC == INT_EN)
void ADC_ISR(void);
STD_ReturnType INT_ADC_init(uint8 priority);
STD_ReturnType INT_ADC_deinit(void);
STD_ReturnType INT_ADC_set_callback_routine(volatile void (*callback) (uint16 * result));
#endif

#if (INT_TMR1 == INT_EN)
void TMR1_ISR(void);
STD_ReturnType INT_TMR1_init(uint8 priority);
STD_ReturnType INT_TMR1_deinit(void);
STD_ReturnType INT_TMR1_set_callback_routine(void (*callback) (void));
#endif

#if (INT_TMR2 == INT_EN)
void TMR2_ISR(void);
STD_ReturnType INT_TMR2_init(uint8 priority);
STD_ReturnType INT_TMR2_deinit(void);
STD_ReturnType INT_TMR2_set_callback_routine(void (*callback) (void));
#endif

#if (INT_TMR3 == INT_EN)
void TMR3_ISR(void);
STD_ReturnType INT_TMR3_init(uint8 priority);
STD_ReturnType INT_TMR3_deinit(void);
STD_ReturnType INT_TMR3_set_callback_routine(void (*callback) (void));
#endif

#if (INT_CCP1 == INT_EN)
void CCP1_ISR(void);
STD_ReturnType INT_CCP1_init(uint8 priority);
STD_ReturnType INT_CCP1_deinit(void);
STD_ReturnType INT_CCP1_set_callback_routine(void (*callback) (void));
#endif

#if (INT_CCP2 == INT_EN)
void CCP2_ISR(void);
STD_ReturnType INT_CCP2_init(uint8 priority);
STD_ReturnType INT_CCP2_deinit(void);
STD_ReturnType INT_CCP2_set_callback_routine(void (*callback) (void));
#endif

#if(INT_MSSP == INT_EN)
void MSSP_ISR(void);
STD_ReturnType INT_MSSP_init(uint8 priority);
STD_ReturnType INT_MSSP_deinit(void);
STD_ReturnType INT_MSSP_set_callback_routine(void (*callback) (void));
#endif

#if(INT_MSSP == INT_EN)
void USART_Tx_ISR(void);
STD_ReturnType INT_USART_Tx_init(uint8 priority);
STD_ReturnType INT_USART_Tx_deinit(void);
STD_ReturnType INT_USART_Tx_set_callback_routine(void (*callback) (void));
#endif

#if(INT_MSSP == INT_EN)
void USART_Tx_ISR(void);
STD_ReturnType INT_USART_Rx_init(uint8 priority);
STD_ReturnType INT_USART_Rx_deinit(void);
STD_ReturnType INT_USART_Rx_set_callback_routine(void (*callback) (void));
#endif

#endif	/* MCAL_INTERNAL_INTERRUPT_H */
