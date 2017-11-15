/**
    @file un-utf8.c
    @author Scott Spencer (wsspence)

    This program opens an input file taken as a command line argument from the user and reads it
    in binary mode with the purpose of reading unicode and matching that unicode to an entry in a
    table containing codes and names associated with those codes.  The table defining which codes
    are associated with different characters is defined in a txt file called unicode.txt which our
    program reads into an array of dynamically allocated memory, then sorts using qsort and a
    comparison function that organizes the array by code value.  Each entry in the array is a
    struct we call CodePoint that contains both the code and the name of the character associated
    with that code.  Once the table is loaded we read in the user's input file in binary mode one
    byte at a time.  That byte will give us some formatting information telling us how many bytes
    are part of this multibyte code (0XXXXXXX for one byte, 110XXXXX for two, 1110XXXX for three,
    11110XXX for four bytes) and each subsequent byte in a multibyte code will begin with ten.
    By masking and bit shifting we remove these formatting bits so we might compare that code with
    the codes in our unicode table.  The program also reports several errors to stderr such as if
    the user gives more than one command line argument, if the input file provided by the user
    cannot be opened, if a multibyte code read by the program hits the end of the file before the
    entire code can be read, if the code does not contain correct formatting bits, if the code does
    not exist in the unicode table, or if the code is unnecessarily long (was represented as a
    multibyte code that could have been represented in fewer bytes).
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

/** Constant for the maximum command line arguments. */
#define MAXARGS 2
/** Constant for the mask check of a one byte code. */
#define ONEBYTE 0x80
/** Constant for the mask check of a two byte code. */
#define TWOBYTE 0xE0
/** Constant for the mask check of a three byte code. */
#define THREEBYTE 0xF0
/** Constant for the mask check of a four byte code. */
#define FOURBYTE 0xF8
/** Constant for shifting bitfields over the "10" format bits. */
#define FORMATSHIFT 6
/** Constant for the mask check of "10" format bits. */
#define FORMATBITS 0x80
/** Constant for incrementing position two bytes. */
#define INCTWOBYTES 2
/** Constant for incrementing position three bytes. */
#define INCTHREEBYTES 3
/** Constant for incrementing position four bytes. */
#define INCFOURBYTES 4
/** Constant for masking off all but lowest eight bits. */
#define MASKEIGHTBITS 0xFF
/** Constant for masking off all but lowest twelve bits. */
#define MASKTWELVEBITS 0xFFF
/** Constant for masking off all but lowest sixteen bits. */
#define MASKSIXTEENBITS 0xFFFF
/** Constant for masking off all but lowest twenty bits. */
#define MASKTWENTYBITS 0xFFFFF
/** Constant for checking the high bits are "110". */
#define CHECKTWOHIBITS 0xC0
/** Constant for checking if high bits are "1110". */
#define CHECKTHREEHIBITS 0xE0
/** Constant for checking if high bits are "11110". */
#define CHECKFOURHIBITS 0xF0
/** Constant for masking off the two highest bits. */
#define REMOVETWOHIBITS 0x3F
/** Constant for masking off the lowest seven bits. */
#define LOWESTSEVENBITS 0xF80
/** Constant for masking off the lowest eleven bits. */
#define LOWESTELEVENBITS 0xF800
/** Constant for masking off the lowest sixteen bits. */
#define LOWESTSIXTEENBITS 0xF0000

/**
    This is our main function which executes at run.  It will read our user's input, delegate
    params to our helper functions as needed, and ultimately print the output .ppm file.

    @param argc int representing the number of command line arguments passed when
    @param argv **char array of strings, each index is one of the command line arguments passed
    @return int for the exit status (either failure or success).
*/
int main(int argc, char **argv) {
    //If the user tries to run the program with invalid command-line arguments (anything other
    //than a single filename and the program call), it will print an error message to standard
    //error and exit with an exit status of 1.
    if (argc != MAXARGS) {
        fprintf(stderr, "usage: un-utf8 <input-file>\n");
        exit(1);
    }

    //Try to open the file whose name the user input in binary mode (input will contain bytes
    //that aren't ordinary 7-bit ASCII characters.
    FILE *fp;
    fp = fopen(argv[1], "rb");

    //if the file cannot be opened, print an two messages to standard error and exit with an exit
    //status of one.
    if (fp == NULL) {
        fprintf(stderr, "Can't open file: ");
        fprintf(stderr, argv[1]);
        fprintf(stderr, "\nusage: un-utf8 <input-file>\n");
        exit(1);
    }


    //Read in a table of unicode characters, mapping each character's code to a name for that
    //character.  Use that table to read each character code in the user's input file and print
    //out the name for each character.
    loadTable();

    //NOTE:
    //If code is one-seven bits, it will be represented by one byte beginning with "0."
    //If code is eight-eleven bits, it will be represented by two bytes.  The first beginning with
    //"110" and the second with "10."
    //If code is twelve-sixteen bits, it will  be represented by three bytes.  The first beginning
    //with "1110" and the second and third with "10."
    //If the code is seventeen to twenty-one bits, it will be represented by four bytes.  The first
    //beginning with "11110" and the second, third, and fourth with "10."


    //loop for every byte in the file
    unsigned int input;
    //read into "input" 1 element of size 1 byte of memory from fp

    int position = 0;
    int increment = 0;
    bool printName;

    while (fread(&input, 1, 1, fp) == 1) {
        //update increment for position
        increment = 1;
        //set printName to true
        printName = true;

        //case of 1 byte read (bin "0XXXXXXX"), let i increment (note, this will also handle null)
        if ( (input & ONEBYTE) == 0x0) {
            //mask off all but the last eight bits (technically seven but we've already established
            //the first bit is 0)
            input = input & MASKEIGHTBITS;
        }

        //case of two bytes read (bin "110XXXXX"), increment i by two
        else if ( (input & TWOBYTE) == CHECKTWOHIBITS) {
            //new int that we can add to the end of input
            unsigned int concat;

            //read another byte into input (if you hit eof before you can, print error)
            if ( fread(&concat, 1, 1, fp) != 1 ) {
                fprintf(stderr, "Incomplete code at %d\n", position);
                //set printName to false
                printName = false;
            }

            //check that concat begins with "10" (as a non-first byte in a multibyte code should)
            else if ( (concat & CHECKTWOHIBITS) != FORMATBITS ) {
                fprintf( stderr, "Invalid byte: 0x%X at %d\n", concat, position + 1 );
                //set printName to false
                printName = false;
            }

            //we know concat's byte will begin with "10" so go ahead and remove the first two bits
            concat = concat & REMOVETWOHIBITS;
            //shift input over and add concat to its end (only shift six since we've removed the
            //first two bits)
            input = input << FORMATSHIFT;
            //bitwise OR to concatenate the two bitfields
            input = input | concat;

            //mask off all but the last twelve bits
            input = input & MASKTWELVEBITS;

            //check if bitfield is overlong (can be represented in shorter byte string)
            //we check this by checking if anything but the lowest seven bits remains.
            //(if there isn't, we could have represented it with 0-seven bits)
            if (  (input & LOWESTSEVENBITS) == 0x00 ) {
                fprintf(stderr, "Invalid encoding: 0x%X at %d\n", (input & MASKTWELVEBITS),
                        position);
                //set printName to false
                printName = false;
            }

            //update increment for position
            increment = INCTWOBYTES;
        }

        //case of three bytes read (bin "1110XXXX"), increment i by three
        else if ( (input & THREEBYTE) == CHECKTHREEHIBITS) {
            //new int bitfields that we can add to the end of input
            unsigned int firstByte;
            unsigned int secondByte;

            //read another two bytes into input
            fread(&firstByte, 1, 1, fp);
            //read another byte into input (if you hit eof before you can, print error)
            if ( fread(&secondByte, 1, 1, fp) != 1 ) {
                fprintf(stderr, "Incomplete code at %d\n", position);
                //set printName to false
                printName = false;
            }

            //check that the second two bytes begin with "10" (as a non-first byte in a multibyte
            //code should)
            else if ( (firstByte & CHECKTWOHIBITS) != FORMATBITS ) {
                fprintf( stderr, "Invalid byte: 0x%X at %d\n", firstByte, position + 1 );
                //set printName to false
                printName = false;
            }
            else if ( (secondByte & CHECKTWOHIBITS) != FORMATBITS ) {
                fprintf( stderr, "Invalid byte: 0x%X at %d\n", secondByte, position + 2 );
                //set printName to false
                printName = false;
            }

            //we know concat's two bytes will begin with "10" so go ahead and remove the first
            //two bits from both bytes
            firstByte = firstByte & REMOVETWOHIBITS;
            secondByte = secondByte & REMOVETWOHIBITS;

            //shift input over and add the two bytes minus their two highest bits
            input = input << FORMATSHIFT;
            input = input | firstByte;
            input = input << FORMATSHIFT;
            input = input | secondByte;

            //mask off all but the last sixteen bits
            input = input & MASKSIXTEENBITS;

            //check if bitfield is overlong (can be represented in shorter byte string)
            //we check this by checking if anything but the lowest 11 bits remains (because if
            //there isn't, we could have represented it in 0-eleven bits)
            if (  (input & LOWESTELEVENBITS) == 0x00 ) {
                fprintf(stderr, "Invalid encoding: 0x%X at %d\n", (input & MASKSIXTEENBITS),
                        position);
                //set printName to false
                printName = false;
            }

            //update increment for position
            increment = INCTHREEBYTES;
        }

        //case of four bytes read (bin "11110XXX"), increment i by four
        else if ( (input & FOURBYTE) == CHECKFOURHIBITS) {
            //new int bitfields that we can add to the end of input
            unsigned int firstByte;
            unsigned int secondByte;
            unsigned int thirdByte;

            //read another three bytes into input
            fread(&firstByte, 1, 1, fp);
            fread(&secondByte, 1, 1, fp);
            //read third byte into input (if you hit eof before you can, print error)
            if ( fread(&thirdByte, 1, 1, fp) != 1 ) {
                fprintf(stderr, "Incomplete code at %d\n", position);
                //set printName to false
                printName = false;
            }

            //check that the last three bytes begin with "10" (as a non-first byte in a multibyte
            //code should)
            else if ( (firstByte & CHECKTWOHIBITS) != FORMATBITS ) {
                fprintf( stderr, "Invalid byte: 0x%X at %d\n", firstByte, position + 1 );
                //set printName to false
                printName = false;
            }
            else if ( (secondByte & CHECKTWOHIBITS) != FORMATBITS ) {
                fprintf( stderr, "Invalid byte: 0x%X at %d\n", secondByte, position +
                        INCTWOBYTES );
                //set printName to false
                printName = false;
            }
            else if ( (thirdByte & CHECKTWOHIBITS) != FORMATBITS ) {
                fprintf( stderr, "Invalid byte: 0x%X at %d\n", thirdByte, position +
                        INCTHREEBYTES );
                //set printName to false
                printName = false;
            }

            //we know concat's two bytes will begin with "10" so go ahead and remove the first two
            //bits from all three bytes
            firstByte = firstByte & REMOVETWOHIBITS;
            secondByte = secondByte & REMOVETWOHIBITS;
            thirdByte = thirdByte & REMOVETWOHIBITS;
            //shift input over and add the three bytes minus their two highest bits
            input = input << FORMATSHIFT;
            input = input | firstByte;
            input = input << FORMATSHIFT;
            input = input | secondByte;
            input = input << FORMATSHIFT;
            input = input | thirdByte;

            //mask off all but the last twenty bits
            input = input & MASKTWENTYBITS;

            //check if bitfield is overlong (can be represented in shorter byte string)
            //we check this by checking if anything remains but the lowest sixteen bits.
            //(if nothing does, we could have represented it in 0-sixteen bits)
            if (  (input & LOWESTSIXTEENBITS) == 0x00 ) {
                fprintf(stderr, "Invalid encoding: 0x%X at %d\n", (input & MASKTWENTYBITS),
                        position);
                //set printName to false
                printName = false;
            }

            //update increment for position
            increment = INCFOURBYTES;
        }
        //otherwise...it must be an invalid code, so print an error message
        else {
            fprintf(stderr, "Invalid byte: 0x%X at %d\n", input, position);
            //set printName to false
            printName = false;
        }

        //report code (if code isn't found, print error message)
        if ( printName && !reportCode(input) ) {
            //otherwise min > max so param code wasn't found in the binary search of table
            fprintf(stderr, "Unknown code: 0x%X at %d\n", input, position);
        }

        //update position
        position += increment;
    }

    //Once we are done with the table we should free its memory by calling freeTable()
    freeTable();
}
