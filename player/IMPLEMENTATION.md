# CS50 Blackjack Final Project Player module
## Implementation Specification

In this document we reference the [Requirements Specification](../REQUIREMENTS.md) and [Design Specification](../DESIGN.md) and focus on the implementation-specific decisions.
Here we focus on the core subset of topics:

-  Data structures/descripts
-  Descriptions
-  Pseudo code for each of the functions
-  Detailed function prototypes and their parameters

## Data structures/descriptions 

Besides external data structures such as Card and Bag, the main data structure 
used here are the Q tables:

```
Q[21 (Maximum number of points the player can have)][11 (Maximum number of points the dealer has that are visible to the player)][2 (Number of actions)]
Q_count[21][11][2]
```

The Q matrix, which consists of two three dimensional arrays stored in global memory, composed of these two files, stores the likelihood of winning based on the number of cards that the player and the dealer each have and on the action to take.
If in a given game the dealer has 3 total points and the player has a total of 10 points, then you would use those numbers to index both the q table and the q count table and use 0 to index a HIT action and 1 to index a STAND action. Then, the Q/Q_count value is taken as a fraction for each action (HIT and STAND) and this fraction denotes how likely you are to win by taking each action. 
These table are loaded and stored from disk to memory by the following functions:

```
void loadQTables();
void saveQTables();
```  

### Usage

The player is eventually compiled to be an executable file that can be run from the command line in the following way:

```
./player <player_name> <ip_address> <port_number>
```

### Implementation

The first thing the player algorithm does is validate the command line parameters. It ensures that there's three arguments and that the port number is an integer greater than zero

It then uses the `loadQTables` function to load the Q tables to global memory. After that it invokes the play function which handles everything going forward.

The play function first tries to set up a connection to the dealer. If sucessful, it sends a 'JOIN' message with the player name. After that it expects to read a begin message from the socket assigned to the conneciton.

It then begins a loop that iterates through every game. In the loop, it reads two card messages from the socket, creates a hand struct to represent the player hand, and adds the two cards received to the player hand. It goes through a similar process to instantiate the dealer hand. 

It then creates a bag of integer arrays where it stores the information of each round and later saves it to the Q tables.

Once both hands are initialized in memory, it reads a decision message from the socket which tells the player to begin the game. It then enters another loop that iterates through every move in the respective game. The first thing it does while in this iteration is it tries to decide on what move to make. If the TRAIN macro is on, it decides randomly between HIT or STAND and send the message decided upon to the dealer socket. Otherwise, it looks up the likelihood of winning with each hand on the q tables and chooses what move to implement based on that. 

It creates a small integer array on heap memory where it saves the dealer's points, the player's points, and the action taken. This array is saved later on the round bag created before beginning the game.

If the decision was to STAND, it exists the move loop, otherwise it reads a card message again and savees the card to the deck. It then reads a decision message and goes through another iteration around the loop. If instead of getting a decision message it obtains a bust message, which means that the player busted and it exists the move loop.

It then reads from the dealer socket a result message which says how the game ended. It then prints that message to the standard output. Based on the result (whether the player won, lost, busted, or a push occurred), and, in conjunction with the bag of arrays that represent the rounds, it updates the q tables with new information.

It then expects either a BEGIN or a QUIT message. If a BEGIN message is sent, it iterates over the game loop once more and begins a new game,  otherwise, it exits the loop, frees all memory, closes the socket connection, and exits completely.
  
### Assumptions

The only assumptions made are that the player name provided to the command line will not be longer than 29 characters and that the ip address and PORT numbers are valid and such a server with those identifications exists.

### Files

* `Makefile` - compilation procedure
* `player.c` - player executable source code
* `training.sh` - bash command that begins training process
* `testing.sh` - bash command that begins testing process

### Compilation

To compile, simply run `make` on the command line.
