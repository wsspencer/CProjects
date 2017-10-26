#Target all for building the executable.
all: shopping

#Compile the programs and link them.
shopping: shopping.c item.c list.c list.h item.h
    gcc -g -Wall -std=c99 shopping.c list.c item.c -o shopping

#Clean up the files leftover after building.
clean:
    rm -f shopping
    rm -f output.txt
