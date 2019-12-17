/*
 * Utility.h
 *
 *  Created on: Dec 6, 2019
 *      Author: chwhite
 */

#include "RSA.h"

#ifndef UTILITY_H_
#define UTILITY_H_

class Utility {
public:
	Utility();
	Utility(char *inFileName, int p, int q);
	virtual ~Utility();
	char* openFile();
	void writeFile(char *message);
	char* genTestMessage();
	bool valTestMessage(char* mess);
	void addOtherPubKeyToRSA(char* key);
	bool IsKeyCorrect();
	bool IsKeyReceived();
	char* getPubKeyString();



private:
	RSA rsa;
	const char* delim = ":";
	char *fileName;
};






#endif /* UTILITY_H_ */
