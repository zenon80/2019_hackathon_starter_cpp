// Author: Denis Roman
// Date: April 27, 2019
// Made for SOU Hackathon 2019

#include "fileutil.h"

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

#ifndef PTHREAD_H
#define PTHREAD_H
#include <pthread.h>
#endif

#define REQWINSOCKVER 2
#define BUFFER_SIZE 1024

typedef struct {
	int valread, port, maxconnections, addrlen;
	SOCKET serversock, new_socket;
	WSADATA wsaData;
	struct sockaddr_in address;
	int is_stopped;
} MultiThreadedServer;

// function pointers in structs get to be a mess, using these functions instead
void server_construct(MultiThreadedServer *server, int portnum, int maxconnections);
int server_init(MultiThreadedServer *server);
int server_run(MultiThreadedServer *server);
void server_stop(MultiThreadedServer *server);

typedef struct {
	SOCKET socket_fd;
	char msgbuffer[BUFFER_SIZE];
	char *message;
} Client;

void client_construct(Client *client, SOCKET clientsocket);
void *client_run(void *c);
void client_handlemessage(Client *client, char *msgbuffer);
void client_handledisconnect(Client *client);
void client_handleconnect(Client *client);
void client_sendmessage(Client *client, const char *msgbuffer);
SOCKET client_getclientsocket(Client *client);

// frees string1
char *string_add(char *string1, const char *string2);
