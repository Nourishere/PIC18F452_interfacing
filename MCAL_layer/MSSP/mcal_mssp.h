/* 
 * File:   mcal_mssp.h
 * Author: nour
 *
 * Created on September 9, 2025, 7:07 PM
 * @Brief: This file contains header configurations for the MSSP module.
 * 	       The MSSP module can work as an SPI module or as an I2C module.
 */

#ifndef MCAL_MSSP_H
#define	MCAL_MSSP_H

/****************** include **********************/
#include "mcal_mssp_cfg.h"
#include "../GPIO/hal_gpio.hk"
#include "../Timer2/hal_tmr2.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../Interrupt/mcal_interrupt_config.h"

/***************** macros  **********************/
#define BUFFER_FULL SSPSTATbits.BF
#define SPI_FLAG PIR1bits.SSPIF
#define I2C_FLAG PIR1bits.SSPIF
#define RWB SSPSTATbits.R_NOT_W
#define S_COND SSPCON2bits.SEN
#define RS_COND SSPCON2bits.RSEN
#define P_COND SSPCON2bits.PEN
#define ACK_COND SSPCON2bits.ACKEN
#define REC_EN SSPCON2bits.RCEN
#define I2C_D_OR_A SSPSTATbits.D_nA
#define I2C_R_OR_W SSPSTATbits.R_nW

/**************** function macros **************/
/* Global configurations */
/* @note: Setting the pin as serial requires input/output configuration */
#define MSSP_SERIAL() (SSPCON1bits.SSPEN=1)
#define MSSP_PIN_IO() (SSPCON1bits.SSPEN=0)

/* Configurations for the SPI module */ 
#if (MSSP_CFG == MSSP_SPI)  
/* SPI clock configuration */
#if (MSSP_SPI_MODE == MSSP_SPI_SLAVE)
/* Set the clock cfg based on SS* pre-cfg */
#if (MSSP_SSB == STD_ON)
#define MSSP_SCK() (SSPCON1bits.SSPM=0x0100) 
#elif (MSSP_SSB == STD_OFF)
#define MSSP_SCK() (SSPCON1bits.SSPM=0b0101) 
#endif
#elif (MSSP_SPI_MODE == MSSP_SPI_MASTER)
/* @note: _x means divided by x */
#define MSSP_TMR2_2() (SSPCON1bits.SSPM=0b0011)
#define MSSP_FOSC_64() (SSPCON1bits.SSPM=0b0010)
#define MSSP_FOSC_16() (SSPCON1bits.SSPM=0b0001)
#define MSSP_FOSC_4() (SSPCON1bits.SSPM=0b0000)
#endif
/* Sample takes part in the middle of data output time */
#define MSSP_SAMPLE_END() (SSPSTATbits.SMP=1)
/* Sample takes part in the end of data output time */
#define MSSP_SAMPLE_MID() (SSPSTATbits.SMP=0)
/* Set up SPI clock mode */
/* @note: The SPI protocol defines three modes for the serial clock
 * 0: The IDLE state is LOW and data changes on the rising edge
 * 1: The IDLE state is LOW and data changes on the falling edge
 * 2: The IDLE state is HIGH and data changes on the falling edge
 * 3: The IDLE state is HIGH and data changes on the rising edge
 */
/* @note: The terms defined in the protocol are Clock Phase (CPHA) and Clock polarity (CPOL) */
#define MSSP_CLK_MODE_0() \
				 do{ \
					(SSPSTATbits.CKE=0); \
					(SSPCON1bits.CKP=0); \
					} \
				while(0)

#define MSSP_CLK_MODE_1() \
				 do{ \
					(SSPSTATbits.CKE=1); \
					(SSPCON1bits.CKP=0); \
					} \
				while(0)

#define MSSP_CLK_MODE_2() \
				 do{ \
					(SSPSTATbits.CKE=0); \
					(SSPCON1bits.CKP=1); \
					} \
				while(0)

#define MSSP_CLK_MODE_3() \
				 do{ \
					(SSPSTATbits.CKE=1); \
					(SSPCON1bits.CKP=1); \
					} \
				while(0)

/* Configurations for the I2C module */ 
#elif (MSSP_CFG == MSSP_I2C)
#if(MSSP_I2C_MODE == MSSP_I2C_SLAVE)
/* Hold and release the clock line (clock stretching) */
#define MSSP_CLK_STR() (SSPCON1bits.CKP=0)
#define MSSP_CLK_REL() (SSPCON1bits.CKP=1)
/* Enable clock stretch for Tx only or Tx and Rx */
#define MSSP_CLK_STR_T() (SSPCON2bits.SEN=0)
#define MSSP_CLK_STR_TR() (SSPCON2bits.SEN=1)
/* Select 7/10 modes */
#if (MSSP_I2C_SSI == STD_OFF)
#define MSSP_7BIT() (SSPCON1bits.SSPM=0b0110)
#define MSSP_10BIT() (SSPCON1bits.SSPM=0b0111)
#elif (MSSP_I2C_SSI == STD_ON) /* Interrupts upon START/STOP bits are enabled */
#define MSSP_7BIT() (SSPCON1bits.SSPM=0b1110)
#define MSSP_10BIT() (SSPCON1bits.SSPM=0b1111)
/* Enable general address call */
#define MSSP_GAC_EN() (SSPCON2bits.GCEN=1)
#define MSSP_GAC_DIS() (SSPCON2bits.GCEN=0)
#endif
#elif(MSSP_I2C_MODE == MSSP_I2C_MASTER)
/* Write first address byte in 10 bit address mode */
/* @note: This macro takes the two MSBs (A9,A8) of the address and the input */
/* One macro is used to write an address with R/W cleared (master sends) 
 * The other is has R/W set to receive (master receives) */
#define ADDR_WR_SEND(x) (0xF0 | ((x & 0x03) << 1))
#define ADDR_WR_REC(x) (0xF1 | ((x & 0x03) << 1))
#if (MSSP_SEND_ONLY == STD_OFF)
/* Send ACK sequence */
#define MSSP_I2C_SEND_ACK() (SSPCON2bits.ACKEN=1)
#define MSSP_I2C_SEND_ACK_BIT(x) (SSPCON2bits.ACKDT=x)
#endif
/* Configure I2C as master */
/* @note: Clock = Fosc/((1+SSPADD)*4) */
#define MSSP_I2C_MASTER_MODE() (SSPCON1bits.SSPM=0b1000)
/* Enable master receive */
#define MSSP_I2C_REN() (SSPCON2bits.RCEN=1)
#define MSSP_I2C_RDIS() (SSPCON2bits.RCEN=0)
/* Send START bit */
#define MSSP_I2C_SEND_START() (SSPCON2bits.SEN=1)
/* Send STOP bit */
#define MSSP_I2C_SEND_STOP() (SSPCON2bits.PEN=1)
/* Send repeated START */
#define MSSP_I2C_SEND_RSTART() (SSPCON2bits.RSEN=1)
#endif
#endif

/*************** data types *******************/
#if (MSSP_CFG == MSSP_SPI)
/* @note: The time at which input data is sampled */
typedef enum{
	middle,
	end
}MSSP_SPI_SAMPLE_T;

typedef enum{
	TMR2_2,
	FOSC_64,	
	FOSC_16,	
	FOSC_4	
}MSSP_SPI_CLK_SRC;

typedef struct{
#if (INT_MSSP == INT_EN)
	void (*MSSP_interrupt_handler) (void); // Use NULL if not needed
#if (INT_PR == INT_EN)
	uint8 priority; /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	MSSP_SPI_SAMPLE_T sample_time;
#if (MSSP_SPI_MODE == MSSP_SPI_MASTER) /* Only available in master mode */
	uint8 clk_mode; /* SPI clock mode according to the standard */
	MSSP_SPI_CLK_SRC clk_src;	
	TMR2_t* tmr2_clk; /* use NULL if not used */
#endif
}MSSP_SPI_t;
#elif (MSSP_CFG == MSSP_I2C)
/* @note: The MSSP I2C is hardcoded to fire interrupts upon 
 * 		  START and STOP bits in slave mode for convinence. 
 */
typedef enum{
    I2C_OK = 0,          // Transmission/reception successful
    I2C_NACK,            // NACK received
    I2C_BUS_COLLISION,   // Bus collision detected
    I2C_OVERRUN,         // Receive overflow (slave mode)
    I2C_TIMEOUT,         // (optional) No response within time
    I2C_ERROR            // General/unknown error
}I2C_Status;
typedef enum{
	_7BIT,
	_10BIT,
}MSSP_I2C_smode;
typedef enum{
	_100K = 100000,
	_308K = 308000,
	_312_5K = 312500,
	_333K = 333000,
	_400K = 400000,
	_1M = 1000000
}MSSP_I2C_speed;
typedef struct{
#if (INT_MSSP == INT_EN)
	void (*MSSP_interrupt_handler) (void); // Use NULL if not needed
#if (INT_PR == INT_EN)
	uint8 priority; /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	MSSP_I2C_smode bit_mode;
#if (MSSP_I2C_MODE == MSSP_I2C_SLAVE)
	uint8 address;
	/* STD_ON -> clk stretch is for Tx and Rx
       STD_OFF -> clk stretch is for Tx only */
	uint8 clk_stretch; /* Use STD_ON or STD_OFF */
#elif (MSSP_I2C_MODE == MSSP_I2C_MASTER)
	MSSP_I2C_speed clk_speed;
#endif
}MSSP_I2C_t;

#endif

/************** function prototypes **********/
#if (MSSP_CFG == MSSP_SPI)
STD_ReturnType MSSP_SPI_initialize(const MSSP_SPI_t * spi);
STD_ReturnType MSSP_SPI_deinitialize(const MSSP_SPI_t * spi);
STD_ReturnType MSSP_SPI_transfer(const MSSP_SPI_t * spi, uint8 tdata, uint8* rdata);
STD_ReturnType MSSP_SPI_write(const MSSP_SPI_t * spi, uint8 tdata);
STD_ReturnType MSSP_SPI_read(const MSSP_SPI_t * spi, uint8* rdata);
STD_ReturnType MSSP_SPI_transfer_buffer(const MSSP_SPI_t * spi, uint8* tdata, uint8* rdata, uint32 length);
STD_ReturnType MSSP_SPI_write_buffer(const MSSP_SPI_t * spi, uint8* tdata, uint32 length);
STD_ReturnType MSSP_SPI_read_buffer(const MSSP_SPI_t * spi, uint8* rdata, uint32 length);
#if (MSSP_SPI_MODE == MSSP_SPI_MASTER)
STD_ReturnType MSSP_SPI_select_slave(const MSSP_SPI_t * spi, uint8 ss_index);
#endif

#elif (MSSP_CFG == MSSP_I2C)
STD_ReturnType MSSP_I2C_initialize(const MSSP_I2C_t * i2c);
STD_ReturnType MSSP_I2C_deinitialize(const MSSP_I2C_t * i2c);
STD_ReturnType MSSP_I2C_send_START(void);
STD_ReturnType MSSP_I2C_send_rSTART(void);
STD_ReturnType MSSP_I2C_send_STOP(void);
STD_ReturnType MSSP_I2C_read_byte(uint8* read, uint8 ack_bit);
STD_ReturnType MSSP_I2C_write_byte(uint8 write);

STD_ReturnType MSSP_I2C_MasterWrite(const MSSP_I2C_t * i2c, uint16 addr, const uint8 *data, uint8 len);
STD_ReturnType MSSP_I2C_MasterRead(const MSSP_I2C_t* i2c, uint16 addr, uint8 *data, uint8 len);
STD_ReturnType MSSP_I2C_MasterWriteRead(const MSSP_I2C_t* i2c, uint16 addr, const uint8 *wdata, uint8 wlen, uint8 *rdata, uint8 rlen);
#endif
#endif	/* MCAL_MSSP_H */

