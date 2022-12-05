/*
 *
 * player.c - Executes the player in the blackjack game
 *
 * usage: player <player name> <server’s IP address> <PORT number>
 * Juan Suarez, Project Team 12, CS 50 Fall 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../network/network.h"
#include <mem.h>
#include <file.h>
#include "../cards/cards.h"
#include <bag.h>
#include <ncurses.h>

/***** Global Variables ****/

// This defines the dimensions
#define maxPlayerPoints 21
#define maxDealerPoints 11
#define numberActions 2

// Q table declartion
float Q[maxPlayerPoints][maxDealerPoints][numberActions] = {0};
int Q_count[maxPlayerPoints][maxDealerPoints][numberActions] = {0};

/********** Function Prototypes ***********/
void loadQTables();
void saveQTables();
void rewardSaver(void* arg, void* item);
void roundBagSaver(void* arg, void* item);
void play(char* player_name, char* ip_address, int port);

/**************** main() ****************/
int main(int argc, char* argv[]) {
	
	// Ensure argument integrity
	if (argc != 4) {
		fprintf(stderr, "%s\n", "Not enough arguments. Usage: player <player name> <server’s IP address> <PORT number>");
		return 1;
	}

	if (atoi(argv[3]) <= 0) {
		fprintf(stderr, "%s\n", "PORT passed is invalid");
		return 1;
	}

	// Extracting variables
	char* player_name = argv[1];
	char* ip_address = argv[2];
	int port = atoi(argv[3]);

	loadQTables();

	play(player_name, ip_address, port);

	saveQTables();
}

/**************** loadQTables() ****************/
/* Loads the Q tables from file, if they exist
 *
 * We return:
 *   Nothing if successful
 *   
 * We guarantee:
 * 	 The Q tables will be loaded with values
 */

void loadQTables() {
	FILE* qFile = fopen("data/qtable", "r");
	FILE* qCountFile = fopen("data/qtablecount", "r");

// Only needed for first iteration of training; skip the files
#ifdef TRAIN
	if (!qFile) {
		if (qCountFile) fclose(qCountFile);
		return;	
	}

	if (!qCountFile) {
		if (qFile) fclose(qFile);
		return;
	}
#else

	// There has to be a set of q tables. Otherwise the algorithm won't work
	mem_assert(qFile, "Error opening q table");
	mem_assert(qCountFile, "Error opening q count table");
#endif
	char* currline;

	// Load Q tables from disk
	for (int i = 4; i < maxPlayerPoints; i++) {
		for (int j = 2; j < maxDealerPoints; j++) {
			for (int k = 0; k < numberActions; k++) {
				if ((currline = file_readLine(qFile))) {
					sscanf(currline, "%f", &Q[i][j][k]);
					mem_free(currline);
				}

				if ((currline = file_readLine(qCountFile))) {
					sscanf(currline, "%d", &Q_count[i][j][k]);
					mem_free(currline);
				}
			}
		}
	}

	// clean up
	fclose(qFile);
	fclose(qCountFile);
}

/**************** saveQTables() ****************/
/* Saves the in-memory Q tables to disk
 *
  * We return:
 *   Nothing if successful
 *   
 * We guarantee:
 * 	 The Q tables will be loaded with values
 */
void saveQTables() {
	FILE* qFile = fopen("data/qtable", "w");
	FILE* qCountFile = fopen("data/qtablecount", "w");

	mem_assert(qFile, "Error openin Q table in saving function");
	mem_assert(qCountFile, "Error opening Q table in saving function");

	// Save to disk in save order as read from disk
	for (int i = 4; i < maxPlayerPoints; i++) {
		for (int j = 2; j < maxDealerPoints; j++) {
			for (int k = 0; k < numberActions; k++) {
				fprintf(qFile, "%f\n", Q[i][j][k]);
				fprintf(qCountFile, "%d\n", Q_count[i][j][k]);
			}
		}
	}

	fclose(qFile);
	fclose(qCountFile);
}


/**************** rewardSaver() ****************/
/* Saves the reward of the round to each state seen
 *
  * We return:
 *   Nothing if successful
 *   
 * We guarantee:
 * 	 the state is saved
 */
void rewardSaver(void* arg, void* item) {
	int* reward = arg;
	int* round = item;
	round[3] = *reward;
} 

/**************** roundBagSaver() ****************/
/* stores the reward from each state 
 * in the round to the Q table
 *
  * We return:
 *   Nothing if successful
 *   
 * We guarantee:
 * 	 the state is saved
 */
void roundBagSaver(void* arg, void* item) {
	int* round = item;
	int player_points = round[0];
	int dealer_points = round[1];
	int action = round[2];
	int reward = round[3];
	if (Q_count[player_points-1][dealer_points-1][action] < 2147483647) { // protect crossing int limit
		Q_count[player_points-1][dealer_points-1][action] += 1; 
		Q[player_points-1][dealer_points-1][action] += (1/(float)Q_count[player_points-1][dealer_points-1][action]) * (reward - Q[player_points-1][dealer_points-1][action]);
	}
}


/**************** play() ****************/
/* 
 * Plays the game until the dealer quits
 * 
 * We return:
 *   Nothing if successful
 *   
 * We guarantee:
 * 	 the states are saved
 */
void play(char* player_name, char* ip_address, int port) {
	// Begin networking set up	
	int socket = connectToDealer(ip_address, port);
	// failed to connect
	if (socket <= 0) exit(99); 

	// Join the game
	char joinmessage[30];
	sprintf(joinmessage, "JOIN %s", player_name);
	// failed to send join 
	if (sendMessage(socket, joinmessage) == -1) exit(99);	
	
	// Recieve Begin message
	char* beginMessage = readMessage(socket);
	// No message received, quit
	if (beginMessage == NULL) {
		exit(99);
	}

	// Check for invalid begin message
	if (strcmp(beginMessage, "BEGIN")) {
		fprintf(stderr, "%s\n", "Unexpected Begin message");
		mem_free(beginMessage);
		exit(99);
	}
	
	// While the dealer begins a game
	while (!strcmp(beginMessage,"BEGIN")) {
		mem_free(beginMessage);

		// Set up new round bag
		bag_t* roundbag = mem_assert(bag_new(), "Round bag not created in play function in TRAIN mode");

		// Setting up hands
		hand_t* phand = mem_assert(newHand(), "Unable to create hand in play function"); // player hand
		hand_t* dhand = mem_assert(newHand(), "Unable to create hand in play function"); // dealer hand
	
		// Getting player hand
		// read the card message
		char* cardm; 
		card_t* card;
		for (int i = 0; i < 2; i++) {
			// Read card message
			cardm = readMessage(socket);
			if (cardm == NULL) {
				fprintf(stderr, "Bad card message: %s\n", cardm);
				exit(99);
			}	
			// Create the card
			card = newPlayerCard(cardm);
			if (card == NULL) {
				fprintf(stderr, "Couldn't create card\n");
				exit(99);
			}
			addToHand(phand, card);
			mem_free(cardm);
		}
	
		// Read card message
		cardm = readMessage(socket);
		if (cardm == NULL) {
			fprintf(stderr, "Bad card message: %s\n", cardm);
			exit(99);
		}	
		// Create the card
		card = newPlayerCard(cardm);
		if (card == NULL) {
			fprintf(stderr, "Couldn't create card\n");
			exit(99);
		}
		addToHand(dhand, card);
		mem_free(cardm);

		// Begin playing game
		// Read decision message
		char* decm = readMessage(socket);
		if (decm == NULL) {
			fprintf(stderr, "Couldn't read decision message\n");
			exit(99);
		}
	
		if (strcmp(decm, "DECISION")) {
			fprintf(stderr, "Unexpected decision message: %s\n", decm);
			exit(99);
		} 
	
		// alloc memory for player's decision (hit or stand)
		char* dec = mem_calloc_assert(6, sizeof(char), "Message for dec not created");
		// create int to hold the player decision
		int decnum = -1;
	
		while (TRUE) {
			int ppoints = getHandScore(phand); // player's hand score
			// If they have more than 20, should always stand
			if (ppoints > maxPlayerPoints - 1) {
				strcpy(dec, "STAND");
				decnum = 1;
			} else {
				// Deciding next move
#ifdef TRAIN
				// If training, choose random action
				if ((rand() % 2)) {
					strcpy(dec, "HIT");
					decnum = 0;
				} else {
					strcpy(dec, "STAND");
					decnum = 1;
				}
#else
				// Get the dealer's score
				int dpoints = getHandScore(dhand);

				// Based on current state, choose decision from q table
				float hit_chance = Q[ppoints-1][dpoints-1][0];
				float stand_chance = Q[ppoints-1][dpoints-1][1];
		
				if (hit_chance >= stand_chance) {
					strcpy(dec, "HIT");
					decnum = 0;
				} else {
					strcpy(dec, "STAND");
					decnum = 1;
				}
#endif
			}
			// Send the message; if it fails, quit
			if (sendMessage(socket, dec) == -1) {
				fprintf(stderr, "%s\n", "Sending message to dealer with move was not possible");
				exit(99);
			}

			// Recording round state (points and decision)
			int* round = mem_calloc_assert(5, sizeof(int), "Unable to create round recording in play function in TRAIN mode");

			round[0] = getHandScore(phand);
			round[1] = getHandScore(dhand);
			round[2] = decnum;
			// Insert into bag of seen states
			bag_insert(roundbag, round);
	
			// Check for Stand, break card receiving loop
			if (!strcmp(dec, "STAND")) break;
	
			// Get next card message
			cardm = readMessage(socket);
			if (cardm == NULL) {
				fprintf(stderr, "Bad card message: %s\n", cardm);
				exit(99);
			}	
			// Create the card
			card = newPlayerCard(cardm);
			if (card == NULL) {
				fprintf(stderr, "Couldn't create card\n");
				exit(99);
			}
			addToHand(phand, card);
			mem_free(cardm);
			
			
			mem_free(decm);
			decm = readMessage(socket);
			// Verify Decision Message
			if (decm == NULL) {
				exit(99);
			}

			// If the player busted, break the card receiving loop
			if (!strcmp(decm, "RESULT BUST")) {
				mem_free(dec);
				break;
			} 
			
			// If didn't receive decision message, quit
			if (strcmp(decm, "DECISION")) {
				fprintf(stderr, "Unexpected decision message: %s\n", decm);
				exit(99);
			}
		} 
		
		// Read a result message from dealer
		char* result;
		if (!strcmp(decm, "RESULT BUST")) {
			// If we already busted, we should expect that
			result = decm;
		}
		else { 
			// read the message from the dealer
			result = readMessage(socket);
			mem_free(decm);
			mem_free(dec);
		}
		
		// quit if no result message received
		if (result == NULL) {
			exit(99);
		} 
		#ifdef TRAIN
		printf("Training Match Result: %s\n", result);
		#else
		// Output the result of the game
		printf("Playing Match Result: %s\n", result);
		#endif
	
		// Saving reward integer
		int reward;
		if (!strcmp(result, "RESULT WIN")) {
			reward = 1;
		} else if (!strcmp(result, "RESULT LOOSE")) {
			reward = -1;
		} else if (!strcmp(result, "RESULT BUST")) {
			reward = -1;
		} else if (!strcmp(result, "RESULT PUSH")) {
			reward = 0;
		} else {
			fprintf(stderr, "%s\n", "Unexpected reward obtained in play function");
			exit(99);
		}
	
		// Freeing memory
		mem_free(result);
		deleteHand(phand);
		deleteHand(dhand);
		
		// Check for next round
		beginMessage = readMessage(socket);	
		if (beginMessage == NULL) {
			exit(99);
		}
		
		// Record Reward
		bag_iterate(roundbag, &reward, rewardSaver);

		// Record match
		bag_iterate(roundbag, NULL, roundBagSaver);

		// Freeing round bag
		bag_delete(roundbag, mem_free);

	}

	// If we got a message other than BEGIN and QUIT
	if (strcmp(beginMessage, "QUIT")) {
		fprintf(stderr, "Strange Message Received %s\n", beginMessage);
		mem_free(beginMessage);
		// Beaking connection
		closeClientConnection(socket);
		exit(0);
	}

	mem_free(beginMessage);
	// Beaking connection
	closeClientConnection(socket);
}	

