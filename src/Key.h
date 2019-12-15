/*
 * key.h
 *
 *  Created on: Dec 14, 2019
 *      Author: chwhite
 */

#ifndef KEY_H_
#define KEY_H_

class Key {
public:
	Key();
	Key(int p, int q);
	Key(int p, int q, int e);
	Key(char *pubKey);
	virtual ~Key();

	int getN();
	// Returns d or e, depending if the object is a private key or public key
	int getExponent();
	bool getIsKeyCorrect();
	char* getPublicKeyString();

private:
	bool isPrivate = false;
	bool isKeyCorrect = false;

	int n;
	//Represents either d or e depending if the object is a private key or public key
	int exponent;

	const char *pubKeyForm = ",";

	void buildKey(int p, int q, int e);
	int findPublicExponent(int z, int n);
	int findGCD(int e, int z);
	int findPrivateExponent(int e, int z);
	void setPublicKeyFromString(char *pubKey);

};

#endif /* KEY_H_ */
