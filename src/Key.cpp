//============================================================================
// Name        : Key.cpp
// Author      : Christoph White
// Version     :
// Copyright   : This was written for Comp6376 at Auburn. This should not be reused for this class
// Do not reuse this without reaching out to me first.
// Description : Refer to the doc in github for documentation
//=================

#include "Key.h"

#include <iostream>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <string>
#include <time.h>

using namespace std;

// Used so we can for the servers public Key
Key::Key() {
	n = 0;
	e = 0;
	d = 0;
}

// Manually setup key values.
Key::Key(int inN, int inE, int inD) {
	n = inN;
	e = inE;
	d = inD;

	isKeyCorrect = validateKey(n, e, d);
	if (!isKeyCorrect) {
		while(!isKeyCorrect) {
			generatePQ();
			buildKey();
		}
	}
	isKeyCorrect = true;

}

// Builds either a private or public key
Key::Key(int inP, int inQ) {
	p = inP;
	q = inQ;
	buildKey();
	if (isKeyCorrect) {
		cout << "Great!! You provided me with a valid p and q" << endl;
	}
	while (!isKeyCorrect) {
		generatePQ();
		buildKey();
	}

}

Key::Key(char *pubKey) {
	setPublicKeyFromString(pubKey);
}

Key::~Key() {
	// TODO Auto-generated destructor stub
}

void Key::buildKey() {
	if (p == 0 || q == 0) {
		return;
	}
	n = p * q;
	int z = (p - 1) * (q - 1);
	if (n <= 50) {
		return;
	}

	e = findPublicExponent(z, n);
	if (e == -1) {
		return;
	}
	d = findPrivateExponent(z, e);

	isKeyCorrect = validateKey(n, e, d);
}

int Key::findPublicExponent(int z, int n) {
	// The possible choices to start e from are the 1st 4 ferment numbers
	int exponents[5] = {3, 5, 17, 257, 655537};
	int rando = rand() % 5;
	int forceIterator = rand() % 2;
	int e = exponents[rando];
	if (forceIterator > 0) {
		// adds even more randomness
		e++;
	}
	if (e > z) {
		while (e > z) {
			rando = rand() % 4;
			e = exponents[rando];
		}
	}
	//int e = 2;
	int gcd;
	while (e < z) {
		gcd = findGCD(e, z);
		if (gcd == 1) {
			return e;
		} else {
			e++;
		}
	}
//	generatePQ();
//	buildKey();
	return -1;
}

int Key::findGCD(int e, int z) {
	int gcd;
	int denom1 = e;
	int denom2 = z;
	while (denom2 != 0) {
		if (denom1 < denom2) {
			//reverse the denomerators.
			gcd = denom1;
			denom1 = denom2;
			denom2 = gcd;
		}
		gcd = denom2;
		denom2 = denom1 % denom2;
		denom1 = gcd;

	}
	return denom1;
}

int Key::findPrivateExponent(int z, int e) {
	int d = 0;
	int k = rand() % 5 + 1;
	d = (1 + (k*z))/e;
	return d;
}

void Key::setPublicKeyFromString(char *pubKey) {
	size_t current, previous = 0;
	string key = (string) pubKey;
	current = key.find(pubKeyForm);

	n = stoi(key.substr(previous, current - previous));
	previous = current + 1;
	current = key.find(pubKeyForm, previous);
	e = stoi(key.substr(previous, current - previous));
	previous = current + 1;
	current = key.find(pubKeyForm, previous);
	if (current != string::npos) {
		// A valid Pubkey was passed and set.
		isKeyCorrect = true;
	}
	return;
}

char* Key::getPublicKeyString() {
	char *nStr = new char[sizeof(int)];
	char *eStr = new char[sizeof(int)];
	char *pubKey;
	sprintf(nStr, "%d", n);
	sprintf(eStr, "%d", e);
	pubKey = new char[strlen(nStr) + strlen(eStr) + strlen(pubKeyForm) + 1];
	strcpy(pubKey, nStr);
	strcat(pubKey, pubKeyForm);
	strcat(pubKey, eStr);
	delete nStr;
	delete eStr;
	return pubKey;
}

char* Key::getPrivateKeyString() {
	char *nStr = new char[2*sizeof(int)];
	char *dStr = new char[2*sizeof(int)];
	char *privKey;
	sprintf(nStr, "%d", n);
	sprintf(dStr, "%d", d);
	privKey = new char[strlen(nStr) + strlen(dStr) + strlen(pubKeyForm)];
	strcpy(privKey, nStr);
	strcat(privKey, pubKeyForm);
	strcat(privKey, dStr);
	delete nStr;
	delete dStr;
	return privKey;
}

int Key::getE() {
	return e;
}

int Key::getD() {
	return d;
}

int Key::getN() {
	return n;
}

bool Key::getIsKeyCorrect() {
	return isKeyCorrect;
}

void Key::printKeys() {
	cout << "Your generated Public key is: " << getPublicKeyString() << endl;
	cout << "Your generated Private key is: " << getPrivateKeyString() << endl;
}

void Key::printKeysAndPQ() {
	cout << "The generated p is: " << p << endl;
	cout << "The generated q is: " << q << endl;
	printKeys();
}

bool Key::validateKey(int n, int e, int d) {
	int m;
	int c;
	int m1;
	for (int i = 0; i < 127 ; i++) {
		m = i;
		c = mod(m, e, n);
		m1 = mod(c, d, n);
		int c1 = mod(m,d,n);
		int m2 = mod(c1,e,n);
		if (m1 != m && c1 != m2) {
			return false;
		}
	}
	return true;

}

void Key::generatePQ() {
	p = genRanNumPrime();
	q = genRanNumPrime();
	// This should be incredibly rare, but incase it does happen, we will regenerate the nums
	if (p == q) {
		return generatePQ();
	}
	if (p <= 2 || q <= 2) {
		return generatePQ();
	}

}

int Key::genRanNumPrime() {
	int upperLimit = 512 + 1;
	int num = rand() % upperLimit + 2;
	int m = num / 2;
	for (int i = 2; i < m;i++) {
		if (num % i == 0) {
			i = 2;
			num = num + 2;
			m = num / 2;
		}
	}
	return num;
}

int mod(int base, int exp, int mod) {
	int result = 1;
	for (int i = 0; i < exp; i++) {
		result = result * base % mod;
	}
	return result;
}


