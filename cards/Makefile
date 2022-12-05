# Makefile for 'cards' module
#
# Alex Fick, Final Project
# CS 50, Fall 2022

U = ../utils

OBJS = cards.o $U/mem.o $U/bag.o
LIBS = $U/utils.a
LIB = cards.a

# comment the following to turn off verbose memory logging
# TESTING=-DTESTING

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$U
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: $(LIB) clean

all: $(LIB)

$(LIB): $(OBJS) 
	ar cr $(LIB) $^ 


cardtest: $(LIB) 
	$(CC) $(CFLAGS) cardtest.c $(LIB) -o $@
	./cardtest

cards.o: $U/bag.h $U/mem.h cards.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f cards.a
	rm -f cardtest
	rm -f core
