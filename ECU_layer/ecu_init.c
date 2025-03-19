/* 
 * File:   ecu_init.c
 * Author: nour
 *
 * Created on February 26, 2025, 7:34 AM
 */
#include "ecu_init.h"
chr_LCD_t LCD1 = {{PORTA_I,PIN0,GPIO_OUT,GPIO_LOW},{PORTA_I,PIN1,GPIO_OUT,GPIO_LOW},_8BIT_MODE
			   ,{{PORTC_I, PIN0, GPIO_OUT, GPIO_LOW},
			     {PORTC_I, PIN1, GPIO_OUT, GPIO_LOW},
			     {PORTC_I, PIN2, GPIO_OUT, GPIO_LOW},
			     {PORTC_I, PIN3, GPIO_OUT, GPIO_LOW},
			     {PORTC_I, PIN4, GPIO_OUT, GPIO_LOW},
			     {PORTC_I, PIN5, GPIO_OUT, GPIO_LOW},
			     {PORTC_I, PIN6, GPIO_OUT, GPIO_LOW},
			     {PORTC_I, PIN7, GPIO_OUT, GPIO_LOW}}
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
	OFF,
	COM_anode
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
	.btn_pin.direction = GPIO_OUT,
	.btn_pin.logic = GPIO_LOW,
	.btn_state = btn_free, 
	.btn_mode = btn_AL /* the button itself is connected to GND */
};
LED_t LED1 = {
  .port = PORTA_I,
  .pin = PIN0,
  .LED_init_status = GPIO_HIGH
};
LED_t LED2 = {
  .port = PORTA_I,
  .pin = PIN1,
  .LED_init_status = GPIO_LOW
};
void ecu_init(void){
   	STD_ReturnType ret = E_OK;
//	ret = LED_intialize(&LED1);
//  ret = LED_intialize(&LED2);
//	ret = btn_intialize(&btn1);
//	ret = btn_intialize(&btn2);
//  ret = relay_intialize(&relay1);
//	ret = dc_motor_intialize(&motor1);
//  ret = dc_motor_intialize(&motor2);
//  ret = seven_segment_intialize(&segment1);   
//	ret = keypad_intialize(&keypad1);
	ret = lcd_intialize(&LCD1);
}

