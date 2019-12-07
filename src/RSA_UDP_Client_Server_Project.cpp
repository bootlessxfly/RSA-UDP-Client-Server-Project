//============================================================================
// Name        : RSA_UDP_Client_Server_Project.cpp
// Author      : Christoph White
// Version     :
// Copyright   : This was written for Comp6376 at Auburn. This should not be reused for this class
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Server.h"
#include "Client.h"


using namespace std;

int interface();

//int runServer();

//int runClient();

int main() {

	int option = interface();
	int result;
	Server server;
	Client client;

    if (option == 1) {
    	//launch server
    	server.init();

    }
    else if (option == 2) {
    	//launch client
    	client.init();
    }

	return 0;
}

int interface() {
	int option = 0;
	cout << "!!!Welecome to the RSA UDP Messenger!!!!!"
			"\n Please enter one of the following options"
			"\n\t1: To launch as server"
			"\n\t2: If you want to communicate with another server as the client"
			"\n\tEnter your option: ";
	cin >> option;

	cout << "\tOption is: " << option << endl;


	return option;
}


