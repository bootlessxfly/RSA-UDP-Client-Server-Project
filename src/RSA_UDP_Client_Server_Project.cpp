//============================================================================
// Name        : RSA_UDP_Client_Server_Project.cpp
// Author      : Christoph White
// Version     :
// Copyright   : This was written for Comp6376 at Auburn. This should not be reused for this class
// Do not reuse this without reaching out to me first.
// Description : Refer to the github document for comments and documentation on implementation
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
					"\n\t3: Generate the perfect Match!!"
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
	else if (option == 3) {
		test();
	}
}

void test() {
	bool pass = false;
	RSA rsa;
	RSA rsa1;
	cout << "!!!Generating Perfect Match!!! ... Could take a while" << endl;
	while (!pass) {

		rsa = RSA(0,0);
		rsa1 = RSA(0,0);
		rsa.addServerPubKey(rsa1.getPublicKeyString());
		if (rsa.checkIfSame()) {
			continue;
		}
		rsa1.addServerPubKey(rsa.getPublicKeyString());
		if (rsa.checkIfSame()) {

			continue;
		}
		pass = rsa.testASCIIEncDec(rsa, rsa1);
	}

	rsa.printKeysAndPQ();
	rsa1.printKeysAndPQ();


}

