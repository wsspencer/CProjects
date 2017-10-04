//This component will contain the main function, and any other functions that don't belong in one
// of the other components (i.e., any functions that don't pertain to manipulating the board or
// handling commands).

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
#include "command.h"

#define DEFAULT_ROWS 5
#define DEFAULT_COLS 7

bool runCommand( char cmd[ CMD_LIMIT + 1 ], int rows, int cols, int board[][ cols ] ) {
	//return true if command is valid (undo and exit are handled in main)
	if ( strcmp( cmd, "left" ) == 0 || strcmp( cmd, "right" ) == 0 || strcmp( cmd, "up" ) == 0
			|| strcmp( cmd, "down" ) == 0 ) {
		return true;
	}
	//return false if command is invalid
	return false;
}

int main(int argc, char** argv) {
	//Initialize variables for argc and argv so they can be used externally
	int rows = DEFAULT_ROWS;
	int cols = DEFAULT_COLS;
	FILE *fp;
	
	char quit[] = "quit";
	char undo[] = "undo";
	char up[] = "up";
	char down[] = "down";
	char left[] = "left";
	char right[] = "right";

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
			exit(1);
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
			exit(1);
		}

		//scan the board size from the config file and update them
		fscanf(fp, "%d %d", &rows, &cols);

		//TO DO: if size is invalid or missing, exit with status of 1 and print "Invalid Configuration"
	}
	
	// Make the board
	int board[ rows ][ cols ];
	//initialize board
	initBoard( rows, cols, board );
	//print the initial board
	printBoard( rows, cols, board );
	
	if (argc > 1) {
		//read moves from the configuration file
		char *move = "";
		int val;
		
		//use skipline to move to the next line in the file, over the newline char?
		//skipline is in command.c....
		skipLine(fp);

		//while getCommand returns true, execute that command
		while( getCommand( fp, move ) ) {
			//check if command has a value with it
			if ( fscanf(fp, "%s%d", move, &val) == 2 ) {
				//do nothing
			}
			//otherwise, figure out what command needs to do
			else {
				fscanf( fp, "%s", move );
				if ( strcmp( move, quit ) == 0 ) {
					return 0;
				}
				else if ( strcmp( move, undo ) == 0 ) {
					//set move = to the last stored command and val = its val
					//then just let the below conditions take care of it
				}
			}
			if (runCommand( move, rows, cols, board )) {
				if ( strcmp( move, left ) == 0 ) {
					moveLeft( val, rows, cols, board );
				}
				else if ( strcmp( move, right ) == 0 ) {
					moveRight( val, rows, cols, board );
				}
				else if ( strcmp( move, up ) == 0 ) {
					moveUp( val, rows, cols, board );
				}
				else if ( strcmp( move, down ) == 0 ) {
					moveDown( val, rows, cols, board );
				}
			}
			//Otherwise the command is not valid
			else {
				printf("Invalid Configuration"); //do I need to print this to stderr?
				exit(1);
			}
			printBoard( rows, cols, board );
			//TO DO: store command in memory
		}

		//close the configuration file
		fclose(fp);
	}
	//if no config file,
	else {
		
		char move[CMD_LIMIT];
		int val;
		
		//scan commands from user in terminal
		while ( scanf( "%s", move ) != EOF ) {
			//retrieve the command and value from line implement scanf for undo and exit
			//check if command has a value with it
			if ( scanf( "%d", &val ) == 1 ) {
				//do nothing
			}
			//otherwise, figure out what command needs to do
			else {
				
				if ( strcmp( move, quit ) == 0 ) {
					return 0;
				}
				else if ( strcmp( move, undo ) == 0 ) {
					//set move = to the last stored command and val = its val
					//then just let the below conditions take care of it
				}
			}
			if ( runCommand( move, rows, cols, board ) ) {
				if ( strcmp( move, left ) == 0 ) {
					moveLeft( val, rows, cols, board );
				}
				else if ( strcmp( move, right ) == 0 ) {
					moveRight( val, rows, cols, board );
				}
				else if ( strcmp( move, up ) == 0 ) {
					moveUp( val, rows, cols, board );
				}
				else if ( strcmp( move, down ) == 0 ) {
					moveDown( val, rows, cols, board );
				}
			}
			//Otherwise the command is not valid
			else {
				printf( "Invalid Configuration" ); //do I need to print this to stderr?
				exit(1);
			}
			printBoard( rows, cols, board );
			//TO DO: store command in memory
		}
	}
	
	return 0;
}