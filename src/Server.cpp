/*
 * Server.cpp
 *
 *  Created on: Dec 5, 2019
 *      Author: chwhite
 */

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
	cout << "Please enter the filename you would like to write to: ";
	cin >> fileName;
	cout << "Please enter the port that this server will listen on: ";
	cin.clear();
	cin >> port;
	cout << endl;

	runServer();

}

void Server::runServer() {
    char buffer[10000];
    Utility util = Utility(fileName);
    const char *message = "Hello Client, message received. Here is my singed message: \n";
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
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    //receive messages
    len = sizeof(cliaddr);
    response = recvfrom(listenfd, buffer, sizeof(buffer),
            0, (struct sockaddr*)&cliaddr,&len); //receive message from client
    if (response == -1) {
    	cout << "There was an error receiving the message from the client, killing server. Please restart" << endl;
    	return;
    }
//    buffer[response] = 0;
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
    sendto(listenfd, full_response, strlen(full_response), 0,
          (struct sockaddr*)&cliaddr, sizeof(cliaddr));

}

