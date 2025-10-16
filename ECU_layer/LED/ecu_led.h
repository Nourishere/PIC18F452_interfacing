/* 
 * File:   ecu_led.h
 * Author: nour
 *
 * Created on October 23, 2024, 7:25 AM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H
/*********** includes ************************/
#include "../../MCAL_layer/GPIO/mcal_gpio.h"
#include "ecu_led_cfg.h"
/*********** data types **********************/
typedef enum{
	LED_OFF,
	LED_ON
}LED_status;
typedef struct{
	uint8 port :3;
	uint8 pin :3;
	uint8 LED_init_status :1;
	uint8 reserved :1;
}LED_t;
/******** function prototypes ****************/
STD_ReturnType LED_check_access(LED_t *led);
STD_ReturnType LED_initialize(LED_t *led);
STD_ReturnType LED_on(LED_t *led);
STD_ReturnType LED_off(LED_t *led);
STD_ReturnType LED_toggle(LED_t *led);

#endif	/* ECU_LED_H */
