//This component defines function for reading and executing user commands.
/**
   @file command.c
   @author W. Scott Spencer

   This program handles all the command functions that deal with configuration files such as
   skipping a line of input until a newline is found so we may process the next one and whether
   or not a line can be successfully read from the input file.
*/

/** Header file containing standard input/output functions we will use. */
#include <stdio.h>
/** Header file containing standard library functions we will use. */
#include <stdlib.h>
/** Header file containing math functions we will use. */
#include <math.h>
/** Header file containing string functions we will use. */
#include <string.h>
/** Header file containing boolean operations we will use */
#include <stdbool.h>
/** Header file containing the command function prototypes */
#include "command.h"


void skipLine( FILE *stream ) {
    //Read through input until we're pointing at the first char after the next newline.
    char c = 'a';
    //Iterate through the line until we hit a newline, ending our stream on the next char
    while ( c != '\n' ) {
        c = fgetc( stream );
    }
}

bool getCommand( FILE *stream, char cmd[ CMD_LIMIT + 2 ] ) {
    //read string into cmd and return true if successful (fgets will stop at an EOF or newline)
    if ( fscanf( stream, "%[^\n]\n", cmd ) != EOF ) {
        return true;
    }

    //return false if fscanf did not successfully read a line of input
    else {
        return false;
    }
}
