# 2019 Shasta Networks / SOU CS Club Hackathon - Telnet Starter Project (10.0.0/posix threading)

A starter project that implements a basic multi-threaded server and client handling code.

## Getting Started

Download this project and import into your IDE of choice.
Choose the files in the directory that pertains to your operating sytem, Windows = src/windows/, Mac/linux = src/unix/.
Build using the compiler commands, linking the files e.g. on Mac ```g++ -std=c++11 -pthread -g src/unix/TelnetServer.cpp src/unix/client/Client.cpp src/unix/server/MultiThreadedServer.cpp```
Alternatively, set your build settings to create an application with the desired TelnetServer.main() entry point.
 
**Note**: the telnet server port is specified as an argument. 

To connect from a terminal, type ```telnet [server.ip.addr.ess] [portNum]``` or use a telnet client to connect to your IP and the specified port.
Putty can also be used to connect to the server

## General Build Notes
This was written using the c++11 standard meaning this flag may need to be set in your build options.

## Windows Build Notes

If you are building on a windows machine be sure to link the Winsock lib.
For MingW this is libwsock32.a.

## Starting Classes
* ```TelnetServer```: The main() entry point into the telnet server. Creates a MultiThreadedServer thread.
* ```MultiThreadedServer```: Starts listening on the specified port for new connections. Creates new Client objects for each connection. 
* ```Client```: Main class to interface with a client. Contains method stubs for handling new connections, disconnects, and receiving/sending text. This will be your direct way to interface with clients.

### Prerequisites

All code is using C++ standard libraries, clients can access the server via standard terminals with ```telnet``` installed.

## References and Useful Links

* [Windows Sockets](https://madwizard.org/tutorials/netcpp/4) - Good tutorials for getting into the Winsock library
* [Linux/Mac Sockets](https://www.geeksforgeeks.org/socket-programming-cc/) - Good overview and basic chat application for Linux/Mac sockets

## Submissions

Submission rules TBD. Will require a GitHub account.

## Versioning

Git should be used for versioning in this project.

## Authors

* **Elijah Anderson-Justis** - Initial work - [Repo](https://github.com/chandl/telnet-starter)
