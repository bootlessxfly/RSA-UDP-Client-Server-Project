//============================================================================
// Name        : RSA.h
// Author      : Christoph White
// Version     :
// Copyright   : This was written for Comp6376 at Auburn. This should not be reused for this class
// Do not reuse this without reaching out to me first.
// Description : Refer to the doc in github for documentation
//=================

#ifndef RSA_H_
#define RSA_H_

#include "Key.h"

class RSA {
public:
	RSA();
	RSA(int p, int q);
	RSA(int n, int e, int d);
	virtual ~RSA();

	void addServerPubKey(char *pubKey);
	char* getPublicKeyString();
	bool getIsKeyGenerated();
	bool getIsKeyReceived();
	char* encrypt(int m);
	char* decrypt(int n);
	int encryptInt(int mNum);
	int decryptInt(int cNum);

	bool testASCIIEncDec(RSA rsa, RSA rsa1);

	bool checkIfSame();

	void printKeys();
	void printKeysAndPQ();

private:
	Key key;
	Key otherKey;

	int testE(int m);
	int testD(int m);

	bool testASCII(RSA rsa, RSA rsa1);

};

#endif /* RSA_H_ */
