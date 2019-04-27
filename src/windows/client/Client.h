#pragma once

class Client{
    /**
     * The socket that a Client is connected to.
     */
    SOCKET socket_fd;
    const static int BUFFER_SIZE = 1024;
    char msgbuffer[BUFFER_SIZE];

public:
    /**
     * Instantiates a Client with the specified socket.
     * @param clientSocket The Socket to run the client on.
     */
    Client(SOCKET clientSocket);

    /**
     * Looks for input and handles the input once received.
     */
    void run();

    /**
     * Method to handle a new message
     * @param msgbuffer The message to handle.
     */
    void handleMessage(char msgbuffer[]);

    /**
     * Method that defines disconnect procedures.
     */
    void handleDisconnect();

    /**
     * Method that handles a new connection.
     */
    void handleConnect();

    /**
     * Sends the client a message.
     * @param msgbuffer The message to send.
     */
    void sendMessage(char msgbuffer[]);

    /**
     * Gets the socket for this Client
     * @return This client's socket.
     */
    SOCKET getClientSocket() {return socket_fd;}
};
