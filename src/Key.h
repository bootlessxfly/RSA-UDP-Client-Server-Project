//============================================================================
// Name        : Key.h
// Author      : Christoph White
// Version     :
// Copyright   : This was written for Comp6376 at Auburn. This should not be reused for this class
// Do not reuse this without reaching out to me first.
// Description : Refer to the doc in github for documentation
//=================

#ifndef KEY_H_
#define KEY_H_

class Key {
public:

	Key();
	Key(int n, int e, int d);
	Key(int p, int q);
	Key(char *pubKey);
	virtual ~Key();

	int getN();
	// Returns d or e, depending if the object is a private key or public key
	int getE();
	int getD();
	bool getIsKeyCorrect();
	char* getPublicKeyString();

	void printKeys();

	void printKeysAndPQ();

private:
	bool isKeyCorrect = false;

	int n = 0;

	int e = 0;

	int d = 0;

	int p = 0;

	int q = 0;

	const char *pubKeyForm = ",";


	char* getPrivateKeyString();
	void buildKey();
	int findPublicExponent(int z, int n);
	int findGCD(int e, int z);
	int findPrivateExponent(int e, int z);
	void setPublicKeyFromString(char *pubKey);
	void generatePQ();
	int genRanNumPrime();

	bool validateKey(int n, int e, int d);

};

int mod(int base, int exp, int mod);



#endif /* KEY_H_ */
