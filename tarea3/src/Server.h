/*
 * Server.h
 *
 *  Created on: Nov 3, 2015
 *      Author: nanoxas
 */

#ifndef SERVER_H_
#define SERVER_H_

#include<sys/socket.h>
#include<arpa/inet.h>
#include <string>
using namespace std;

class Server {
public:
	Server(string, int, string);
	virtual ~Server();

	int socket_s, socket_c, length;
	string path;
	string currentUser;
	bool logged;
	struct sockaddr_in server, client;
	void start_listen();
	void start_recieve();
	void send_message(string);






	//Methods



};

#endif /* SERVER_H_ */
