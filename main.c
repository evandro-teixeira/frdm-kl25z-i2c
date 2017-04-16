/*
*
*/
#include "MKL25Z4.h"
#include "i2c.h"
//#include "gpio.h"
#include "ds1307.h"
 
data_ds1307 rtc, rtc_in;
uint32_t i = 0;
 
int main(void)
{
        // Inicializa barramento I2C - DS1307
	Init_DS1307();
 
        // prepara o dados para ser salvos no RTC DS1307
        // 02:17 - 7 (sabado) - 15/04/17
	rtc_in.minutes = 17; // minutos 
	rtc_in.hours = 2;    // horas
	rtc_in.day = 7;      // dia da semana
	rtc_in.date = 15;    // dia do mes
	rtc_in.month = 4;    // mes
	rtc_in.year = 17;    // ano
 
	Write_DS1307(rtc_in); // Configura o RTC DS1307
 
	while(1)
    	{		
  		for(i=0;i<500000;i++); // delay
 
  		Read_DS1307(&rtc); // Le dados do RTC DS1307
 
  		rtc.date;
  		rtc.day;
  		rtc.hours;
  		rtc.minutes;
  		rtc.month;
  		rtc.seconds;
  		rtc.year;
  		//i = 0;
	}
}
