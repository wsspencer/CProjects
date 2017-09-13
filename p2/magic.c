/**
    @file magic.c
    @author Scott Spencer (wsspence)

    This program reads the user's input and highlights anything it interprests as a magic number.
 */

/** Header file for standard input/output functions we will use */
#include <stdio.h>
/** Header file for the standard boolean data type */
#include <stdbool.h>

/** Constant sequence of chars representing the color RED */
#define RED "\033[31m" //(033 is octal representation of ascii decimal 27)
/** Constant sequence of chars representing the default color */
#define RESET "\033[0m"

/**
    This function takes a char data type and returns a boolean representing whether or not that
    char is a numeric digit.

    @param ch char that we want to know if it is a digit
    @return boolean representing if the parameter is a digit
*/
bool isDigit( char ch ) {
    //Check if param is a digit.
    if ( ch >= '0' && ch <= '9' ) {
        return true;
    }
    return false;
}

/**
    This function takes a char data type and returns a boolean representing whether or not that
    char is an alphabetical value.

    @param ch char that we want to know if it is alphabetical.
    @return boolean representing if the parameter is alphabetical.
*/
bool isAlpha( char ch ) {
    //Check if param is a letter.
    if ( ch >= 'a' && ch <= 'z' ) {
        return true;
    }
    else if ( ch >= 'A' && ch <= 'Z' ) {
        return true;
    }
    return false;
}

/**
    This function prints the character on the stream to output highlighted red, as well as any
    digits that follow that char, then changes the printing color back to the default.

    @return void
*/
void highlightNumber() {
    //If token is an integer, highlight it.
    char ch = getchar();

    //Change print color to red.
    printf( RED );

    //Print the digit (since it's back on the stream).
    while ( isDigit( ch ) ) {
        printf( "%c", ch );
        ch = getchar();
    }
    //Change color back to default.
    printf( RESET );

    //Print the next char (if it's not an EOF).
    if ( ch != EOF ) {
        printf( "%c", ch );
    }
}

/**
    This function prints the char on the stream to output in the default color, as well as
    any chars that follow it (as long as they are numeric digits, alphabetical letters, or
    underscores.

    @return void
*/
void skipIdentifier() {
    //If token is identifier, skip highlighting any digits in this token (move stream to
    //next char after token).
    char ch = getchar();

    //Loop, printing the chars of the token, until the end of the token
    //(excluding tabs since Jenkins tells me that's wrong).
    while ( isAlpha( ch ) || isDigit( ch ) || ch == '_' ) {
        printf( "%c", ch );
        ch = getchar();
    }

    //Print the next char (if it's not an EOF).
    if ( ch != EOF ) {
        printf( "%c", ch );
    }
}

/**
    This is our main function which runs the program, reading in the users input and delegating
    tasks to our helper functions based on that input.

    @return int representing exit success
*/
int main() {
    char ch;

    //Read user input one char at a time.
    ch = getchar();
    while ( ch != EOF ) {
        //Check if ch is the beginning of an identifier (letter or underscore).
        if ( isAlpha( ch ) || ch == '_' ) {
            //iterate through token
            ungetc( ch, stdin );
            skipIdentifier();
        }

        //Check if ch is a digit (that isn't part of an identifier).
        else if ( isDigit( ch ) ) {
            //Put ch back on stream to be read again, then highlight it.
            ungetc( ch, stdin );
            highlightNumber();
        }

        //Print the character.
        else {
            printf( "%c", ch );
        }

        ch = getchar();
    }
    //Exit successfully.
    return 0;
}
