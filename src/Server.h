/*
 * Server.h
 *
 *  Created on: Dec 5, 2019
 *      Author: chwhite
 */

#ifndef SERVER_H_
#define SERVER_H_

class Server {
public:
	Server();
	virtual ~Server();

	void init();

private:
	int port;
	char *fileName;
	void runServer();
};


#endif /* SERVER_H_ */
