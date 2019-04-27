#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include "Client.h"

Client::Client(int clientSocket) {
    socket_fd = clientSocket;
    memset(msgbuffer, 0, BUFFER_SIZE);
    handleConnect();
}

void Client::run() {
    while(true){
        memset(msgbuffer, 0, BUFFER_SIZE);
        //Grab the message and add it to the buffer
        int valread = read(socket_fd, msgbuffer, BUFFER_SIZE);

        //process the message that came in
        if(valread == 0)
        handleMessage(msgbuffer);
    }
}

void Client::handleMessage(char msgbuffer[]){

}

void Client::handleDisconnect(){

}

void Client::handleConnect(){

}

void Client::sendMessage(char msgbuffer[]) {
    send(socket_fd , msgbuffer , strlen(msgbuffer) , 0);
}
