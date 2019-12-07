/*
 * Server.cpp
 *
 *  Created on: Dec 5, 2019
 *      Author: chwhite
 */

#include "Server.h"

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>

using namespace std;


Server::Server() {
	// TODO Auto-generated constructor stub
	port = -1;
}

Server::~Server() {
	// TODO Auto-generated destructor stub
}

void Server::init() {
	cout << "Starting the server side application" << endl;
	cout << "Please enter the port that this server will listen on: ";
	cin >> port;
	cout << endl;

	runServer();

}

void Server::runServer() {
    char buffer[10000];
    char *message = "Hello Client, message received. Here is my singed message: \n";
    char *signed_message = " Singed Bob(Seller) Date: November 24, 2019.";
    char *full_signed_message;
    int listenfd;
    socklen_t len;
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
    int n = recvfrom(listenfd, buffer, sizeof(buffer),
            0, (struct sockaddr*)&cliaddr,&len); //receive message from client
    buffer[n] = '\0';
    puts(buffer);

    //Sign message
    full_signed_message = new char(strlen(message) + strlen(buffer) + strlen(signed_message));
    strcpy(full_signed_message, message);
    strcat(full_signed_message, buffer);
    strcat(full_signed_message, signed_message);

    // send confirmation
    sendto(listenfd, full_signed_message, 1000, 0,
          (struct sockaddr*)&cliaddr, sizeof(cliaddr));

}

