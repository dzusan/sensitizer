/*
 * modbus.h
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */


#ifndef MODBUS_H_
#define MODBUS_H_

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"
#include "utils.h"
#include <stdlib.h>

void request(uint8_t *startByte, int num);
void response(uint8_t *startByte, int num);
uint16_t CRC16 (const uint8_t *puchMsg, int usDataLen, uint16_t crc_initialize );
uint8_t Write(uint8_t node, uint16_t reg, uint16_t length, uint16_t *data);
uint8_t Read(uint8_t node, uint16_t reg, uint16_t length, uint16_t *data);
uint8_t Com(uint8_t node, uint8_t com, uint16_t data);

#endif /* MODBUS_H_ */
