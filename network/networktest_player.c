/* 
 * networktest_player - testing program for network
 *
 * This program simulates a player that connects to 
 * and communicate with a dealer.
 * 
 * usage: ./player
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

const char* SERV_ADDR = "129.170.64.104";
const int PORT = 8092;

/**************** main() ****************/
int main(const int argc, char *argv[]) {
    // initialize two int for storing sockets
    int socket;
    char* buffer;

    // check if player is connected with dealer
    socket = connectToDealer(SERV_ADDR, PORT);
    if (socket == -1) {
        printf("connectToDealer failed\n");
        exit(1);
    }

    printf("player is connected with dealer\n connected socket:%d\n", socket);

    if (sendMessage(socket, "JOIN di") == -1) {
        printf("sending JOIN di failed\n");
    } else {
        printf("player: sent JOIN di\n");
    }


    buffer = readMessage(socket);
    if (strcmp(buffer,"BEGIN") == 0) {
        printf("player: received BEGIN from dealer\n");  
    } else {
        printf("didn't receive BEGIN, received %s\n", buffer);
    }
    free(buffer);
    buffer = NULL;

    buffer = readMessage(socket);
    if (strcmp(buffer,"CARD Seven of Diamonds") == 0) {
        printf("player: received CARD Seven of Diamonds from dealer\n");  
    } else {
        printf("didn't receive CARD Seven of Diamonds, received %s\n", buffer);
    }
    free(buffer);

    if (sendMessage(socket, "STAND") == -1) {
        printf("sending STAND failed\n");
    } else {
        printf("player: sent STAND\n");
    }

    closeClientConnection(socket);

    exit(0);
}