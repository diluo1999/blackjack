# CS50 Blackjack Final Project
## Testing

### Unit Test

Network: see [Network Testing.md](network/TESTING.md).

Cards: see [Cards Testing.md](cards/TESTING.md).

Player: see [Player Testing.md](player/TESTING.md).

Dealer: see [Dealer Testing.md](dealer/TESTING.md).


### Integration Test

The player and dealer unit-tests serve as integration tests for the full project. They concurrently run the dealer and player programs, which integrates all four modules: cards, network, dealer and player. We can examine the results in the output files. 

To run these tests, navigate to either the dealer or player directories respectively, and simply run `make test` in the terminal.

### Other notes
We tested player and dealer with valgrind in manual tests to detect memory leaks. There are none that we know of.