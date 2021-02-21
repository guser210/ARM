/*
 * HM11.cpp
 *
 *  Created on: Feb 14, 2021
 *      Author: gvargas
 */

#include "HM11.h"


HM11::HM11() {
	// TODO Auto-generated constructor stub

}

HM11::~HM11() {
	// TODO Auto-generated destructor stub
}

void HM11::addCharacter(char c){

	if( c == '\r' || c == '\n' || characterCount >= 50){
		memcpy(message,buffer, sizeof(buffer));
		memset(buffer, 0, sizeof(buffer));
		newMessage = 1;
		characterCount = 0;
	}else{
		buffer[characterCount++] = c;
	}
}

void HM11::parseCommand(){
	char temp[50];

	memcpy(temp,message,sizeof(message));

	char* ptr = strtok(temp,":");

	sprintf(field,"%s",ptr);

	ptr = strtok(NULL,":");

	sprintf(valueString,"%s",ptr);

	value = atoi(valueString);

	newMessage = 0;

}
