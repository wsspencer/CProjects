# We're using the default rules for make, but we're using
# these variables to get them to do exactly what we want.
CC = gcc
CFLAGS = -g -Wall -std=c99
LDLIBS = -lm

# This is a common trick.  All is the first target, so it's the
# default.  We use it to build all of the executables we want.
all: nonde

#link nonde with command, label, and parse objects
nonde: nonde.o command.o label.o parse.o

#link command with label and parse objects
command: command.o label.o parse.o

#link label.c and label.h
label: label.o

#linke parse.c and parse.h
parse: parse.o

# Another common trick, a clean rule to remove temporary files, or
# files we could easily rebuild.
clean:
	rm -f command command.o
	rm -f label label.o
	rm -f parse parse.o
	rm -f nonde nonde.o
	rm -f output.txt
