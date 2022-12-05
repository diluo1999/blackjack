# CS50 Blackjack Final Project Cards module
## Implementation Specification

In this document we reference the [Requirements Specification](../REQUIREMENTS.md) and [Design Specification](../DESIGN.md) and focus on the implementation-specific decisions.
Here we focus on the core subset of topics:

-  Data structures/descripts
-  Descriptions
-  Pseudo code for each of the functions
-  Detailed function prototypes and their parameters

## Data structures/descriptions 

We use three data structures: 
1. *card*, to model each card in the deck
2. *hand*, to model a hand of cards
3. *deck*, to model a deck of cards

The card is distinguished by its suit and number. It also holds an integer value of the score for that card. 

The 'hand' and 'deck' structures leverage the bag ADT to hold the cards. In fact, the 'deck' is simply a wrapper of the bag ADT. 

The 'hand' holds a bag of the card structs in the hand, as well as a total score for the cards in the hand.

The 'deck' starts filled with 52 shuffled cards. Each time `pullCard` is called on a deck, it removes a card from the deck. 
### Usage

The *cards* module is defined in `cards.h` and implemented in `cards.c`, and exports the following functions:

```c
deck_t* newDeck(void);
hand_t* newHand(void);
card_t* newPlayerCard(char* cardString);
card_t* pullCard(deck_t* deck);
void addToHand(hand_t* hand, card_t* card);
int getHandScore(hand_t* hand);
void deleteDeck(deck_t* deck);
void deleteHand(hand_t* hand);
void cardTest(void);
```

### Implementation

The *card* is represented as a `struct card` containing an `enum suits` for the suit of the card, an `enum nums` for the vount on the card, and an `int val` for the value of the card. 

The *hand* is represented as a `struct hand` containing an `bag_t* cards` for a bag of the cards, and an `int score` for the value of the hand. 

The *deck* is represented as a `bag_t`. 

To create a new deck in `newDeck`, first a new bag is created, then an array of 52 numbers is created and shuffled, and then a card is created for each array and inserted into the bag. That deck bag is then returned.

To create a new card for the deck in `newCard`, the id of the card is passed in, and the suit is calculated by taking (id-1)/13+1, and the number is calculated by taking (id-1)%13+1. For the value, we look at the card number: if 1, then value will be 11 (this represents an ace), if greater then 10, then the value will be 10, otherwise the value will just be the number.

To create a new card for the player based on a message from the dealer in `newPlayerCard`, the card string in passed into the function, scanned to extract the number and suit of the card, then we search a suit and number array for a match that will determine the suit/num int value, and then the val of the card is determined with the same logic as above.

To translate a card to a string, as is done by the dealer, `cardToString` takes in the target (who is receiving the card) and the card info itself, and extract the matching strings from the metadata stored in the card to pass back the message formatted as outlined in the design spec.

To pull a card from a deck in `pullCard`, we simply call bag_extract on the deck bag object.

To add a card to a hand in `addToHand`, the card is added to the hand's bag, and the score is updated based on the value of the card. If the new score is above 21, then bag_iterate is called to search for aces and reset the value of them to 1 until the score is less than 21. 

The `findAces` function is an item func the bag_iterate function, which cycles through the cards in the hand, if it finds a card with a value of 11, it is set to 1, and the score is decremented by 10. This takes advantage of the flexibility of the value of an ace. 

The `deleteHand` method calls `bag_delete` with the `cardDelete` (which just frees the card) item func, then frees the hand.

The `deleteDeck` method simply calls `bag_delete` with the `cardDelete` (which just frees the card) item func.

### Assumptions

No assumptions beyond those that are clear from the spec.


### Files

* `Makefile` - compilation procedure
* `cards.h` - the interface
* `cards.c` - the implementation

### Compilation

To compile, simply `make cards.o`.

