#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <thread>
#include "MultiThreadedServer.h"
#include "../client/Client.h"

//Initialize the static address object
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
    int opt = 1;
    addrlen = sizeof(address);

    // create a socket
    // socket(int domain, int type, int protocol)
    if ((server_fd =  socket(AF_INET, SOCK_STREAM, 0)) == 0){
        error = 1;
    }

    // Set the socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        error = 2;
    }

    /* setup the host_addr structure for use in bind call */
    // server byte order
    address.sin_family = AF_INET;
    // automatically be filled with current host's IP address
    address.sin_addr.s_addr = INADDR_ANY;
    // convert short integer value for port must be converted into network byte order
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        error = 3;
    }

    if (listen(server_fd, maxconnections) < 0){
        error = 4;
    }

    if(error == 0){
        run();
    }
    return error;
}

int MultiThreadedServer::run(){
    int error = 0;
    fprintf(stdout, "Server initialized on port: %d\n", port);
    //Client *client;
    while(!is_stopped){
        fprintf(stdout, "Waiting for new client to connect\n");
        if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
            error = 5;
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
    close(server_fd);
}
