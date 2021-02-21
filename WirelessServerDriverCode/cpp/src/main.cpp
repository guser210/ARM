/*
 * main.cpp
 *
 *  Created on: Feb 20, 2021
 *      Author: gvargas
 */

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "NRF24L01.h"

extern "C"{
	SPI_HandleTypeDef hspi1;
	TIM_HandleTypeDef htim1;
}

int maincpp(){

	NRF24L01 nrf(&hspi1, CSN_GPIO_Port, CE_GPIO_Port, CSN_Pin, CE_Pin);
	nrf.config();

	nrf.modeRX();

	uint8_t counter = 0;

	char msg[32];

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1500);

	double angle;
	double degPerTick = 2000 / 180;
	while(1){



		if( nrf.newMessage() == 1)
		        {
		            nrf.receiveMessage(msg);
		            angle = atoi(msg);
		            __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1500 + ( angle * degPerTick ));


		        }
		//		sprintf(msg, "NRF %d", counter++);
//		nrf.sendMessage(msg);
//		HAL_Delay(500);
//

	}
	return 0;
}


