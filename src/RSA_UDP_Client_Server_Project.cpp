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

int main() {
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

