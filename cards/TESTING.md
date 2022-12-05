### Testing

The `cardtest.c` program runs the cards built-in test method. This built in method creates a deck and a hand, pulling a card from the deck, inserting it into the hand, and outputting the card and the hand score at each insertion to the hand. It then tests creating cards from string, and does the same insert and output process. This effectively tests every function included in the cards module.

To test, simply navigate to the cards directory, then run `make test`.

It runs with valgrind to verify there are no memory leaks.