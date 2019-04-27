// Author: Denis Roman
// Date: April 27, 2019
// Made for SOU Hackathon 2019

#include "server.h"

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef WINSOCK2_H
#define WINSOCK2_h
#include <winsock2.h>
#endif


int main(int argc, char **argv) {
	
	int maxconnections = 10;
	
	if (argc < 2) {
		printf("ERROR, no port provided.\n");
		return 1;
	}
	
	int portnum = strtol(argv[1], NULL, 10);
	if (!portnum) {
		printf("ERROR, invalid port.\n");
		return 1;
	}
	
	if (argc > 3) {
		maxconnections = strtol(argv[2], NULL, 10);
		if (!maxconnections) {
			printf("ERROR, invalid max connections.\n");
			return 1;
		}
	}
	
	
	MultiThreadedServer server;
	server_construct(&server, portnum, maxconnections);
	
	int errorval = server_init(&server);
	
	switch (errorval) {
		case 1:
			printf("Failed to initialize Winsock.\n");
			break;
		case 2:
			printf("Failed on Winsock version.\n");
			break;
		case 3:
			printf("Failed to create server socket.\n");
			break;
		case 4:
			printf("Failed to bind server socket to: %d\n", portnum);
			break;
		case 5:
			printf("Failed to set socket to listen state.\n");
			break;
		case 6:
			printf("Failed on accepting a new connection.\n");
			break;
		default:
			printf("Server shutting down.\n");
	}
	
	return errorval;
}
