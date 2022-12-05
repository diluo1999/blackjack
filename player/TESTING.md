### Testing

The testing.sh script serves as the unit test for the player. It first makes a file for output, then navigates to the parent directory to make all dependencies, then navigates back, and tests the program with a bunch of invalid command line arguments. Then, to test functionality, we invoke a valid call to the dealer and invoke the player. We also test this with the pre-compiled dealer to check for compatibility. The output of the test is saved to testing.out.

This is very similar to the unittest of the dealer.

To run this test, simply navigate to the player directory, then run `make test`.