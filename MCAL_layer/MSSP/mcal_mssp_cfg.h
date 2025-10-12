/* 
 * File:   mcal_mssp_cfg.h
 * Author: nour
 *
 * Created on September 9, 2025, 7:07 PM
 * @Brief: This file contains pre-configurations for the MSSP module.
 * 	       The MSSP module can work as an SPI module or as an I2C module.
 */

#ifndef MCAL_MSSP_CFG_H
#define	MCAL_MSSP_CFG_H
#include "../mcal_std_types.h"

#define MSSP_MODULE STD_OFF /* Turn the whole off */
/* @note: As for the purposes for such driver, the MSSP module is pre-configured to utilize
 *        either the SPI protocol or the I2C protocol. Multiplexing such features is difficult and 
 *        might prove inconsistent.
 */
#define MSSP_SPI 0
#define MSSP_I2C 1
#define MSSP_SPI_MASTER 1
#define MSSP_SPI_SLAVE 0
#define MSSP_I2C_MASTER 1
#define MSSP_I2C_SLAVE 0
/* @note: The SS* pin can be used as a chip select (CS*) for the MSSP module working in SPI as a slave
 * 		  or it can be ignored by the MSSP module and used as a normal GPIO.
 * 		  The hardware checks automatically for the SS* pin to decide whether the SPI slave should listen
 * 		  or not.  
 */
#define MSSP_CFG MSSP_I2C/* use MSSP_SPI or MSSP_I2C */

#if (MSSP_CFG == MSSP_SPI)
#define MSSP_SPI_MODE MSSP_SPI_MASTER /* use MSSP_SPI_SLAVE or MSSP_SPI_MASTER */
#define MSSP_SSB STD_ON /* use STD_OFF or STD_ON */
#if (MSSP_SPI_MODE == MSSP_SPI_MASTER)
#define MSSP_RECEIVE_ONLY STD_OFF /* use STD_ON or STD_OFF */
#endif
#elif (MSSP_CFG == MSSP_I2C)
#define MSSP_I2C_MODE MSSP_I2C_MASTER
#define MSSP_I2C_GEN_ADD STD_ON /* General address call feature is enabled */
#define MSSP_I2C_CLKS STD_ON /* Clock stretching is enabled (slave mode only) */
#if (MSSP_I2C_MODE == MSSP_I2C_SLAVE)
/* @note: Interrupts upon START and STOP bits are always on in master mode
 * 		  but can be configured for the slave mode
 */
#define MSSP_I2C_SSP STD_ON /* MSSP I2C Start Stop Interrupts */
#endif
#endif
#endif	/* MCAL_MSSP_CFG_H */

