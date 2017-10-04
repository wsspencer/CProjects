all: puzzle

puzzle: puzzle.c board.c command.c board.h command.h
	gcc -g -Wall -std=c99 puzzle.c command.c board.c -o puzzle

clean:
	rm -f puzzle
	rm -f output.txt
