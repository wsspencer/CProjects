CC = gcc
CFLAGS = -g -Wall -std=c99 

all:		board command puzzle

board:		board.o board.h

command:	command.o command.h

puzzle:		puzzle.o board.h

clean:
	rm -f board board.o
	rm -f command command.o
	rm -f output.txt
