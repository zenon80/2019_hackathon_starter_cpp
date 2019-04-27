// Author: Denis Roman
// Date: April 27, 2019
// Made for SOU Hackathon 2019

#include "server.h"

////////////////
void server_construct(MultiThreadedServer *server, int portnum, int maxconnections) {
	server->port = portnum;
	server->maxconnections = maxconnections;
	server->is_stopped = 0;
}

int server_init(MultiThreadedServer *server) {
	
	int error = 0;
	server->addrlen = sizeof(server->address);
	
	// init winsock
	if (WSAStartup(MAKEWORD(REQWINSOCKVER, 0), &(server->wsaData)) != 0) {
		return 1;
	} else if (LOBYTE(server->wsaData.wVersion) < REQWINSOCKVER) {
		return 2;
	}
	
	server->serversock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server->serversock == INVALID_SOCKET) {
		error = 3;
		server_stop(server);
	}
	
	// init socket info
	server->address.sin_family = AF_INET;
	server->address.sin_port = htons(server->port);
	server->address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	
	// bind socket
	if (bind(server->serversock, (struct sockaddr *)(&(server->address)), sizeof(server->address)) != 0) {
		error = 4;
		server_stop(server);
	}
	
	// set socket to listen
	if (listen(server->serversock, server->maxconnections) != 0) {
		error = 5;
		server_stop(server);
	}
	
	if (!error) {
		error = server_run(server);
	}
	
	return error;
}

int server_run(MultiThreadedServer *server) {
	
	int error = 0;
	printf("Server initialized on port: %d\n", server->port);
	while (!(server->is_stopped)) {
		
		server->new_socket = accept(server->serversock, (struct sockaddr *)(&(server->address)), &(server->addrlen));
		if (server->new_socket == INVALID_SOCKET) {
			error = 6;
			server_stop(server);
		}
		
		printf("Connection accepted\n");
		
		// client stuff ////////
		Client *client = malloc(sizeof(Client));
		client_construct(client, server->new_socket);
		
		// run client in new thread
		pthread_t thread_id;
		
		pthread_create(&thread_id, NULL, client_run, (void *) client);
	}
	
	return error;
}

void server_stop(MultiThreadedServer *server) {
	server->is_stopped = 1;
	if (server->serversock != INVALID_SOCKET) {
		closesocket(server->serversock);
	}
	WSACleanup();
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
////////////////////
// client functions
void client_construct(Client *client, SOCKET clientsocket) {
	client->socket_fd = clientsocket;
	memset(client->msgbuffer, 0, BUFFER_SIZE);
	client->message = malloc(sizeof(char));
	*(client->message) = '\0';
	client_handleconnect(client);
}

void *client_run(void *c) {
	Client *client = (Client *)c;
	int valread = recv(client->socket_fd, client->msgbuffer, BUFFER_SIZE, 0); // throw away initial data
	
	while(1) {
		memset(client->msgbuffer, 0, BUFFER_SIZE);
		
		// receive data
		int valread = recv(client->socket_fd, client->msgbuffer, BUFFER_SIZE, 0);
		//recv(client->socket_fd, client->msgbuffer, BUFFER_SIZE, 0); // ignore newline
		
		// receive disconnect
		if (valread == 0) {
			break;
		} else if (valread == SOCKET_ERROR) {
			// handle
			break;
		}
		
		// process data
		client_handlemessage(client, client->msgbuffer);
	}
	
	client_handledisconnect(client);
	printf("Client disconnected.\n");
	// free allocated memory
	free(client);
}

void client_handlemessage(Client *client, char *msgbuffer) {
	client->message = string_add(client->message, msgbuffer);
}

char *string_add(char *string1, const char *string2) {
	
	int newsize = strlen(string1)+strlen(string2) + 1;
	char *newstring = malloc(sizeof(char)*newsize);
	int i;
	for (i = 0; i < strlen(string1); i++) newstring[i] = string1[i];
	for (i = strlen(string1); i < newsize-1; i++) newstring[i] = string2[i-strlen(string1)];
	
	newstring[newsize-1] = '\0';
	free(string1);
	
	return newstring;
}

void client_handledisconnect(Client *client) {
	client_sendmessage(client, "Goodbye.\r\n");
	
	// block unimaginative clients from erasing interesting messages
	if (strlen(client->message) > 0) {
	
		FileData f;
		f.length = strlen(client->message);
		f.data = client->message;
		
		int code = WriteFileData(&f, "message.txt", "wb");
		if (code) {
			printf("%s\n", TranslateError(code));
		}
	}
	
	free(client->message);
	
	closesocket(client->socket_fd);
}

void client_handleconnect(Client *client) {
	
	client_sendmessage(client, "Welcome!\r\n\r\n");
	FileData f;
	if (!ReadFileData(&f, "message.txt", "rb")) {
		
		// success reading message.txt
		
		// add null terminator
		char *newstring = malloc(f.length+1);
		int i;
		for (i = 0; i < f.length; i++) newstring[i] = f.data[i];
		newstring[f.length] = '\0';
		
		client_sendmessage(client, "Last message:\r\n");
		client_sendmessage(client, newstring);
		
		free(f.data);
	}
	
	client_sendmessage(client, "\r\nLeave a new message (disconnect when done):\r\n");
}

void client_sendmessage(Client *client, const char *msgbuffer) {
	send(client->socket_fd, msgbuffer, strlen(msgbuffer), 0);
}

SOCKET client_getclientsocket(Client *client) {
	return client->socket_fd;
}

	