/* 
 * File:   ecu_LCD.h
 * Author: nour
 *
 * Created on March 10, 2025, 11:07 AM
 */

#ifndef ECU_LCD_H
#define	ECU_LCD_H
/********* includes *************/
#include "../ecu_init.h"
/*********  macros  *************/
#define _4BIT_MODE 0
#define _8BIT_MODE 1
#define LCD_1ROW 1
#define LCD_2ROWS 2
#define LCD_4ROWS 4
#define LCD_ROW1_BASE_LOC 0x80
#define LCD_ROW2_BASE_LOC 0xC0
#define LCD_ROW3_BASE_LOC 0x80
#define LCD_ROW4_BASE_LOC 0x80
//Commands
#define LCD_CLS 0x01
#define LCD_RET_HOME 0x02
#define LCD_ENTRY_MODE 0x06
#define LCD_CUR_OFF_DISP_OFF 0x08
#define LCD_CUR_OFF_DISP_ON 0x0C
#define LCD_CUR_ON_BLK_ON 0x0F
#define LCD_CUR_ON_BLK_OFF 0x0E
#define LCD_SHIFT_ON_MOV_RIGHT 0x1C
#define LCD_SHIFT_ON_MOV_LEFT  0x18
#define LCD_4BIT_1LINE_5x8  0x20
#define LCD_4BIT_1LINE_5x10 0x24
#define LCD_4BIT_2LINE_5x8  0x28
#define LCD_4BIT_2LINE_5x10 0x2C
#define LCD_8BIT_1LINE_5x8  0x30
#define LCD_8BIT_1LINE_5x10 0x34
#define LCD_8BIT_2LINE_5x8  0x38
#define LCD_8BIT_2LINE_5x10 0x3C
#define LCD_SET_DDRAM_START 0x80
#define LCD_SET_CGRAM_START 0x40

/********* data types ***********/
typedef struct{
	pin_config_t lcd_rs; /* register select */ 
	pin_config_t lcd_en; /* enable pin */ 
	uint8 bit_mode;
	pin_config_t lcd_data[8];
}chr_LCD_t;
/**** function declarations *****/
static STD_ReturnType lcd_check_access(const chr_LCD_t *lcd);
static STD_ReturnType lcd_send_4bits(const chr_LCD_t *lcd, uint8 val);
static STD_ReturnType lcd_send_8bits(const chr_LCD_t *lcd, uint8 val);
static STD_ReturnType lcd_send_en(const chr_LCD_t *lcd);
static STD_ReturnType lcd_set_cursor(const chr_LCD_t *lcd, uint8 row, uint8 col);
STD_ReturnType lcd_initialize(const chr_LCD_t * lcd);
STD_ReturnType lcd_send_command(const chr_LCD_t * lcd, uint8 command);
STD_ReturnType lcd_send_char_data(const chr_LCD_t * lcd, uint8 data);
STD_ReturnType lcd_send_char_data_position(const chr_LCD_t * lcd, uint8 row, uint8 col, uint8 data);
STD_ReturnType lcd_send_string(const chr_LCD_t * lcd, uint8* str);
STD_ReturnType lcd_send_string_position(const chr_LCD_t * lcd, uint8 row, uint8 col, uint8* str);
STD_ReturnType lcd_send_custom_char(const chr_LCD_t * lcd, uint8 row, uint8 col, const uint8 _chr[], uint8 mem_pos);

static STD_ReturnType byte_to_str(uint8 val, uint8* str);
static STD_ReturnType short_to_str(uint16 val, uint8* str);
static STD_ReturnType int_to_str(uint32 val, uint8* str);
#endif	/* ECU_LCD_H */
