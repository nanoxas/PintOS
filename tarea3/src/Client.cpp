/*
 * Client.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: nanoxas
 */

#include "Client.h"
#include <iostream>
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>
#include<sstream>
#include <vector>
#include <fstream>

using namespace std;

Client::Client() {
	// TODO Auto-generated constructor stub
	this->socket_c= socket(AF_INET, SOCK_STREAM, 0);
	if (socket_c == -1)
	{
		cout << "Could not create socket"<< endl;
	}


}

std::vector<std::string> &split_str(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split_str(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split_str(s, delim, elems);
	return elems;
}


bool contains_str(string str1,string str2){


	std::size_t found = str1.find(str2);
	if(found!=std::string::npos){
		return true;
	}
	return false;


}

bool Client::connect_client(string ip, int port){

	this->server.sin_addr.s_addr = inet_addr(ip.c_str());
	this->server.sin_family = AF_INET;
	this->server.sin_port = htons(port);

	cout << "CONNECTED to "<< ip<<":"<< port << endl;



	int sucess = connect(this->socket_c, (struct sockaddr*)&this->server, sizeof(this->server));
	if(sucess<0){
		cout << "CONNECTION FAILED" << endl;
		return false;
	}
	else{
		cout << "CONNECTED to "<< ip<<":"<< port << endl;
		return true;
	}


}

void Client::recieve(){


	char server_reply[2000000];
	while(true){
		if( recv(this->socket_c, server_reply, 2000000 , 0) < 0)
		{
			cout << "RECIEVE FAILED" << endl;
		}
		else{

			string buffer =  server_reply;
			//printf("%s", buffer.c_str());

			int pos=buffer.find('|');

			string length = buffer.substr(0,pos);
			//printf("%d\n",pos);
			//printf("%c\n",length.c_str());
			string show = buffer.substr(pos+1, atoi(length.c_str()));
			cout << show<< endl;

			cout << "RECIEVE Success" << endl;

			cout << "\nInsert a command:\n" << endl;

			string to_server;
			cin >> to_server;

			if(contains_str(to_server,"get")){


				vector<string> args = split_str(to_server,',');
				cout<<"C: GET"<<endl;
				cout<< "C: Name: "<< args[1]<<endl;
				cout<< "C: END"<< endl;
				send_message(to_server);




			}
			else if(contains_str(to_server, "user"))
			{
				vector<string> args = split_str(to_server,',');
				cout<<"C: USER"<<endl;
				cout<< "C: Name: "<< args[1]<<endl;
				cout<< "C: END"<< endl;
				send_message(to_server);
			}
			else if(contains_str(to_server,"put")){


				string line;
				string content = "\n";
				vector<string> aux = split_str(to_server,',');
				vector<string> aux1= split_str(aux[1],'/');

				string b="";
				b.append(aux[1]);

				ifstream file (b.c_str());
				int count=0;
				if(file.is_open()){
					while(getline(file, line)){
						content.append(line);
						content.append("\n");
						count+=line.length();
					}

				}
				string fm;

				cout<< "C: PUT" << endl;
				cout<< "C: Name: "<< aux1[aux1.size()-1] << endl;
				cout<< "C: Length: "<< count << endl;
				cout <<content<<endl;




				fm.append("put,");
				fm.append(aux1[aux1.size()-1]);
				fm.append(",");
				fm.append(content);
				send_message(fm);


			}
			else if(contains_str(to_server,"ls")){

				cout<< "C: LS"<< endl;
				cout<< "C: END" <<endl;
				send_message(to_server);

			}
			else if(contains_str(to_server,"rm")){

				cout<< "C: RM"<< endl;
				vector<string> aux = split_str(to_server,',');
				cout<< "C: Name: "<<aux[1]<<endl;
				cout<< "C: END" <<endl;

				send_message(to_server);



			}
			else if(contains_str(to_server,"share")){

				cout<< "C: SHARE"<< endl;
				vector<string> aux = split_str(to_server,',');
				cout<< "C: Name: "<<aux[1]<<endl;
				cout<< "C: END" <<endl;
				send_message(to_server);

			}
			else if(contains_str(to_server,"close")){

				cout<< "C: CLOSE"<< endl;
				cout<< "C: END" <<endl;
				send_message(to_server);
				close(socket_c);



			}



		}
	}

}

void Client::send_message(string message){

	//agregar el largo
	stringstream ss;
	ss<< strlen(message.c_str());
	string fmessage= ss.str();
	fmessage.append("|");
	fmessage.append(message);
	write(socket_c, fmessage.c_str(), strlen(fmessage.c_str()));

}




Client::~Client() {
	// TODO Auto-generated destructor stub
}

