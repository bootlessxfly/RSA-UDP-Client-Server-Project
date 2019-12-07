/*
 * Client.cpp
 *
 *  Created on: Dec 5, 2019
 *      Author: chwhite
 */

#include "Client.h"

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

using namespace std;

Client::Client() {
	ip_addr = new char[100];
	port = -1;
}

Client::~Client() {
	// TODO Auto-generated destructor stub
}

void Client::init() {
	cout << "\n\nThe client application is starting, please follow on screen prompt" << endl;
	cout << "Please enter the IP Address of the server: ";
	cin >> ip_addr;
	cout << "Please enter the port Number of the server: ";
	cin.clear();
	cin >> port;
	cout << endl;

	runClient();
}

int Client::runClient() {
    char buffer[10000];
    int response; //response of sending and receiving
    char *message = "Contract: This is an agreement between Alice (Buyer) and Bob (Seller) to purchase Property XYZ at Lake Martin, Alabama, for the price of $900,000.  Signed: Alice (Buyer)  Date: November 15, 2019.";
    int sockfd, n;
    struct sockaddr_in servaddr;

    // set servaddr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr(ip_addr);
    servaddr.sin_port = htons(port);
    servaddr.sin_family = AF_INET;

    // create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // connect to server
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed. \n");
        return -1;
    }

    // Send the message
    response = sendto(sockfd, message, 1000, 0, (struct sockaddr*)NULL, sizeof(servaddr));
    if (response == -1) {
    	cout << "Unable to send message to server" << endl;
    }

    // waiting for response
    response = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
    if (response == -1) {
    	cout << "Unable to retrieve message from server" << endl;
    }
    puts(buffer);
	return 0;
}

