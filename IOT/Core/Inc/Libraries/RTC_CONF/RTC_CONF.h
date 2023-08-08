#ifndef RTC_CONF__H
#define RTC_CONF__H

#ifdef __cplusplus
extern "C"{
#endif

#include "main.h"
#define TIME_SKIP 360

// Zone offsets for some regions
#define UTC_OFFSET_IDLW          -120
#define UTC_OFFSET_HAWAII        -100
#define UTC_OFFSET_ALASKA        -90
#define UTC_OFFSET_US_PT         -80
#define UTC_OFFSET_US_MONT       -70
#define UTC_OFFSET_US_CANTRAL    -60
#define UTC_OFFSET_US_ESAST      -50
#define UTC_OFFSET_CA_ATALN      -40
#define UTC_OFFSET_GREENLAND     -30
#define UTC_OFFSET_MAIN           0
#define UTC_OFFSET_AMSTERDAM      10
#define UTC_OFFSET_BEIRUT         20
#define UTC_OFFSET_AMAN           30
#define UTC_OFFSET_ISTANBUL       30
#define UTC_OFFSET_KUWAIT         30
#define UTC_OFFSET_TEHRAN         35
#define UTC_OFFSET_BAKU           40
#define UTC_OFFSET_KABUL          45



// Structure of Time
typedef struct __TIME_RTC{
	uint8_t  Second;
	uint8_t  Minute ;
	uint8_t  Hour ;
    uint8_t  Month;
    uint8_t  Day;
    uint8_t  Year;
    uint8_t  preYear ;
}TIME_RTC;

// prototypes of functions
void TIME_UTC_Offset(int8_t offset);

void TIME_Config( uint16_t fYear ,uint8_t fMonth,  uint8_t fDay ,uint8_t fHour, uint8_t fMin, uint8_t fSec);
void TIME_GET(TIME_RTC *Time);
void TIME_GET_GMT(TIME_RTC *Time);

void ALARM_Config(uint16_t fYear ,uint8_t fMonth,  uint8_t fDay ,uint8_t fHour, uint8_t fMin, uint8_t fSec);
void ALARM_PlusM(uint8_t n);

uint16_t Daycall(uint16_t m);
uint16_t Monthcall(uint16_t D);

#ifdef __cplusplus
}
#endif

#endif
