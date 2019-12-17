/*
 * Server.h
 *
 *  Created on: Dec 5, 2019
 *      Author: chwhite
 */

#include "Utility.h"
#include <sys/socket.h>


#ifndef SERVER_H_
#define SERVER_H_

class Server {
public:
	Server();
	virtual ~Server();

	void init();

private:
	int p;
	int q;
	int port;
	char *fileName;
	int runServer();
	Utility util;

	int getPublicKeyExchange(char* pubKey, int listenfd,struct sockaddr_in cliaddr, socklen_t len);
};

#endif /* SERVER_H_ */
