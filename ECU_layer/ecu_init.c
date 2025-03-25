/* 
 * File:   ecu_init.c
 * Author: nour
 *
 * Created on February 26, 2025, 7:34 AM
 */
#include "ecu_init.h"
//chr_LCD_t LCD1 = {{PORTD_I,PIN2,GPIO_OUT,GPIO_LOW},{PORTD_I,PIN3,GPIO_OUT,GPIO_LOW},_8BIT_MODE
//			   ,{{PORTC_I, PIN4, GPIO_OUT, GPIO_LOW},
//			     {PORTC_I, PIN5, GPIO_OUT, GPIO_LOW},
//			     {PORTC_I, PIN6, GPIO_OUT, GPIO_LOW},
//			     {PORTC_I, PIN7, GPIO_OUT, GPIO_LOW},
//			     {PORTD_I, PIN4, GPIO_OUT, GPIO_LOW},
//			     {PORTD_I, PIN5, GPIO_OUT, GPIO_LOW},
//			     {PORTD_I, PIN6, GPIO_OUT, GPIO_LOW},
//			     {PORTD_I, PIN7, GPIO_OUT, GPIO_LOW}}
//};
chr_LCD_t LCD1 = {
    .lcd_rs.port=PORTD_I,
    .lcd_rs.pin=PIN2,
    .lcd_rs.direction=GPIO_OUT,
    .lcd_rs.logic=GPIO_HIGH,
    .lcd_en.port=PORTD_I,
    .lcd_en.pin=PIN3,
    .lcd_en.direction=GPIO_OUT,
    .lcd_en.logic=GPIO_HIGH,
    _8BIT_MODE,
    .lcd_data[0].port=PORTC_I,
    .lcd_data[0].pin=PIN4,
    .lcd_data[0].direction=GPIO_OUT,
    .lcd_data[0].logic=GPIO_HIGH,
    .lcd_data[1].port=PORTC_I,
    .lcd_data[1].pin=PIN5,
    .lcd_data[1].direction=GPIO_OUT,
    .lcd_data[1].logic=GPIO_HIGH,
    .lcd_data[2].port=PORTC_I,
    .lcd_data[2].pin=PIN6,
    .lcd_data[2].direction=GPIO_OUT,
    .lcd_data[2].logic=GPIO_HIGH,
    .lcd_data[3].port=PORTC_I,
    .lcd_data[3].pin=PIN7,
    .lcd_data[3].direction=GPIO_OUT,
    .lcd_data[3].logic=GPIO_HIGH,
    .lcd_data[4].port=PORTD_I,
    .lcd_data[4].pin=PIN4,
    .lcd_data[4].direction=GPIO_OUT,
    .lcd_data[4].logic=GPIO_HIGH,
    .lcd_data[5].port=PORTD_I,
    .lcd_data[5].pin=PIN5,
    .lcd_data[5].direction=GPIO_OUT,
    .lcd_data[5].logic=GPIO_HIGH,
    .lcd_data[6].port=PORTD_I,
    .lcd_data[6].pin=PIN6,
    .lcd_data[6].direction=GPIO_OUT,
    .lcd_data[6].logic=GPIO_HIGH,
    .lcd_data[7].port=PORTD_I,
    .lcd_data[7].pin=PIN7,
    .lcd_data[7].direction=GPIO_OUT,
    .lcd_data[7].logic=GPIO_HIGH,
};
LED_t led_arr[4] = {
 {PORTA_I,PIN0,GPIO_LOW},
 {PORTA_I,PIN1,GPIO_LOW},
 {PORTA_I,PIN2,GPIO_LOW},
 {PORTA_I,PIN3,GPIO_LOW}
};
keypad_t keypad1 = {
	{PORTB_I,PIN4,PORTB_I,PIN3,PORTB_I,PIN2,PORTB_I,PIN1},//rows
	{PORTD_I,PIN5,PORTD_I,PIN4,PORTD_I,PIN3,PORTD_I,PIN2},//columns
	pull_down //connection type
};
seven_segment_t segment1 = {
	PORTA_I,
	PIN0,
	PORTA_I,	
	PIN1,
	PORTA_I,	
	PIN2,
	PORTA_I,	
	PIN3,
	OFF
};
dc_motor_t motor1 = {
	PORTA_I,
	PIN1,
	GPIO_LOW,
	PORTA_I,
	PIN2,
	GPIO_LOW,
};
dc_motor_t motor2 = {
	PORTE_I,
	PIN7,
	GPIO_HIGH,
	PORTC_I,
	PIN1,
	GPIO_HIGH,
};
		
relay_t relay1 = {
	PORTA_I,
	PIN1,
	relay_off
};	
btn_t btn1 = {
	.btn_pin.port = PORTA_I,
	.btn_pin.pin  = PIN4,
	.btn_pin.direction = GPIO_IN,
	.btn_pin.logic = GPIO_HIGH,
	.btn_state = btn_free, 
	.btn_mode = btn_AH /* the button itself is connected to HIGH */
};
btn_t btn2 = {
	.btn_pin.port = PORTE_I,
	.btn_pin.pin  = PIN1,
	.btn_pin.direction = GPIO_IN,
	.btn_pin.logic = GPIO_LOW,
	.btn_state = btn_free, 
	.btn_mode = btn_AL /* the button itself is connected to GND */
};
LED_t LED1 = {
  .port = PORTD_I,
  .pin = PIN1,
  .LED_init_status = GPIO_LOW
};
LED_t LED2 = {
  .port = PORTA_I,
  .pin = PIN1,
  .LED_init_status = GPIO_LOW
};
pin_config_t testing = {
	PORTD_I,	
	PIN1,
	GPIO_OUT,
	GPIO_HIGH
};
STD_ReturnType ecu_init(void){
    STD_ReturnType ret = E_OK;
    ret = lcd_initialize(&LCD1);
    return ret;
}