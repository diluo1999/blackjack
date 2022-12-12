# CS50 Final Project: Blackjack

## Description

See the course web site for [detailed description of the project](https://www.cs.dartmouth.edu/~tjp/cs50/project/index.html)

This repo is a copy of the original private repo created by GitHub Classroom. In the project, 4 authors pushed 145 commits to main with 52 push requests. 16 branches were created. There were 12,990 additions and 7,972 deletions in total.

## Documentation

[Requirements Specification](REQUIREMENTS.md)

[Design Specification](DESIGN.md)

[Implementation Specification](IMPLEMENTATION.md)

[Testing Specification](TESTING.md)

## Notes
### Training (# of Games)
In our training, we were primarily limited by time. We also understood that the value of a specific index in the Q table wouldn't matter in playing beyond being greater or less than the opposite action for the same state. We saw that even just after 5,000 tests, the two states would not flip in terms of average reward. Since we had the ability to, however, we decided to run an approximately 1,000,000 extra games overnight. With the run time running <5 games per second, for the 7 hours, it probably got through around 100,000-125,000 games. Combined with other training done before it, we trainied around a total of 150,000 games probably. We hope this would eliminate any fluctuations between preference of one action over the other. Training was done on babylon1 server, so that is the IP address being used.

### NCurses

We did implement a version of the game in ncurses. It nicely lays out the process of the execution of the dealer: it opens with a welcome message, then the user can press any key to continue, which will bring the user to the first game, and then the user can press any key to continue through the games, until the number of games declared have been played. 

To use the ncurses version of dealer, please navigate to the dealer directory, run `make`, then use the command line syntax `./ncdealer <number of games> <port>` to run the program. 

The logic and implementation is almost exactly the same as the normal dealer program.

### Potential Flaws

Occassionally, when running a large load of games (>500) at once, we have seen connection issues pop up. These are very rare, and are likely due to a network instability issue that is out of our control. It gets handled with the message "reading message failed: Connection reset by peer" and the player is able to keep going with a new game.

## Acknowledgement

Credits to fellow programmers Alexander Fick (afick), Juan Felipe Suarez (jfsuarezb), Jake Olson (jake-olson), learning fellow Jordan Mann (jrmann100), and TA Bansharee Ireen (banshee56).