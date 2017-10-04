//This component will contain the main function, and any other functions that don't belong in one
// of the other components (i.e., any functions that don't pertain to manipulating the board or
// handling commands).

#includes "board.h"
#includes "command.h"

#define DEFAULT_ROWS 5
#define DEFAULT_COLS 7

bool runCommand( char cmd[ CMD_LIMIT + 1], int rows, int cols, int board[][ cols ] ) {
	//run a command?
	//return true if command is valid
	//return fals if command is invalid
}

int main(int argc, char** argv) {
	//Initialize variables for argc and argv so they can be used externally
	int rows = DEFAULT_ROWS;
	int cols = DEFAULT_COLS;

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
		File *fp = fopen(filename, "r");
		
		//If we cannot open the given config file name (fopen yields null pointer), exit with a
		//status of 1 and print to standard error.
		if ( fp == NULL ) {
			fprintf( stderr, "%s", "Can't open config file: " + filename +
						"\nusage: puzzle [config-file]\n");
			exit(1);
		}

		//scan the board size from the config file and update them
		fscanf(fp, "%d %d", &rows, &cols);

		//TO DO: if size is invalid or missing, exit with status of 1 and print "Invalid Configuration"

		//Don't close the file yet, we still need to read the moves from it
	}

	// Make the board
	int board[ rows ][ cols ];
	
	//initialize board
	initBoard( rows, cols, board );
	
	//perform moves to the board (IF there was a configuration file found)
	if (argc > 1) {
		//read moves from the configuration file
		char move[];
		int val;
		
		//use skipline to move to the next line in the file, over the newline char?
		//skipline is in command.c....
		skipLine(fp);

		//while getCommand returns true, execute that command
		while( getCommand( fp ) ) {
			//use file pointer to read in command?
			fscanf(fp, "%s %d", move, &val);
			//If there are invalid move commands, print "Invalid Configuration" and exit with a
			//status of 1.
			//Loop for as many times as the user input, performing the proper move each loop
			for (int i = 0; i < val; i++) {
				if (runCommand( move, rows, cols, board )) {
				//Command will execute
				}
				else {
					printf("Invalid Configuration"); //do I need to print this to stderr?
					exit(1);
				}
			}
		}

		//close the configuration file
		fclose(fp);
	}
	
	return 0;
}