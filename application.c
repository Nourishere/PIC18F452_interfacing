/* 
 * File:   application.c
 * Author: nour
 *
 * Created on October 23, 2024, 7:29 AM
 */
#include "application.h"

STD_ReturnType ret = E_NOT_OK;

int main(void){
	ret = application_initialize();	
	while(1){ /* Loop infinitely */


	}
	return 0;
}
STD_ReturnType application_initialize(void){
#if (__ECU == STD_ON)
	ret = ecu_init();
#endif
	/* Initialize modules here */	
	return ret;
}

