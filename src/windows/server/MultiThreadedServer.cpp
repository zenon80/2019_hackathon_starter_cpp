#define WIN32_LEAN_AND_MEAN 1
#include <winsock2.h>
#include <unistd.h>
#include <sys/types.h>
#include <thread>
#include "MultiThreadedServer.h"
#include "../client/Client.h"

sockaddr_in MultiThreadedServer::address = {0};

MultiThreadedServer::MultiThreadedServer(){
    port = 8080;
    maxconnections = 10;
    is_stopped = false;
}

MultiThreadedServer::MultiThreadedServer(int portnum, int maxconns){
    port = portnum;
    maxconnections = maxconns;
    is_stopped = false;
}

int MultiThreadedServer::init(){
    int error = 0;
    addrlen = sizeof(address);

    // Setup the windows server socket
    if (WSAStartup(MAKEWORD(REQWINSOCKVER, 0), &wsaData) != 0){
        return 1;
    }else if (LOBYTE(wsaData.wVersion) < REQWINSOCKVER){
        return 2;
    }

    serversock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serversock == INVALID_SOCKET){
        error = 3;
        stop();
    }

    //Initialize the socket info
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    //bind the socket address
    if (bind(serversock, (sockaddr *)&address, sizeof(address))!=0){
        error = 4;
        stop();
    }

    //Set the socket to listen
    if (listen(serversock, maxconnections) != 0){
        error = 5;
        stop();
    }

    if(error == 0){
        error = run();
    }

    return error;
}

int MultiThreadedServer::run(){
    int error = 0;
    printf("Server initialized on port: %d\n", port);
    //Client *client;
    while(!is_stopped ){
        printf("Waiting for new client to connect\n");
        new_socket = accept(serversock, (sockaddr *)&address, &addrlen);
        if(new_socket == INVALID_SOCKET){
            error = 6;
            stop();
        }

        Client client(new_socket);

        std::thread client_thread(&Client::run, client);
        client_thread.detach();
    }

    return error;
}

void MultiThreadedServer::stop(){
    is_stopped = true;
    if(serversock != INVALID_SOCKET){
        closesocket(serversock);
    }
    WSACleanup();
}
