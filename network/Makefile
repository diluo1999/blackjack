# Makefile for CS50 Blackjack Final Project network module
# 
# Di Luo
# Nov. 6 2022
# CS 50, Fall 2022

# object files, and the target library
OBJS = network.o
LIB = network.a

# $^ refers to “the dependencies listed in this rule”
# $@ refers to “the target of this rule”
# add -DAPPEST for functional tracking report
# add -DMEMTEST for memory tracking report
# (and run `make clean; make` whenever you change this)
FLAGS = # -DAPPTEST # -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: $(LIB) clean

########### network ##################

$(LIB): $(OBJS)
	ar cr $(LIB) $^

# network source dependencies
network.o: network.h

# test network module with networktest_dealer and networktest_player
# run 'make test_dealer' first, then run 'make test_player'
test_dealer: $(LIB)
	$(CC) $(CFLAGS) networktest_dealer.c $(LIB) -o dealer
	./dealer > testing_dealer.out

test_player: $(LIB)
	$(CC) $(CFLAGS) networktest_player.c $(LIB) -o player
	./player > testing_player.out

# test network module with networktest and valgrind
# run 'make valgrind_dealer' first, then run 'make valgrind_player'
valgrind_dealer: $(LIB)
	$(CC) $(CFLAGS) networktest_dealer.c $(LIB) -o dealer
	$(VALGRIND) ./dealer

valgrind_player: $(LIB)
	$(CC) $(CFLAGS) networktest_player.c $(LIB) -o player
	$(VALGRIND) ./player

# clean up after our compilation
clean:
	rm -f core
	rm -f *.a *~ *.o *.out network dealer player