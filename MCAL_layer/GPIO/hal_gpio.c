/* 
 * File:   hal_gpio.c
 * Author: nour
 *
 * Created on October 23, 2024, 6:42 AM
 */

#include "hal_gpio.h"

/*reference to data direction registers*/
volatile uint8 * _TRIS_registers[] = {&TRISA,&TRISB,&TRISC,&TRISD,&TRISE};
/*reference to data latch register*/
volatile uint8 * _LAT_registers[] = {&LATA,&LATB,&LATC,&LATD,&LATE};
/*reference to port status register*/
volatile uint8 * _PORT_registers[] = {&PORTA,&PORTB,&PORTC,&PORTD,&PORTE};

/*@brief: Checks for invalid combinations between pins and ports.
 *@param: A pointer to a pin_config_t type which specifies a GPIO pin.
 *@return: E_OK if access is valid and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_check_access(const pin_config_t * _pin_config){
	STD_ReturnType ret = E_OK;
	if ((NULL == _pin_config) || ( (_pin_config -> port == PORTA_I) && ( (_pin_config -> pin) > PORTA_PIN_MAX_NUMBER - 1) ) || 
		( ((_pin_config -> port == PORTB_I) || (_pin_config -> port == PORTC_I) || (_pin_config -> port == PORTD_I) )
		 && ( (_pin_config -> pin) > PORTBCD_PIN_MAX_NUMBER - 1) ) ||( (_pin_config -> port == PORTE_I) &&
		 ((_pin_config -> pin) > PORTE_PIN_MAX_NUMBER - 1))	){
		ret = E_NOT_OK;	
	}
	else{}
	return ret;
}
#if PIN_CONFIGURATION == ENABLED
/*@brief: Initializes a GPIO pin by setting its initial port, pin, direction, and logic.
 *@param: A pointer to a pin_config_t type which specifies a GPIO pin.
 *@return: E_OK if initialization is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_pin_initialize(const pin_config_t * _pin_config){
	STD_ReturnType ret = E_OK;
	if ( (E_NOT_OK == GPIO_check_access(_pin_config)) ){
		ret = E_NOT_OK;	
	}
	else{
		ret = GPIO_pin_direction_initialize(_pin_config);
		ret = GPIO_pin_write_logic(_pin_config, _pin_config -> logic);
	}
    return ret;
}
#endif
#if PIN_CONFIGURATION == ENABLED
/*@brief: Initializes only the direction of the GPIO pin.
 *@param: A pointer to a pin_config_t type which specifies a GPIO pin.
 *@return: E_OK if initialization is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_pin_direction_initialize(const pin_config_t * _pin_config){
	STD_ReturnType ret = E_OK;
	if ( (E_NOT_OK == GPIO_check_access(_pin_config)) ){
		ret = E_NOT_OK;	
	}
	else{
		switch(_pin_config -> direction){
			case GPIO_OUT :
				CLR_BIT(*(_TRIS_registers[_pin_config->port]), _pin_config -> pin);
				break;
			case GPIO_IN :
				SET_BIT(*(_TRIS_registers[_pin_config-> port]),_pin_config -> pin);	
				break;
			default : 
				ret = E_NOT_OK;
		}	
	}
	return ret;
}
#endif
#if PIN_CONFIGURATION == ENABLED
/*@brief: Checks on the direction of a GPIO pin.
 *@param: A pointer to a pin_config_t type which specifies a GPIO pin and a pointer to a direction_t 
 *		  type which specifies GPIO pin direction.
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_pin_get_direction_status(const pin_config_t * _pin_config, direction_t* dic_status ){
	STD_ReturnType ret = E_OK;
	if ( (E_NOT_OK == GPIO_check_access(_pin_config)) || NULL == dic_status){
		ret = E_NOT_OK;	
	}
	else{
		*dic_status = READ_BIT(*_TRIS_registers[_pin_config -> port], _pin_config -> pin); 	
	}
	return ret;
}
#endif
#if PIN_CONFIGURATION == ENABLED
/*@brief: Writes logic on a GPIO pin.
 *@param: A pointer to a pin_config_t type which specifies a GPIO pin and a value of type 
 * 		  logic_t which specifies the logic on the pin. 
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_pin_write_logic(const pin_config_t * _pin_config, logic_t logic){
	STD_ReturnType ret = E_OK;
	if ( (E_NOT_OK == GPIO_check_access(_pin_config)) ){
		ret = E_NOT_OK;	
	}
	else{
		switch(logic){
			case(GPIO_LOW):
				CLR_BIT(*(_LAT_registers[_pin_config->port]), _pin_config -> pin);	
			break;
			case(GPIO_HIGH):
				SET_BIT(*(_LAT_registers[_pin_config->port]), _pin_config -> pin);	
			break;
			default:
			 ret = E_NOT_OK;		
		}	
	}
	return ret;
}
#endif
#if PIN_CONFIGURATION == ENABLED
/*@brief: Reads the logic on a pin.
 *@param: A pointer to a pin_config_t type which specifies a GPIO pin and a pointer to
 * 		  a logic_t type which specifies the logic on the pin. 
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_pin_read_logic(const pin_config_t * _pin_config, logic_t* logic){
	STD_ReturnType ret = E_OK;
	if ( (E_NOT_OK == GPIO_check_access(_pin_config)) || NULL == logic){
		ret = E_NOT_OK;	
	}
	else{
		*logic = READ_BIT(*_PORT_registers[_pin_config -> port], _pin_config -> pin); 	
	}
	return ret;
}
#endif
#if PIN_CONFIGURATION == ENABLED
/*@brief: Toggles the logic on the pin. 
 *@param: A pointer to a pin_config_t type which specifies a GPIO pin.
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_pin_toggle_logic(const pin_config_t * _pin_config){
	STD_ReturnType ret = E_OK;
	if ( (E_NOT_OK == GPIO_check_access(_pin_config)) ){
		ret = E_NOT_OK;	
	}
	else{
		TOGG_BIT(*(_LAT_registers[_pin_config -> port]), _pin_config -> pin);	
	}
	return ret;
}
#endif
#if PORT_CONFIGURATION == ENABLED
/*@brief: Initializes the direction of a GPIO port.
 *@param: A port index from the tyep port_index and an uint8 integer that specifies the direction 
		  of each of the port's pins. For example 0x07 sets the first three pins to high and the
		  the rest to low.
 *@return: E_OK if initialization is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_port_direction_initialize(port_index port, uint8 direction){
	STD_ReturnType ret = E_OK;
	//portA is indexed 0
	if ( (port > PORT_MAX_NUMBER - 1) || ((port == PORTA_I) && (direction > 0x7F)) || 
       ((port == PORTE_I) && (direction > 0x07)) || 
       ((port >= PORTB_I && port <= PORTD_I) && (direction > 0x7F)) ){
		ret = E_NOT_OK;
	}
	else{
      *(_TRIS_registers[port]) = direction;
    }
    return ret;
}
#endif
#if PORT_CONFIGURATION == ENABLED
/*@brief: Reads the direction of port.
 *@param: A port index from the type port_index and a pointer to an uin8 integer that will hold
		  the direction of the port.
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_port_get_direction_status(port_index port, uint8 *direction_status){
	STD_ReturnType ret = E_OK;
	if (NULL == direction_status && (port > PORT_MAX_NUMBER - 1) ){
		ret = E_NOT_OK;	
	}
	else{
		*direction_status = *_TRIS_registers[port];	
	
	}
	return ret;
}
#endif
#if PORT_CONFIGURATION == ENABLED
/*@brief: Writes logic to a port.
 *@param: A port index from the type port_index and an uin8 integer that will hold the port's logic.
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_port_write_logic(port_index port, uint8 logic){
	STD_ReturnType ret = E_OK;
	if ( (port > PORT_MAX_NUMBER - 1) || ((port == PORTA_I) && (logic > 0x7F)) || 
       ((port == PORTE_I) && (logic > 0x07)) || 
       ((port >= PORTB_I && port <= PORTD_I) && (logic > 0x7F)) ){
		ret = E_NOT_OK;
	}
	else{
		*_LAT_registers[port] = logic;
	}
	return ret;
}
#endif
#if PORT_CONFIGURATION == ENABLED
/*@brief: Reads logic on a port.
 *@param: A port index from the type port_index and a pointer to an uin8 integer that will hold the port's logic. 
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_port_read_logic(port_index port, uint8* logic){

	STD_ReturnType ret = E_OK;
	if (NULL == logic || (port > PORT_MAX_NUMBER - 1)){
		ret = E_NOT_OK;	
	}
	else{
		*logic = *_PORT_registers[port];	
	}
	return ret;
}
#endif
#if PORT_CONFIGURATION == ENABLED
/*@brief: Toggles the logic on a port.
 *@param: A port index of type port_index.
 *@return: E_OK if operation is successful and E_NOT_OK otherwise.
 */       
STD_ReturnType GPIO_port_toggle_logic(port_index port){

	STD_ReturnType ret = E_OK;
	if ( (port > PORT_MAX_NUMBER - 1) ){
		ret = E_NOT_OK;	
	}
	else{
		*_LAT_registers[port] ^= 0xFF; 
	}
	return ret;
}
#endif
