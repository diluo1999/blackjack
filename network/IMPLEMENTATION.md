# CS50 Blackjack Final Project Network module
# Implementation Spec

This directory holds `network` module used by `dealer` and `player` in the CS50 Blackjack Final Project.

For background knowledge such as TCP/IP protocol, see [Networking and Socket Programming](https://www.cs.dartmouth.edu/~tjp/cs50/reading/sockets/).

## Usage

To build `network.a`, run `make`.

To clean up, run `make clean`.

To test network module, first run `make test_dealer` in a terminal and then run `make test_player` in another terminal. 

To test network module, first run `make valgrind_dealer` in a terminal and then run `make valgrind_player` in another terminal. 

## Control flow

The network module is implemented in one file `network.c`, with six functions.

### 1. connectToDealer (for player)

Create a socket and connect the socket to the address of the server.

Pseudocode:

    call socket() with safety check to create a socket
    server IP address setup
    convert IPv4 and IPv6 addresses from test to binary form
    call connect() with safety check to connect to dealer
    return socket created

### 2. setUpDealerSocket (for dealer)

Create a socket, bind the socket to an address, listen for connections, and accept a connection.

Pseudocode:

    call socket() with safety check to create socket for listening connection
    call setsockopt() with safety check to attach socket to the PORT
    server IP address setup
    call bind() with safety check to bind socket to the PORT
    call listen() with safety check
    while true
        if accept() returns valid value (meaning connection request received)
            break
    assign listening socket to the pointer from the function parameters
    assign connection socket to the pointer from the function parameters
    return 0

### 3. readMessage

Read message from the socket (from dealer or player).

Pseudocode:

    allocate memory for buffer and safety check
    read message from the socket and store it in the buffer and safety check
    return buffer

### 4. sendMessage

Write message to the socket (from dealer or player).

Pseudocode:

    write to the socket and safety check
    call nanosleep() to freeze for 25ms
    return 0

### 5. closeServerSocket (for dealer)

Close the socket connected with player and shotdown the listening socket.

Pseudocode:

    close dealer's connection
    shutdown the listening socket

### 6. closeClientConnection (for player)

Close the socket connected with dealer.

Pseudocode:

    close player's connection

## Function prototypes

### network

```c
int connectToDealer(const char* server_addr, const int PORT);
int setUpDealerSocket(const int PORT, int* connected_socket, int* listening_socket);
char* readMessage(const int socket);
int sendMessage(const int socket, char* message);
void closeServerSocket(const int connectedSocket, const int listeningSocket);
void closeClientConnection(const int socket);
```

## Acknowledgement

[Networking and Socket Programming](https://www.cs.dartmouth.edu/~tjp/cs50/reading/sockets/)

[Client Demo Program](http://www.cs.dartmouth.edu/~tjp/cs50/project/client.c)

[Server Demo Program](http://www.cs.dartmouth.edu/~tjp/cs50/project/server.c)

[Socket Programming in C/C++](https://www.geeksforgeeks.org/socket-programming-cc/)

[TCP client/server example](https://cs.dartmouth.edu/~tjp/cs50/examples/client-server/)

[Time Delay in C](https://www.geeksforgeeks.org/time-delay-c/)
