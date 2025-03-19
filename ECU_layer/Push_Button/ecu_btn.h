/* 
 * File:   ecu_btn.h
 * Author: nour
 *
 * Created on November 26, 2024, 2:11 PM
 */

#ifndef ECU_BTN_H
#define	ECU_BTN_H
/* ******** includes ********* */
#include "ecu_btn_cfg.h"
#include "../../MCAL_layer/GPIO/hal_gpio.h"
/* ******** data types ******* */
typedef enum{
	btn_pressed,
	btn_free
}btn_status;
typedef enum{
	btn_AL, /* This means the btn is connected to GND, normally high (use pull up resistor, or active low) */
	btn_AH /* This means the btn is connected to Vcc, normally low (use pull down resistor, or active high) */
}btn_mode_t;
typedef struct{
	pin_config_t btn_pin;
	btn_status btn_state;
	btn_mode_t btn_mode;
}btn_t;
/* ****** Function declarations * */
STD_ReturnType btn_intialize(const btn_t* btn);
STD_ReturnType btn_read_state(const btn_t* btn, btn_status *btn_s);
#endif	/* ECU_BTN_H */

