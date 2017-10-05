/**
    @file board.c
    @author W. Scott Spencer

    This file handles all the functions that deal with our board.  This includes
    initiating the board with values, printing the board, checking if the board is
    in the initial/solved state, and performing movement operations on the board
    such as up, down, left, and right.
*/

/** Header file containing standard input/output functions we will use. */
#include <stdio.h>
/** Header file containing standard library functions we will use. */
#include <stdlib.h>
/** Header file containing math functions we will use. */
#include <math.h>
/** Header file containing string functions we will use. */
#include <string.h>
/** Header file containing boolean operations we will use. */
#include <stdbool.h>
/** Header file containing the function prototypes for these functions. */
#include "board.h"

/** Constant sequence of chars representing the color BLUE */
#define BLUE "\033[34m" //(033 is octal representation of ascii decimal 27)
/** Constant sequence of chars representing the default color */
#define DEFAULT "\033[0m"

/**
    This function is documented in board.h.
*/
void initBoard( int rows, int cols, int board[][ cols ] ) {
    int counter = 0;
    int i = 0;
    int j = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            counter++;
            board[i][j] = counter;
        }
    }
    //set to initial state
}

/**
    This function checks whether or not our board is in the initial/solved state.
    @param rows int the number of rows in our board.
    @param cols int the number of columns in our board.
    @param board[][ cols ] int a 2 dimensional array holding the tiles of our board.
    @return bool telling us whether or not the board is in the solved state.
*/
static bool checkSolved( int rows, int cols, int board[][ cols ] ) {
    int counter = 0;
    for ( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
            counter++;
            if ( board[i][j] != counter ) {
                return false;
            }
        }
    }
    return true;
}

/**
    This function is documented in board.h.
*/
void printBoard( int rows, int cols, int board[][ cols ] ) {
    //TO DO: loop through and print the board to standard output.  (if it's in the solved state,
    //print it in blue)

    bool solved = false;
    //check if puzzle is solved, if it is, change font color to blue
    printf("\n");
    if ( checkSolved( rows, cols, board ) ) {
        printf( BLUE );
        solved = true;
    }

    for ( int i = 0; i < rows; i++ ) {
        //print starting space
        for ( int j = 0; j < cols; j++ ) {
            printf( " %2d", board[i][j]);
        }
        printf("\n");
    }
    //change font color back to default
    if ( solved ) {
        printf( DEFAULT );
    }
    printf("> ");
}

/**
    This function search for a tile in the board and returns whether or not it exists on the board.
    @param tile int the number of the tile we want to perform the operation on.
    @param rows int the number of rows in our board.
    @param cols int the number of columns in our board.
    @param board[][ cols ] int a 2 dimensional array holding the tiles of our board.
    @param *r int a pointer passed by reference representing our target tile's row.
    @param *c int a pointer passed by reference representing our target tile's column.
    @return bool telling us whether or not the tile was found on the board.
*/
static bool findTile( int tile, int rows, int cols, int board[][ cols ], int *r, int *c ) {
    for ( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
            if ( board[i][j] == tile ) {
                *r = i;  //Set the pointer value of r to the variable i
                *c = j;  //Set the pointer value of c to the variable j
                return true;
            }
        }
    }
    return false;
}

/**
    This function is documented in board.h.
*/
bool moveUp( int tile, int rows, int cols, int board[][ cols ] ) {
    int targCol;
    int targRow;

    //If this doesn't change pointer vals, just go back to searching manually
    //check if tile is in board?
    if ( findTile(tile, rows, cols, board, &targRow, &targCol) == false ) {
        printf("Invalid command\n"); //?
        return false;
    }

    //save the value of the first element in target column before it's overwritten
    int temp = board[0][targCol];
    //loop through the values in the target column, moving each value "up" until the last (which
    //requires the first value which we have stored in temp).
    for ( int i = 0; i < rows - 1; i++ ) {
        board[i][targCol] = board[i + 1][targCol];
    }
    //set the last value in the target column to temp
    board[rows - 1][targCol] = temp;

    return true; //?
}

/**
    This function is documented in board.h.
*/
bool moveDown( int tile, int rows, int cols, int board[][ cols ] ) {
    int targCol;
    int targRow;

    //If this doesn't change pointer vals, just go back to searching manually
    //check if tile is in board?
    if ( findTile(tile, rows, cols, board, &targRow, &targCol) == false ) {
        printf("Invalid command\n"); //?
        return false;
    }
    //save the value of the last element in target column before it's overwritten
    int temp = board[rows - 1][targCol];
    //loop through the values in target column, moving each value "down" except the first (which
    //requires the last value which we have stored in temp).
    for ( int i = rows - 1; i > 0; i-- ) {
        board[i][targCol] = board[i - 1][targCol];
    }
    //set the last value in the target column to temp
    board[0][targCol] = temp;

    return true; //?
}

/**
    This function is documented in board.h.
*/
bool moveLeft( int tile, int rows, int cols, int board[][ cols ] ) {
    int targCol;
    int targRow;

    //If this doesn't change pointer vals, just go back to searching manually
    //check if tile is in board?
    if ( findTile(tile, rows, cols, board, &targRow, &targCol) == false ) {
       printf("Invalid command\n"); //?
        return false;
    }
    //save the value of the first element in target row before it's overwritten
    int temp = board[targRow][0];
    //loop through the values in the target row, moving each value "up" until the last (which
    //requires the first value which we have stored in temp).
    for ( int i = 0; i < cols - 1; i++ ) {
        board[targRow][i] = board[targRow][i + 1];
    }
    //set the last value in the target row to temp
    board[targRow][cols - 1] = temp;

    return true; //?
}

/**
    This function is documented in board.h.
*/
bool moveRight( int tile, int rows, int cols, int board[][ cols ] ) {
    int targCol;
    int targRow;

    //If this doesn't change pointer vals, just go back to searching manually
    //check if tile is in board?
    if ( findTile(tile, rows, cols, board, &targRow, &targCol) == false ) {
        printf("Invalid command\n"); //?
        return false;
    }
    //save the value of the last element in target column before it's overwritten
    int temp = board[targRow][cols - 1];
    //loop through the values in the target row, moving each value "left" except the last (which
    //requires the last value which we have stored in temp).
    for ( int i = cols - 1; i > 0; i-- ) {
        board[targRow][i] = board[targRow][i - 1];
    }
    //set the first value in the target column to temp
    board[targRow][0] = temp;

    return true; //?
}
