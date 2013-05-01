/*
 * periph_setup.h
 *
 *  Created on: 23.12.2012
 *      Author: dzusan
 */

#ifndef PERIPH_SETUP_H_
#define PERIPH_SETUP_H_

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"
#include "utils.h"

/*Test LED blink frequency*/
#define RTC_PRESCALER 240000

void SetupLED(void);
void SetupRTC(void);
void Setup_RTC_NVIC(void);
void SetupUSART(void);
void Setup_USART_NVIC(void);
void SetupCAN(void);
void SetupTIM(void);

#endif /* PERIPH_SETUP_H_ */
