/**
    @file codepoints.h
    @author Scott Spencer (wsspence)

    This is a header file that outlines all the operations involving our unicode table and its
    navigation in codepoints.c.  It will build the unicode table by dynamically allocating memory
    as it reads entries in the unicode.txt file in our source directory, it will be responsible for
    the function that frees that memory once
*/

/** Header file containing boolean type and functions we will use. */
#include <stdbool.h>
/** Header file containing standard input/output functions we will use. */
#include <stdio.h>
/** Header file containing string functions we will use. */
#include <string.h>
/** Header file containing standard library functions we will use. */
#include <stdlib.h>

/** Struct/type-definition for the CodePoint type. */
typedef struct CodePoint{
    //numeric code
    unsigned int code;

    //string name (won't be longer than 90 chars, but make room for null terminator)
    char name[91];

} CodePoint;

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
int compareCP( const void * a, const void * b );

/**
    This function is used to load the unicode table as an array of dynamically allocated memory
    containing instances of the CodePoint struct.  Once all values are loaded from the unicode.txt
    file provided in the source directory, this function will call qsort to sort the values.

    @return void
*/
void loadTable();

/**
    This function is used to free the table of CodePoints.  It's pretty simple really, it makes
    use of the free() function call and frees all of the memory at once since the table is
    allocated as one contiguous block of memory instead of separately allocated blocks.

    @return void
*/
void freeTable();

/**
    This function is used to print the names of chars associated with the unicode values in the
    file given by the user.  A unicode bitfield is passed as a parameter, and a binary search
    is then performed on our table array, if in this search we find the value, we print it and
    return true.  If in the binary search min becomes > than max, we have exhausted the table
    without finding the appropriate value and thus print nothing and return false.

    @param code unisgned int representing a bitfield of unicode we are looking for in the table.
    @return bool telling us if the code was found in the unicode table.
*/
bool reportCode( unsigned int code );
