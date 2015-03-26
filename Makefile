# The compiler:
CC = g++

# Compiler flags:
CFLAGS = -g -Wall

all: datalog

datalog: datalog.o parser.o
	$(CC) $(CFLAGS) -o datalog datalog.o parser.o

datalog.o: datalog.cpp
	$(CC) $(CFLAGS) -c datalog.cpp

parser.o: parser.cpp
	$(CC) $(CFLAGS) -c parser.cpp

clean:
	$(RM) datalog datalog.o parser.o
