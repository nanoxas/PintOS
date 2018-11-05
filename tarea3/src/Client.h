/*
 * Client.h
 *
 *  Created on: Nov 3, 2015
 *      Author: nanoxas
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <sys/socket.h>
#include "Server.h"

using namespace std;

class Client {
public:
	Client();
	virtual ~Client();
	int socket_c;
	struct sockaddr_in server;



	//Methods
	bool connect_client(string, int);
	void listContent();
	bool put(string, int, string);
	string get(string);
	bool rm(string);
	bool share(string);
	void recieve();
	void send_message(string);






};

#endif /* CLIENT_H_ */
