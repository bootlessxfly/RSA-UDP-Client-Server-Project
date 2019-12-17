/*
 * Client.cpp
 *
 *  Created on: Dec 5, 2019
 *      Author: chwhite
 */

#include "Client.h"
#include "Utility.h"

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
//	cout
//			<< "Please enter the fileName of the contract you would like to send: ";
//	cin >> fileName;
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

int Client::getPublicKeyExchange(int sockfd, socklen_t len) {
		int response = 0;
		char* pubKey = new char[sizeof(char) + 2*sizeof(int)];
		pubKey = util.getPubKeyString();
		if (pubKey == 0) {
			cout << "There was an error building the public key string. This is most likely a bug." << endl;
			return -1;
		}
		response = sendto(sockfd, pubKey, strlen(pubKey) + 1, 0,
				(struct sockaddr*) NULL, len);
		if (response == -1) {
			cout << "Unable to send publicKey to server to server" << endl;
			return -1;
		}
		cout << "Public key was sent to the server, waiting for server's public key..." << endl;
		// Will reuse this variable to receive the servers public key
		pubKey = new char[sizeof(char) + 2*sizeof(int)];
		// The key is format is size int + char + int
		response = recvfrom(sockfd, pubKey, sizeof(pubKey), 0,
				(struct sockaddr*) NULL, NULL);
		if (response == -1) {
			cout << "Unable to retrieve pubKey from the server server." << endl;

			return -1;
		}
		pubKey[response] = 0;

		util.addOtherPubKeyToRSA(pubKey);
		if (!util.IsKeyReceived()) {
			cout << "There was an issue storing the public key. If the public key was not sent in a bad format, this is a bug." << endl;
			return -1;
		}

		cout << "Public key received and stored. Public key is: " << pubKey << endl;

		return 0;
}

// Used for testing the encryption
//char* Client::genTestMessage(RSA rsa) {
//	char* mess = new char[(128* sizeof(int)) + (128 * sizeof(char))];
//	char* c;
//	for (int i = 0; i < 128; i++) {
//		if (i == 0) {
//			c = rsa.encrypt(i);
//			strcat(c,Utility::delim);
//		}
//		else {
//			strcat(c, rsa.encrypt(i));
//			strcat(c,Utility::delim);
//		}
////		cout << c << endl;
//	}
//	cout << "The test string is: " << c << endl;
//	return c;
//}

int Client::runClient() {
//	RSA rsa = RSA(p,q);
	char* testMess;
	int response; //response of sending and receiving
//	if (!rsa.getIsKeyGenerated()) {
//		cout << "There was an issue generating the keys, check your p and q. Otherwise file a bug." << endl;
//		return -1;
//	}
	// commented out for testing
	util = Utility(fileName, p, q);
	if (!util.IsKeyCorrect()) {
		cout << "There was an issue generating the keys, check your p and q. Otherwise file a bug." << endl;
		return -1;
	}
//
//	char *message = util.openFile();
	// The buffer size will be the size of the message being sent, plus 200 bytes buffer space for the extra response from the contract server.
//	char buffer[strlen(message) + 200];
//	if (message == NULL) {
//		return -1;
//	}
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

	// Send the public key to server
//	pubKey = util.getPubKeyString();
//	if (pubKey == 0) {
//		cout << "There was an error building the public key string. This is most likely a bug." << endl;
//		return -1;
//	}
//	response = sendto(sockfd, pubKey, strlen(pubKey) + 1, 0,
//			(struct sockaddr*) NULL, sizeof(servaddr));
//	if (response == -1) {
//		cout << "Unable to send publicKey to server to server" << endl;
//		return -1;
//	}
//	cout << "Public key was sent to the server, waiting for server's public key..." << endl;
//	// Will reuse this variable to receive the servers public key
//	pubKey = new char[sizeof(char) + 2*sizeof(int)];
//	// The key is format is size int + char + int
//	response = recvfrom(sockfd, pubKey, sizeof(pubKey), 0,
//			(struct sockaddr*) NULL, NULL);
//	if (response == -1) {
//		cout << "Unable to retrieve pubKey from the server server." << endl;
//
//		return -1;
//	}
//	pubKey[response] = 0;
//
//	util.addOtherPubKeyToRSA(pubKey);
//	if (!util.IsKeyReceived()) {
//		cout << "There was an issue storing the public key. If the public key was not sent in a bad format, this is a bug." << endl;
//		return -1;
//	}
//
//	cout << "Public key received and stored. Public key is: " << pubKey << endl;
	response = getPublicKeyExchange(sockfd, sizeof(servaddr));
	testMess = util.genTestMessage();
	response = sendto(sockfd, testMess, strlen(testMess) + 1, 0,
			(struct sockaddr*) NULL, sizeof(servaddr));

	response = recvfrom(sockfd, testMess, 10000, 0,
			(struct sockaddr*) NULL, NULL);

	testMess[response] = 0;

	if (util.valTestMessage(testMess)) {
		cout << "We pass!!" << endl;
	}
	else {
		cout << "We fail!!" << endl;
	}






	// Send the message
//	response = sendto(sockfd, message, strlen(message) + 1, 0,
//			(struct sockaddr*) NULL, sizeof(servaddr));
//	if (response == -1) {
//		cout << "Unable to send message to server" << endl;
//		return -1;
//	}
//
//	// waiting for response.
//	response = recvfrom(sockfd, buffer, sizeof(buffer), 0,
//			(struct sockaddr*) NULL, NULL);
//	if (response == -1) {
//		cout << "Unable to retrieve message from server." << endl;
//
//		return -1;
//	}
//	buffer[response] = 0;
//	puts(buffer);
	return 0;
}

