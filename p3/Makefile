#Target all for building the executable.
all: puzzle

#Compile the programs and link them.
puzzle: puzzle.c board.c command.c board.h command.h
	gcc -g -Wall -std=c99 puzzle.c command.c board.c -o puzzle

#Clean up the files leftover after building.
clean:
	rm -f puzzle
	rm -f output.txt
