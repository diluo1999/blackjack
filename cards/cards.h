/* 
 * cards.h - header file for 'cards' module
 * 
 * A 'card' is a model of a playing card. 
 * A 'deck' is a model of a deck of playing cards. 
 * It is a wrapper of the bag module. 
 * It starts full with 52 shuffled cards, 
 * and a card can be removed one at a time.
 * A 'hand' is a model of a hand of playing cards. It starts empty.
 * It is a wrapper of a bag but also has a score field that 
 * holds the total score of the cards it holds.
 * 
 *
 * Alex Fick, Final Project
 * CS 50, Fall 2022
 */

#ifndef __CARDS_H
#define __CARDS_H

#include <stdio.h>
#include "../utils/bag.h"


/**************** global types ****************/
typedef struct card card_t;  // opaque to users of the module
typedef struct hand hand_t;  // opaque to users of the module
typedef bag_t deck_t;  // opaque to users of the module

/**************** functions ****************/

/**************** newDeck ****************/
/* Create a new (shuffled) deck.
 *
 * We return:
 *   pointer to a new deck, or NULL if error.
 * We guarantee:
 *   The deck is initialized shuffled.
 * Caller is responsible for:
 *   later calling deleteDeck.
 */
deck_t* newDeck(void);

/**************** newHand ****************/
/* Create a new (empty) hand. 
 *
 * We return:
 *   pointer to a new hand, or NULL if error.
 * We guarantee:
 *   The hand begins empty.
 *   The score is initialized to 0 and card bag is empty
 * Caller is responsible for:
 *   later calling deleteHand.
 */
hand_t* newHand(void);

/**************** newPlayerCard ****************/
/* Create a new card from a string passed by the player. 
 *
 * We return:
 *   pointer to a new card, or NULL if error.
 * Caller is responsible for:
 *      adding the card to a hand.
 *      later calling deleteHand.
 */
card_t* newPlayerCard(char* cardString);

/**************** cardToString ****************/
/* Create a string message from a card object 
 *
 * We return:
 *   char pointer of the card string.
 * Caller is responsible for:
 *      saying whose card it is.
 */
char* cardToString(char* target, card_t* card);

/**************** pullCard ****************/
/* Extract a card from the deck, 
 * return pointer to the card
 *
 * Caller provides:
 *   a valid deck pointer.
 * We guarantee:
 *   a NULL or empty deck is ignored.
 *   the card is removed from the bag.
 */
card_t* pullCard(deck_t* deck);

/**************** addToHand ****************/
/* Add a card to a hand of cards. 
 * Update the score of the hand.
 *
 * Caller provides:
 *   valid hand and card pointer.
 * We return:
 *   return nothing.
 * We guarantee:
 *   the card gets added to the hand.
 * Caller is responsible for:
 *   deleting the hand once it is done being used.
 */
void addToHand(hand_t* hand, card_t* card);

/**************** getHandScore ****************/
/* Return the score of the hand provided by the caller.
 * 
 * Caller provides
 *   a valid pointer to a hand
 * We return:
 *   The score if the deck is valid
 *   0 if the deck is not valid.
 */
int getHandScore(hand_t* hand);

/**************** deleteDeck ****************/
/* Delete the whole deck. 
 *
 * Caller provides:
 *   a valid deck pointer.
 * We guarantee:
 *   we ignore NULL deck.
 * Notes:
 *   the order in which items are deleted is undefined.
 */
void deleteDeck(deck_t* deck);


/**************** deleteHand ****************/
/* Delete the whole hand. 
 *
 * Caller provides:
 *   a valid hand pointer.
 * We guarantee:
 *   we ignore NULL hand.
 * Notes:
 *   the order in which items are deleted is undefined.
 */
void deleteHand(hand_t* hand);

/**************** cardTest ****************/
/* Tests the functions in the cards module
 *
 * We guarantee:
 *   we print the state of the hand at each step.
 */
void cardTest(void);

#endif // __CARDS_H