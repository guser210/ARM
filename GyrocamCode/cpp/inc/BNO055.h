/*
 * BNO055.h
 *
 *  Created on: Feb 14, 2021
 *      Author: gvargas
 */

#ifndef BNO055_H_
#define BNO055_H_
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class BNO055 {

private: // vars
	I2C_HandleTypeDef* pDevice;
	uint8_t slave_addr;
	uint8_t data[22];

	int sizeOfSettings = sizeof(memorySettings);
public: // vars
	double heading;
	double pitch;
	double roll;

	uint8_t calibrationData;
	uint8_t calMag;
	uint8_t calAcc;
	uint8_t calGyr;
	uint8_t calSys;

	uint8_t memorySettings[32];
	//TODO: calibration code.
	uint8_t calibrate = 0;


public: // funcs
	int readReg(unsigned char reg, unsigned char *data, uint8_t size );
	int writeReg(unsigned char reg, unsigned char *data, uint8_t size);

	void reset();
	void enableNDOF();

	void getData();

	void initMemory();
	void readMemory(unsigned char* data, int size, int location);
	void writeMemory(unsigned char* data, int size, int location);

	//TODO: calibration code.
	void enableConfig();
public:
	BNO055(I2C_HandleTypeDef* pDev, uint8_t slave = 0x29);
	virtual ~BNO055();
};

#endif /* BNO055_H_ */
