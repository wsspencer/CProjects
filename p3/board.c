//This component defines functions for working with the board.

/** Header file containing standard input/output functions we will use. */
#include <stdio.h>
/** Header file containing standard library functions we will use. */
#include <stdlib.h>
/** Header file containing math functions we will use. */
#include <math.h>
/** Header file containing string functions we will use. */
#include <string.h>
#include <stdbool.h>
#include "board.h"

/** Constant sequence of chars representing the color BLUE */
#define BLUE "\033[34m" //(033 is octal representation of ascii decimal 27)
/** Constant sequence of chars representing the default color */
#define DEFAULT "\033[0m"


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

//helper bool method to test if puzzle is in initial/solved state
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
		printf(" ");
        for ( int j = 0; j < cols; j++ ) {
            printf( "%2d ", board[i][j]);
        }
        printf("\n");
    }
    //change font color back to default
	if ( solved ) {
		printf( DEFAULT );
	}
	printf("> ");
}

//NEEDS INTERNAL LINKAGE (static)
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

bool moveUp( int tile, int rows, int cols, int board[][ cols ] ) {
    int targCol;
    int targRow;
    
    //If this doesn't change pointer vals, just go back to searching manually
    //check if tile is in board? 
    if ( findTile(tile, rows, cols, board, &targRow, &targCol) == false ) {
        printf("error message?"); //?
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

bool moveDown( int tile, int rows, int cols, int board[][ cols ] ) {
    int targCol;
    int targRow;
    
    //If this doesn't change pointer vals, just go back to searching manually
    //check if tile is in board? 
    if ( findTile(tile, rows, cols, board, &targRow, &targCol) == false ) {
        printf("error message?"); //?
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

bool moveLeft( int tile, int rows, int cols, int board[][ cols ] ) {
    int targCol;
    int targRow;
    
    //If this doesn't change pointer vals, just go back to searching manually
    //check if tile is in board? 
    if ( findTile(tile, rows, cols, board, &targRow, &targCol) == false ) {
        printf("error message?"); //?
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

bool moveRight( int tile, int rows, int cols, int board[][ cols ] ) {
    int targCol;
    int targRow;
    
    //If this doesn't change pointer vals, just go back to searching manually
    //check if tile is in board? 
    if ( findTile(tile, rows, cols, board, &targRow, &targCol) == false ) {
        printf("error message?"); //?
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