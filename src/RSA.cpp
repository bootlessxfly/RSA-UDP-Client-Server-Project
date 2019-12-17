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
	srand(time(NULL));
	key = Key(p, q);
	if (!key.getIsKeyCorrect()) {
		return;
	}

}

RSA::RSA(int n, int e, int d) {
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

char* RSA::decrypt(char* c) {
	return 0;
}




int RSA::testE(int m) {
	int test = m;
	cout << "Initial Message: " << test << endl;
	test = encryptInt(test);
	cout << "Generated Cipher is: " << test << endl;
	return test;
}

int RSA::testD(int m) {
//	cout << "Cipher is: " << m << endl;
	int n = decryptInt(m);
	cout << "Message is: " << n << endl;
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
		cout << "PASSED!!" << endl;
		return true;
	}
	else {
		cout << "FAIL!!" << endl;
		return false;
	}
}

bool RSA::testASCII(RSA rsa, RSA rsa1) {
	bool passed = true;
	for (int i = 33; i < 127 ; i++) {
		int m = rsa.testE(i);
		int n = rsa1.testD(m);
		if (i == n) {
			cout << i << " has passed" << endl;
		}
		else {
			passed = false;
			cout << i << " has FAILED" << endl;
		}
	}

	for (int i = 33; i < 127 ; i++) {
		int m = rsa1.testE(i);
		int n = rsa.testD(m);
		if (i == n) {
			cout << i << " reversed has passed" << endl;
		}
		else {
			passed = false;
			cout << i << " reversed FAILED" << endl;
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

