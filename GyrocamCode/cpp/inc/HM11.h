/*
 * HM11.h
 *
 *  Created on: Feb 14, 2021
 *      Author: gvargas
 */

#ifndef HM11_H_
#define HM11_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class HM11 {

private: // vars
	int characterCount = 0;
	char buffer[50];
	char message[50];

public: // vars
	int newMessage = 0;
	char field[50];
	char valueString[50];
	int value;

public: // funcs
	void addCharacter(char c);
	void parseCommand();
public:
	HM11();
	virtual ~HM11();
};

#endif /* HM11_H_ */
