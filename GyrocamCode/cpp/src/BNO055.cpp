/*
 * BNO055.cpp
 *
 *  Created on: Feb 14, 2021
 *      Author: gvargas
 */

#include "BNO055.h"

BNO055::BNO055(I2C_HandleTypeDef* pDev, uint8_t slave) {
	// TODO Auto-generated constructor stub
	this->pDevice = pDev;
	this->slave_addr = slave << 1;

	initMemory();

}

BNO055::~BNO055() {
	// TODO Auto-generated destructor stub
}

int BNO055::readReg(unsigned char reg, unsigned char *data, uint8_t size){
	HAL_StatusTypeDef ret;

	ret = HAL_I2C_Mem_Read(pDevice, slave_addr | (1<<0), reg, 1, data, size, 1000);

	return (int) ret;
}

int BNO055::writeReg(unsigned char reg, unsigned char *data, uint8_t size){
	HAL_StatusTypeDef ret;

	ret = HAL_I2C_Mem_Write(pDevice, slave_addr, reg, 1, data, size, 1000);

	return (int) ret;
}

void BNO055::reset(){

	enableConfig();

	data[0] = 0b00100000;
	writeReg(0x3f, data, 1);
	HAL_Delay(600);


}

void BNO055::enableNDOF(){

	HAL_GPIO_WritePin(LEDR_GPIO_Port, LEDR_Pin, GPIO_PIN_SET);
	readMemory(data, 1, sizeOfSettings - 24);

	if( data[0] == 1 && calibrate == 0){
		readMemory(data, 22, sizeOfSettings - 23);
		enableConfig();
		writeReg(0x55, data, 22);
		HAL_GPIO_WritePin(LEDR_GPIO_Port, LEDR_Pin, GPIO_PIN_RESET);
	}


	data[0] = 0b11000000;
	writeReg(0x3f, data, 1);

	data[0] = 0b00001100;
	writeReg(0x3d, data, 1);
	HAL_Delay(7);
}

void BNO055::getData(){
	readReg(0x35, data, 1);
	calibrationData = data[0];

	calMag = (calibrationData & 0b00000011);
	calAcc = (calibrationData & 0b00001100)>>2;
	calGyr  = (calibrationData & 0b00110000)>>4;
	calSys = (calibrationData & 0b11000000)>>6;

	readReg(0x1a, data, 6);

	heading =   (double) ((int32_t)(((int8_t)data[1])<<8 | data[0]));
	roll =   (double) ((int32_t)(((int8_t)data[3])<<8 | data[2]));
	pitch =   (double) ((int32_t)(((int8_t)data[5])<<8 | data[4]));

	heading /=16;
	roll /= 16;
	pitch /= 16;

	//TODO: calibration code.
	if( calibrate == 1 ){
		reset();
		enableNDOF();
		calibrate = 2;
	}else if( calibrate == 2 && calibrationData == 0xff){
		enableConfig();
		readReg(0x55, data, 22);
		writeMemory(data, 22, sizeOfSettings - 23);
		data[0] = 1;
		writeMemory(data, 1, sizeOfSettings - 24);

		calibrate = 0;
		enableNDOF();
	}

}


void BNO055::initMemory(){
	readMemory(memorySettings, sizeOfSettings, 0);

	if( memorySettings[0] == 'I') return;

	uint8_t temp[sizeOfSettings] = {0};
	temp[1] = 'I';

	writeMemory(temp, sizeOfSettings, 0);
}


void BNO055::readMemory(unsigned char* data, int size, int location){
	location *= 8;

	for( int index = 0; index < size; index++){
		data[index] =  (unsigned int)(*(uint64_t*)(0x0801F800 + location));
		location += 8;
	}

}

void BNO055::writeMemory(unsigned char* data, int size, int location){

	readMemory(memorySettings, sizeOfSettings, 0);

	size = size > sizeOfSettings ? sizeOfSettings : size;
	location = location >= sizeOfSettings ? sizeOfSettings : location;

	for( int index = 0; index < size; index++){
		location = location >= sizeOfSettings ? sizeOfSettings : location;
		memorySettings[location++] = data[index];
	}

	FLASH_EraseInitTypeDef epage;
	epage.TypeErase = FLASH_TYPEERASE_PAGES;
	epage.Page = 63;
	epage.NbPages = 1;

	uint32_t error = 0;
	HAL_StatusTypeDef ret = HAL_FLASH_Unlock();

	ret = HAL_FLASHEx_Erase(&epage, &error);

	for( int index = 0; index < sizeOfSettings; index++){
		ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x0801F800 + (index * 8), memorySettings[index]);
	}

	ret = HAL_FLASH_Unlock();

}

void BNO055::enableConfig(){
	// TODO: calibration code.
	data[0] = 0;
	writeReg(0x3d, data, 1);
	HAL_Delay(19);

}
















