# CS50 Blackjack Final Project Network module
## Testing

Note: default server IP address is 129.170.64.104. To run with different IP address, go to `networktest_player.c` and modify `SERV_ADDR` to your IP address. To get your IP address, run

```
ip addr
```

or

```
wget -O - -q https://checkip.amazonaws.com
```
 
Two testing files `networktest_dealer.c` and `networktest_player.c` are implemented to simulate dealer and player respectively. 

The executable of `networktest_dealer.c` is `dealer` and that of `networktest_player.c` is `player`. Two executables should be run on two separate terminals. `dealer` initializes a socket by calling setUpDealerSocket() and waits for `player` to connect. `player` initializes a socket and connects to `dealer` by calling connectToDealer(). Once connected, `dealer` and `player` read from the socket with readMessage() and write to the socket with sendMessage() as below.

player:
1. send "JOIN di"
1. read ("BEGIN")
1. read ("CARD Seven of Diamonds")
1. send "STAND"

dealer:
1. read ("JOIN di")
1. send "BEGIN"
1. send "CARD Seven of Diamons"
1. read "STAND"

The messages are a simplified version of game play. Test results stored in testing_dealer.out and testing_player.out prove the validity of the module. Defensive programming techniques are employed throughout the testing files.

At the end, `dealer` calls closeServerSocket() to close connection and to stop listening. `player` calls closeClientConnection() to close connection.

Valgrind testing can also be run as stated in Usage section.
