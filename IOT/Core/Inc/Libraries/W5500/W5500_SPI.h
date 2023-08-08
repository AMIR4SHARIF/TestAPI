/*
 * W5500_SPI.h
 *
 *  Created on: Mar 14, 2023
 *      Author: Amir Sharif
 */

#ifndef INC_W5500_SPI_H_
#define INC_W5500_SPI_H_


void W5500_Select(void);
void W5500_Unselect(void);
void W5500_Restart(void);
void W5500_ReadBuff(uint8_t* buff, uint16_t len);
void W5500_WriteBuff(uint8_t* buff, uint16_t len);
uint8_t W5500_ReadByte(void) ;
void W5500_WriteByte(uint8_t byte);


#endif /* INC_W5500_SPI_H_ */
