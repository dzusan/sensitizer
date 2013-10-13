/*
 * handlers.h
 *
 *  Created on: 16.09.2013
 *      Author: dzusan
 */


#ifndef HANDLERS_H_
#define HANDLERS_H_

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"
#include "utils.h"
#include "calc.h"
#include <stdlib.h>

void CAN_requestHandler(void);
void ErrHandler(void);

#endif /* HANDLERS_H_ */
