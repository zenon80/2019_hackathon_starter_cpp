/* The port number is passed as an argument */
#include <netinet/in.h>
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

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    //Takes in the port as an argument
    int portnum = atoi(argv[1]);

    //Takes in the max number of connections as an argument
    if (argc > 3) {
        maxconnections = atoi(argv[2]);
    }

    MultiThreadedServer server(portnum, maxconnections);
    int errorval = server.init();

    //Catch the error values
    switch(errorval){
        case 1:
            error("Failed to create the server socket");
            break;
        case 2:
            error("Failed to set socket options");
            break;
        case 3:
            error("Failed to bind server socket to the given port");
            break;
        case 4:
            error("Failed to listen on server socket");
            break;
        case 5:
            error("Failed on accept");
            break;
        default:
            fprintf(stdout, "Server shutting down\n");
    };

    return errorval;
}
