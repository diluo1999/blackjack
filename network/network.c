/* 
 * network.c - CS50 Blackjack Final Project 'network' module
 *
 * See network.h for more information
 * 
 * Di Luo
 * Nov. 6 2022
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>       // player
#include <arpa/inet.h>  // player
#include <sys/socket.h> 
#include <sys/types.h>  // player
#include <unistd.h>     // read, write, close
#include <netinet/in.h> // dealer

/**************** functions declaration ****************/
int nanosleep(const struct timespec *req, struct timespec *rem);

/**************** global functions ****************/

/**************** connectToDealer() ****************/
/* See network.h for more information */
int connectToDealer(const char* dealer_addr, const int PORT) {
    int sock = 0, player_fd;
    struct sockaddr_in serv_addr; // address of the server
    
    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Socket creation error\n");
        return -1;
    }
    // Initialize the fields of the server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from test to binary form
    if (inet_pton(AF_INET, dealer_addr, &serv_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address/ Address not supported\n");
        return -1;
    }

    // Connect the socket to that server 
    if ((player_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        fprintf(stderr, "Connection failed\n");
        return -1;
    }

    return sock;
}

/**************** setUpDealerSocket() ****************/
/* See network.h for more information */
int setUpDealerSocket(const int PORT, int* connected_socket, int* listening_socket) {
    int dealer_fd = 0;
    int new_socket = 0;
    struct sockaddr_in serv_addr;
    int opt = 1;
    int addrlen = sizeof(serv_addr);

    // Creating socket file descriptor
    if ((dealer_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }

    // Attaching socket to the PORT
    if (setsockopt(dealer_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        return -1;
    }
    // Initialize the fields of the server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Binding socket to the PORT
    if (bind(dealer_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        return -1;
    }

    if (listen(dealer_fd, 1) < 0) {
        perror("listen failed");
        return -1;
    }

    // keep calling accept() until player connects
    while (true) {
        if ((new_socket = accept(dealer_fd, (struct sockaddr*)&serv_addr, (socklen_t*)&addrlen)) != -1) {
            break;
        }
    }

    // store two sockets into pointers in the function parameter
    *listening_socket = dealer_fd;
    *connected_socket = new_socket;
    
    return 0;
}

/**************** readMessage() ****************/
/* See network.h for more information */
char* readMessage(const int socket) {
    // allocate the buffer for storing message
    char* buffer = calloc(30, sizeof(char));
    // safety check
    if (buffer == NULL) {
        perror("calloc failed");
        return NULL;
    }

    // read messagea and safety check
    if (read(socket, buffer, 30) < 0) {
        perror("reading message failed");
        free(buffer);
        return NULL;
    }

    return buffer;
}

/**************** sendMessage() ****************/
/* See network.h for more information */
int sendMessage(const int socket, char* message) {
    // send message and safety check
    if (write(socket, message, strlen(message)+1) < 0) {
        perror("writing message failed");
        return -1;
    }

    // sleep for 25ms
    if(nanosleep((const struct timespec[]){{0, 25000000L}}, NULL) < 0 )   
    {
        printf("nano sleep failed\n");
        return -1;
    }

    return 0;
}

/**************** closeServerSocket() ****************/
/* See network.h for more information */
void closeServerSocket(const int connected_socket, const int listening_socket) {
    close(connected_socket);
    shutdown(listening_socket, SHUT_RDWR);
}

/**************** closeClientConnection() ****************/
/* See network.h for more information */
void closeClientConnection(const int socket) {
    close(socket);
}
