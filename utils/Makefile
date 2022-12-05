# Makefile for utils library
# 	This is basically a library that contains useful stuff
# Taken from CS50 TSE
# CS50 Fall 2022

OBJS = mem.o file.o bag.o
LIB = utils.a

# Build the library
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: Object files depend on header files
mem.o: mem.h
file.o: file.h
bag.o: bag.h

.PHONY: clean

clean:
	rm -rf core
	rm -rf utils.a *.o
