/* 
 * File:   delay.c
 * Author: nour
 *
 * Created on October 22, 2025, 3:34 PM
 * @Brief: This source contains implementation of delay functions which
 * 		   use the three hardware timers on the device.
 */
#include "delay.h"
static STD_ReturnType TMR_find_presc(uint32 delay, uint8 timer);

static const TMR_PRESC_SEL prescalers[] = { TMR_PRESC_1_1, TMR_PRESC_1_2, TMR_PRESC_1_4,
	TMR_PRESC_1_8, TMR_PRESC_1_16, TMR_PRESC_1_32,
	TMR_PRESC_1_64, TMR_PRESC_1_128, TMR_PRESC_1_256};

static TMR0_t tmr0_local = {
#if (INT_TMR0== INT_EN)
	.TMR0_interrupt_handler = __TMR0_delay,
#if (INT_PR == INT_EN)
	.priority = INT_PHIGH, /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	.prescaler = TMR_PRESC_1_1, 
	.preloaded_value = 0,
	.mode = _16BIT,
	.edge = RISING,
	.clk_source = INT
};

static TMR1_t tmr1_local = {
#if (INT_TMR1== INT_EN)
	.TMR1_interrupt_handler = __TMR1_delay,
#if (INT_PR == INT_EN)
	.priority = INT_PHIGH,
#endif
#endif
	.prescaler = TMR_PRESC_1_1,
	.preloaded_value = 0,
	.rdwr_mode = _16BIT,
	.clk_source = INT,
	.clk_sync = SYNC_OFF
};

static TMR2_t tmr2_local = {
#if (INT_TMR2== INT_EN)
	.TMR2_interrupt_handler = __TMR2_delay,
#if (INT_PR == INT_EN)
	.priority = INT_PHIGH, /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	.preloaded_value = 0,
	.prescaler = TMR_PRESC_1_1,
	.postscaler = TMR2_POSTSC_1_1,
};

static TMR3_t tmr3_local = {
#if (INT_TMR2== INT_EN)
	.TMR3_interrupt_handler = __TMR3_delay,// use NULL if unwanted
#if (INT_PR == INT_EN)
	.priority = INT_PHIGH, /* @ref: mcal_interrupt_config.h -> INT_PLOW & INT_PHIGH */	
#endif
#endif
	.preloaded_value = 0,
	.rdwr_mode = _16BIT,
	.clk_source = INT,
	.prescaler = TMR_PRESC_1_1,
	.clk_sync = SYNC_OFF
};

uint32 fosc = (uint32) _XTAL_FREQ;

/* @Brief: Delay time in ms using the Timer0 module.
 * @Param: A delay time as a uint32.
 * @Return: E_OK upon success.
 */
STD_ReturnType TMR0_delayms(uint32 delay){
	STD_ReturnType ret = E_OK;
	uint16 scale = 0;
#if ((INT_TMR0 == INT_DIS) || (INT_INTERRUPT_MODULE == STD_OFF))
	return E_NOT_OK; /* Interrupts are required */
#endif
		ret = TMR_find_presc(delay, _TMR0); /* Finds the best prescaler */
		ret &= TMR_PRESC2VAL(tmr0_local.prescaler, &scale); /* Convert the prescaler to a integer */
		uint32 ticks = (fosc / 4 / scale) * delay/1000;
		/* Load the preloaded value */
		tmr0_local.preloaded_value = (uint16) (65536 - (ticks & 0xFFFF));
		/* Initialize the timer */
        ret &= TMR0_initialize(&tmr0_local);
		/* Start the timer */
		ret &= TMR0_start(&tmr0_local);
	return ret;
}

/* @Brief: Delay time in ms using the Timer1 module.
 * @Param: A delay time as a uint32.
 * @Return: E_OK upon success.
 */
STD_ReturnType TMR1_delayms(uint32 delay){
	STD_ReturnType ret = E_OK;
	uint16 scale = 0;
#if ((INT_TMR1 == INT_DIS) || (INT_INTERRUPT_MODULE == STD_OFF))
	return E_NOT_OK; /* Interrupts are required */
#endif
		ret = TMR_find_presc(delay, _TMR1); /* Finds the best prescaler */
		ret &= TMR_PRESC2VAL(tmr1_local.prescaler, &scale); /* Convert the prescaler to a integer */
		uint32 ticks = (fosc / 4 / scale) * delay/1000;
		/* Load the preloaded value */
		tmr1_local.preloaded_value = (uint16) (65536 - (ticks & 0xFFFF));
		/* Initialize the timer */
        ret &= TMR1_initialize(&tmr1_local);
		/* Start the timer */
		ret &= TMR1_start(&tmr1_local);
	return ret;
}

/* @Brief: Delay time in ms using the Timer2 module.
 * @Param: A delay time as a uint32.
 * @Return: E_OK upon success.
 */
STD_ReturnType TMR2_delayms(uint32 delay){
	STD_ReturnType ret = E_OK;
	uint16 scale = 0;
#if ((INT_TMR1 == INT_DIS) || (INT_INTERRUPT_MODULE == STD_OFF))
	return E_NOT_OK; /* Interrupts are required */
#endif
		ret = TMR_find_presc(delay, _TMR2); /* Finds the best prescaler */
		ret &= TMR_PRESC2VAL(tmr2_local.prescaler, &scale); /* Convert the prescaler to a integer */
		uint32 ticks = (fosc / 4 / scale) * delay/1000;
		/* Load the preloaded value */
		tmr2_local.preloaded_value = (uint8) (265 - (ticks & 0xFF));
		/* Initialize the timer */
        ret &= TMR2_initialize(&tmr2_local);
		/* Start the timer */
		ret &= TMR2_start(&tmr2_local);
	return ret;
}

/* @Brief: Delay time in ms using the Timer3 module.
 * @Param: A delay time as a uint32.
 * @Return: E_OK upon success.
 */
STD_ReturnType TMR3_delayms(uint32 delay){
	STD_ReturnType ret = E_OK;
	uint16 scale = 0;
#if ((INT_TMR1 == INT_DIS) || (INT_INTERRUPT_MODULE == STD_OFF))
	return E_NOT_OK; /* Interrupts are required */
#endif
		ret = TMR_find_presc(delay, _TMR3); /* Finds the best prescaler */
		ret &= TMR_PRESC2VAL(tmr3_local.prescaler, &scale); /* Convert the prescaler to a integer */
		uint32 ticks = (fosc / 4 / scale) * delay/1000;
		/* Load the preloaded value */
		tmr3_local.preloaded_value = (uint16) (65536 - (ticks & 0xFFFF));
		/* Initialize the timer */
        ret &= TMR3_initialize(&tmr3_local);
		/* Start the timer */
		ret &= TMR3_start(&tmr3_local);
	return ret;
}

/* @brief: Find the best, least, prescaler to use in the delay routine.
 * @param: A delay uint32 that is passed to the delay routine.
 * @return: E_OK upon success and E_NOT_OK otherwise.
 */
static STD_ReturnType TMR_find_presc(uint32 delay, uint8 timer){
	STD_ReturnType ret = E_OK;
	uint16 pre_val = 0; 	
	uint32 ticks=0;
	char i;
	if(timer == _TMR0){
		for(i=0; i < 9;i++){
			ret = TMR_PRESC2VAL(prescalers[i], &pre_val);
			ticks = ((fosc/4000) * delay) / (uint32) pre_val;
			if(ticks <= 65536){
				/* This is the best usable prescaler */
				tmr0_local.prescaler = prescalers[i];
				break;
			}
			else if(i == 8) /* last iteration and no prescaler found */
				ret = E_NOT_OK;
		}
	}
	else if(timer == _TMR1){
		for(i=0; i < 4;i++){
			ret = TMR_PRESC2VAL(prescalers[i], &pre_val);
			ticks = ((fosc/4000) * delay) / (uint32) pre_val;
			if(ticks <= 65536){
				/* This is the best usable prescaler */
				tmr1_local.prescaler = prescalers[i];
				break;
			}
			else if(i == 3) /* last iteration and no prescaler found */
				ret = E_NOT_OK; 
		}
	}
	else if(timer == _TMR2){
		for(i=0; i < 5;i+=2){
			ret = TMR_PRESC2VAL(prescalers[i], &pre_val);
			ticks = ((fosc/4000) * delay) / (uint32) pre_val;
			if(ticks <= 256){
				/* This is the best usable prescaler */
				tmr2_local.prescaler = prescalers[i];
				break;
			}
			else if(i == 4) /* last iteration and no prescaler found */
				ret = E_NOT_OK; 
		}
	} 
	else if(timer == _TMR3){
		for(i=0; i < 4;i++){
			ret = TMR_PRESC2VAL(prescalers[i], &pre_val);
			ticks = ((fosc/4000) * delay) / (uint32) pre_val;
			if(ticks <= 65536){
				/* This is the best usable prescaler */
				tmr3_local.prescaler = prescalers[i];
				break;
			}
			else if(i == 4) /* last iteration and no prescaler found */
				ret = E_NOT_OK; 
		}
	}
	else
		ret = E_NOT_OK;

	return ret;
}
void __TMR0_delay(void){
}
void __TMR1_delay(void){
}
void __TMR2_delay(void){
}
void __TMR3_delay(void){
}
