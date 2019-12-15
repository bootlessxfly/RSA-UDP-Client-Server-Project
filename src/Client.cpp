/*
 * Client.cpp
 *
 *  Created on: Dec 5, 2019
 *      Author: chwhite
 */

#include "Client.h"
#include "Utility.h"
#include "RSA.h"

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <limits.h>
#include <string.h>

using namespace std;

Client::Client() {
	ip_addr = new char[100];
	port = -1;
	fileName = new char[PATH_MAX];
	p = 0;
	q = 0;
}

Client::~Client() {
	// TODO Auto-generated destructor stub
	delete ip_addr;
	delete fileName;
}

void Client::init() {
	int response;
	cout
			<< "\n\nThe client application is starting, please follow on screen prompt"
			<< endl;
	cout << "Please enter the information for generating the key pair below: "
			<< endl;
	cout << "Enter p: ";
	cin >> p;
	cout << "Enter q: ";
	cin >> q;
	cout
			<< "Please keep record of these values if you would like to able to reuse the values"
			<< endl;
	cout
			<< "Please enter the fileName of the contract you would like to send: ";
	cin >> fileName;
	cout << "Please enter the IP Address of the server: ";
	cin >> ip_addr;
	cout << "Please enter the port Number of the server: ";
	cin.clear();
	cin >> port;
	cout << endl;

	response = runClient();
	if (response == 0) {
		cout
				<< "Comunication with contract server was successful. Check the log to see what server sent back"
				<< endl;
	} else {
		cout
				<< "Something went wrong in the communication with server, check log to determine where the issue occured"
				<< endl;
	}
}

int Client::runClient() {
	RSA rsa = RSA(p,q);
	if (!rsa.getIsKeyGenerated()) {
		cout << "There was an issue generating the keys, check your p and q. Otherwise file a bug." << endl;
		return -1;
	}
	Utility util = Utility(fileName);
	int response; //response of sending and receiving
	char *message = util.openFile();
	// The buffer size will be the size of the message being sent, plus 200 bytes buffer space for the extra response from the contract server.
	char buffer[strlen(message) + 200];
	if (message == NULL) {
		return -1;
	}
	int sockfd;
	struct sockaddr_in servaddr;

	// set servaddr
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_addr.s_addr = inet_addr(ip_addr);
	servaddr.sin_port = htons(port);
	servaddr.sin_family = AF_INET;

	// create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	// connect to server
	if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
		printf("\n Error : Connect Failed. \n");
		return -1;
	}

	// Send the message
	response = sendto(sockfd, message, strlen(message) + 1, 0,
			(struct sockaddr*) NULL, sizeof(servaddr));
	if (response == -1) {
		cout << "Unable to send message to server" << endl;
		return -1;
	}

	// waiting for response.
	response = recvfrom(sockfd, buffer, sizeof(buffer), 0,
			(struct sockaddr*) NULL, NULL);
	if (response == -1) {
		cout << "Unable to retrieve message from server." << endl;

		return -1;
	}
	buffer[response] = 0;
	puts(buffer);
	return 0;
}

