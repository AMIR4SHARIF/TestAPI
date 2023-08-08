#include "RTC_CONF.h"

uint8_t   EndOfMonth[12]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint32_t  TIME_CR=0;
uint16_t  preYear;
int8_t   preOffset;

/*-------------------------- RTC Founctions section ------------------------------*/

void TIME_UTC_Offset(int8_t offset){
	preOffset = offset;
}
void TIME_Config( uint16_t fYear ,uint8_t fMonth,  uint8_t fDay ,uint8_t fHour, uint8_t fMin, uint8_t fSec)
{
	preYear = (uint8_t)(fYear/100);
    LL_RTC_TIME_Set(RTC,( ((fYear%100)*31536000) + ((Daycall(fMonth-1)+fDay)*86400)+
		                (fHour * 3600) + (fMin * 60) + fSec ));
}

void TIME_GET(TIME_RTC *Time)
{
  uint16_t  Daytemp ;
  Time->preYear = preYear;
  TIME_CR = LL_RTC_TIME_Get(RTC) + (uint32_t)(preOffset*360);
  Time->Year    = (uint8_t)(TIME_CR/31536000);
  Daytemp      = (uint16_t)((TIME_CR%31536000)/86400);
  Time->Month   = Monthcall(Daytemp);
  Time->Day     = Daytemp - Daycall(Time->Month-1);
  Time->Hour    = (TIME_CR/3600) % 24;
  Time->Minute  = (TIME_CR % 3600) / 60;
  Time->Second  = (TIME_CR % 3600) % 60;
}

void TIME_GET_GMT(TIME_RTC *Time)
{
  uint16_t  Daytemp ;
  Time->preYear = preYear;
  TIME_CR = LL_RTC_TIME_Get(RTC);
  Time->Year    = (uint8_t)(TIME_CR/31536000);
  Daytemp      = (uint16_t)((TIME_CR%31536000)/86400);
  Time->Month   = Monthcall(Daytemp);
  Time->Day     = Daytemp - Daycall(Time->Month-1);
  Time->Hour    = (TIME_CR/3600) % 24;
  Time->Minute  = (TIME_CR % 3600) / 60;
  Time->Second  = (TIME_CR % 3600) % 60;
}


void ALARM_Config(uint16_t fYear ,uint8_t fMonth,  uint8_t fDay ,uint8_t fHour, uint8_t fMin, uint8_t fSec)
{
	TIME_RTC Alarm;
	Alarm.Year   = fYear%100;
	Alarm.Month  = fMonth ;
	Alarm.Day    = fDay;
	Alarm.Hour   = fHour;
	Alarm.Minute = fMin;
    Alarm.Second = fSec;
    LL_RTC_ALARM_Set(RTC,((Alarm.Year*31536000) + ((Daycall(Alarm.Month-1)+Alarm.Day)*86400)+
		                (Alarm.Hour * 3600) +(Alarm.Minute * 60) + Alarm.Second));
}

void ALARM_PlusM(uint8_t n){
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_DisableIT_ALR(RTC);
	LL_RTC_ALARM_Set(RTC,(LL_RTC_ALARM_Get(RTC)+(n*TIME_SKIP)));
	LL_RTC_EnableIT_ALR(RTC);
	LL_RTC_EnableWriteProtection(RTC);
}


uint16_t Monthcall(uint16_t D){
	uint16_t X=0;
	for(int i=0;i<12;i++)
	{
	 X+=EndOfMonth[i];
	 if(D<X) return i+1;
	}
	return 0;
}

uint16_t Daycall(uint16_t m){
	uint16_t X=0;
	for(int i=0;i<m;i++)
	{
	 X+=EndOfMonth[i];
	}
	return X;
}


/*--------------------------- END RTC Founctions section ------------------------------*/


