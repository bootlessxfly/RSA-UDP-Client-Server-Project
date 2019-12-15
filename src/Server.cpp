/*
 * Server.cpp
 *
 *  Created on: Dec 5, 2019
 *      Author: chwhite
 */

#include "Server.h"
#include "Utility.h"
#include "RSA.h"

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <limits.h>

using namespace std;

Server::Server() {
	// TODO Auto-generated constructor stub
	port = -1;
	fileName = new char[PATH_MAX];
	p = 0;
	q = 0;
}

Server::~Server() {
	// TODO Auto-generated destructor stub
	delete fileName;
}

void Server::init() {
	cout << "Starting the server side application" << endl;
	cout << "Please enter the information for generating the key pair below: "
			<< endl;
	cout << "Enter p: ";
	cin >> p;
	cout << "Enter q: ";
	cin >> q;
	cout
			<< "Please keep record of these values if you would like to able to reuse the values"
			<< endl;
	cout << "Please enter the filename you would like to write to: ";
	cin >> fileName;
	cout << "Please enter the port that this server will listen on: ";
	cin.clear();
	cin >> port;
	cout << endl;

	int response = runServer();

	if (response == 0) {
		cout
				<< "Comunication with contract client was successful. Check the log to see what server sent back"
				<< endl;
	} else {
		cout
				<< "Something went wrong in the communication with client, check log to determine where the issue occured"
				<< endl;
	}

}

int Server::runServer() {
	RSA rsa = RSA(p,q);
	if (!rsa.getIsKeyGenerated()) {
		cout << "There was an issue generating the keys, check your p and q. Otherwise file a bug." << endl;
		return -1;
	}
	char buffer[10000];
	Utility util = Utility(fileName);
	const char *message =
			"Hello Client, message received. Here is my singed message: \n";
	const char *signed_message = " Signed Bob(Seller) Date: November 24, 2019.";
	char *full_signed_message;
	char *full_response;
	int listenfd;
	socklen_t len;
	int response;
	struct sockaddr_in servaddr, cliaddr;
	bzero(&servaddr, sizeof(servaddr));

	// Create socket
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	servaddr.sin_family = AF_INET;

	// bind server
	bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

	//receive public key

	//send public key

	//receive messages
	len = sizeof(cliaddr);
	response = recvfrom(listenfd, buffer, sizeof(buffer), 0,
			(struct sockaddr*) &cliaddr, &len); //receive message from client
	if (response == -1) {
		cout
				<< "There was an error receiving the message from the client, killing server. Please restart"
				<< endl;
		return -1;
	}
	puts(buffer);
	full_signed_message = new char[strlen(buffer) + strlen(signed_message)];
	strcpy(full_signed_message, buffer);
	strcat(full_signed_message, signed_message);
	util.writeFile(full_signed_message);
	//Sign message
	full_response = new char[strlen(full_signed_message) + strlen(message)];

	// Build response
	strcpy(full_response, message);
	strcat(full_response, full_signed_message);
	puts(full_response);
	// send confirmation
	response = sendto(listenfd, full_response, strlen(full_response), 0,
			(struct sockaddr*) &cliaddr, sizeof(cliaddr));
	if (response = -1) {
		cout << "There was an issue sending the message back to the client" << endl;
		return -1;
	}

	return 0;

}

