/**
    @file codepoints.c
    @author Scott Spencer (wsspence)

    This file defines all the operations involving our unicode table and its navigation.
    It will build the unicode table by dynamically allocating memory as it reads entries in
    the unicode.txt file in our source directory, it will be responsible for the function that
    frees that memory once
*/

/** Header file containing CodePoint type and definitions for functions we will use. */
#include "codepoints.h"
/** Header file containing boolean type and functions we will use. */
#include <stdbool.h>
/** Header file containing standard input/output functions we will use. */
#include <stdio.h>
/** Header file containing string functions we will use. */
#include <string.h>
/** Header file containing standard library functions we will use. */
#include <stdlib.h>

/** Constant for the size of our string buffer. */
#define BUFFER 101

/** Variable for the dynamically allocated array of CodePoints that is our unicode table. */
static struct CodePoint *table;
/** Variable for the size of our unicode table. */
static int size;

/**
    This function will be used by qsort to sort our CodePoint table in order of code values.
    When qsort calls this function it will pass two instances of CodePoints as parameters,
    which we will extract the code values from and subtract from one another, returning the
    difference and letting qsort know if a is bigger (+), b is bigger (-) or the two are
    equal (0).

    @param a const void * an instance of a CodePoint to compare (we cast it to CodePoint)
    @param b const void * an instance of a CodePoint to compare (we cast it to CodePoint)
    @return int representing the difference between codepoint codes
*/
int compareCP( const void * a, const void * b ) {
    //cast params to CodePoints
    CodePoint *cpA = (CodePoint *) a;
    CodePoint *cpB = (CodePoint *) b;
    //return the difference between codepoint a and codepoint b's codes
    return ( cpA->code - cpB->code );
}

/**
    This function is used to load the unicode table as an array of dynamically allocated memory
    containing instances of the CodePoint struct.  Once all values are loaded from the unicode.txt
    file provided in the source directory, this function will call qsort to sort the values.

    @return void
*/
void loadTable() {
    // load the table defined in unicode.txt into memory by way of our static list of Codepoint
    // structs.  It's static in visibility but dynamically allocated in memory.  (Even though
    // the size of the table is not arbitrary we still must allocate the table in memory
    // dynamically since there is not enough stack memory to hold it in its entirety, therefore
    // we will use heap memory to store it.

    //try to open unicode.txt for reading (not a binary file so just read it as ASCII plain text)
    FILE *fp = fopen("unicode.txt", "r");

    //if we fail to open the file, print error message to standard error and exit with status of 1
    if ( fp == NULL ) {
        fprintf( stderr, "Invalid unicode table file" );
        exit(1);
    }

    //read each line of the unicode file.  Each line is organized as such:
    //numeric value for a code point | tab char | name of the code point | end of line char

    //malloc initial memory for table and set its initial size and capacity
    int cap = 10;
    size = 0;
    table = (struct CodePoint *) malloc(cap * sizeof(CodePoint));


    //scan the code and name for each codepoint struct until EOF
    //%x because codes are in hex
    char string[BUFFER];
    while ( fgets(string, BUFFER, fp) ) {
        unsigned int nextCode;
        char nextName[91];
        sscanf(string, "%x %[^\n]", &nextCode, nextName);

        if ( size + 1 >= cap ) {
            cap *= 2;
            table = realloc(table, cap * sizeof(CodePoint));
        }

        table[size].code = nextCode;
        strcpy( table[size].name, nextName );

        //increment the size of the table since scan was successful
        size++;

    }

    //use qsort to sort the codepoints table given its #elements, sizeof a codepoint,
    //and the comparison function we defined above
    qsort( table, size, sizeof(CodePoint), compareCP );

    //close the file
    fclose(fp);
}

/**
    This function is used to free the table of CodePoints.  It's pretty simple really, it makes
    use of the free() function call and frees all of the memory at once since the table is
    allocated as one contiguous block of memory instead of separately allocated blocks.

    @return void
*/
void freeTable() {
    free(table);
}

/**
    This function is used to print the names of chars associated with the unicode values in the
    file given by the user.  A unicode bitfield is passed as a parameter, and a binary search
    is then performed on our table array, if in this search we find the value, we print it and
    return true.  If in the binary search min becomes > than max, we have exhausted the table
    without finding the appropriate value and thus print nothing and return false.

    @param code unisgned int representing a bitfield of unicode we are looking for in the table.
    @return bool telling us if the code was found in the unicode table.
*/
bool reportCode( unsigned int code ) {
    //BINARY SEARCH OF THE ARRAY SINCE IT HAS BEEN QSORTED

    int min = 0;
    int max = size - 1;
    int mid = ( min + max ) / 2;

    while ( min <= max ) {
        if ( table[mid].code < code ) {
            min = mid + 1;
        }
        else if ( table[mid].code == code ) {
            printf( table[mid].name );
            printf("\n");
            return true;
        }
        else {
            max = mid - 1;
        }

        mid = ( min + max ) / 2;
    }
    return false;
}