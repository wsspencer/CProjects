/**
    @file puzzle.c
    @author W. Scott Spencer

    This file handles the functions that handle our program.  It includes main which runs the
    program, runCommand which checks if a command can be run.  The purpose of our program is to
    run a puzzle game.  The puzzle starts off in a sequential, solved, state and is altered by
    the user in an attempt to re-solve the puzzle.  The board can be altered by moving a selected
    tile up, left, down, or right.  If the move is horizontal, the entire row moves with the
    selected tile and if the move is verticle the entire column moves with the selected tile.
*/

/** Header file containing standard input/output functions we will use. */
#include <stdio.h>
/** Header file containing standard library functions we will use. */
#include <stdlib.h>
/** Header file containing math functions we will use. */
#include <math.h>
/** Header file containing string functions we will use. */
#include <string.h>
/** Header file containing boolean operations we will use here. */
#include <stdbool.h>
/** Header file containing function prototypes for functions in board.c we use. */
#include "board.h"
/** Header file containing function prototypes for functions in command.c we use. */
#include "command.h"

/* Constant representing the number of rows a board has when no config file is present. */
#define DEFAULT_ROWS 5
/* Constant representing the number of columns a board has when no config file is present. */
#define DEFAULT_COLS 7
/* Constant representing the string of the "quit" command */
#define QUIT "quit"
/* Constant representing the string of the "undo" command */
#define UNDO "undo"
/* Constant representing the string of the "up" command */
#define UP "up"
/* Constant representing the string of the "down" command */
#define DOWN "down"
/* Constant representing the string of the "left" command */
#define LEFT "left"
/* Constant representing the string of the "right" command */
#define RIGHT "right"
/* Constant for an exit code definition */
#define EXIT_ERROR 1
/* Global variable for the history of commands' target tiles */
char cmdVals[ NUMCOMMANDS ];
/* Global variable for the history of commands */
char history[ NUMCOMMANDS ][ COMMANDLEN ];
/* Global variable for the number of commands stored in memory */
int histLen;

/**
    This function takes a command and determines whether or not that command can be run, and
    if the command can be run, it runs that command.
    @param cmd[ CMD_LIMIT + 1 ] char array holding the command passed by the user.
    @param rows int the number of rows in our board.
    @param cols int the number of columns in our board.
    @param board[][ cols ] int a 2 dimensional array holding the tiles of our board.
    @return bool telling us whether or not the passed command could be run.
*/
bool runCommand( char cmd[ CMD_LIMIT + 1 ], int rows, int cols, int board[][ cols ] ) {
    //return true if command is valid (undo and exit are handled in main)
    int val;
    char move[ CMD_LIMIT ];
    sscanf( cmd, "%s %d", move, &val);
    bool tileChecker = false;
    if ( strcmp( move, LEFT ) == 0 || strcmp( move, RIGHT ) == 0 || strcmp( move, UP ) == 0
            || strcmp( move, DOWN ) == 0 ) {
        //now perform commands
        if ( strcmp( move, LEFT ) == 0 ) {
            tileChecker = moveLeft( val, rows, cols, board );
        }
        else if ( strcmp( move, RIGHT ) == 0 ) {
            tileChecker = moveRight( val, rows, cols, board );
        }
        else if ( strcmp( move, UP ) == 0 ) {
            tileChecker = moveUp( val, rows, cols, board );
        }
        else if ( strcmp( move, DOWN ) == 0 ) {
            tileChecker = moveDown( val, rows, cols, board );
        }
        //store command in memory
        if ( histLen < NUMCOMMANDS ) {
            strcpy( history[ histLen  ], move );
            cmdVals[ histLen ] = val;
            histLen++;
        }
        //If histLen = 10, drop the oldest command out of memory to add the newest one
        else {
            for ( int i = 0; i < NUMCOMMANDS - 1; i++ ) {
                strcpy( history[ i ], history[ i + 1 ] );
                cmdVals[ i ] = cmdVals[ i + 1 ];
            }
            strcpy( history[ histLen - 1 ], move );
            cmdVals[ histLen - 1] = val;
            //do not increment histlen because we are at maximum
        }
    }
    //return false if command is invalid
    return tileChecker;
}

/**
    This is the main function which delegates tasks to other functions and performs tasks that
    run our program.
    @param argc int the number of command line arguments passed in the terminal function call.
    @param argv char** 2d char array, it holds all the command line arguments passed by the user.
    @return int the exit status of the program upon termination.
*/
int main(int argc, char** argv) {
    //Initialize variables for argc and argv so they can be used externally
    int rows = DEFAULT_ROWS;
    int cols = DEFAULT_COLS;
    FILE *fp;
    histLen = 0;

    //check for configuration file.  If it exists, store it in the char array variable "filename."
    //(which we create as a pointer so we don't have to define its size)
    //(to avoid segfault, check argc, or the argument count.  This will be 1 + whatever arguments
    //the user input, since the call to run the program counts as the first argument.  All
    //arguments are stored in the argument vector, or, the argv array)
    if (argc > 1) {
        //If there is a command line argument other than the program call and the configuration
        //file, exit with status of 1 and print to standard error.
        if (argc > 2) {
            fprintf( stderr, "%s", "usage: puzzle [config-file]\n" );
            exit(EXIT_ERROR);
        }

        char* filename;
        filename = argv[1];
        //Use filename to create file pointer and open the configuration file for reading.
        fp = fopen(filename, "r");

        //If we cannot open the given config file name (fopen yields null pointer), exit with a
        //status of 1 and print to standard error.
        if ( fp == NULL ) {
            fprintf( stderr, "Can't open config file: " );
            fprintf( stderr, "%s", filename );
            fprintf( stderr, "\nusage: puzzle [config-file]\n" );
            exit(EXIT_ERROR);
        }

        //scan the board size from the config file and update them
        //also check to make sure there are two numbers at the beginning of the config file
        if ( fscanf(fp, "%d %d", &rows, &cols) != 2 ) {
            fprintf( stderr, "Invalid configuration\n" );
            exit(EXIT_ERROR);
        }

        //if size is invalid or missing, exit with status of 1 and print "Invalid Configuration"
    }

    // Make the board
    int board[ rows ][ cols ];
    //initialize board
    initBoard( rows, cols, board );

    if (argc > 1) {
        //read moves from the configuration file
        char line[ CMD_LIMIT + 2 ];
        char move[ CMD_LIMIT ];
        int val;
        //use skipline to move to the next line in the file, over the newline char?
        //skipline is in command.c....
        skipLine(fp);

        //while getCommand returns true, execute that command
        while ( getCommand( fp, line ) ) {
            //check if command has a value with it
            sscanf( line, "%s", move );
            //check if command is valid
            if ( runCommand( line, rows, cols, board ) ) {
                //If command is valid but tile is not found, return in error
            }
            //otherwise, figure out what command needs to do
            else if ( strcmp( move, QUIT ) == 0 ) {
                return 0;
            }
            else if ( strcmp( move, UNDO ) == 0 ) {
                //set val = last stored command val
                val = cmdVals[ histLen - 1 ];

                //Now reverse the move so it can effectively be "undone"
                if ( strcmp( history[ histLen - 1 ], UP ) == 0 ) {
                    moveDown( val, rows, cols, board );
                }
                else if ( strcmp( history[ histLen - 1 ], DOWN ) == 0 ) {
                    moveUp( val, rows, cols, board );
                }
                else if ( strcmp( history[ histLen - 1 ], RIGHT ) == 0 ) {
                    moveLeft( val, rows, cols, board );
                }
                else if ( strcmp( history[ histLen - 1 ], LEFT ) == 0 ) {
                    moveRight( val, rows, cols, board );
                }
                strcpy( history[ histLen - 1 ], "" );
                cmdVals[ histLen - 1 ] = 0;
                if ( histLen > 0 ) {
                    histLen--;
                }
            }

            //command is invalid
            else {
				fprintf( stderr, "Invalid configuration\n" );
                exit(EXIT_ERROR);
            }
        }
        //close the configuration file
        fclose(fp);
    }
    //print the board
    printBoard( rows, cols, board );
    //if no config file, start here

        char move[CMD_LIMIT];
        char line[CMD_LIMIT + 1];
        int val;

        while ( fgets( line, sizeof( line ), stdin ) != NULL ) {
            //retrieve the command and value from line implement scanf for undo and exit
            //check if command has a value with it
            //parse the line
            sscanf( line, "%s", move );
            if ( strcmp( move, QUIT ) == 0 ) {
                //quit program
                return 0;
            }
            else if ( strcmp( move, UNDO ) == 0 ) {
                //set val = last stored command val
                val = cmdVals[ histLen - 1 ];

                //Now reverse the move so it can effectively be "undone"
                if ( strcmp( history[ histLen - 1 ], UP ) == 0 ) {
                    moveDown( val, rows, cols, board );
                }
                else if ( strcmp( history[ histLen - 1 ], DOWN ) == 0 ) {
                    moveUp( val, rows, cols, board );
                }
                else if ( strcmp( history[ histLen - 1 ], RIGHT ) == 0 ) {
                    moveLeft( val, rows, cols, board );
                }
                else if ( strcmp( history[ histLen - 1 ], LEFT ) == 0 ) {
                    moveRight( val, rows, cols, board );
                }
                strcpy( history[ histLen - 1 ], "" );
                cmdVals[ histLen - 1 ] = 0;
                if ( histLen > 0 ) {
                    histLen--;
                }
            }
            else if ( runCommand( line, rows, cols, board ) ) {
                //Do nothing, runCommand will run the command
                //We don't want undo here because it will store the command in memory
            }
            //Otherwise the command is not valid
            else {
                //check if string ends in null terminator, if it doesn't the line is too
                //long and we should skip to the next.

                //line feed so we don't keep reading an oversize line
                printf( "Invalid command\n" ); //do I need to print this to stderr?
            }
            //print board
            printBoard( rows, cols, board );
        }
    return 0;
}
