/*
 * ds1307.c
 *
 *  Created on: 14/04/2017
 *      Author: evandro
 */
#include "ds1307.h"

void Init_DS1307(void)
{
	i2c_Init(I2C1,ALT0,MULT0,0x14);
}
/****************************************************************************************
*
*****************************************************************************************/
bool Read_DS1307(data_ds1307 *dt)
{
	uint8_t data[8];
	uint8_t i = 0;
	uint8_t dado0;
	uint8_t dado1;
	uint8_t dado2;

	i2c_ReadMultRegister(I2C1,END_DS1307,0x00,7,&data);

	dado0 = (uint8_t)(data[0] & 0b00001111);
	dado1 = (uint8_t)(data[0] & 0b01110000);
	dado1 = (uint8_t)(dado1 >> 4);
	dt->seconds = (uint8_t)((dado1*10)+dado0);

	dado0 = (uint8_t)(data[1] & 0b00001111);
	dado1 = (uint8_t)(data[1] & 0b01110000);
	dado1 = (uint8_t)(dado1 >> 4);
	dt->minutes = (uint8_t)((dado1*10)+dado0);

	if((data[2] & 0b01000000) == 0)
	{
		// Modo 24 Horas
		dado0 = (uint8_t)(data[2] & 0b00001111);
		dado1 = (uint8_t)(data[2] & 0b00010000);
		dado1 = (uint8_t)(dado1 >> 4);
		dado2 = (uint8_t)(data[2] & 0b00100000);
		dado2 = (uint8_t)(dado2 >> 5);
		dt->hours = (uint8_t)( (dado2*20) + (dado1*10) + dado0 );
	}
	else
	{
		// Modo 12 Horas
		dado0 = (uint8_t)(data[2] & 0b00001111);
		dado1 = (uint8_t)(data[2] & 0b00010000);
		dado1 = (uint8_t)(dado1 >> 4);
		dado2 = (uint8_t)(data[2] & 0b00100000);
		dado2 = (uint8_t)(dado2 >> 5);
		dt->hours = (uint8_t)((dado1*10) + dado0);
	}

	dt->day = (uint8_t)(data[3]);

	dado0 = (uint8_t)(data[4] & 0b00001111);
	dado1 = (uint8_t)(data[4] & 0b00110000);
	dado1 = (uint8_t)(dado1 >> 4);
	dt->date = (uint8_t)((dado1*10)+dado0);

	dado0 = (uint8_t)(data[5] & 0b00001111);
	dado1 = (uint8_t)(data[5] & 0b00010000);
	dado1 = (uint8_t)(dado1 >> 4);
	dt->month = (uint8_t)((dado1*10)+dado0);

	dado0 = (uint8_t)(data[6] & 0b00001111);
	dado1 = (uint8_t)(data[6] & 0b11110000);
	dado1 = (uint8_t)(dado1 >> 4);
	dt->year = (uint8_t)((dado1*10)+dado0);
}
/****************************************************************************************
*
*****************************************************************************************/
bool Write_DS1307(data_ds1307 dt)
{
	uint8_t data[8];
	uint8_t aux = 0;
	/*
	data[0]= dt.minutes;
	data[1]= dt.hours;
	data[2]= dt.day;
	data[3]= dt.date;
	data[4]= dt.month;
	data[5]= dt.year;
	*/
/*
	data[0]= 40;
	data[1]= 1;
	data[2]= 7;
	data[3]= 15;
	data[4]= 4;
	data[5]= 17;
*/
	//data[0] = 0x01;

	// minutos
	if(dt.minutes < 60)
	{
		//dado0  = (uint8_t)((dt.minutes / 10) << 4);
		//dado0 += (uint8_t)(dt.minutes % 10);
		data[0]  = (uint8_t)((dt.minutes / 10) << 4);
		data[0] += (uint8_t)(dt.minutes % 10);
	}
	else
	{
		return false;
	}
	// horas
	if(dt.hours <= 9)
	{
		//dado1 = dt.hours;
		data[1] = dt.hours;
		//teste = 0b00000001;
	}
	else if((dt.hours > 9)&&(dt.hours < 19))
	{
		aux     = (uint8_t)(dt.hours % 10);
		data[1] = (uint8_t)(0b00010000 + aux);
		//teste |= 0b00000010;
	}
	else if((dt.hours > 19)&&(dt.hours < 23))
	{
		aux     = (uint8_t)(dt.hours % 10);
		data[1] = (uint8_t)(0b00100000 + aux);
		//teste |= 0b00000010;
	}
	else
	{
		return false;
	}

	//  dia da semana
	if((dt.day >= 1)&&(dt.day <= 7))
	{
		//dado2 = (uint8_t)(dt.day);
		data[2] = (uint8_t)(dt.day);
	}
	else
	{
		return false;
	}
	// mes
	if((dt.month >= 1)&&(dt.month <= 12))
	{
		//dado4  = (uint8_t)((dt.mount / 10) << 4);
		//dado4 += (uint8_t)(dt.mount % 10);
		data[4]  = (uint8_t)((dt.month / 10) << 4);
		data[4] += (uint8_t)(dt.month % 10);
 	}
	else
	{
		return false;
	}
	//dia do mes
	if((dt.date>=1)&&(dt.date<=31))
	{
		//dado3  = (uint8_t)((dt.date / 10) << 4);
		//dado3 += (uint8_t)(dt.date % 10);
		data[3]  = (uint8_t)((dt.date / 10) << 4);
		data[3] += (uint8_t)(dt.date % 10);
	}
	else
	{
		return false;
	}
	// ano
	if(dt.year < 100)
	{
		//dado5  = (uint8_t)((dt.year / 10) << 4);
		//dado5 += (uint8_t)(dt.year % 10);
		data[5]  = (uint8_t)((dt.year / 10) << 4);
		data[5] += (uint8_t)(dt.year % 10);
	}
	else
	{
		return false;
	}

	i2c_WriteMultRegister(I2C1,END_DS1307,0x01,6,&data);

	return true;
}
/****************************************************************************************
*
*****************************************************************************************/
