//============================================================================
// Name        : tarea3.cpp
// Author      : Yo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include "Client.h"
#include <stdlib.h>
#include <cstdio>
#include <dirent.h>

using namespace std;

void init_file_permissions(Server *s, string filePath)
{
	
	ofstream myfile;
	cout << filePath << endl;
	myfile.open(filePath.c_str());
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (s->path.c_str())) != NULL) {
	/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			myfile << ent->d_name << ";Admin;notshared\n";
		}
		closedir (dir);
		myfile.close();
	}
}

int main(int argc, char* argv[]) {



	if(*argv[1]=='c'){
		Client* c = new Client();
		string st = argv[2];
		int port = atoi(argv[3]);
		c->connect_client(st, port);
		c->recieve();



	}
	else if(*argv[1]=='s'){

		//string st = argv[2];
		//int port = atoi(argv[3]);

		string path;
		cout<< "Ingrese El path de fileserv.conf"<<endl;
		cin<<path;
		string fileServPath = path;
		std::ifstream infile(fileServPath.c_str());
		string line;
		std::getline(infile, line);
		cout<< line<<endl;
		int port = atoi(line.c_str());

		std::getline(infile, line);
		cout<< line<<endl;

		string rootPath = line;
		cout <<  port << ":" << rootPath << endl;

		Server * s = new Server("127.0.0.1", port, rootPath.c_str());
		
		/* Open or create the registered users files */
		/*FILE * file = fopen("registeredU.txt","r");
		if(file == NULL)
		{
			file = fopen("registeredU.txt","w");
		}
		fclose(file);*/

		/* Open or create the repository file list */
		string filePath = s->path;
		filePath.append("/filePermissions.txt");
		FILE * file1 = fopen(filePath.c_str(),"r");
		if(file1 == NULL)
		{
			file1 = fopen("filePath.c_str()","w");
			fclose(file1);
			init_file_permissions(s, filePath);
		}
		

		s->start_listen();
		//s->start_recieve();
	}
	else{
		cout << "USAGE: use ./tarea3 s <IP> <PORT> to start a server in the specified address "<< endl;
		cout << "use ./tarea3 c <IP> <PORT> to connect to a server in the specified address" << endl;
	}



	return 0;
}
