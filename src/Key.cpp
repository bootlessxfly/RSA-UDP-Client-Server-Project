/*
 * key.cpp
 *
 *  Created on: Dec 14, 2019
 *      Author: chwhite
 */

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

using namespace std;

// Used so we can for the servers public Key
Key::Key() {
	n = 0;
	exponent = 0;
}

// Builds either a private or public key
Key::Key(int p, int q) {
//	if (p == q) {
//		cout << "p and q are the same, this is not allowed in this implementation." << endl;
//		return;
//	}
	isPrivate = false;

	buildKey(p, q, 0);

}

Key::Key(int p, int q, int e) {
	isPrivate = true;

	buildKey(p, q, e);

}

Key::Key(char *pubKey) {
	setPublicKeyFromString(pubKey);
}

Key::~Key() {
	// TODO Auto-generated destructor stub
}

void Key::buildKey(int p, int q, int e) {
	n = p * q;
	int z = (p - 1) * (q - 1);
	if (n <= 50) {
		cout
				<< "The values you entered for p and q generated n that is not greater than 50. This is not strong enough. Try again"
				<< endl;
		isKeyCorrect = false;
		return;
	}

	if (isPrivate) {
		cout << "Generating the Private Key: " << endl;
		exponent = findPrivateExponent(z, e);
		if (exponent > -1) {
			isKeyCorrect = true;
		}
	} else {
		cout << "Generating the Public Key: " << endl;
		exponent = findPublicExponent(p, q, z, n);
		if (exponent > -1) {
			isKeyCorrect = true;
		}
	}
}

int Key::findPublicExponent(int p, int q, int z, int n) {
	int e = 2; // The lowest number that e could start out with
	int gcdTemp;
	int denom1 = e;
	int denom2 = z;
	while (e < n) {
		denom1 = e;
		denom2 = z;
		while (true) {
			gcdTemp = denom1 % denom2;
			if (gcdTemp == 0) {
				break;
			}
			denom1 = denom2;
			denom2 = gcdTemp;

		}
		if (denom2 == 1) {
			cout << e << endl;
			return e;
		} else {
			e++;
		}
	}
	cout << "Nothing Found" << endl;
	return -1;
}

int Key::findPrivateExponent(int z, int e) {
	int d = 0;
	while (true) {
		if (fmod(e * d, z) == 1) {
			cout << d << endl;
			return d;
		}
		d++;
	}

	return -1;
}

void Key::setPublicKeyFromString(char *pubKey) {
	size_t current, previous = 0;
	string key = (string) pubKey;
	current = key.find(pubKeyForm);

	n = stoi(key.substr(previous, current - previous));
	previous = current + 1;
	current = key.find(pubKeyForm, previous);
	exponent = stoi(key.substr(previous, current - previous));
	previous = current + 1;
	current = key.find(pubKeyForm, previous);
	if (current != string::npos) {
		// A valid Pubkey was passed and set.
		isKeyCorrect = true;
	}
	return;
}

char* Key::getPublicKeyString() {
	if (isPrivate) {
		// This can only be used for public keys;
		return 0;
	}
	char *nStr = new char[sizeof(int)];
	char *eStr = new char[sizeof(int)];
	char *pubKey;
	sprintf(nStr, "%d", n);
	sprintf(eStr, "%d", exponent);
	pubKey = new char[strlen(nStr) + strlen(eStr) + strlen(pubKeyForm)];
	strcpy(pubKey, nStr);
	strcat(pubKey, pubKeyForm);
	strcat(pubKey, eStr);
	return pubKey;
}

int Key::getExponent() {
	return exponent;
}

bool Key::getIsKeyCorrect() {
	return isKeyCorrect;
}

