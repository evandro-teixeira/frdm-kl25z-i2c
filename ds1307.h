/*
 * ds1307.h
 *
 *  Created on: 14/04/2017
 *      Author: evandro
 */

#ifndef SOURCES_DS1307_H_
#define SOURCES_DS1307_H_

#include "MKL25Z4.h"
#include "i2c.h"
#include "stdbool.h"

#define END_DS1307 0b01101000//0x68

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}data_ds1307;

void Init_DS1307(void);
bool Read_DS1307(data_ds1307 *dt);
bool Write_DS1307(data_ds1307 dt);
//uint8_t Read_DS1307_Byte(uint8_t end);

#endif /* SOURCES_DS1307_H_ */
