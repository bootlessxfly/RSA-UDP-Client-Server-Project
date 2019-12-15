/*
 * RSA.h
 *
 *  Created on: Dec 6, 2019
 *      Author: chwhite
 */

#ifndef RSA_H_
#define RSA_H_

#include "Key.h"

class RSA {
public:
	RSA(int p, int q);
	virtual ~RSA();

	void addServerPubKey(char *pubKey);
	bool getIsKeyGenerated();
	bool getIsKeyReceived();

private:
	Key privateKey;
	Key publicKey;
	Key otherPubKey;

	bool isKeyGenerated = false;
	bool isKeyReceived = false;
};

#endif /* RSA_H_ */
