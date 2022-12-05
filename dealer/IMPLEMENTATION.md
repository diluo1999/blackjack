# CS50 Blackjack Final Project Dealer module
## Implementation Specification

In this document we reference the [Requirements Specification](../REQUIREMENTS.md) and [Design Specification](../DESIGN.md) and focus on the implementation-specific decisions.
Here we focus on the core subset of topics:

-  Data structures/descripts
-  Descriptions
-  Pseudo code for each of the functions
-  Detailed function prototypes and their parameters

## Data structures/descriptions 

We leverage the data structures provided by cards; otherwise, no major data structures are used.
### Usage

The *dealer* module is defined and implemented in `dealer.c`, and exports the following functions:

```c
void getNewCard(deck_t* deck, hand_t* hand, char* type, int connectedSocket, bool send);
char* findResult(int playerHandScore, int dealerHandScore, int connectedSocket);
static void parseArgs(const int argc, char* argv[], int* games, int* port);
int main(int argc, char* argv[]);
```

### Implementation

The dealer begins in `main` by calling `parseArgs`, to verify that the required arguments have been provided accurately. It then sets up the dealer socket, which connects the player to the dealer.

The *dealer* then checks if the player has joined by waiting for a join message, verifying with a print statement if successful. Then begin the first game.
- NOTE: The dealer is kept in sync with the player by making predetermined messages to the player, like BEGIN, DECISION, and CARD rank of suit. This tells the player the point at which they are at in the game. Similarly, the player messages the dealer HIT or STAND when it is supposed to, and the dealer reads those messages accordingly.

Create a deck struct (from the `cards` class). Tell the player to begin. Create a hand struct, also from the `cards` class. Deal two cards to the player hand, using `getNewCard` to send the player the number and suit, and to add those cards to the player deck. Do the same thing for the dealer hand, but only send the info of one card to the player.
Tell the player to make a decision: hit or stand. If the player hits, draw them another card, otherwise do nothing.
If the player goes over 21, fasttrack them to a bust result.
If the player is not over 21, prompt them to make the same decision until they bust or choose to stand.
Then run a loop to calculate the dealer's hand. Draw cards until the dealer has a score of 17 or greater.
Finally, use the `findResult` method to compare the player and dealer's scores, so that the game either ends in a WIN, LOOSE, BUST, or PUSH result.
Then start the next game or quit.

### Assumptions

No assumptions beyond those that are clear from the spec.


### Files

* `Makefile` - compilation procedure
* `dealer.c` - the implementation of dealer
* `ncdealer.c` - the implementation of ncurses dealer
* `training.sh` - the training script
* `testing.sh` - the testing script

### Compilation

To compile, simply call `make`.

