/**
    @file command.h
    @author W. Scott Spencer

    This is a header file that contains the prototypes for global variables, constants, and
    functions that we use for commands that aid our reading the input configuration file.
*/

/* This is a constant for the maximum number of performed commands we will store in memory. */
#define NUMCOMMANDS 10
/* This is a constant for the maximum length of commands that we are storing in memory. */
#define COMMANDLEN 30
/* This is a constant for the maximum length of a command we will accept. */
#define CMD_LIMIT 30

/* This global variable defined elsewhere holds the history of our commmands. */
extern char history[ NUMCOMMANDS ][ COMMANDLEN ];

/* This global variable defined elsewhere holds the number of commands held in memory. */
extern int histLen;

/**
    This function skips through the current line and the newline character to get our stream
    to the desired next line.
    @param *stream FILE pointer stream to our configuration file.
    @return void
*/
void skipLine( FILE *stream );

/**
    This function retrieves a line from the configuration file and returns whether or not a
    command could be retrieved.
    @param *stream FILE pointer stream to our configuration file
    @param cmd[ CMD_LIMIT + 2 ] the command we are reading into.
    @return bool telling us if a command could be retrieved or not.
*/
bool getCommand( FILE *stream, char cmd[ CMD_LIMIT + 2 ] );
