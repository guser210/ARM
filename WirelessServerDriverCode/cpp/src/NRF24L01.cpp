/*
 * NRF24L01.cpp
 *
 *  Created on: Feb 20, 2021
 *      Author: gvargas
 */

#include "NRF24L01.h"

NRF24L01::NRF24L01() {
	// TODO Auto-generated constructor stub

}

NRF24L01::~NRF24L01() {
	// TODO Auto-generated destructor stub
}

NRF24L01::NRF24L01(SPI_HandleTypeDef* port, GPIO_TypeDef* csnPort, GPIO_TypeDef* cePort, uint16_t csn, uint16_t ce){
	this->port = port;
	this->csnPort = csnPort;
	this->cePort = cePort;

	this->csn = csn;
	this->ce = ce;

	ceLow();
	csnHigh();


}

uint8_t NRF24L01::readReg( uint8_t reg){
	reg = 0b00011111 & reg;
	uint8_t result = 0;

	csnLow();
	HAL_SPI_Transmit(port, &reg, Size, Timeout);
	HAL_SPI_Receive(port, &result, Size, Timeout);
	csnHigh();

	return result;

}

void NRF24L01::writeReg( uint8_t reg, uint8_t data){
	writeReg(reg, &data, 1);
}

void NRF24L01::writeReg( uint8_t reg, uint8_t *data, uint8_t sizeOfData){
	reg = 0b00100000 | ( 0b00011111 & reg);
	uint8_t buffer[ sizeOfData + 1] {reg};

	memcpy(buffer + 1, data, sizeOfData);

	csnLow();
	HAL_SPI_Transmit(port, buffer, sizeOfData + 1, Timeout);
	csnHigh();

}

void NRF24L01::config(){

	ceLow();
	csnHigh();
	HAL_Delay(11);

	writeReg(0, 0b00001010); // config;
	HAL_Delay(1);

	writeReg(1, 0); // no ack.

	writeReg(5, 60);

	writeReg(0x0a, (uint8_t*)"gyroc", 5);
	writeReg(0x10, (uint8_t*)"gyroc", 5);

	writeReg(0x11, 32);

}

void NRF24L01::modeTX(){
	uint8_t config = readReg(0);

	config &= ~(1<<0);
	writeReg(0, config);
	HAL_Delay(1);
	ceLow();

}
void NRF24L01::modeRX(){
	uint8_t config = readReg(0);

	config |= (1<<0);
	writeReg(0, config);
	HAL_Delay(1);
	ceHigh();
}

void NRF24L01::sendMessage(char* msg){
	uint8_t buffer[33] {0b10100000};

	memcpy(buffer + 1, msg, 32);

	csnLow();

	HAL_SPI_Transmit(port, buffer, 33, Timeout);
	csnHigh();

	ceHigh();
	HAL_Delay(1);
	ceLow();

	writeReg(7, 0b00110000);

}

void NRF24L01::receiveMessage( char* msg){
	uint8_t reg = 0b01100001;

	csnLow();

	HAL_SPI_Transmit(port, &reg, Size, Timeout);

	HAL_SPI_Receive(port, (uint8_t*)msg, 32, Timeout);

	csnHigh();

	writeReg(7, 0b01000000);
}

uint8_t NRF24L01::newMessage(){
	return !( readReg(0x17) & 0b00000001);
}







































