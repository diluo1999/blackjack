# CS50 Blackjack Final Project
## Requirements Spec

> In a requirements spec, **shall do** means **must do**.

The Blackjack game consists of two programs, a dealer and a player. The player has two modes: a training mode to determine the optimal move in a given state, and a player mode to play the game with the knowledge of those optimal moves.

The player program **shall**:

1. Execute the player from a command line with usage syntax
   * `./player <player name> <server’s IP address> <PORT number>` 

1. validate the player received exactly three command-line arguments and that 
    * `server’s IP address` is a valid IP address that is hosting a blackjack game, and
    * `PORT number` is a valid dealer that is hosting a blackjack game;

1. Connect to the server using a socket
1. Have a training mode that plays many games with the dealer, choosing random actions  
1. Load the Q values table and Q count table from disk. Be able to save those tables to disk.
1. Have a play mode where it makes optimal decisions based on the values in the Q table

The dealer program **shall**:

1. Execute the dealer from a command line with usage syntax
   * `./dealer <number of games> <port>` 
    
1. Set up a server socket listening for a client/player to connect
1. Once the client connects, pass messages back and forth to the client, doing the following
    2. Create a deck of 52 cards for each game
    2. Shuffle the deck
    2. Deal cards to the player by sending messages with the card suit and rank as string 
    2. Receive hit/stand messages from the client
    2. Calculate the result of the game and send message to the client
    2. Reset and play again 
1. Send quit message to client to finish


The blackjack game *may assume* that

* only the messages of JOIN, BEGIN, CARD, DEALER, DECISION, HIT or STAND, RESULT, QUIT will be sent between the client and server.
* Card information will be passed with the syntax of “<Number> of <Suit>”, such as “Nine of Hearts”

