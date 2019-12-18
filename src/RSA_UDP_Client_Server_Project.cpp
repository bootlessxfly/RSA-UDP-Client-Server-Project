//============================================================================
// Name        : RSA_UDP_Client_Server_Project.cpp
// Author      : Christoph White
// Version     :
// Copyright   : This was written for Comp6376 at Auburn. This should not be reused for this class
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Server.h"
#include "Client.h"
#include "RSA.h"

#include <iostream>

using namespace std;

void interface();

void test();

int main() {
	srand(time(NULL));
	//test();
	interface();

	return 0;
}

void interface() {
	int option = 0;
	Server server;
	Client client;
	cout
			<< "!!!Welecome to the RSA UDP Messenger!!!!!"
					"\n Please enter one of the following options"
					"\n\t1: To launch as server"
					"\n\t2: If you want to communicate with another server as the client"
					"\n\tEnter your option: ";
	cin >> option;

	cout << "\tOption is: " << option << endl;

	if (option == 1) {
		//launch server
		server.init();

	} else if (option == 2) {
		//launch client
		client.init();
	}
}

void test() {

	// This ar valid values
	//RSA rsa = RSA(46,67);
	//RSA rsa1 = RSA(89,91);

//	RSA rsa = RSA(4,67);
//	RSA rsa1 = RSA(89,91);


	bool pass = false;
	while (!pass) {
		cout << "\n!!!Next!!!" << endl;
//		RSA rsa = RSA(151,3);
//		RSA rsa1 = RSA(5,91);
		RSA rsa = RSA(0,0);
		RSA rsa1 = RSA(0,0);
		rsa.addServerPubKey(rsa1.getPublicKeyString());
		if (rsa.checkIfSame()) {
			continue;
		}
		rsa1.addServerPubKey(rsa.getPublicKeyString());
		if (rsa.checkIfSame()) {

			continue;
		}
		cout << "Other pub key: " << rsa1.getPublicKeyString() << endl;
		cout << "Other pub key2: " << rsa.getPublicKeyString() << endl;
		pass = rsa.testASCIIEncDec(rsa, rsa1);
	}
}

