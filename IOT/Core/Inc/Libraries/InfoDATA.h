#ifndef INC_INFODATA_H_
#define INC_INFODATA_H_

#include "stdbool.h"

//uint8_t *JSONBUF; //JSON addres

// Data Information structure
	typedef struct{
		double TEMP;
		double HUMID;
		uint16_t LPG;
		uint16_t SMK;
		bool rel1;
		bool rel2;
		bool rel3;
		bool rel4;
		char UinqID[17];
		char Version[17];
	}InfoDATA_craetor;


// Orders from client structure
	typedef struct{
		uint8_t toggle_rel;
	}OrderDATA_craetor;

// Login Information structure
	typedef struct{
		char user[33];
		char pass[33];
		char token[17];
	}LoginDATA_craetor;

// Register Information structure
	typedef struct{
		char user[33];
		char pass[33];
		char UinqID[13];
	}RegDATA_craetor;


#endif /* INC_INFODATA_H_ */
