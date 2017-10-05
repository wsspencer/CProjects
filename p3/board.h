/**
    @file board.h
    @author W. Scott Spencer

    This file is a header file containing global variables and function prototypes for functions
    that deal with our board.
*/

/* A constant representing the number of rows in our board when no config file is found. */
#define DEFAULT_ROWS 5
/* A constant representing the number of columns in our board if no config file is found. */
#define DEFAULT_COLS 7

/**
    This function initializes the board's tile values, each tile is an int array index that
    is ordered sequentially, counting left to right and top to bottom, with 1 occupying the
    top left index.

    @param rows int the number of rows in our board.
    @param cols int the number of columns in our board.
    @param board[][ cols ] int a 2 dimensional array holding the tiles of our board.
    @return void
*/
void initBoard( int rows, int cols, int board[][ cols ] );

/**
    This function prints our puzzle board.  Each tile is printed separated by a space and
    occupying at least 2 digit width.
    @param rows int the number of rows in our board.
    @param cols int the number of columns in our board.
    @param board[][ cols ] int a 2 dimensional array holding the tiles of our board.
	@return void
*/
void printBoard( int rows, int cols, int board[][ cols ] );

/**
    This function moves the column of the target tile up by one, with the top-most tile
    overflowing back to the vacant bottom position.
    @param rows int the number of rows in our board.
    @param cols int the number of columns in our board.
    @param board[][ cols ] int a 2 dimensional array holding the tiles of our board.
*/
bool moveUp( int tile, int rows, int cols, int board[][ cols ] );

/**
    This function moves the column of the target tile down by one, with the bottom-most tile
    overflowing back to the vacant bottom position.
    @param rows int the number of rows in our board.
    @param cols int the number of columns in our board.
    @param board[][ cols ] int a 2 dimensional array holding the tiles of our board.
*/
bool moveDown( int tile, int rows, int cols, int board[][ cols ] );

/**
    This function moves the row of the target tile to the right by one, with the right-most tile
    overflowing back to the vacant left-most position.
    @param rows int the number of rows in our board.
    @param cols int the number of columns in our board.
    @param board[][ cols ] int a 2 dimensional array holding the tiles of our board.
*/
bool moveRight( int tile, int rows, int cols, int board[][ cols ] );

/**
    This function moves the row of the target tile to the left by one, with the left-most tile
    overflowing back to the vacant right-most position.
    @param rows int the number of rows in our board.
    @param cols int the number of columns in our board.
    @param board[][ cols ] int a 2 dimensional array holding the tiles of our board.
*/
bool moveLeft( int tile, int rows, int cols, int [][ cols ] );