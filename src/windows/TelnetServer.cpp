/* The port number is passed as an argument */
#define WIN32_LEAN_AND_MEAN 1
#include <winsock2.h>
#include "server/MultiThreadedServer.h"
#include <stdio.h>
#include <stdlib.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int maxconnections = 10;

    //int n;
    if (argc < 2) {
        printf("ERROR, no port provided\n");
        exit(1);
    }

    int portnum = atoi(argv[1]);

    if (argc > 3) {
        maxconnections = atoi(argv[2]);
    }

    MultiThreadedServer server(portnum, maxconnections);
    int errorval = server.init();

    //Catch the error values
    switch(errorval){
        case 1:
            error("Failed to initialize Winsock");
            break;
        case 2:
            error("Failed on Winsock Version.");
            break;
        case 3:
            error("Failed to create server socket");
            break;
        case 4:
            error("Failed to bind server socket to: " + portnum);
            break;
        case 5:
            error("Failed to set socket to listen state");
            break;
        case 6:
            error("Failed on accepting a new connection");
            break;
        default:
            printf("Server shutting down\n");
    };

    return errorval;
}
