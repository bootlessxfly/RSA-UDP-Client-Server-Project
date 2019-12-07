/*
 * Client.h
 *
 *  Created on: Dec 5, 2019
 *      Author: chwhite
 */

#ifndef CLIENT_H_
#define CLIENT_H_

class Client {
public:
	Client();
	virtual ~Client();
	void init();



private:
	int port;
	char *ip_addr;
	int runClient();

};

#endif /* CLIENT_H_ */
