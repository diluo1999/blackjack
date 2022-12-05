### Testing

The testing.sh script serves as the unit test for the dealer. It first makes a file for output, then navigates to the parent directory to make all dependencies, then navigates back, and tests the program with a bunch of invalid command line arguments. Then, to test functionality, we invoke a valid call to the dealer and invoke the player. The output of the test is saved to testing.out.

This is very similar to the unittest of the player.

To run this test, simply navigate to the dealer directory, then run `make test`.