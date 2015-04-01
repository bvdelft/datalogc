# The compiler:
CC = g++

# Compiler flags:
CFLAGS = -g -Wall -std=c++11

all: datalog

bruteforce.o: bruteforce.cpp
	$(CC) $(CFLAGS) -c bruteforce.cpp

datalog: datalog.o parser.o bruteforce.o structures.o
	$(CC) $(CFLAGS) -o datalog datalog.o parser.o bruteforce.o structures.o

datalog.o: datalog.cpp bruteforce.h structures.h
	$(CC) $(CFLAGS) -c datalog.cpp

parser.o: parser.cpp structures.h
	$(CC) $(CFLAGS) -c parser.cpp

structures.o: structures.cpp
	$(CC) $(CFLAGS) -c structures.cpp

clean:
	$(RM) datalog datalog.o parser.o
