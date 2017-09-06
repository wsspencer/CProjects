# We're using the default rules for make, but we're using
# these variables to get them to do exactly what we want.
CC = gcc
CFLAGS = -g -Wall -std=c99
LDLIBS = -lm

# This is a common trick.  All is the first target, so it's the
# default.  We use it to build both of the executables we want.
all: magic frame

magic: magic.o

frame: frame.o

# Another common trick, a clean rule to remove temporary files, or
# files we could easily rebuild.
clean:
	rm -f magic magic.o
	rm -f frame frame.o
	rm -f output.txt
	rm -f output.ppm
