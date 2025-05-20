/* 
 * File:   mcal_eeprom.c
 * Author: nour
 *
 * Created on April 29, 2025, 2:29 PM
 */

#include "mcal_eeprom.h"

/* @Brief: Read data from the EEPROM.
 * @Param: Addr stores the 1-byte long address, and a pointer to a 
 * 		   a uint8 data stores the read data.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType EEPROM_read(uint8 addr, uint8* data){
	STD_ReturnType ret = E_OK;	
	if(NULL == data || addr > 0xFF)
		ret = E_NOT_OK;
	else{
		EEPROM_WR_ADDR(addr);
		EEPROM_DATA_ACCESS();
		EEPROM_FLAEEP_DATACC();
		EEPROM_ALLOW_RD_CYLES();
		/* wait two instuctions for reading */
		NOP();
		NOP();
		*data = EEDATA; 
	}
	return ret;
}

/* @Brief: Write data to the EEPROM.
 * @Param: uint8 Addr stores the 1-byte long address, and 
 * 		   uint8 data stores the read data.
 * @Return: E_OK upon success and E_NOT_OK otherwise.
 */
STD_ReturnType EEPROM_write(uint8 addr, uint8 data){
	STD_ReturnType ret = E_OK; 
	uint8 flh=0;uint8 fll=0;uint8 flg=0;uint8 flp=0;
	if(addr > 0xFF)
		ret = E_NOT_OK;
	else{
           /* Disable priority */ 
#if (INT_PR == INT_EN)
		if(INTCON1bits.GIEH==1){
			INT_GHPDIS(); 
			flh=1;
		}
		if(INTCON1bits.GIEL==1){
			INT_GLPDIS(); 
			fll=1;
		}
#else
		if(INTCON1bits.GIE==1){
			INT_GDIS();
			flg=1;
		}
		if(INTCON1bits.PEIE==1){
        	INT_PEDIS();
			flp=1;
		}
#endif
		EEPROM_WR_ADDR(addr);
		EEPROM_WR_DATA(data);
		EEPROM_DATA_ACCESS();
		EEPROM_FLAEEP_DATACC();
		EEPROM_ALLOW_WR_CYCLES();
		EECON2_WR(0x55);
		EECON2_WR(0xAA);
		EEPROM_WR_EN();
		/* Wait until writing is finished */
		for(;EECON1bits.WR;){
			}
        /* Restore back priority */ 
#if (INT_PR == INT_EN)
		if(flh){
			INT_GHPEN(); 
		}
		if(fll){
			INT_GLPEN(); 
		}
#else
		if(flg)
        	INT_GEN();
		if(flp)
        	INT_PEEN();
#endif

	}

	return ret;
}

/* @Brief: Clear the content of the EEPROM 
 * @Param: void.
 * @Return: Returns E_OK if erase is successful and E_NOT_OK otherwise. 
 */
STD_ReturnType EEPROM_erase(void){
	STD_ReturnType ret = E_OK;
	uint8 i = 0;uint8 *data = NULL;
	while(i <= 255){
		ret = ret && EEPROM_write(i,0xFF);
		i++;
	}
	return ret;
}
