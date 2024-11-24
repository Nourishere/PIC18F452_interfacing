/* 
 * File:   application.c
 * Author: nour
 *
 * Created on October 23, 2024, 7:29 AM
 */
#include "application.h"
#include "/home/nour/programs/microchip/xc8/v2.50/pic/include/builtins.h"

/*We are operating in the HS configuration of the internal oscillator which has
  an clock speed of 8MHz/16MHz*/
 #define _XTAL_FREQ 8000000
//This macro is used in the __delay_ms() function
//#define _XTAL_FREQ 16000000


LED_t LED1 = {
  .port = PORTA_I,
  .pin = PIN3,
  .LED_init_status = GPIO_HIGH
};
LED_t LED2 = {
  .port = PORTA_I,
  .pin = PIN2,
  .LED_init_status = GPIO_HIGH
};
LED_t LED3 = {
  .port = PORTA_I,
  .pin = PIN1,
  .LED_init_status = GPIO_HIGH
};
LED_t LED4 = {
  .port = PORTA_I,
  .pin = PIN0,
  .LED_init_status = GPIO_HIGH
};


uint8 dec = 0;

int main(void){
    STD_ReturnType ret = E_OK;
    application_intialize();
 
   
    return (EXIT_SUCCESS);
}

void application_intialize(){
    STD_ReturnType ret = E_OK;
	ret = LED_intialize(&LED1);
    ret = LED_intialize(&LED2);
    ret = LED_intialize(&LED3);
    ret = LED_intialize(&LED4);

}
