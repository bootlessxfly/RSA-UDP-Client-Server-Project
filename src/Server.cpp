//============================================================================
// Name        : Server.cpp
// Author      : Christoph White
// Version     :
// Copyright   : This was written for Comp6376 at Auburn. This should not be reused for this class
// Description : Refer to the doc in github for documentation
//=================

#include "Server.h"
#include "Utility.h"

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <limits.h>
#include <string>

using namespace std;

Server::Server() {
	// TODO Auto-generated constructor stub
	port = -1;
	fileName = new char[PATH_MAX];
}

Server::~Server() {
	// TODO Auto-generated destructor stub
	delete fileName;
}

void Server::init() {
	cout << "Starting the server side application" << endl;
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

bool Server::validateKeys(int listenfd, struct sockaddr_in cliaddr, socklen_t len) {
	bool otherPassed = false;
	bool isCorrect = false;
	char* testMess = new char[1000000];
	int response;
	response = recvfrom(listenfd, testMess, 1000000, 0,
			(struct sockaddr*) &cliaddr, &len); //receive message from client
	testMess[response] = 0;






	if (util.valTestMessage(testMess)) {
		//return true;
		isCorrect = true;
	}
	else {
		response = sendto(listenfd, util.badKey, strlen(util.badKey), 0,
				(struct sockaddr*) &cliaddr, sizeof(cliaddr));
		response = getPublicKeyExchange(listenfd, cliaddr, len, true);
		return false;
	}



	testMess = new char[(128* sizeof(int)) + (128 * sizeof(char))];
	testMess = util.genTestMessage();

	response = sendto(listenfd, testMess, strlen(testMess), 0,
			(struct sockaddr*) &cliaddr, sizeof(cliaddr));



	// wait for response
	testMess = new char[1000000];
	response = recvfrom(listenfd, testMess, 1000000, 0,
			(struct sockaddr*) &cliaddr, &len); //receive message from client
	if (response == -1) {
		cout << "SOmething went wrong" << endl;
		return false;
	}


	testMess[response] = 0;

	if (strcmp(testMess, util.badKey) == 0) {
		util = Utility(fileName, 0, 0);
		response = sendto(listenfd, util.badKey, strlen(util.badKey), 0,
				(struct sockaddr*) &cliaddr, sizeof(cliaddr));
		getPublicKeyExchange(listenfd, cliaddr, len, false);
		return false;
	}
	else {
		otherPassed = true;
	}
	if (otherPassed && isCorrect) {
		if (util.checkSameKey()) {
			response = sendto(listenfd, util.badKey, strlen(util.badKey), 0,
					(struct sockaddr*) &cliaddr, sizeof(cliaddr));
			response = getPublicKeyExchange(listenfd, cliaddr, len, true);
			return false;
		}
		cout << "Got the ACK from the client" << endl;
		return true;
	}

	return isCorrect;

}

int Server::getPublicKeyExchange(int listenfd, struct sockaddr_in cliaddr, socklen_t len, bool regenKey) {
	char* pubKey = new char[sizeof(char) + 2*sizeof(int)];
	int response = recvfrom(listenfd, pubKey, sizeof(pubKey), 0,
				(struct sockaddr*) &cliaddr, &len); //receive message from client
	if (regenKey) {
		util = Utility(fileName, 0, 0);
	}
	if (response == -1) {
		cout
				<< "There was an error receiving the message from the client, killing server. Please restart"
				<< endl;
		return -1;
	}
	pubKey[response] = 0;
	util.addOtherPubKeyToRSA(pubKey);
	if (!util.IsKeyReceived()) {
		cout << "There was an issue storing the public key. If the public key was not sent in a bad format, this is a bug." << endl;
		return -1;
	}
	//send public key
	pubKey = util.getPubKeyString();
	response = sendto(listenfd, pubKey, strlen(pubKey), 0,
			(struct sockaddr*) &cliaddr, sizeof(cliaddr));
	if (response == -1) {
		cout << "There was an issue sending the public key back to the client" << endl;
		return -1;
	}
	return 0;
}

int Server::exchangeMessages(int listenfd, struct sockaddr_in cliaddr, socklen_t len) {
	int response;
	const char *signed_message = " Signed Bob(Seller) Date: November 24, 2019.";
	char *full_signed_message;
	char* message;

	char buffer[10000];

	response = recvfrom(listenfd, buffer, sizeof(buffer), 0,
			(struct sockaddr*) &cliaddr, &len); //receive message from client

	if (response == -1) {
		cout
				<< "There was an error receiving the message from the client, killing server. Please restart"
				<< endl;
		return -1;
	}
	buffer[response] = 0;
	cout << "We received the following encrypted message: \n" << buffer << endl;
	message = util.decryptMess(buffer);
	cout << "We have confirmed the digital signature and have the following contract:\n" << message << endl;
	util.writeFile(message, false, false);
	full_signed_message = new char[strlen(message) + strlen(signed_message)];
	strcpy(full_signed_message, message);
	strcat(full_signed_message, signed_message);
	cout << "Signing contract wither server user and date. Here is the final contract: \n" << full_signed_message <<endl;
	util.writeFile(full_signed_message, true, false);
	delete[] message;
	cout << "Encrypting contract with server private and customer public key..." << endl;
	message = util.encryptMess(full_signed_message);
	util.writeFile(message, true, true);
	cout << "Sending encrypted contract ... " << endl;



	//Sign message
	// send confirmation
	response = sendto(listenfd, message, strlen(message), 0,
			(struct sockaddr*) &cliaddr, sizeof(cliaddr));
	if (response == -1) {
		cout << "There was an issue sending the message back to the client" << endl;
		return -1;
	}
	return 0;
}

int Server::runServer() {
	cout << "Server is started ... " << endl;
	util = Utility(fileName, p, q);
	if (!util.IsKeyCorrect()) {
		cout << "There was an issue generating the keys, check your p and q. Otherwise file a bug." << endl;
		return -1;
	}
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
	len = sizeof(cliaddr);

	bool isGenerated = false;

	response = getPublicKeyExchange(listenfd, cliaddr, len, false);

	cout << "Generating Keys, validating that work, and the sending them to client" << endl;
	while(!isGenerated) {
		isGenerated = validateKeys(listenfd, cliaddr, len);
	}

	util.printKeys();
//	//receive messages
	response = exchangeMessages(listenfd, cliaddr, len);
	if (response == -1) {
		cout << "Error sending the message. Check logs" << endl;
		return -1;
	}


	return 0;

}

