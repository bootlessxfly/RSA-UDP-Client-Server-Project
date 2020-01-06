//============================================================================
// Name        : Server.h
// Author      : Christoph White
// Version     :
// Copyright   : This was written for Comp6376 at Auburn. This should not be reused for this class
// Do not reuse this without reaching out to me first.
// Description : Refer to the doc in github for documentation
//=================

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
