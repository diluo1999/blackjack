# CS50 Blackjack Final Project
# Design Specification

According to the [Requirements Spec](REQUIREMENTS.md), the Blackjack game consists of two programs, a dealer and a player. This document describes the implementation-independent design decisions. Here we focus on:

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures

## User interface
### Player 
As described in the [Requirements Spec](REQUIREMENTS.md), the player’s only interface with the user is on the command-line; it must always have 3 arguments.

```bash
$ player <player name> <server’s IP address> <PORT number>
```
For example, my name is Alex, and I would like to play with the dealer on port 8090 on the IP address 192.158.1.38. The syntax to do some would be 
```bash
$ ./player Alex 192.158.1.38 8090
```

### Dealer 
As described in the [Requirements Spec](REQUIREMENTS.md), the player’s only interface with the user is on the command-line; it must always have 2 arguments.

```bash
$ ./dealer  <num games> <PORT>
```
For example, we would like to play 10 games with Alex on port 8090. The syntax to do so would be 
```bash
$ ./dealer 10 8090
```

## Inputs and outputs
### Player
*Input:* Per the [Requirements spec](REQUIREMENTS.md), the player must be able to read in the Q table and Q count table from disk. To standardize, we will use the directory ‘data/qtable’ for the Q table, and ‘data/qtablecount’ for the Q count table. The player will read these tables into 3D matrices in the program, to be used and updated.

*Output:* The player updates these Q tables, and writes them to file before finishing execution of a training session.  

## Functional decomposition into modules
### Player
We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules. Uses an ifdef to determine whether to play or train
 2. *loadQTables*, which loads the Q counts table and loads the Q averages Table
 3. *play*, which communicates with the dealer and plays the games, choosing the optimal move in every given situation based on the Q tables, unless it is in training mode, in which case it chooses an action randomly.
 4. *updateQTables*, which updates the values in the Q averages table and the Q counts table based on the state of cards seen and the result of the game.
 5. *saveQTables*, which re-writes the Q table files on the disk with the updated Q tables. 
  
### Dealer
1. *main*, which parses arguments and initializes other modules.
2. *play*, which plays the games by sending messages back and forth to the player.
3. *getNewCard*, which pulls a card from the deck, adds it to a hand, and sends the appropriate message to the player.
4. *findResult*, which determines the result of the game.

### Other Modules 

We define a couple of other modules to provide data structures and helper functions to the player and dealer, especially for dealing with the network and card aspects of the dealer and player. 

In network module, we expect: 

1. *setUpDealerSocket*, which opens a socket of a dealer for a player to connect to.
2. *connectToDealer*, which connects a player to the dealer socket at a specific Port and IP
3. *sendMessage*, which sends a message to a socket
4. *readMessage*, which reads a message from a socket
5. *closeServerSocket*, which ends the connection and closes the server socket
6. *closeClientConnection*, which disconnects the client from the server.

In card module, which we leverage the ADT with, we expect: 

Structs:
1. *Struct card*, to model each card in the deck
2. *Struct hand*, to model a hand of cards
3. *Struct deck*, to model a deck of cards
   
Functions: 

1. *newPlayerCard*, creates a new card from a player message, returns a pointer to that card
2. *newDeck*, creates a new deck, returns a pointer to that deck
3. *newHand*, creates a new hand, returns a pointer to that hand
4. *newPlayerCard*, creates a card object from a string, return a pointer to that card
5. *cardToString*, creates a message string from a card object, returns that string
6. *pullCard*, removes a card from the deck, returns a pointer to the card
7. *addToHand*, which adds a card to a hand of cards.
8. *deleteDeck*, which deletes a deck and all of the cards in the deck
9. *deleteHand*, which deletes a hand and all of the cards in the hand
10. *getHandScore*, which returns the score of the cards in the hand.

## Pseudo code for logic/algorithmic flow

The player will run as follows:

    parse the command line, validate parameters, initialize other modules
    use loadQTables to load the Q tables from the disk into memory
    connect to the dealer server
    while game is being played  
        Receive “begin” from dealer
        Receive “CARD Rank of Suit” from dealer twice
        Receive “DEALER Rank of Suit” from dealer
        Receive “DECISION” from dealer
        Send “HIT” or “STAND” to dealer (use an ifdef statement to determine whether to train or play)
        While hitting
            Receive “CARD Rank of Suit” from dealer
            Receive “DECISION” from dealer
            Send “HIT” or “STAND” to dealer
        Receive “RESULT” from dealer
        Use updateQTables to update the Q tables
        Receive “QUIT” from dealer and end loop, or Receive “BEGIN” and continue to next iteration
    Use saveQTables to save the updated Q tables to disk

The dealer will run as follows:

    parse the command line, validate 0 parameters, initialize other modules
    Set up a server socket, wait for player to connect
    For the number of games to play:
        Create a new shuffled deck of cards
        Send “BEGIN” to the player
        Pull a card from the deck
        Add the card to a hand for the player
        Send the card’s “CARD Rank of Suit” to the player
        Pull a card from the deck
        Add the card to a hand for the player
        Send the card’s “CARD Rank of Suit” to the player
        Pull a card from the deck
        Add the card to a hand for the dealer
        Pull a card from the deck
        Add the card to a hand for the dealer
        Send the card’s “DEALER Rank of Suit” to the player
        Send “DECISION” to the player
        Receive “HIT” or “STAND” from player 
        While the player hits 
            Pull a card from the deck
            Add the card to a hand for the player
            Send the card’s “CARD Rank of Suit” to the player
            Receive “HIT” or “STAND” from player 
        While dealer hand total < 17
            Pull a card from the deck    
            Add the card to a hand for the dealer
        Use findResult to determine the outcome of the game
        Send “RESULT WIN|LOOSE|PUSH|BUST”  to the player
    Send “QUIT” to the player


## Major data structures

Helper modules provide all the data structures we need:

- *deck* which is a *bag* of 52 *card* structs
- *hand* which is a *bag* of *card* structs
