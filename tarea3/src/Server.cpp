/*
 * Server.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: nanoxas
 */

#include "Server.h"
#include <iostream>
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>
#include<sstream>
#include <dirent.h>
#include <fstream>
#include <vector>
#include <cstdio>
using namespace std;



inline bool exists (const std::string& name) {
	return ( access( name.c_str(), F_OK ) != -1 );
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}


Server::Server(string ip, int port,string path) {


	if(exists(path)){


		this->path=path;

		this->socket_s = socket(AF_INET , SOCK_STREAM , 0);
		if (this->socket_s == -1)
		{
			cout << "COULD NOT CREATE SOCKET" <<endl;
		}

		this->server.sin_family = AF_INET;
		this->server.sin_addr.s_addr = inet_addr(ip.c_str());
		this->server.sin_port = htons( port );

		if( bind(socket_s,(struct sockaddr *)&this->server , sizeof(this->server)) < 0)
		{
			cout << "Bind Failed" << endl;
		}
		// TODO Auto-generated constructor stub
	}
	else{
		cout << "Repository path doesn't exist" << endl;
		exit(-1);
	}

}

Server::~Server() {
	// TODO Auto-generated destructor stub
}
void Server::start_listen(){

	listen(this->socket_s,3);
	cout<< "Waiting for incomming connections...."<<endl;
	this->length= sizeof(struct sockaddr_in);
	//this->socket_c = accept(socket_s, (struct sockaddr*)&this->client, (socklen_t*)&this->length);

	while(this->socket_c = accept(socket_s, (struct sockaddr*)&this->client, (socklen_t*)&this->length)){
		cout << "SUCCESS" << endl;

		char* message;
		message = " ";
		write(socket_c , message , strlen(message));
		while(true){
			start_recieve();
		}

	}


	if(this->socket_c<0){
		cout<<"FAILED" << endl;
	}
}

bool contains(string str1,string str2){


	std::size_t found = str1.find(str2);
	if(found!=std::string::npos){
		return true;
	}
	return false;


}



void Server::start_recieve(){
	char client_message[2000000];
	int read_size;

	while((read_size= recv(socket_c, client_message, 2000000,MSG_DONTWAIT))>0){
		//DO STH with msg
		string buffer =  client_message;

		int pos=buffer.find('|');
		string length = buffer.substr(0,pos);
		string show = buffer.substr(pos+1, atoi(length.c_str()));
		cout << show<< endl;
		vector<string> commandArray = split(show,',');




			if(contains(commandArray[0], "get")&&logged){

				cout << "get"<< endl;
				string line;
				string content = "\n";
				vector<string> aux = split(show,',');
				string b = this-> path;
				b.append("/");
				b.append(aux[1]);

				string filePath = this->path;
				filePath.append("/filePermissions.txt");
				std::ifstream infile(filePath.c_str());
				bool auth = false;
				while (std::getline(infile, line))
				{
					vector<string> aux = split(line,';');
					if(commandArray[1].compare(aux[0]) == 0)
					{
						if(currentUser.compare(aux[1]) == 0 || aux[2].compare("shared") ==0 )
						{
							auth =true;
						}
					}
					else
					{
					}
				}

				if(auth)
				{
					ifstream file (b.c_str());
					int count=0;
					if(file.is_open()){
						while(getline(file, line)){
							content.append(line);
							content.append("\n");
							count+=line.length();
						}
						stringstream asd;
						asd<<count;
						string send;
						send.append("S: OK\n");
						send.append("S: Length: ");
						send.append(asd.str());
						send.append("\n");
						send.append("S: ");
						send.append(content);
						send.append("\nS:END");
						send_message(send);

					}
					else{
						send_message("S: Fail\nS: File Not Found\nS: END");
					}
				}
				else
				{
					string res;
					res.append("S: Fail\nS: Message: File ");
					res.append(commandArray[1]);
					res.append(" is not owned by ");
					res.append(this->currentUser);
					res.append("\nS: END");
					send_message(res);
				}




			}


			else if(contains(commandArray[0], "put")&&logged){
				cout << "put"<< endl;
				cout<<show<<endl;
				vector<string> aux = split(show,',');
				string pth;
				pth.append(this->path);
				pth.append("/");
				pth.append(aux[1].c_str());
				ofstream outputFile(pth.c_str());
				cout<<aux[1]<<endl;
				outputFile << aux[2]<<endl;
				cout<<aux[2]<<endl;
				outputFile.close();

				string filePath = this->path;
				filePath.append("/filePermissions.txt");
				std::ofstream fileout(filePath.c_str(),std::ios_base::app);
				fileout << aux[1] << ";" << this->currentUser << ";notshared" << endl;
				fileout.close();
				string sendm= "S: OK\nS: Message: File ";
				sendm.append(aux[1]);
				sendm.append(" saved\n S: END");
				send_message(sendm);
			}
			else if(contains(commandArray[0], "ls")&&logged){

				DIR *dir;
				struct dirent *ent;
				string list;
				if ((dir = opendir (this->path.c_str())) != NULL) {
					/* print all the files and directories within directory */
					while ((ent = readdir (dir)) != NULL) {
						printf ("%s\n", ent->d_name);
						list.append(ent->d_name);
						list.append("\n");
					}
					closedir (dir);
					string header = "S: OK\nS: Message: Repository List\nS: ";
					list.append("\nS: END");
					header.append(list);

					send_message(header);

				} else {
					printf("Could not open Directory");
				}
			}
			else if(contains(commandArray[0], "rm")&&logged){
				cout << "rm" << endl;

				vector<string> aux = split(show,',');
				string pth = this->path;
				pth.append("/");
				pth.append(aux[1]);

				string line;
				string filePath = this->path;
				filePath.append("/filePermissions.txt");
				std::ifstream infile(filePath.c_str());
				string filePath2 = this->path;
				filePath2.append("/filePermissions2.txt");
				ofstream fileout(filePath2.c_str());
				bool auth = false;
				while (std::getline(infile, line))
				{
					vector<string> aux = split(line,';');
					if(commandArray[1].compare(aux[0]) == 0)
					{
						if(currentUser.compare(aux[1]) == 0 )
						{
							auth =true;
						}
						else
						{
							fileout << line << "\n";
						}
					}
					else
					{
						fileout << line << "\n";
					}
				}

				infile.close();
				fileout.close();
				if(auth)
				{
					if( remove( pth.c_str()) != 0 )
					{
						string res;
						res.append("S: FAILED\nS: FILE COULD NOT BE DELETED\nS: END");
						send_message(res);
					}
					else
					{

						string res;
						res.append("S: OK\nS: Message: File ");
						res.append(aux[1]);
						res.append(" deleted\nS: END");
						send_message(res);

					}
					remove(filePath.c_str());
					rename(filePath2.c_str(), filePath.c_str());
				}
				else
				{
					remove(filePath2.c_str());
					string res;
					res.append("S: Fail\nS: Message: File ");
					res.append(commandArray[1]);
					res.append(" is not owned by ");
					res.append(this->currentUser);
					res.append("\nS: END");
					send_message(res);
				}



			}
			else if(contains(commandArray[0], "share")&&logged){
				string filePath = this->path;
				filePath.append("/filePermissions.txt");
				std::ifstream infile(filePath.c_str());
				string filePath2 = this->path;
				filePath2.append("/filePermissions2.txt");
				ofstream fileout(filePath2.c_str());
				string line;
				bool auth = false;
				while (std::getline(infile, line))
				{
					vector<string> aux = split(line,';');
					if(commandArray[1].compare(aux[0]) == 0)
					{
						if(currentUser.compare(aux[1]) == 0)
						{
							fileout << commandArray[1] << ";" << aux[1] << ";shared\n";
							auth =true;
						}
					}
					else
					{
						fileout << line << "\n";
					}
				}

				infile.close();
				fileout.close();
				if(auth)
				{
					remove(filePath.c_str());
					rename(filePath2.c_str(), filePath.c_str());
					string res;
					res.append("S: OK\nS: Message: File ");
					res.append(commandArray[1]);
					res.append(" shared by user ");
					res.append(this->currentUser);
					res.append("\nS: END");
					send_message(res);
				}
				else
				{
					remove(filePath2.c_str());
					string res;
					res.append("S: Fail\nS: Message: File ");
					res.append(commandArray[1]);
					res.append(" is not owned by ");
					res.append(this->currentUser);
					res.append("\nS: END");
					send_message(res);
				}



			}
			else if(contains(commandArray[0], "close")&&logged){
				cout << "close"<< endl;
				string header = "S: OK\nS: Message: Bye\nS: END";
				send_message(header);
				close(socket_c);
				cout<<"Client Disconnected" << endl;
				logged=false;
				start_listen();



			}


		else if(contains(commandArray[0], "user"))
		{
			//cout << "Current User: " << this->currentUser << endl;
			vector<string> aux = split(show,',');
			string user = aux[1];
			/*std::ifstream infile("users.txt");
				string line;
				bool auth = false;r
				while (std::getline(infile, line))
				{
					if(user.compare(line) == 0)
					{
						auth = true;
					}
				}*/

			/*if(auth)
				{*/
			string send;
			send.append("S: OK\n");
			send.append("S: Message: User identified as ");
			send.append(user);
			send.append("\nS: END");
			this->currentUser = user;
			send_message(send);
			logged=true;

			/*}
				else
				{
					send_message("S: Fail\nS: User not valid\nS: END");
				}*/


		}
		if(logged==false){
			string send;
			send.append("S: FAILED\n");
			send.append("S: Message: User unidentified ");
			send.append("\nS: END");
			send_message(send);
		}


		/*int rc= read(socket_c,client_message,sizeof(client_message));

		if(rc<=0){
			cout<<"Client Disconnected" << endl;
			fflush(stdout);
			start_listen();
		}*/


	}

	/*int read_size;
	while((read_size= recv(socket_c, client_message, 2000,MSG_DONTWAIT))>0){
		//DO STH with msg

		cout << client_message<< endl;
		string str ="S: ASD\nS: AUTH\nEND";



		write(socket_c, str.c_str(), strlen(str.c_str()));
		if(read_size==0){
			cout<<"Client Disconnected" << endl;
		}


	}*/



}


void Server::send_message(string message){

	stringstream ss;
	ss<< strlen(message.c_str());
	string fmessage= ss.str();
	fmessage.append("|");
	fmessage.append(message);
	write(socket_c, fmessage.c_str(), strlen(fmessage.c_str()));
}







