/* 
 * network.h - header file for 'network' module
 *
 * Handle:
 * Server set up/tear down
 * Socket connection/close
 * Message passing from dealer to player, and player to dealer
 * 
 * Di Luo
 * Nov. 6 2022
 * CS 50, Fall 2022
 */

#ifndef __NETWORK_H
#define __NETWORK_H

#include <stdio.h>

/**************** global functions ****************/

/**************** connectToDealer() ****************/
/** Connect player to dealer.
 * 
 * Caller provides:
 *      a valid string for server's IP address,
 *      a valid PORT number.
 * We return:
 *      a valid communication socket (> 0),
 *      -1 if socket creation error, invalid address, address not supported, or connection failed.
*/
int connectToDealer(const char* server_addr, const int PORT);

/**************** setUpDealerSocket() ****************/
/** Set up a dealer socket listening for players to connect.
 * 
 * Caller provides:
 *      a valid PORT number,
 *      two int pointers to store listening socket and connected socket
 * We return:
 *      0 if sockets are successfully created and dealer connects with player,
 *      -1 if socket creation failed, setsockopt failed, bind failed, or listen failed.
*/
int setUpDealerSocket(const int PORT, int* connected_socket, int* listening_socket);

/**************** readMessage() ****************/
/** Player/dealer reads a message from dealer/player.
 * 
 * Caller provides:
 *      a valid socket.
 * We return:
 *      a string of message from dealer/player,
 *      NULL if calloc failed or reading message failed.
 * Caller is responsible for:
 *      later freeing the memory allocated for the string.
*/
char* readMessage(const int socket);

/**************** sendMessage() ****************/
/** Player/dealer sends a message to dealer/player.
 * 
 * Caller provides:
 *      a valid socket,
 *      a valid string of message to dealer/player.
 * We return:
 *      -1 if calloc failed or writing message failed,
 *      0 if message is written to the socket.
*/
int sendMessage(const int socket, char* message);

/**************** closeServerSocket() ****************/
/** Close the connected socket and the listening socket of the server (dealer)
 * 
 * Caller provides:
 *      a valid connected socket,
 *      a valid listening socket.
*/
void closeServerSocket(const int connectedSocket, const int listeningSocket);

/**************** closeClientConnection() ****************/
/** Close the connection socket of the client (player)
 * 
 * Caller provides:
 *      a valid socket.
*/
void closeClientConnection(const int socket);

#endif // __NETWORK_H