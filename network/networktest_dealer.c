/* 
 * networktest_dealer - testing program for network
 *
 * This program simulates a dealer that initializes the server 
 * and communicate with player.
 * 
 * usage: ./dealer
 *  and ctrl-C to exit.
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

#include "network.h"

/**************** file-local constants ****************/
#define PORT 8092    // server port number 

/**************** main() ****************/
int main(const int argc, char *argv[]) {
    // initialize two int for storing sockets
    int connected_socket;
    int listening_socket;
    char* buffer;

    // check if dealer is connected with player
    if (setUpDealerSocket(PORT, &connected_socket, &listening_socket) == -1) {
        printf("setUpDealerSocket failed\n");
        exit(1);
    }

    printf("dealer is connected with player\n connected socket: %d\n listening socket: %d\n", connected_socket, listening_socket);

    buffer = readMessage(connected_socket);
    if (strncmp(buffer,"JOIN di", strlen("JOIN di")) == 0) {
        printf("dealer: received JOIN di from player\n");  
    } else {
        printf("didn't receive JOIN di, received %s\n", buffer);
    }
    free(buffer);
    buffer = NULL;

    if (sendMessage(connected_socket, "BEGIN") == -1) {
        printf("sending BEGIN failed\n");
    } else {
        printf("dealer: sent BEGIN\n");
    }

    if (sendMessage(connected_socket, "CARD Seven of Diamonds") == -1) {
        printf("sending CARD Seven of Diamonds failed\n");
    } else {
        printf("dealer: sent CARD Seven of Diamonds\n");
    }

    buffer = readMessage(connected_socket);
    if (strncmp(buffer,"STAND", strlen("STAND")) == 0) {
        printf("dealer: received STAND from player\n");  
    } else {
        printf("didn't receive STAND, received %s\n", buffer);
    }
    free(buffer);

    closeServerSocket(connected_socket, listening_socket);

    exit(0);
}