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

using namespace std;

Utility::Utility(char *inFileName) {
	// TODO Auto-generated constructor stub
	fileName = inFileName;
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

