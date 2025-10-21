/* 
 * File:   mcal_mssp.c
 * Author: nour
 *
 * Created on September 9, 2025, 7:07 PM
 * @Brief: This file contains implementations for the MSSP module.
 * 	       The MSSP module can work as an SPI module or as an I2C module.
 */
#include "mcal_mssp.h"

#if (MSSP_MODULE == STD_ON)
//Practical tip: If you're only ever using the MSSP as master, configure the SS̅ pin as a digital output high. That way, it never accidentally pulls low and disables your SPI master.
uint8 I2C_initialized=0;
uint8 SPI_initialized=0;
uint8 addr_high = 0xF0;
#if (MSSP_CFG == MSSP_I2C)
uint8 MSSP_I2C_BRG_cfg(MSSP_I2C_speed SCL_freq);
#endif
pin_config_t SDI_on = {
	PORTC_I,
	PIN4,
	GPIO_IN,
	GPIO_LOW
};
pin_config_t SDI_off = {
	PORTC_I,
	PIN4,
	GPIO_OUT,
	GPIO_LOW
};
pin_config_t SDO_on = {
	PORTC_I,
	PIN5,
	GPIO_OUT,
	GPIO_LOW
};
pin_config_t SDO_off = {
	PORTC_I,
	PIN5,
	GPIO_IN,
	GPIO_LOW
};
pin_config_t SCK_in = {
	PORTC_I,
	PIN3,
	GPIO_IN,
	GPIO_LOW
};
pin_config_t SCK_out = {
	PORTC_I,
	PIN3,
	GPIO_OUT,
	GPIO_LOW
};
pin_config_t SCL_out = {
	PORTC_I,
	PIN3,
	GPIO_OUT,
	GPIO_LOW
};
pin_config_t SCL_in = {
	PORTC_I,
	PIN3,
	GPIO_IN,
	GPIO_LOW
};
pin_config_t SDA_out = {
	PORTC_I,
	PIN4,
	GPIO_OUT,
	GPIO_LOW
};
pin_config_t SDA_in = {
	PORTC_I,
	PIN4,
	GPIO_IN,
	GPIO_LOW
};
#if(MSSP_CFG == MSSP_SPI)
/* @brief: Initialize the MSSP module regardless of the configuration.
 * @param: A pointer to a struct of type MSSP_SPI_t. 
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType MSSP_SPI_initialize(const MSSP_SPI_t * spi){
	STD_ReturnType ret = E_OK;
	if(NULL == spi)
		ret = E_NOT_OK;
	else{
/* Initialize the interrupt */
#if (INT_MSSP == INT_EN)
        INT_MSSP_init(spi -> priority); // spi -> priority is garbage if the feature is off
		INT_MSSP_set_callback_routine(spi -> MSSP_interrupt_handler);
#if (INT_PR == INT_EN)
		INT_MSSP_init(spi -> priority);
#elif(INT_PR == INT_DIS)
		INT_MSSP_init(1); //Garbage value
#endif
#endif
#if (MSSP_CFG == MSSP_SPI) 
		/* Configure SDO and SDI as serial pins */
		MSSP_SERIAL();
		/* Configure SDO as output and SDI and input */
		ret = ret & GPIO_pin_initialize(&SDO_on);
		ret = ret & GPIO_pin_initialize(&SDI_on);
		/* Configure sample time */
		switch(spi -> sample_time){
			case(middle):
				MSSP_SAMPLE_MID(); 
				break;
			case(end):
				MSSP_SAMPLE_END();
				break;
			default:
				ret = E_NOT_OK;
				break;
		}
#if (MSSP_SPI_MODE== MSSP_SPI_MASTER) 
		/* Configure clock source */
		switch(spi -> clk_src){
			case(TMR2_2):
				MSSP_TMR2_2();
				if(NULL == spi -> tmr2_clk)
					ret = E_NOT_OK;
				else
					TMR2_initialize(spi -> tmr2_clk);
				break;
			case(FOSC_64):
				MSSP_FOSC_64();
				break;
			case(FOSC_16):
				MSSP_FOSC_16();
				break;
			case(FOSC_4):
				MSSP_FOSC_4();
				break;
			default:
				ret = E_NOT_OK;
				break;
		} 
		/* Configure clock mode */	
		switch(spi -> clk_mode){
			case(0):	
				MSSP_CLK_MODE_0();
				break;
			case(1):	
				MSSP_CLK_MODE_1();
				break;
			case(2):	
				MSSP_CLK_MODE_2();
				break;
			case(3):	
				MSSP_CLK_MODE_3();
				break;
			default:	
				ret = E_NOT_OK; 
				break;
		}
		/* Set up clock line as output */
		ret = ret & GPIO_pin_initialize(&SCK_out);
#if (MSSP_SEND_ONLY == STD_OK) /* If the master receives only */
		ret = GPIO_pin_initialize(&SDO_off);
#endif		
#elif (MSSP_SPI_MODE== MSSP_SPI_SLAVE) 
		/* Set up clock line as input */
		ret = ret & GPIO_pin_initialize(&SCK_in);
		/* Set up SS pin */
		MSSP_SCK();
#else
		ret = E_NOT_OK;
#endif
		if(ret == E_OK)
			SPI_initialized=1; /* Indicate that SPI has been initialized */
#else
		ret = E_NOT_OK;
#endif
	}
	return ret;
}
/* @brief: Deinitialize the MSSP SPI module. 
 * @param: A pointer to a struct of type MSSP_SPI_t.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType MSSP_SPI_deinitialize(const MSSP_SPI_t * spi){
	STD_ReturnType ret = E_OK;
	if(NULL == spi || SPI_initialized == 0) /* You cannot deinitialize a module that hasn't be initialized */
		ret = E_NOT_OK;
	else{
		SPI_initialized=0;
		/* Deinitialize the interrupt */
		INT_MSSP_deinit();
		/* Reset control registers */
		SSPSTATbits.SMP=0;
		SSPSTATbits.CKE=0;
		SSPCON1 = 0x00;

		SPI_initialized=0;
	}
	return ret;
}

/* @brief: Write a byte to SDO line and receive a byte from SDI line. 
 * @param: A pointer to a struct of type MSSP_SPI_t
 * 		   A uint8 specifying the data to be sent
 * 		   A pointer to a uint8 for the received data 
 * @return: E_OK upon success
 * 		    E_NOT_OK otherwise
 */
STD_ReturnType MSSP_SPI_transfer(const MSSP_SPI_t * spi, uint8 tdata, uint8* rdata){
	STD_ReturnType ret = E_OK;
	if(spi == NULL || rdata == NULL)
		ret = E_NOT_OK;
	else{
		/* Loading into the SSPBUF starts the transfer */
		SSPBUF = tdata;
		/* Wait for transfer to complete */
		while(SPI_FLAG == 0);
		INT_MSSP_CLRF();
		/* Write the received bit */
		*rdata = SSPBUF;
	}
	return ret;
}

/* @brief: Write a byte to the SDO output. 
 * @param: A pointer to a struct of type MSSP_SPI_t and a uint8 holding the
 * 	       data to be transmitted.
 * @return: E_OK upon success and E_NOT_OK otherwise
 */
STD_ReturnType MSSP_SPI_write(const MSSP_SPI_t * spi, uint8 tdata){
	STD_ReturnType ret = E_OK;
	if(NULL == spi)
		ret = E_NOT_OK;
	else{
		/* Loading into the SSPBUF starts the transfer */
		SSPBUF = tdata;
		/* Wait for sending to complete */
		while(SPI_FLAG == 0);
		INT_MSSP_CLRF();
	}
	return ret;
}
/* @brief: Read a byte from the SDI input. 
 * @param: A pointer to a struct of type MSSP_SPI_t and a pointer to a 
 * 	       uint8 holding to hold the received data.
 * @return: E_OK upon success and E_NOT_OK otherwise
 */
STD_ReturnType MSSP_SPI_read(const MSSP_SPI_t * spi, uint8* rdata){
	STD_ReturnType ret = E_OK;
	if(spi == NULL || rdata == NULL)
		ret = E_NOT_OK;
	else{
		/* Load dummy data */
		SSPBUF = 0xFF;
		/* Wait for reception to complete */
		while(SPI_FLAG == 0);
		INT_MSSP_CLRF();
		/* Read content */
		*rdata = SSPBUF;
	}
	return ret;
}
/* @brief: Write a stream of bytes to the SDO line and read a stream of bytes from the SDI line.  
 * @param: A pointer to a struct of type MSSP_SPI_t.
 * 		   A pointer to a character array holding the transmitted bytes.
 * 		   A pointer to a character array to hold the received bytes. 
 * 		   A uint32 specifying the length of the byte stream.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType MSSP_SPI_transfer_buffer(const MSSP_SPI_t * spi, uint8* tdata, uint8* rdata, uint32 length){
	STD_ReturnType ret = E_OK;
	uint32 i = length;
	if(spi == NULL || rdata == NULL || tdata == NULL)
		ret = E_NOT_OK;
	else{
		while(i--){
			/* Loading into the SSPBUF starts the transfer */
			SSPBUF = *tdata++;
			/* Wait for transfer to complete */
			while(SPI_FLAG == 0);
			INT_MSSP_CLRF();
			/* Write the received bit */
			*rdata++ = SSPBUF;
		}
	}
	return ret;
}
/* @brief: Write a stream of bytes to the SDO line.  
 * @param: A pointer to a struct of type MSSP_SPI_t.
 * 		   A pointer to a character array holding the transmitted bytes.
 * 		   A uint32 specifying the length of the byte stream.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType MSSP_SPI_write_buffer(const MSSP_SPI_t * spi, uint8* tdata, uint32 length){
	STD_ReturnType ret = E_OK;
	uint32 i = length;
	if(spi == NULL || tdata == NULL)
		ret = E_NOT_OK;
	else{
		while(i--){
			/* Start the transfer */
			SSPBUF = *tdata++;
			/* Wait for transfer to complete */
			while(SPI_FLAG == 0);
			INT_MSSP_CLRF();
		}
	}
	return ret;
}
/* @brief: Read a stream of bytes from the SDI line.  
 * @param: A pointer to a struct of type MSSP_SPI_t.
 * 		   A pointer to a character array to hold the received bytes. 
 * 		   A uint32 specifying the length of the byte stream.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType MSSP_SPI_read_buffer(const MSSP_SPI_t * spi, uint8* rdata, uint32 length){
	STD_ReturnType ret = E_OK;
	uint32 i = length;
	if(spi == NULL || rdata == NULL)
		ret = E_NOT_OK;
	else{
		while(i--){
			/* Load dummy value into SSPBUF */ 
			SSPBUF = 0xFF;
			/* Wait for transfer to complete */
			while(SPI_FLAG == 0);
			INT_MSSP_CLRF();
			/* Write the received bit */
			*rdata++ = SSPBUF;
		}
	}
	return ret;
}
#elif(MSSP_CFG == MSSP_I2C)
/* @brief: Initialize the MSSP module for I2C mode. 
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType MSSP_I2C_initialize(const MSSP_I2C_t * i2c){
	STD_ReturnType ret = E_OK;
	if(NULL == i2c)
		ret = E_NOT_OK;	
	else{
		/* Set up serial pins */
		MSSP_SERIAL();
		/* Set up the interrupt */
#if (INT_MSSP == INT_EN)
        INT_MSSP_init(i2c -> priority); // spi -> priority is garbage if the feature is off
		INT_MSSP_set_callback_routine(i2c -> MSSP_interrupt_handler);
#if (INT_PR == INT_EN)
		INT_MSSP_init(i2c -> priority);
#elif(INT_PR == INT_DIS)
		INT_MSSP_init(1); //Garbage value
#endif
#endif
#if (MSSP_I2C_MODE == MSSP_I2C_MASTER)
		/* Set up master mode */
		MSSP_I2C_MASTER_MODE();
		/* Set up the clock */
		SSPADD = MSSP_I2C_BRG_cfg(i2c -> clk_speed);

#elif (MSSP_I2C_MODE == MSSP_I2C_SLAVE)
		/* Set the configuration for slave mode */
		if(i2c -> bit_mode == _7BIT)
			MSSP_7BIT();
		else if(i2c -> bit_mode == _10BIT)
			MSSP_10BIT();
		/* Set the address */
		SSPADD = (uint8)(i2c->address << 1);
		/* Set SCL and SDA as inputs */
		ret = gpio_pin_direction_initialize(&(SCL_in));
		ret = ret && gpio_pin_direction_initialize(&(SDA_in));
		/* Configure clock stretching */
		if(i2c -> clk_stretch == STD_ON)
			MSSP_CLK_STR_TR();
		else if(i2c -> clk_stretch == STD_OFF)
			MSSP_CLK_STR_T();
		else
			ret = E_NOT_OK;
#else
	ret = E_NOT_OK;
#endif
	if(ret == E_OK)
		I2C_initialized = 1;
	}
	return ret;
}
/* @brief: Deinitialize the MSSP I2C module.
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType MSSP_I2C_deinitialize(const MSSP_I2C_t * i2c){
	STD_ReturnType ret = E_OK;
	if(NULL == i2c || I2C_initialized == 0) /* You cannot deinitialize a module that hasn't be initialized */
		ret = E_NOT_OK;
	else{
		/* Deinitialize interrupts */
		ret = INT_MSSP_deinit();
		/* Reset control registers */
		SSPSTATbits.SMP=0;
		SSPSTATbits.CKE=0;
		SSPCON1 = 0x00;
		SSPCON2 = 0x00;

		I2C_initialized = 0;
	}
	return ret;
}
#if(MSSP_I2C_MODE == MSSP_I2C_MASTER)
/* @brief: Send a START signal to start the transfer.
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * @return: E_OK upon success.
 * 			E_BUS_COLL upon bus collision.
 * 			E_NOT_OK otherwise.
 * @note: Works only on master mode.
 * 	      A START condition is driving SDA LOW when SCL is HIGH
 */
STD_ReturnType MSSP_I2C_send_START(void){
	STD_ReturnType ret = E_OK;
	if(I2C_initialized == 0)
		ret = E_NOT_OK;
	else{
		/* Check and wait for the bus to be IDLE */
		while(RWB || S_COND || RS_COND || P_COND || ACK_COND || REC_EN);
		/* Clear interrupt flag */
		INT_MSSP_CLRF();
		/* Initiate a START condition on SDA and SCL pins (cleared by HW) */
		MSSP_I2C_SEND_START();
		while(SSPCON2bits.SEN == 1);		
		/* START condition should assert the interrupt flag */
		if(INT_MSSP_F==1)
			INT_MSSP_CLRF();
		else
			ret = E_NOT_OK;
		/* Check for bus collision */
		if(PIR2bits.BCLIF == 1){
			ret = E_BUS_COLL;
			PIR2bits.BCLIF=0;
		}
	}
	return ret;
}
/* @brief: Send a repeated START signal.
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * @return: E_OK upon success.
 * 			E_BUS_COLL upon bus collision.
 * 			E_WR_COLL upon SSPBUF write collision.
 * 			E_NOT_OK otherwise.
 * @note: Works only on master mode.
 */
STD_ReturnType MSSP_I2C_send_rSTART(void){
	STD_ReturnType ret = E_OK;
	if(I2C_initialized == 0)
		ret = E_NOT_OK;
	else{
		/* Check if the device is busy */
		while(ACK_COND || S_COND || RS_COND || P_COND || REC_EN);
		/* Clear interrupt flag */
		INT_MSSP_CLRF();
		/* Initiate a repeated START condition on SDA and SCL pins (cleared by HW) */
		MSSP_I2C_SEND_RSTART();
		while(SSPCON2bits.RSEN == 1);		
		/* START condition should assert the interrupt flag */
		if(INT_MSSP_F==1)
			INT_MSSP_CLRF();
		else
			ret = E_NOT_OK;
		/* A write collision happens when the SSPBUF is updated 
           in a repeated START condition */ 
		if(SSPCON1bits.WCOL==1)
			ret = E_WR_COLL;
		/* Check for bus collision */
		if(PIR2bits.BCLIF == 1){
			ret = E_BUS_COLL;
			PIR2bits.BCLIF=0;
		}
	}
	return ret;
}
/* @brief: Send a STOP signal.
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * @return: E_OK upon success.
 * 			E_BUS_COLL upon bus collision.
 * 			E_NOT_OK otherwise.
 * @note: Works only on master mode.
 */
STD_ReturnType MSSP_I2C_send_STOP(void){
	STD_ReturnType ret = E_OK;
	if(I2C_initialized == 0)
		ret = E_NOT_OK;
	else{
		/* Check and wait for the bus to be IDLE */
		while(RWB || S_COND || RS_COND || P_COND || ACK_COND || REC_EN);
		/* Clear interrupt flag */
		INT_MSSP_CLRF();
		/* Initiate a START condition on SDA and SCL pins (cleared by HW) */
		MSSP_I2C_SEND_STOP();
		while(SSPCON2bits.PEN == 1);		
		/* START condition should assert the interrupt flag */
		if(INT_MSSP_F==1)
			INT_MSSP_CLRF();
		else
			ret = E_NOT_OK;
		/* Check for bus collision */
		if(PIR2bits.BCLIF == 1){
			ret = E_BUS_COLL;
			PIR2bits.BCLIF=0;
		}
	}
	return ret;
}
#endif
/* @brief: Read a byte from the SDA bus. This can either be a data 
 * 		   byte or an address byte. 
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * 		   A pointer to a uint8 to hold the received byte.
 * 		   A uint8 to hold the acknowledgment bit: (Useful only in master reception mode)
 * 		   1 -> send NACK (End reception)
 * 		   0 -> send ACK (Continue reception)
 * @return: E_OK upon success.
 * 			E_BUS_COLL upon bus collision.
 * 			E_NOT_OK otherwise.
 * @note: The (ack_bit) is only used in master reception mode.
 */
STD_ReturnType MSSP_I2C_read_byte(uint8* read, uint8 ack_bit){
	STD_ReturnType ret = E_OK;
	if(I2C_initialized == 0 || (ack_bit != 0 && ack_bit != 1))
		ret = E_NOT_OK;
	else{
#if(MSSP_I2C_MODE == MSSP_I2C_MASTER)
#if (MSSP_SEND_ONLY == STD_OFF)  
		/* Enable RCEN bit */
		MSSP_I2C_REN();
		/* Wait for reception to complete */
		while(!BUFFER_FULL);	
		/* Read data */
		*read = SSPBUF;
		/* Clear the interrupt flag */
		INT_MSSP_CLRF();
		/* Send acknowledgment */
		MSSP_I2C_SEND_ACK_BIT(ack_bit);
		MSSP_I2C_SEND_ACK();
		/* Wait for ACK condition to finish */
		while(ACK_COND);	
#elif(MSSP_I2C_MODE == MSSP_I2C_SLAVE)
		/* Wait for the interrupt */
		while(!I2C_FLAG);
		INT_MSSP_CLRF();
		/* Read the data */
		if(I2C_D_OR_A == 1 && I2C_R_OR_W==0) /* Last byte was data and it was written by master */
			*read = SSPBUF; /* Valid data byte */
		else
			uint8 dummy = SSPBUF; /* Dummy read to clear the BF bit */
			*read = 0xFF; /* Dummy value */
		/* Release the clock */
		MSSP_CLK_REL();
#endif
		/* Check for bus collision */
		if(PIR2bits.BCLIF == 1){
			ret = E_BUS_COLL;
			PIR2bits.BCLIF=0;
		}
	}
	return ret;
}
#endif
/* @brief: Write a byte to the SDA bus. This can either be a data 
 * 		   byte or an address byte. 
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * 		   A pointer to a uint8 to hold the received byte.
 * @return: E_OK upon success.
 * 			E_BUS_COLL upon bus collision.
 * 			E_NOT_OK otherwise.
 */
STD_ReturnType MSSP_I2C_write_byte(uint8 write){
	STD_ReturnType ret = E_OK;
	if(I2C_initialized == 0)
		ret = E_NOT_OK;
	else{
#if (MSSP_I2C_MODE == MSSP_I2C_MASTER)
		/* Write the data */
		SSPBUF = write;
		/* Wait for transfer to complete */
		while(!I2C_FLAG);
		INT_MSSP_CLRF();
		/* Check for ACK */
		if(ACKSTAT == 0) /* Transmit complete */
			ret = E_OK;
		else if(ACKSTAT == 1) /* Transmit not successful */
			ret = E_NOT_OK;
#elif (MSSP_I2C_MODE == MSSP_I2C_SLAVE)
		/* Write the data */
		SSPBUF = write;
		/* Wait for transfer to complete */
		while(!I2C_FLAG);
		INT_MSSP_CLRF();
#endif
	}
	return ret;
}

#if (MSSP_I2C_MODE == MSSP_I2C_MASTER)
/* @brief: Write a data stream to a slave.
 * 		   (START -> address -> data -> ... -> STOP)
 * @param: A pointer to a struct of type MSSP_I2C_t. 
 * 		   A uint16 holding either an 7 bit address or a 10 bit address.
 * 		   A uint8 array holding data to be written.
 * 		   A uint8 holding the length of data to be written.
 * @return: E_OK upon success.
 * 			E_BUS_COLL upon bus collision.
 * 			E_NOT_OK otherwise.
 * @note: The address parameter is seven bits from the LSB.
 */
STD_ReturnType MSSP_I2C_MasterWrite(const MSSP_I2C_t * i2c, uint16 addr, const uint8 *data, uint8 len){
	STD_ReturnType ret = E_OK;
	uint8 ACK_bit=0,i=0;
	if(i2c == NULL || data == NULL || I2C_initialized == 0)
		ret = E_NOT_OK;
	else{
		/* Send START condition */
		ret = MSSP_I2C_send_START();
		if(ret == E_NOT_OK || ret == E_BUS_COLL)
			return ret;
		/* Send address */
		if(i2c -> bit_mode == _7BIT){
			ret = MSSP_I2C_write_byte((addr << 1) | 0);	
		}
		else if(i2c -> bit_mode == _10BIT){
			/* Send first address byte */
			ret = MSSP_I2C_write_byte(ADDR_WR_SEND((uint8)addr >> 8));
			/* Send second address byte */
			if(ret == E_OK)
				ret = MSSP_I2C_write_byte((uint8)(addr & 0x00FF));
		}
		/* Send data */
		for(i=0;i<len;i++){
			ret = MSSP_I2C_write_byte(data[i]);
			if(ACKSTAT)
				return E_NOT_OK;	
		}
		/* Send STOP condition */
		ret = MSSP_I2C_send_STOP();
	}
	return ret;
}
/* @brief: Read a stream of data as a master. 
 * 		   START -> address -> read data -> ... -> STOP
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * 		   A uint16 holding either a 7 bit address or a 10 bit address. 
 * 		   A pointer to a uint8 array holding the read data.
 * 		   A uint8 holding the amount of data to be read.
 * @return: E_OK upon success.
 * 			E_BUS_COLL upon bus collision.
 * 			E_NOT_OK otherwise.
 * @note: The address parameter is seven bits from the LSB.
 */
STD_ReturnType MSSP_I2C_MasterRead(const MSSP_I2C_t* i2c, uint16 addr, uint8 *data, uint8 len){
	STD_ReturnType ret = E_OK;
	uint8 ACK_bit=0,i=0;
	if(i2c == NULL || data == NULL || I2C_initialized == 0)
		ret = E_NOT_OK;
	else{
		/* Send START condition */
		ret = MSSP_I2C_send_START();
		if(ret == E_NOT_OK || ret == E_BUS_COLL)
			return ret;
		/* Send address */
		if(i2c -> bit_mode == _7BIT){
			ret = MSSP_I2C_write_byte(( ((uint8)addr) << 1) | 1);	
		}
		else if(i2c -> bit_mode == _10BIT){
			/* Send first address byte */
			ret = MSSP_I2C_write_byte(ADDR_WR_SEND((uint8)addr >> 8));
			/* Send second address byte */
			if(ret == E_OK)
				ret = MSSP_I2C_write_byte((uint8)(addr & 0x00FF));
			/* Send repeated START */
			if(ret == E_OK)
				ret = MSSP_I2C_send_rSTART();
			/* Send first part of the address with R/W = 1 */
			if (ret == E_OK)
				ret = MSSP_I2C_write_byte(ADDR_WR_REC((uint8)addr >> 8));
		}
		/* Read data */
		for(i=0;i<len-1;i++){
			ret = MSSP_I2C_read_byte(&data[i], 0);
			if(ACKSTAT)
				return E_NOT_OK;	
		}
		/* Read last byte */
			ret = MSSP_I2C_read_byte(&data[i], 1);
		/* Send STOP condition */
		ret = MSSP_I2C_send_STOP();
	}
	return ret;
}

/* @brief: Write wlen bytes and then receive rlen bytes as master. 
 * 		   START -> addr -> send data -> ... -> rSTART -> read data -> ... -> STOP
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * 		   A uint16 holding either a 7 bit address or a 10 bit address. 
 * 		   A pointer to a uint8 array holding the write data.
 * 		   A uint8 holding the amount of data to be written.
 * 		   A pointer to a uint8 array holding the read data.
 * 		   A uint8 holding the amount of data to be read.
 * @return: E_OK upon success.
 * 			E_BUS_COLL upon bus collision.
 * 			E_NOT_OK otherwise.
 * @note: The address parameter is seven bits from the LSB.
 */
STD_ReturnType MSSP_I2C_MasterWriteRead(const MSSP_I2C_t* i2c, uint16 addr, const uint8 *wdata, uint8 wlen, uint8 *rdata, uint8 rlen){
	STD_ReturnType ret = E_OK;
	uint8 i=0;
	if(wdata == NULL || rdata == NULL || I2C_initialized == 0)
		ret = E_NOT_OK;
	else{
		/* Send START condition */
		ret = MSSP_I2C_send_START();
		if(ret == E_NOT_OK || ret == E_BUS_COLL)
			return ret;
		/* Send address */
		if(i2c -> bit_mode == _7BIT){
			ret = MSSP_I2C_write_byte(( ((uint8)addr) << 1) | 0);	
		}
		else if(i2c -> bit_mode == _10BIT){
			/* Send first address byte */
			ret = MSSP_I2C_write_byte(ADDR_WR_SEND((uint8)addr >> 8));
			/* Send second address byte */
			if(ret == E_OK)
				ret = MSSP_I2C_write_byte((uint8)(addr & 0x00FF));
		}
		/* Send data */
		for(i=0;i<wlen;i++){
			ret = MSSP_I2C_write_byte(wdata[i]);
			if(ACKSTAT)
				return E_NOT_OK;	
		}
		/* Send repeated START */
		ret = MSSP_I2C_send_rSTART();
		if(ret == E_NOT_OK || ret == E_BUS_COLL)
			return ret;
		/* Send address */ 
		if(i2c -> bit_mode == _7BIT){
			ret = MSSP_I2C_write_byte(( ((uint8)addr) << 1) | 1);	
		}
		else if(i2c -> bit_mode == _10BIT){
			/* Send first address byte */
			ret = MSSP_I2C_write_byte(ADDR_WR_REC((uint8)addr >> 8));
		}
		/* Read data */
		for(i=0;i<rlen-1;i++){
			ret = MSSP_I2C_read_byte(&rdata[i], 0);
			if(ACKSTAT)
				return E_NOT_OK;	
		}
		/* Read last byte */
			ret = MSSP_I2C_read_byte(&rdata[i], 1);
		/* Send STOP condition */
		ret = MSSP_I2C_send_STOP();
	}
	return ret;
}

/* @brief:(H) Calculate the Baud rate generator (BRG) value given  
 * 	      the intended SCL frequency.  
 * @param: A type MSSP_I2C_speed specifying all possible frequencies
 * @ref: mcal_mssp.h -> MSSP_I2C_speed 
 * @return: The BRG_val generated.
 */
uint8 MSSP_I2C_BRG_cfg(MSSP_I2C_speed SCL_freq){
	STD_ReturnType ret = E_OK;
        uint8 BRG_val;
	BRG_val = (_XTAL_FREQ / (SCL_freq * 4.0)) - 1.0; 		
	return ((uint8) BRG_val);	
}

#elif(MSSP_I2C_MODE == MSSP_I2C_SLAVE)
/* @brief: Read a byte as a slave from a master.
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * 	       A pointer to a uint8 to hold the read data.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType MSSP_I2C_SlaveRead(const MSSP_I2C_t* i2c, uint8* read){
	STD_ReturnType ret = E_OK;
	int dummy;
    if (NULL == i2c || NULL == read)
        return E_NOT_OK;
    // Wait for flag (Address or Data received)
    while (!I2C_FLAG);
    I2C_FLAG = 0;
    if (!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
        // MASTER Writes → Master sends address 
        // Address + Write
        dummy = SSPBUF;      // Clear BF
        SSPCON1bits.CKP = 1;         // Release clock
    }
    else if (SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
        // Data received from master
        *read = SSPBUF;              // Read byte
        SSPCON1bits.CKP = 1;         // Release clock
    }
    // Handle overflow/collision
    if (SSPCON1bits.SSPOV || SSPCON1bits.WCOL) {
        dummy = SSPBUF;
        SSPCON1bits.SSPOV = 0;
        SSPCON1bits.WCOL = 0;
        SSPCON1bits.CKP = 1;
    }
    return E_OK;
}

/* @brief: Read a byte as a slave from a master.
 * @param: A pointer to a struct of type MSSP_I2C_t.
 * 	       A pointer to a uint8 to hold the read data.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType MSSP_I2C_SlaveWrite(const MSSP_I2C_t* i2c, uint8 write){
	int dummy;
	STD_ReturnType ret = E_OK;
    if (NULL == i2c || NULL == read)
        return E_NOT_OK;
    // Wait for flag (Address or Data received)
    while (!I2C_FLAG);
    I2C_FLAG = 0;
    if (!SSPSTATbits.D_nA && SSPSTATbits.R_nW) {
        // MASTER READ → Master requests data
        dummy = SSPBUF; // Clear BF
        SSPBUF = write;      // Load data to send
        SSPCON1bits.CKP = 1;             // Release clock
    }
    else if (SSPSTATbits.D_nA && SSPSTATbits.R_nW) {
        // MASTER READ → Data sent, prepare next byte if needed
        if (!SSPSTATbits.BF)
            SSPBUF = write; // Optional: stream more data
        SSPCON1bits.CKP = 1;
    }
    // Handle overflow/collision if any
    if (SSPCON1bits.SSPOV)
		ret = E_BUS_COLL;
	else if(SSPCON1bits.WCOL)
		ret = E_WR_COLL;
    dummy = SSPBUF;
    SSPCON1bits.SSPOV = 0;
    SSPCON1bits.WCOL = 0;
    SSPCON1bits.CKP = 1;
	}
	return ret;
}
#endif
#endif
#endif
