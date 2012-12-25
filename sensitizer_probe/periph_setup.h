/*
 * periph_setup.h
 *
 *  Created on: 23.12.2012
 *      Author: dzusan
 */

#ifndef PERIPH_SETUP_H_
#define PERIPH_SETUP_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/*Test LED blink frequency*/
#define RTC_PRESCALER 8000

void SetupLED(void);
void SetupRTC(void);
void Setup_RTC_NVIC(void);
void SetupUSART(void);
void Setup_USART_NVIC(void);

#endif /* PERIPH_SETUP_H_ */
