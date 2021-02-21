/*
 * NRF24L01.h
 *
 *  Created on: Feb 20, 2021
 *      Author: gvargas
 */



#ifndef NRF24L01_H_
#define NRF24L01_H_

#include "main.h"
#include <string.h>

class NRF24L01 {
private: //vars.
	SPI_HandleTypeDef* port;
	GPIO_TypeDef *csnPort;
	GPIO_TypeDef *cePort;
	uint16_t csn;
	uint16_t ce;

	uint16_t Size = 1;
	uint32_t Timeout = 2000;
public: //vars.

public: // funcs.


	void csnLow(){ HAL_GPIO_WritePin(csnPort, csn, GPIO_PIN_RESET);}
	void csnHigh(){ HAL_GPIO_WritePin(csnPort, csn, GPIO_PIN_SET);}

	void ceLow(){ HAL_GPIO_WritePin(cePort, ce, GPIO_PIN_RESET);}
	void ceHigh(){ HAL_GPIO_WritePin(cePort, ce, GPIO_PIN_SET);}


	NRF24L01(SPI_HandleTypeDef* port, GPIO_TypeDef* csnPort, GPIO_TypeDef* cePort, uint16_t csn, uint16_t ce);

	uint8_t readReg( uint8_t reg);

	void writeReg( uint8_t reg, uint8_t data);
	void writeReg( uint8_t reg, uint8_t *data, uint8_t sizeOfData);


	void config();

	void modeTX();
	void modeRX();

	void sendMessage(char* msg);
	void receiveMessage( char* msg);

	uint8_t newMessage();



public:
	NRF24L01();
	virtual ~NRF24L01();
};

#endif /* NRF24L01_H_ */
