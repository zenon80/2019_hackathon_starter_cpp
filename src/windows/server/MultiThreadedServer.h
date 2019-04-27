#pragma once

class MultiThreadedServer{
    int valread, port, maxconnections, addrlen;
    const int REQWINSOCKVER = 2;
    SOCKET serversock, new_socket;
    WSADATA wsaData;
    static sockaddr_in address;
    bool is_stopped;

    /**
    * Called to cleanly shutdown the server
    */
    void stop();
public:
    /**
    * Default constructor to create the MultiThreadedServer
    */
    MultiThreadedServer();

    /**
    * Constructor to create a MultiThreadedServer with a port and max
    * connections
    * @param portnum The port to run the server on
    * @param maxcionns The max number of connections allowed at single time
    */
    MultiThreadedServer(int portnum, int maxconns);

    /**
    * Called to startup the server
    */
    int init();

    /**
    * Handles input/output from/to the Client
    */
    int run();
};
