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
	int p = 0;
	int q = 0;
	int port;
	char *fileName;
	int runServer();
	Utility util;

	bool fullKeyGeneration(int listenfd, struct sockaddr_in cliaddr, socklen_t len);

	bool validateKeys(int listenfd, struct sockaddr_in cliaddr, socklen_t len);

	int getPublicKeyExchange(int listenfd,struct sockaddr_in cliaddr, socklen_t len, bool regKey);
	int exchangeMessages(int listenfd, struct sockaddr_in cliaddr, socklen_t len);
};

#endif /* SERVER_H_ */
