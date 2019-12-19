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

bool Client::validateKeys(int sockfd, socklen_t len) {
	char* testMess;
	int response;
	bool isCorrect = false;

	testMess = util.genTestMessage();
	response = sendto(sockfd, testMess, strlen(testMess) + 1, 0,
			(struct sockaddr*) NULL, len);
	if (response == -1) {
		cout << "Error sending a validation the server" << endl;
		exit(-1);
	}

	response = recvfrom(sockfd, testMess, 10000, 0,
			(struct sockaddr*) NULL, NULL);

	if (response == -1) {
		cout << "Error receiving a validation from the server" << endl;
		exit(-1);
	}

	testMess[response] = 0;

	if (strcmp(testMess, util.badKey) == 0) {
		getPublicKeyExchange(sockfd, len, true);
		return false;
	}

	if (util.valTestMessage(testMess)) {
		if (testMess[strlen(testMess) - 1] == ':') {
			if (util.checkSameKey()) {
				response = sendto(sockfd, util.badKey, strlen(util.badKey), 0,
						(struct sockaddr*) NULL, len);
				if (response == -1) {
					cout << "Error sending we NACK to server" << endl;
				}
				testMess = new char[(128* sizeof(int)) + (128 * sizeof(char))];
				response = recvfrom(sockfd, testMess, 10000, 0,
						(struct sockaddr*) NULL, NULL);
				if (response == -1) {
					cout << "Server closed but we have a bad key!!, this is a bug" << endl;
					exit(-1);
				}
				getPublicKeyExchange(sockfd, len, true);
				return false;
			}
			cout << "Server has ACKED us hard" << endl;
			response = sendto(sockfd, 0, 0, 0,
					(struct sockaddr*) NULL, len);
			// Send an ack?
			return true;
		}
		isCorrect = true;
	}
	else {
		response = sendto(sockfd, util.badKey, strlen(util.badKey), 0,
				(struct sockaddr*) NULL, len);
		if (response == -1) {
			cout << "Error sending we NACK to server" << endl;
		}
		testMess = new char[(128* sizeof(int)) + (128 * sizeof(char))];
		response = recvfrom(sockfd, testMess, 10000, 0,
				(struct sockaddr*) NULL, NULL);
		if (response == -1) {
			cout << "Server closed but we have a bad key!!, this is a bug" << endl;
			exit(-1);
		}



		getPublicKeyExchange(sockfd, len, true);
		return isCorrect;
	}

	// send a confirmation

	return isCorrect;
}

int Client::getPublicKeyExchange(int sockfd, socklen_t len, bool regenKey) {
	if (regenKey) {
		util = Utility(fileName, 0, 0);
	}
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
			exit(-1);
		}
		// Will reuse this variable to receive the servers public key
		pubKey = new char[sizeof(char) + 2*sizeof(int)];
		// The key is format is size int + char + int
		response = recvfrom(sockfd, pubKey, sizeof(pubKey), 0,
				(struct sockaddr*) NULL, NULL);
		if (response == -1) {
			cout << "Unable to retrieve pubKey from the server server." << endl;

			exit(-1);
		}
		pubKey[response] = 0;

		util.addOtherPubKeyToRSA(pubKey);
		if (!util.IsKeyReceived()) {
			cout << "There was an issue storing the public key. If the public key was not sent in a bad format, this is a bug." << endl;
			exit(-1);
		}

		return 0;
}

int Client::exchangeMessages(int sockfd, socklen_t len) {
	char *message = util.openFile();

	int response;
	 //The buffer size will be the size of the message being sent, plus 200 bytes buffer space for the extra response from the contract server.
	if (message == NULL) {
		return -1;
	}
	cout << "Unsigned contract loaded as: \n" << message << endl;
	char *cipher = util.encryptMess(message);
	util.writeFile(cipher, false, true);
	delete[] message;
	cout << "Encrypted unsigned contract ... " << endl;
	char buffer[strlen(cipher) + 1000];
	response = sendto(sockfd, cipher, strlen(cipher) + 1, 0,
			(struct sockaddr*) NULL, len);
	if (response == -1) {
		cout << "Unable to send message to server" << endl;
		exit(-1);
	}

	// waiting for response.
	response = recvfrom(sockfd, buffer, sizeof(buffer), 0,
			(struct sockaddr*) NULL, NULL);
	if (response == -1) {
		cout << "Unable to retrieve message from server." << endl;

		return -1;
	}
	buffer[response] = 0;

	cout << "Decrypted Signed contract received as:\n" << buffer << endl;
	util.writeFile(buffer, true, true);
	cout << "Decrypting the message with customer private and server  public key ... " << endl;
	message = util.decryptMess(buffer);
	cout << "Contract was validated and dencrypted as:\n" << message << endl;
	util.writeFile(message, true, false);


	return 0;
}

int Client::runClient() {

	cout << "Client started ..." << endl;

	int response; //response of sending and receiving

	bool isGenerated = false;

	// commented out for testing
	util = Utility(fileName, p, q);
	if (!util.IsKeyCorrect()) {
		cout << "There was an issue generating the keys, check your p and q. Otherwise file a bug." << endl;
		return -1;
	}
//

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

	// Send keys to server, and validate the keys
	cout << "Finding you the perfect key ... wish me luck" << endl;
	getPublicKeyExchange(sockfd, sizeof(servaddr), false);
	while (!isGenerated) {
		isGenerated = validateKeys(sockfd, sizeof(servaddr));
	}

	util.printKeys();



	// Send the message
	response = exchangeMessages(sockfd, sizeof(servaddr));
	if (response == -1 ) {
		cout << "There was an issue exchange messages. Check logs for more more details" << endl;
	}
	return 0;
}

