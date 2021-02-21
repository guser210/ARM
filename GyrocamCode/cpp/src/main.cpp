/*
 * main.cpp
 *
 *  Created on: Feb 14, 2021
 *      Author: gvargas
 */

#include "main.h"
#include "HM11.h"
#include "BNO055.h"
#include "NRF24L01.h"
extern "C" {
	void log_i(char *data, ...);
	I2C_HandleTypeDef hi2c1;
	SPI_HandleTypeDef hspi1;
}

HM11 phone;

BNO055 gyro(&hi2c1);

void rxChar(char c){

	phone.addCharacter(c);

}


uint8_t data[22];
void maincpp(){
	gyro.reset();
	gyro.enableNDOF();

	NRF24L01 nrf(&hspi1, CSN_GPIO_Port, CE_GPIO_Port,CSN_Pin,CE_Pin);
	nrf.config();

	nrf.modeTX();

	char msg[32];


	while(1){

		gyro.getData();


		log_i("R:%.2f\r",gyro.roll);

		  sprintf(msg,"%.2f",gyro.roll);
		        msg[30] = 'R';
		        msg[31] = 'O';

		        nrf.sendMessage(msg);


		//log_i("H:%.2f\rR:%.2f\rP:%.2f\rC:%d\rCm:%d\rCa:%d\rCg:%d\rCs:%d\rCF:%d\r", gyro.heading, gyro.roll,gyro.pitch,gyro.calibrationData,gyro.calMag,gyro.calAcc,gyro.calGyr,gyro.calSys,gyro.calibrate);
		if( phone.newMessage == 1){
			HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_SET);
			phone.parseCommand();

			if( strcmp(phone.field,"Calibrate") == 0 ){
				gyro.calibrate = phone.value;
			}

			HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_RESET);
		}
	}
}



