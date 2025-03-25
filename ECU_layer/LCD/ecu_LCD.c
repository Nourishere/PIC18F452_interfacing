/* 
 * File:   ecu_LCD.c
 * Author: nour
 *
 * Created on March 10, 2025, 11:08 AM
 */
#include "ecu_LCD.h"

/* Note on March 25, 2025, 23:56:52.
   (This driver is still under testing and debugging.)
*/

/*@brief: check the accessed ports and pins of the lcd
 *@param: Takes a pointer to an lcd of type chr_LCD_t 
 *@return: Returns E_OK if access is validated and E_NOT_OK otherwise
 */       
static STD_ReturnType lcd_check_access(const chr_LCD_t *lcd){
	STD_ReturnType ret = E_OK;
	uint8 i;
	if((E_NOT_OK == GPIO_check_access(&(lcd -> lcd_rs))) || (E_NOT_OK == GPIO_check_access(&(lcd -> lcd_en)))){
		ret = E_NOT_OK;
	}
	else{
		if(lcd -> bit_mode == _4BIT_MODE){
			for(i=0;i<4;i++){
				ret = GPIO_check_access(&(lcd -> lcd_data[i+4]));
				if (ret == E_NOT_OK){ 
					return E_NOT_OK;
                }
				else{}
			}
		} 
		else if(lcd -> bit_mode == _8BIT_MODE){
			for(i=0;i<8;i++){
				ret = GPIO_check_access(&(lcd -> lcd_data[i]));
				if (ret == E_NOT_OK){ 
					return E_NOT_OK;
                }
				else{}
			}
		}
		else
			ret = E_OK;
	}
	return ret;
}

/*@brief: Intializes an LCD by setting the register select, enable, and data pins.
 *@param: Takes a pointer to an lcd of type chr_LCD_t.
 *@return: Returns E_OK if intializtion is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType lcd_initialize(const chr_LCD_t * lcd){
	STD_ReturnType ret = E_OK;
	uint8 i;
	if(NULL == lcd || (lcd -> bit_mode != _4BIT_MODE && lcd -> bit_mode != _8BIT_MODE) || (E_NOT_OK == lcd_check_access(lcd)) )
		ret = E_NOT_OK;
	else{
		ret = GPIO_pin_initialize(&(lcd -> lcd_rs)); 
		ret = GPIO_pin_initialize(&(lcd -> lcd_en)); 
		if((lcd -> bit_mode) == 0){ /* 4bit mode */ 
			for(i=0;i<4;i++){
				ret = GPIO_pin_initialize(&(lcd -> lcd_data[i+4]));
            }
			__delay_ms(50);
			ret =lcd_send_command(lcd,LCD_4BIT_2LINE_5x8); 
			__delay_ms(5);
			ret =lcd_send_command(lcd,LCD_4BIT_2LINE_5x8); 
			__delay_us(150);
			ret =lcd_send_command(lcd,LCD_4BIT_2LINE_5x8); 
			ret =lcd_send_command(lcd,LCD_CLS); 
			ret =lcd_send_command(lcd,LCD_RET_HOME); 
			ret =lcd_send_command(lcd,LCD_ENTRY_MODE); 
			ret =lcd_send_command(lcd,LCD_SET_DDRAM_START); 
        }	
		else{/* 8bit mode */ 
			for(i=0;i<8;i++){
				ret = GPIO_pin_initialize(&(lcd -> lcd_data[i]));
            }
			__delay_ms(50);
			ret =lcd_send_command(lcd,LCD_8BIT_2LINE_5x8); 
			__delay_ms(5);
			ret =lcd_send_command(lcd,LCD_8BIT_2LINE_5x8); 
			__delay_us(150);
			ret =lcd_send_command(lcd,LCD_8BIT_2LINE_5x8); 
			ret =lcd_send_command(lcd,LCD_CLS); 
			ret =lcd_send_command(lcd,LCD_RET_HOME); 
			ret =lcd_send_command(lcd,LCD_ENTRY_MODE); 
			ret =lcd_send_command(lcd,LCD_SET_DDRAM_START); 
			}
		}
	return ret;
}
/*@brief: Sends a command to the lcd. The rs is set low. 
		  Check datasheet for possible commands. 
 *@param: Takes a pointer to an lcd of type chr_LCD_t.
 *@return: Returns E_OK if successful and E_NOT_OK otherwise.
 */       
STD_ReturnType lcd_send_command(const chr_LCD_t * lcd, uint8 command){
	STD_ReturnType ret = E_OK;
	if(NULL == lcd)
		ret = E_NOT_OK;
	else{
		ret = GPIO_pin_write_logic(&(lcd -> lcd_rs), GPIO_LOW);
		if((lcd -> bit_mode) == 0){ /*4bit mode */ 
			ret = lcd_send_4bits(lcd,command >> 4);
			ret = lcd_send_en(lcd);
			ret = lcd_send_4bits(lcd,command);
		}
		else{/* 8bit mode */
			ret = lcd_send_8bits(lcd,command);
			ret = lcd_send_en(lcd);
		}
	}
	return ret;
}
/*@brief: Sends a character to the lcd. The latest cursor position is used.
 *@param: Takes a pointer to an lcd of type chr_LCD_t and data to be written.
 *@return: Returns E_OK if successful and E_NOT_OK otherwise.
 */       
STD_ReturnType lcd_send_char_data(const chr_LCD_t * lcd, uint8 data){
	STD_ReturnType ret = E_OK;
	if(NULL == lcd)
		ret = E_NOT_OK;
	else{
		ret = GPIO_pin_write_logic(&(lcd -> lcd_rs), GPIO_HIGH);
		if(lcd -> bit_mode == _4BIT_MODE){ /*4bit mode */ 
			ret = lcd_send_4bits(lcd,data >> 4);
			ret = lcd_send_en(lcd);
			ret = lcd_send_4bits(lcd,data);
			ret = lcd_send_en(lcd);
		}
		else{/* 8bit mode */
			ret = lcd_send_8bits(lcd,data);
			ret = lcd_send_en(lcd);
		}
	}
	return ret;
}
/*@brief: Sends a character to the lcd given the position. 
 *@param: Takes a pointer to an lcd of type chr_LCD_t, data to be written, row number, and column number.
 *@return: Returns E_OK if successful and E_NOT_OK otherwise.
 */       
STD_ReturnType lcd_send_char_data_position(const chr_LCD_t * lcd, uint8 row, uint8 col, uint8 data){
	STD_ReturnType ret = E_OK;
	if(NULL == lcd)
		ret = E_NOT_OK;
	else{
		ret = lcd_set_cursor(lcd, row, col); 
		ret = lcd_send_char_data(lcd, data);
	}
	return ret;
}
/*@brief: Sends a string to the lcd. The latest cursor position is used 
 *@param: Takes a pointer to an lcd of type chr_LCD_t and the string to be displayed.
 *@return: Returns E_OK if successful and E_NOT_OK otherwise.
 */       
STD_ReturnType lcd_send_string(const chr_LCD_t * lcd, uint8* str){
	STD_ReturnType ret = E_OK;
	if(NULL == lcd)
		ret = E_NOT_OK;
	else{
		while(*str != '\0')
			ret = lcd_send_char_data(lcd,*str++);
	}
	return ret;
}
/*@brief: Sends a string to the lcd.
 *@param: Takes a pointer to an lcd of type chr_LCD_t, the string to be displayed, the row, and the column.
 *@return: Returns E_OK if successful and E_NOT_OK otherwise.
 */       
STD_ReturnType lcd_send_string_position(const chr_LCD_t * lcd, uint8 row, uint8 col, uint8* str){
	STD_ReturnType ret = E_OK;
	if(NULL == lcd)
		ret = E_NOT_OK;
	else{
		ret = lcd_set_cursor(lcd,row,col);
		while(*str != '\0')
			ret = lcd_send_char_data(lcd,*str++);
	}
	return ret;
}
/*@brief: Sends a customized character to the lcd. 
 *@param: Takes a pointer to an lcd of type chr_LCD_t, row, column, array containing the custom character in binary format,
		  and the CGRAM memory address to be used. 
 *@return: Returns E_OK if successful and E_NOT_OK otherwise.
 */       
STD_ReturnType lcd_send_custom_char(const chr_LCD_t * lcd, uint8 row, uint8 col, const uint8 _chr[], uint8 mem_pos){
	STD_ReturnType ret = E_OK;
	uint8 lc = 0;
	if(NULL == lcd)
		ret = E_NOT_OK;
	else{
		ret = lcd_send_command(lcd, LCD_SET_CGRAM_START + (--mem_pos *8)); //This just sets the CGRAM addr
		for(;lc<=7;lc++)
			ret = lcd_send_char_data(lcd, _chr[lc]); //This writes in the CGRAM
		ret = lcd_send_char_data_position(lcd,row,col,mem_pos); //This writes on the lcd itself
	}
	return ret;
}

/* (H) Helper functions */

/*@brief: (H) Converts a byte (uint8) to a string for lcd operations.
 *@param: Takes a char value and a pointer to a string that will contain the converted characters 
 *@return: Returns E_OK if successful and E_NOT_OK otherwise.
 */       
static STD_ReturnType byte_to_str(uint8 val, uint8* str){
	STD_ReturnType ret = E_OK;
	if(NULL == str)
		ret = E_NOT_OK;
	else{
		memset(str,'\0',4);
		snprintf((char*)str,4,"%i", val);
	}

	return ret;
}
/*@brief: (H) Converts a byte (uint16) to a string for lcd operations.
 *@param: Takes a (uint16) value and a pointer to a string that will contain the converted characters 
 *@return: Returns E_OK if successful and E_NOT_OK otherwise.
 */       
static STD_ReturnType short_to_str(uint16 val, uint8* str){
	STD_ReturnType ret = E_OK;
	if(NULL == str)
		ret = E_NOT_OK;
	else{

		memset(str,'\0',6);
		snprintf((char*)str,6, "%i", val);
	}
	return ret;
}
/*@brief: (H) Converts a byte (uint32) to a string for lcd operations.
 *@param: Takes a (uint32) value and a pointer to a string that will contain the converted characters 
 *@return: Returns E_OK if successful and E_NOT_OK otherwise.
 */       
static STD_ReturnType int_to_str(uint32 val, uint8* str){
	STD_ReturnType ret = E_OK;
	if(NULL == str)
		ret = E_NOT_OK;
	else{
		memset(str, '\0',11);
		snprintf((char*) str,11,"%i", val);
	}
	return ret;
}
/*@brief: (H) Writes data on the four data pins 
 *@param: Takes an lcd of type chr_LCD_t, the the data (val) to be written
 *@return: Returns E_OK on success and E_NOT_OK otherwise 
 */       
static STD_ReturnType lcd_send_4bits(const chr_LCD_t *lcd, uint8 val){
	STD_ReturnType ret = E_OK;
	uint8 i=0;
	if (NULL == lcd)
		ret = E_NOT_OK;
	else{
		for(i=0;i<4;i++){
			ret = GPIO_pin_write_logic(&(lcd -> lcd_data[i+4]), (val >> i) & (uint8) 0x01);
            if(ret==E_NOT_OK){
                break;
            }
        }
	}
	return ret;
}
/*@brief: (H) Writes data on the eight data pins 
 *@param: Takes an lcd of type chr_LCD_t, the the data (val) to be written
 *@return: Returns E_OK on success and E_NOT_OK otherwise 
 */       
static STD_ReturnType lcd_send_8bits(const chr_LCD_t *lcd, uint8 val){
	STD_ReturnType ret = E_OK;
	uint8 i=0;
	if (NULL == lcd)
		ret = E_NOT_OK;
	else{
		for(i=0;i<8;i++){
			ret = GPIO_pin_write_logic(&(lcd -> lcd_data[i]), (val >> i) & ((uint8) 0x01));
            if(ret==E_NOT_OK){
                break;
            }
        }
	}
	return ret;
}
/*@brief: (H) Sends an enable signal, which is a low-triggred pulse, to the lcd.
 *@param: Takes an lcd of type char_LCD_t 
 *@return: Returns E_OK on success and E_NOT_OK otherwise 
 */       
static STD_ReturnType lcd_send_en(const chr_LCD_t *lcd){
	STD_ReturnType ret = E_OK;
	if (NULL == lcd)
		ret = E_NOT_OK;
	else{
		ret = GPIO_pin_write_logic(&(lcd -> lcd_en), GPIO_HIGH);	
		__delay_us(15);
		ret = GPIO_pin_write_logic(&(lcd -> lcd_en), GPIO_LOW);	
	}
	return ret;
}
/*@brief: (H) Sets the cursor to a specific location on the lcd.
 *@param: Takes an lcd of type char_LCD_t, row, and column numbers.
 *@return: Returns E_OK on success and E_NOT_OK otherwise 
 */       
static STD_ReturnType lcd_set_cursor(const chr_LCD_t *lcd, uint8 row, uint8 col){
	STD_ReturnType ret = E_OK;
	col--;
	switch(row){
		case 1: lcd_send_command(lcd, (LCD_SET_DDRAM_START + row)); break;
		case 2: lcd_send_command(lcd, (LCD_SET_DDRAM_START + 40 + row)); break;
		default:ret = E_NOT_OK;
	}
	return ret;
}
