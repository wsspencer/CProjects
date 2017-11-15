#Target all for building the executable.
all: un-utf8

#Compile the programs and link them.
un-utf8: un-utf8.c codepoints.c codepoints.h
	gcc -g -Wall -std=c99 un-utf8.c codepoints.c -o un-utf8

#Clean up the files leftover after building.
clean:
	rm -f un-utf8
	rm -f output.txt
