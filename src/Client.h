/*
 * Client.h
 *
 *  Created on: Dec 5, 2019
 *      Author: chwhite
 */

#include <sys/socket.h>
#include "Utility.h"


#ifndef CLIENT_H_
#define CLIENT_H_

class Client {
public:
	Client();
	virtual ~Client();
	void init();

private:
	int passCount = 0;
	int p = 0;
	int q = 0;
	Utility util;
	int port;
	char *ip_addr;
	char *fileName;
	int runClient();

	bool fullKeyGeneration(int sockfd, socklen_t len);

	bool validateKeys(int sockfd, socklen_t len);

	int getPublicKeyExchange(int sockfd, socklen_t len, bool regenKey);
	int exchangeMessages(int sockfd, socklen_t len);

};

#endif /* CLIENT_H_ */
