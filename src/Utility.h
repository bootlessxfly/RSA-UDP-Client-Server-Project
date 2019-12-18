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

	const char* badKey = "!";

	Utility();
	Utility(char *inFileName, int p, int q);
	virtual ~Utility();
	char* openFile();
	void writeFile(char *message, bool isSigned, bool isEncrypted);
	char* genTestMessage();
	bool valTestMessage(char* mess);
	void addOtherPubKeyToRSA(char* key);
	bool IsKeyCorrect();
	bool IsKeyReceived();
	char* getPubKeyString();

	bool checkSameKey();

	char* encryptMess(char* mess);
	char*decryptMess(char* cipher);

	void printKeys();



private:
	RSA rsa;
	const char* delim = ":";
	char *fileName;
};






#endif /* UTILITY_H_ */
