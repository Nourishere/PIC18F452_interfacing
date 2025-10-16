/* 
 * File:   mcal_usart.c
 * Author: nour
 *
 * Created on August 16, 2025, 11:41 AM
 * @Brief: This source contains driver implementations
 * 		   for the USART module. 
 */
#include "mcal_usart.h"

#if (USART_MODULE == STD_ON)
static STD_ReturnType USART_baud_initialize(const USART_Tx_t* usart_tx); 
static STD_ReturnType USART_parity_generator(USART_parity parity, uint8 data, uint8* k);
static STD_ReturnType USART_parity_checker(USART_parity parity, uint8 data, uint8* status);

/* @brief: Initialize the transmitter USART. 
 * @param: A pointer to a struct of type USART_TX_t.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType USART_Tx_initialize(const USART_Tx_t* usart_tx){
	STD_ReturnType ret = E_OK;  
#if (INT_USART_TX == INT_EN && INT_PR == INT_EN)
	INT_USART_Tx_init(usart_tx -> priority);
#elif(INT_USART_TX == INT_EN && INT_PR == INT_DIS)
	INT_USART_Tx_init(1); //Garbage value
#endif
	if(NULL== usart_tx)
		ret = E_NOT_OK;
	else{
		/* Set up pin RC6&RC7 as serial */	
		USART_PIN_SERIAL();
		/* Set up baud rate */
		ret = USART_baud_initialize(usart_tx);
		/* Set up the interrupt */

		/* Start the transmission */
		USART_TX_EN();
		/* Set up transmission width */
#if(_9BIT == EN)
		USART_TX9();
		TXSTAbits.TX9D = usart_tx -> _9th;
#elif(_9BIT == DIS)
		USART_TX8();
#else
		ret = ret && E_NOT_OK;
#endif
		/* Enable transmission */
		USART_TX_EN();
#if(USART_MODE == _ASYNC)
		USART_ASYNC();	
#elif(USART_MODE == _SYNC_MASTER)
		USART_SYNC();	
		USART_CLKS_MASTER(); /* The clock source is driven from the BRG */
		(RCSTAbits.CREN=0);
		(RCSTAbits.SREN=0);
#elif(USART_MODE == _SYNC_SLAVE)
		USART_SYNC();	
		RT_CLKS_SLAVE(); /* The clock is driven externally */
		(RCSTAbits.CREN=0);
		(RCSTAbits.SREN=0);
#else
		ret = E_NOT_OK;
#endif
        }
	return ret;
}

/* @brief: Initialize the USART receiver. 
 * @param: A pointer to a USART_Rx_t type.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType USART_Rx_initialize(const USART_Rx_t* usart_rx){
	STD_ReturnType ret = E_OK;  
	if(NULL== usart_rx)
		ret = E_NOT_OK;
	else{
#if (INT_USART_TX == INT_EN && INT_PR == INT_EN)
		INT_USART_Rx_init(usart_rx -> priority);
#elif(INT_USART_TX == INT_EN && INT_PR == INT_DIS)
		INT_USART_Tx_init(1); //Garbage value
#endif
		/* Set up baud rate */
		ret = USART_baud_initialize(usart_rx);
		/* Set up pin RC6&RC7 as serial */	
		USART_PIN_SERIAL();
		/* Disable bits, temporarly, according to datasheet */
		(RCSTAbits.CREN=0);
		(RCSTAbits.SREN=0);
		/* Set up the interrupt */
		
		/* Set up transmission width */
#if(_9BIT == EN)
		USART_RX9();
		RCSTAbits.RX9D = usart_rx -> _9th;
#elif(_9BIT == DIS)
		USART_RX8();
#else
		ret = ret && E_NOT_OK;
#endif
		/* Single/Continuous reception */
		if(usart_rx -> reception == USART_single)
			USART_1RX_ON();
		else if(usart_rx -> reception == USART_continous)
			USART_RX_CONTINOUS_ON();	
		else
			ret = E_NOT_OK;
		/* Enable receiving */
		USART_RX_EN();
#if(USART_MODE == _ASYNC)
			USART_ASYNC();
#elif(USART_MODE == _SYNC_MASTER)
			USART_SYNC();
			USART_CLKS_MASTER();
			USART_1RX_OFF();
			USART_RX_CONTINOUS_OFF();
#elif(USART_MODE == _SYNC_SLAVE)
			USART_SYNC();
			USART_CLKS_SLAVE();
#else
			ret = E_NOT_OK;
#endif
	}
	return ret;
}   
/* @brief: Start transmitting raw 8/9 bit data.
 * @param: A pointer to a struct of type USART_Tx_t.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 * @note: Only the first eight or nine bits are used.
 */
STD_ReturnType USART_send_raw(const USART_Tx_t* usart_tx, uint16 data){
	STD_ReturnType ret = E_OK;
	if(NULL == usart_tx)
		ret = E_NOT_OK;
	else{
		/* Set baud rate */
		ret = USART_baud_initialize(usart_tx);
#if (_9BIT == EN)
#if (USART_PARITY == _OFF)
		TXSTAbits.TX9D = (data >> 8) & 0x01;  
#elif (USART_PARITY == _ON)
		/* Generate 9th bit based on parity */
		uint8 k=0;
		ret = ret && USART_parity_generator(usart_tx -> parity, data, &k);
		TXSTA1bits.TX9D=k;
#else
		ret = E_NOT_OK;
#endif
#endif
		/* Loading data into TXREG starts the transmission */
		TXREG = data & 0x00FF;
	}
	return ret;
}
/* @brief: Receive raw data. 
 * @param: A pointer to a USART_TX_t type and a pointer to a uint16 
 * 		   to store the read data.
 * @return: E_OK upon success.
 * 		    E_OVR if overrun. 
 * 		    E_FERR upon framing errors.
 * 		   	E_RX_PARITY_ERR if parity check fails.
 * 		    E_NOT_OK otherwise.
 * @note: If 9-Bit mode is used, the 9th bit is stored in the 16-bit wide 
 * 	      data variable. This is unlike the USART_receive_character routine.
 * 	      @ref: hal_usart.h -> USART_receive_character.
 */
STD_ReturnType USART_receive_raw(const USART_Tx_t* usart_rx, uint16* data){
	STD_ReturnType ret = E_OK;
	if(NULL == usart_rx || data == NULL)
		ret = E_NOT_OK;
	else{
		/* Wait for transmission to be completed */
		while(!RCIF);
		/* Read the data */
#if (_9BIT == EN)
		*data = (uint16)(RCREG) | (uint16)((RCSTAbits.RX9D) << 8);
#if (USART_PARITY == _ON)
		uint8 parity_status=0;
		/* Check parity */
		ret = ret && USART_parity_checker(usart_rx -> parity, *data, &parity_status);
		if(parity_status == 1)
			ret = E_RX_PARITY_ERR;
		
#endif
#elif (_9BIT == DIS)
		*data = RCREG & 0x00FF;
#else  
		ret = E_NOT_OK;
#endif
		if(OVERRUN_BIT == 1){
			ret = E_OVR;
			RCSTAbits.CREN=0;
		}
		if(FRAME_BIT == 1){
			ret = E_FERR;
		    RCSTAbits.CREN=0;
		}
	}
	return ret;
}
/* @brief: Send a NULL-terminated string. 
 * @param: A pointer to a struct of type USART_Tx_t and a pointer to
 * 	       an array of characters that contains the intended data. 
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType USART_send_string(const USART_Tx_t* usart_tx, char* str){
    STD_ReturnType ret = E_OK;
	if(NULL == usart_tx)
		ret = E_NOT_OK;
	else{
		while(*str != '\0'){
			while(!PIR1bits.TXIF); /* Wait for transmission to end */
			/* Configure parity bit if used */
#if (_9BIT == EN)
#if (USART_PARITY == _OFF)
			TXSTAbits.TX9D = usart_tx -> 9th;  
#elif (USART_PARITY == _ON)
            uint8 k=0;
			/* Write 9th bit based on parity */
			ret = ret && USART_parity_generator(usart_tx -> parity,*str, &k);
            TXSTA1bits.TX9D=k;
#else
			ret = E_NOT_OK;	
#endif
#endif
		/* Loading data into TXREG starts the transmission */
			TXREG = *str;
			str++;
		}	
	}
    return ret;
}

/* @brief: Send a single character. 
 * @param: A pointer to a struct of type USART_Tx_t and a character to send.
 * @return: E_OK if transmission is successfull and E_NOT_OK otherwise.
 */
STD_ReturnType USART_send_char(const USART_Tx_t* usart_tx, char character){
    STD_ReturnType ret = E_OK;
	if(NULL == usart_tx)
		ret = E_NOT_OK;	
	else{
		/* Set baud rate */
		ret = USART_baud_initialize(usart_tx);
#if (_9BIT == EN)
#if (USART_PARITY == _OFF)
		TXSTAbits.TX9D = usart_tx -> _9th;  
#elif (USART_PARITY == _ON)
        uint8 k=0;
		/* Write 9th bit based on parity */
		ret = ret && USART_parity_generator(usart_tx -> parity, character, &k);
        TXSTA1bits.TX9D=k;
#else
		ret = E_NOT_OK;
#endif
#endif
		/* Loading data into TXREG starts the transmission */
		TXREG = character;
	}
    return ret;
}

/* @brief: Receive a string of characters. 
 * @param: A pointer to a struct of type UsART_Rx_t and a pointer to a 
 * 	       character array to store the received string in.
 * @return: E_OK upon success
 * 		    E_OVR upon overrun
 * 		    E_FERR upon framing error
 * 		    E_RX_PARITY_ERR upon parity error
 * 		    E_NOT_OK otherwise
 * @note: 9-bit transmission is not supported. Parity is supported, though.
 * 		  The sender should send appropriate data according to the device's 
 * 		  pre configuration. In case of parity, 9 bits should be sent and 
 * 		  parity checking is done accordingly. 
 */
STD_ReturnType USART_receive_string(USART_Rx_t* usart_rx, char* str){
	STD_ReturnType ret = E_OK;
	if(NULL == usart_rx || str == NULL)
		ret = E_NOT_OK;
	else{
		while(*str != '\0' && RCSTAbits.CREN){
			while(!RCIF){
#if (_9BIT == EN)
#if (USART_PARITY == _ON)
				uint8 k=0;
				/* Reading RCREG starts the transmission */
				*str = RCREG;
				ret = ret && USART_parity_checker(usart_rx -> parity,*str, &k);
				if(k == 1)
					return E_RX_PARITY_ERR;
#else
				ret = E_NOT_OK;	/* 9bit transmission not supported */
#endif
#endif
				*str = RCREG; /* Read the data */
				str++;
			}
		}
		if(OVERRUN_BIT == 1){
			ret = E_OVR;
			RCSTAbits.CREN=0;
		}
		if(FRAME_BIT == 1){
			ret = E_FERR;
		    RCSTAbits.CREN=0;
		}
	}
	return ret;
}

/* @brief: Receive a single character.
 * @param: A pointer to a struct of type USART_RX_t
 * 		    and a pointer to a char to store received data..
 * @return: E_OK upon success
 * 			E_OVR upon overrun
 * 			E_FERR upon framing error
 * 			E_RX_PARITY_ERR upon parity error
 * 			E_NOT_OK otherwise.
 */
STD_ReturnType USART_receive_char(USART_Rx_t* usart_rx, char* character){
	STD_ReturnType ret = E_OK;
	if(NULL == usart_rx || character == NULL)
		ret = E_NOT_OK;
	else{
		/* Wait for transmission to be completed */
		while(!RCIF);
		/* Read the data */
#if (_9BIT == EN)
#if (USART_PARITY == _ON)
		uint8 parity_status=0;
		/* Check parity */
		ret = ret && USART_parity_checker(usart_rx -> parity, *character, &parity_status);
		if(parity_status == 1)
			ret = E_RX_PARITY_ERR;
#elif (USART_PARITY == _OFF)	
		*character= RCREG; /* Must read first to see the 9th bit */
		usart_rx -> _9th = RCSTAbits.RX9D; /* Read the 9th bit into the usart_rx struct */
#else
		ret = E_NOT_OK;
#endif
#elif (_9BIT == DIS)
		*character= RCREG;
#else  
		ret = E_NOT_OK;
#endif
		if(OVERRUN_BIT == 1){
			ret = E_OVR;
			RCSTAbits.CREN=0;
		}
		if(FRAME_BIT == 1){
			ret = E_FERR;
		    RCSTAbits.CREN=0;
		}
	}
	return ret;
}

/* @brief:(H) Set up the SPBRG register and the baud mode.
 * @param: A pointer to a struct of type USART_Tx_t.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 * @note: The Baud rate generator (BRG) is used in both the
 * 	      transmitter and recierver parts of the module. Setting
 * 	      the baud rate here (for the transmitter) automatically 
 * 	      sets it for the receiver.
 * @note: High speed baud mode does not work with the synchronous mode.
 */
static STD_ReturnType USART_baud_initialize(const USART_Tx_t* usart_tx){
	STD_ReturnType ret = E_OK;
	if (NULL == usart_tx)
		ret = E_NOT_OK;
	else{
		if(usart_tx -> baud_mode == USART_high_speed){
			USART_HS(); 
#if (USART_MODE == _ASYNC)
				SPBRG = (uint8)((_XTAL_FREQ / ((usart_tx -> baud_rate) * 16.0))-1);
#else
				ret = E_NOT_OK; /* HS mode does not work with synchronous UART */
#endif
		}
		else if(usart_tx -> baud_mode == USART_low_speed){
			USART_LS(); 
#if (USART_MODE == _ASYNC)
				SPBRG =(uint8)((_XTAL_FREQ / ((usart_tx -> baud_rate) * 64.0))-1);		
#elif (USART_MODE == _SYNC_MASTER || USART_MODE == _SYNC_SLAVE)
				SPBRG = (uint8)((_XTAL_FREQ / ((usart_tx -> baud_rate) * 4.0))-1);		
#else
				ret = E_NOT_OK;
#endif
		}
		else
			ret = E_NOT_OK;
	}	
	return ret;
}
/* @brief:(H) Generate parity based on the parity type and the data contents.
 * @param: A type USART_parity specifying even or odd parity, a uint8 for the data
 * 		   and a pointer to a uint8 for which the 9th data bit data is to be stored.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
static STD_ReturnType USART_parity_generator(USART_parity parity, uint8 data, uint8* k){
	STD_ReturnType ret = E_OK;
	uint8 i=1,j=0;
	if(NULL == k)
		ret = E_NOT_OK;
	else{
		for(i=0; i<8;i++){
			if(data & (1 << i))		
				j++;
		}
		if(parity == USART_even)
			*k = (j % 2 == 0)?0:1;
		else if(parity == USART_odd)
			*k = (j % 2 == 0)?1:0;
		else
			ret = E_NOT_OK;
	}
	return ret;
}
/* @brief:(H) Checks parity.
 * @param: A USART_parity type for even or odd parity, data contents,
 * 	       and a pointer to a uint8 for the result of the checking.
 * @return: E_OK if checking went good and E_NOT_OK otherwise.
 * 	        Status is 0 if parity is correct and 1 if not correct.
 * @note: This routine reads bit value RX9D from a SFR so timing is
 * 		  essential.
 */
static STD_ReturnType USART_parity_checker(USART_parity parity, uint8 data, uint8* status){
	STD_ReturnType ret = E_OK;
	if(status == NULL)
		ret = E_NOT_OK;
	else{
#if (USART_PARITY == _ON)
		uint8 i = 0,j=0;
		for(; i<8;i++){
			if(data & (1 << i))		
				j++;
		}
		if(RCSTAbits.RX9D) /* Don't forget the parity bit! */
			j++;
		if((parity == USART_even && (j % 2 == 0)) || (parity == USART_odd && (j % 2 != 0)))
			*status = 0; /* parity correct */
		else 
			*status = 1; /* parity incorrect */
#else
	ret = E_NOT_OK;
#endif
	}
	return ret;
}
#endif

