/*
 * RSA.cpp
 *
 *  Created on: Dec 6, 2019
 *      Author: chwhite
 */

#include "RSA.h"
#include "Key.h"

#include <iostream>

using namespace std;

RSA::RSA(int p, int q) {
	publicKey = Key(p, q);
	if (!publicKey.getIsKeyCorrect()) {
		return;
	}
	privateKey = Key(p, q, publicKey.getExponent());
	if (publicKey.getIsKeyCorrect() && privateKey.getIsKeyCorrect()) {
		isKeyGenerated = true;
	}

}

RSA::~RSA() {
	// TODO Auto-generated destructor stub
}

void RSA::addServerPubKey(char *pubKey) {
	otherPubKey = Key(pubKey);
	if (otherPubKey.getIsKeyCorrect()) {
		isKeyReceived = true;
	}
}

bool RSA::getIsKeyGenerated() {
	return isKeyGenerated;
}

bool RSA::getIsKeyReceived() {
	return isKeyReceived;
}

