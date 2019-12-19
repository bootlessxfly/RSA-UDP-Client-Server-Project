/*
 * RSA.cpp
 *
 *  Created on: Dec 6, 2019
 *      Author: chwhite
 */

#include "RSA.h"
#include "Key.h"


#include <iostream>
#include <math.h>
#include <string.h>

using namespace std;

RSA::RSA() {

}

RSA::RSA(int p, int q) {
	//srand(time(NULL));
	key = Key(p, q);
	if (!key.getIsKeyCorrect()) {
		return;
	}

}

RSA::RSA(int n, int e, int d) {
	//srand(time(NULL));
	key = Key(n, e, d);
}

RSA::~RSA() {
	// TODO Auto-generated destructor stub
}

char* RSA::encrypt(int m) {
	char* mess = new char[sizeof(int)];
	sprintf(mess, "%d", encryptInt(m));

	return mess;
}

char* RSA::decrypt(int c) {
//	int n = stoi(c);
	char a = static_cast<char>(decryptInt(c));
	char b[2];
	b[0] = a;
 	char* m = new char[2];
	strcpy(m,b);
	return m;
	//return 0;
}


bool RSA::checkIfSame() {
	if (key.getN() == otherKey.getN() && key.getE() == otherKey.getE()) {
		return true;
	}
	return false;
}

void RSA::printKeys() {
	key.printKeys();
	cout << "The other person public key is: " << otherKey.getPublicKeyString() << endl;
}

void RSA::printKeysAndPQ() {
	key.printKeysAndPQ();
	cout << "The other person public key is: " << otherKey.getPublicKeyString() << endl;
}

int RSA::testE(int m) {
	int test = m;
	test = encryptInt(test);
	return test;
}

int RSA::testD(int m) {
	int n = decryptInt(m);
	return n;
}

int RSA::encryptInt(int mInt) {
	int result = 1;
	int n = key.getN();
	int exponent = key.getD();
	result = mod(mInt, exponent, n);
	n = otherKey.getN();
	exponent = key.getE();
	result = mod(result, exponent, n);
	return result;
}

int RSA::decryptInt(int cNum) {
	int n = key.getN();
	int exponent = key.getD();
	int result = mod(cNum, exponent, n);
	n = otherKey.getN();
	exponent = otherKey.getE();
	return mod(result, exponent, n);
}



char* RSA::getPublicKeyString() {
	return key.getPublicKeyString();
}

void RSA::addServerPubKey(char *pubKey) {
	otherKey = Key(pubKey);
}

bool RSA::testASCIIEncDec(RSA rsa, RSA rsa1) {
	if (testASCII(rsa, rsa1)) {
		return true;
	}
	else {
		return false;
	}
}

bool RSA::testASCII(RSA rsa, RSA rsa1) {
	bool passed = true;
	for (int i = 33; i < 127 ; i++) {
		int m = rsa.testE(i);
		int n = rsa1.testD(m);
		if (i == n) {
		}
		else {
			passed = false;
		}
	}

	for (int i = 33; i < 127 ; i++) {
		int m = rsa1.testE(i);
		int n = rsa.testD(m);
		if (i == n) {
		}
		else {
			passed = false;
		}
	}
	return passed;
}

bool RSA::getIsKeyGenerated() {
	return key.getIsKeyCorrect();
}

bool RSA::getIsKeyReceived() {
	return otherKey.getIsKeyCorrect();
}

