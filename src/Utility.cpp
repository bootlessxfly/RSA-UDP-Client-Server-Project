/*
 * Utility.cpp
 *
 *  Created on: Dec 6, 2019
 *      Author: chwhite
 */

#include "Utility.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <string.h>
#include <strings.h>

using namespace std;

Utility::Utility() {
	fileName = new char[1];
}

Utility::Utility(char *inFileName, int p, int q) {
	// TODO Auto-generated constructor stub
	fileName = inFileName;
	rsa = RSA(p,q);
}

Utility::~Utility() {
	// TODO Auto-generated destructor stub
	// No need to delete file, as it is owned by another object.
	// Either client or server will clean up its own file pointer.
}

char* Utility::openFile() {
	char *message;
	string line;
	string lineBuffer = "";
	ifstream f(fileName);
	if (!f.is_open()) {
		cout << "The file: " << fileName << " can not be opened" << endl;
		return NULL; //returns null if file can not be opened.
	}
	while (getline(f, line)) {
		lineBuffer = lineBuffer + line;
	}

	message = new char[lineBuffer.size() + 1];
	strcpy(message, lineBuffer.c_str());

	return message;
}

void Utility::writeFile(char *message) {
	ofstream file(fileName);
	file.write(message, strlen(message));
	file.close();

}

bool Utility::valTestMessage(char* mess) {
	bool pass = true;
	size_t current, previous = 0;
	int i = 0;
	int n;
	string key = (string) mess;
	current = key.find(delim);
	while (current != string::npos) {
		n = stoi(key.substr(previous, current - previous));
		previous = current + 1;
		current = key.find(Utility::delim, previous);
		n = rsa.decryptInt(n);
		cout << n << endl;
		if (n != i) {
			pass = false;
		}
		i++;
	}

	return pass;
}

void Utility::addOtherPubKeyToRSA(char* key) {
	rsa.addServerPubKey(key);
}

bool Utility::IsKeyCorrect() {
	return rsa.getIsKeyGenerated();
}

bool Utility::IsKeyReceived() {
	return rsa.getIsKeyReceived();
}

char* Utility::getPubKeyString() {
	return rsa.getPublicKeyString();
}

char* Utility::genTestMessage() {
	char* mess = new char[(128* sizeof(int)) + (128 * sizeof(char))];
	char* c;
	for (int i = 0; i < 128; i++) {
		if (i == 0) {
			c = rsa.encrypt(i);
			strcat(c,Utility::delim);
		}
		else {
			strcat(c, rsa.encrypt(i));
			strcat(c,Utility::delim);
		}
//		cout << c << endl;
	}
	cout << "The test string is: " << c << endl;
	return c;
}

char* Utility::encryptMess(char* mess) {
	char* message = new char[strlen(mess) * 4];
	char* c;
	int cm;
	for (int i = 0; i < strlen(mess) ; i++) {
		cm = mess[i];
		c = rsa.encrypt(cm);
		if (i == 0) {
			strcpy(message, c);
			strcat(message, delim);
		}
		else {
			strcat(message, c);
			// add something to keep this from adding delim at end
			strcat(message, delim);
		}
	}

	return message;
}

