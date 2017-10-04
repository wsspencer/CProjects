CC = gcc
CFLAGS = -g -Wall -std=c99

all: puzzle.o board.o puzzle.o

puzzle.o: board.h command.h

board.o: board.h

command.o: command.h

clean:
	rm -f board board.o
	rm -f command command.o
	rm -f puzzle puzzle.o
	rm -f output.txt
