/* 
 * File:   hal_gpio.h
 * Author: nour
 *
 * Created on October 23, 2024, 7:06 AM
 */

#ifndef HAL_GPIO_H
#define	HAL_GPIO_H

/**************** Includes ***********************/
#include "../device_config.h"
#include <xc.h>
#include "../mcal_std_types.h"
#include "hal_gpio_cfg.h"

/*************** Macros **************************/
#define PORT_MAX_NUMBER 5
#define PORTA_PIN_MAX_NUMBER 7
#define PORTBCD_PIN_MAX_NUMBER 8
#define PORTE_PIN_MAX_NUMBER 3
#define BIT_MASK (uint8)0x01

/*************** Function-like macros ************/
#define _REG(_x) (*((volatile uint8 *)(_x)))
#define SET_BIT(REG,BIT_POS) (REG |= (1 << BIT_POS))
#define CLR_BIT(REG,BIT_POS) (REG &= ~(1 << BIT_POS))
#define TOGG_BIT(REG,BIT_POS) (REG ^= (1 << BIT_POS))
#define READ_BIT(REG,BIT_POS) ((REG  >> BIT_POS) & BIT_MASK)

/*************** Data types **********************/
typedef enum{
	GPIO_LOW,
	GPIO_HIGH
}logic_t;
typedef enum{
	GPIO_OUT,
	GPIO_IN,
}direction_t;
typedef enum{
	PORTA_I,
	PORTB_I,
	PORTC_I,
	PORTD_I,
	PORTE_I	
}port_index;
typedef enum{
	 PIN0,
	 PIN1,
	 PIN2,
	 PIN3,
	 PIN4,
	 PIN5,
	 PIN6,
	 PIN7
}pins;

typedef struct{
	uint8 port :3;
	uint8 pin  :3;
	uint8 direction :1;
	uint8 logic :1;
}pin_config_t;
typedef struct{
	uint8 port :3;
	uint8 pin  :3;
	uint8 logic :1;
}pin_config_simple_t;
typedef struct{
	uint8 port :3;
	uint8 pin  :3;
}port_pin_t;
/*************** function declarations ***************************/
STD_ReturnType GPIO_check_access(const pin_config_t * _pin_config);

STD_ReturnType GPIO_pin_initialize(const pin_config_t * _pin_config);
STD_ReturnType GPIO_pin_direction_initialize(const pin_config_t * _pin_config);
STD_ReturnType GPIO_pin_get_direction_status(const pin_config_t * _pin_config, direction_t* dic_status);
STD_ReturnType GPIO_pin_write_logic(const pin_config_t * _pin_config, logic_t logic);
STD_ReturnType GPIO_pin_read_logic(const pin_config_t * _pin_config, logic_t* logic);
STD_ReturnType GPIO_pin_toggle_logic(const pin_config_t * _pin_config);

STD_ReturnType GPIO_port_direction_initialize(port_index port, uint8 logic);
STD_ReturnType GPIO_port_get_direction_status(port_index port, uint8 *direction_status);
STD_ReturnType GPIO_port_write_logic(port_index port, uint8 logic);
STD_ReturnType GPIO_port_read_logic(port_index port, uint8* logic);
STD_ReturnType GPIO_port_toggle_logic(port_index port);
#endif	/* HAL_GPIO_H */

