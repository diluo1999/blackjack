/*
 *
 * dealer.c - Executes the dealer in the blackjack game
 *
 * usage: ./dealer <number of games> <port>
 * Jake Olson, Project Team 12, CS 50 Fall 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/bag.h"
#include "../cards/cards.h"
#include "../network/network.h"
#include <time.h>

/********** Function Prototypes ***********/
void getNewCard(deck_t* deck, hand_t* hand, char* type, int connectedSocket, bool send);
char* findResult(int playerHandScore, int dealerHandScore, int connectedSocket);
static void parseArgs(const int argc, char* argv[], int* games, int* port);

/**************** main() ****************/
int main(int argc, char* argv[]) {
    // Validate command line args
    int numGames = 0;
    int port = 0;
    parseArgs(argc, argv, &numGames, &port);

	// Set up server socket for player
    int connectedSocket;
    int listeningSocket;
    bool bust;

    srand(time(0)); // set the seed for deck creation/shuffle

    // check if dealer is connected with player
    if (setUpDealerSocket(port, &connectedSocket, &listeningSocket) == -1) {
        printf("Failed to set up server socket\n");
        exit(1);
    }
    printf("Dealer is connected with player\n Connected socket: %d\n Listening socket: %d\n\n", connectedSocket, listeningSocket);

    // check for join message, pull out the name
    char* joinMessage = readMessage(connectedSocket);
    char* name;
    if (strncmp(joinMessage,"JOIN ", strlen("JOIN ")) == 0) {
        name = mem_malloc(sizeof(char)*(strlen(joinMessage)-4));
        sscanf(joinMessage, "JOIN %s", name);
        printf("%s joined the game!\n\n", name); 
    } else {
            printf("didn't receive JOIN message, received %s\n", joinMessage);
    }
    free(joinMessage);
    joinMessage = NULL;

    // for each game:
    for(int i = 0; i < numGames; i++) {
        bust = false;
        // create new, shuffled deck
        deck_t* deck = newDeck();

        // Tell player to begin the game 
        if (sendMessage(connectedSocket, "BEGIN") == -1) {
            printf("sending BEGIN failed\n");
        } else {
            printf("dealer: sent BEGIN %d\n", i+1);
        }


        // deal cards to the player, sending messages 
        // with card suit and rank as a string, like "Seven of Hearts"
        
        // create playerHand and add two cards, send the info to player
        hand_t* playerHand = newHand();
        getNewCard(deck, playerHand, "CARD", connectedSocket, true);
        getNewCard(deck, playerHand, "CARD", connectedSocket, true);


        // create dealer hand and add two cards, send the info to player
        hand_t* dealerHand = newHand();
        getNewCard(deck, dealerHand, "DEALER", connectedSocket, true);
        getNewCard(deck, dealerHand, "DEALER", connectedSocket, false);

        // Ask player to make a decision
        if (sendMessage(connectedSocket, "DECISION") == -1) {
            printf("sending DECISION failed\n");
        } else {
            printf("dealer: sent DECISION\n");
        }
        
        // Hear decision from player, let hit until stand
        char* decision = readMessage(connectedSocket);
        if (decision != NULL) {
            // Determine what the player action is
            if (strncmp(decision,"HIT", strlen("HIT")) == 0) {
                printf("dealer: received HIT from player\n");  
            } else if (strncmp(decision,"STAND", strlen("STAND")) == 0) {
                printf("dealer: received STAND from player\n");  
            } else {
                printf("didn't receive HIT or STAND, received %s\n", decision);
            }
            // Let the player decide to hit until they stand
            while(strncmp(decision, "HIT", strlen("HIT")) == 0) {
                // Create the new card, add it to the hand, send info to player
                getNewCard(deck, playerHand, "CARD", connectedSocket, true);
                // If busted, stop iteration
                if(getHandScore(playerHand) > 21) {
                    bust = true;
                    break;
                }

                // Send the next decision message
                if (sendMessage(connectedSocket, "DECISION") == -1) {
                    printf("sending DECISION failed\n");
                } else {
                    printf("dealer: sent DECISION\n");
                }

                free(decision);
                decision = NULL;

                // Receive next decision
                decision = readMessage(connectedSocket);
                if (strncmp(decision,"HIT", strlen("HIT")) == 0) {
                    printf("dealer: received HIT from player\n");  
                } else if (strncmp(decision,"STAND", strlen("STAND")) == 0) {
                    printf("dealer: received STAND from player\n");  
                } else {
                    printf("didn't receive HIT or STAND, received %s\n", decision);
                }
            }
            free(decision);
        } else continue;
        
        // deal the dealer until the hand total is greater than 16
        while (!bust && getHandScore(dealerHand) < 17) {
            getNewCard(deck, dealerHand, "DEALER", connectedSocket, false);
        }


        // Calculate game's result and send message to client
        char* finalResult = findResult(getHandScore(playerHand), getHandScore(dealerHand), connectedSocket);
        if (sendMessage(connectedSocket, finalResult) == -1) {
            printf("sending %s failed\n", finalResult);
        } else {
            printf("%s's result: %s\n", name, finalResult);
        }

        // Reset and play again
        deleteHand(playerHand);
        deleteHand(dealerHand);
        deleteDeck(deck);

        printf("---------------\n");
    }
    mem_free(name);
    // when finished, send a QUIT message to the client
    sendMessage(connectedSocket, "QUIT");
    closeServerSocket(connectedSocket, listeningSocket);
    return 0;
}

/**************** parseArgs() ****************/
/* Validates command line arguments
 *
 * We return:
 *   Nothing if successful
 *   Exit w/ Non-Zero status if an error is found and error message to stderr
 */
static void parseArgs(const int argc, char* argv[], int* games, int* port) {
	// Check input arguments: # of arguments
	if (argc != 3) {
		fprintf(stderr, "usage: ./dealer <number of games> <port>\n");
		exit(1);
	}
    if(sscanf(argv[1], "%d", games) != 1) {
        fprintf(stderr, "Incorrect usage. Number of games must be an integer.\n");
        exit(2);
    }
    if (*games < 1) {
        fprintf(stderr, "Incorrect usage. Number of games must be at least 1.\n");
        exit(3);
    }
    if(sscanf(argv[2], "%d", port) != 1) {
        fprintf(stderr, "Incorrect usage. Port must be an integer.\n");
        exit(4);
    }
    if (*port < 1) {
        fprintf(stderr, "Incorrect usage. Port is invalid.\n");
        exit(5);
    }
}

/**************** getNewCard() ****************/
/* helper function to get a card and perform the associated actions:
 * pulls a card from the deck, adds it to the hand, 
 * sends proper message to the player if it is not a facedown card
 *
 * We return:
 *   Nothing if successful
 *   Error message if error sending message
 */
void getNewCard(deck_t* deck, hand_t* hand, char* type, int connectedSocket, bool send) {
        card_t* card = pullCard(deck);
        addToHand(hand, card);
        char* message = cardToString(type, card);
        if (send) {
            if (sendMessage(connectedSocket, message) == -1) {
                printf("sending %s failed\n", message);
            } else {
                printf("dealer: sent %s\n", message);
            }
            
        } 
        mem_free(message);
}

/**************** findResult() ****************/
/* helper function to determine the result of the game,
 * and construct result message string
 * We return:
 *   char* of message string if successful
 */
char* findResult(int playerHandScore, int dealerHandScore, int connectedSocket) {
    if (playerHandScore > 21) { // player bust condition
        return "RESULT BUST";
    } else if (dealerHandScore > 21 || playerHandScore > dealerHandScore) { // player win
        return "RESULT WIN";
    } else if (playerHandScore < dealerHandScore) { // player loss
        return "RESULT LOOSE";
    } else if (playerHandScore == dealerHandScore) { // tie condition
        return "RESULT PUSH";
    }
    return "FAILURE";
}