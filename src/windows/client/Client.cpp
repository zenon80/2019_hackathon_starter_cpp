#define WIN32_LEAN_AND_MEAN 1
#include <winsock2.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include "Client.h"

Client::Client(SOCKET clientSocket) {
    socket_fd = clientSocket;
    memset(msgbuffer, 0, BUFFER_SIZE);
    handleConnect();
}

void Client::run() {
    while(true){
        memset(msgbuffer, 0, BUFFER_SIZE);
        //Grab the message and add it to the buffer
        int valread = recv(socket_fd, msgbuffer, BUFFER_SIZE, 0);

        //Received a disconnect
        if(valread == 0){
            break;
        } else if(valread == SOCKET_ERROR){
            //Encountered an error, handle as needed
            break;
        }

        //process the message that came in
        handleMessage(msgbuffer);
    }

    handleDisconnect();
}


void Client::handleMessage(char msgbuffer[]){

}

void Client::handleDisconnect(){
    closesocket(socket_fd);
}

void Client::handleConnect(){

}

void Client::sendMessage(char msgbuffer[]) {
    send(socket_fd , msgbuffer , strlen(msgbuffer) , 0);
}
