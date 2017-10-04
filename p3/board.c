//This component defines functions for working with the board.

#includes "board.h"

#define DEFAULT_ROWS 5
#define DEFAULT_COLS 7

void initBoard( int rows, int cols, int board[][ cols ] ) {
	int counter = 1;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			board[i][j] = counter;
			counter++;
		}
	}
}

void printBoard( int rows, int cols, int board[][ cols ] ) {
	//TO DO: loop through and print the board to standard output.  (if it's in the solved state, 
	//print it in blue)
	for ( int i = 0; i < rows; i++ ) {
		for ( int j = 0; j < cols; j++ ) {
			printf( "%d ", board[rows][cols]);
		}
		printf("\n");
	}
}

//NEEDS INTERNAL LINKAGE
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
	int *targCol;
	int *targRow;
	
	//If this doesn't change pointer vals, just go back to searching manually
	//check if tile is in board? 
	if ( findTile(tile, rows, cols, board, targRow, targCol) == false ) {
		printf("error message?");
	}
	
	//save the value of the first element in target column before it's overwritten
	int temp = board[0][*targCol];
	//loop through the values in the target column, moving each value "up" until the last (which
	//requires the first value which we have stored in temp).
	for ( int i = 0; i < rows - 1; i++ ) {
		board[i][*targCol] = board[i + 1][*targCol];
	}
	//set the last value in the target column to temp
	board[rows - 1][*targCol] = temp;
}

bool moveDown( int tile, int rows, int cols, int board[][ cols ] ) {
	int *targCol;
	int *targRow;
	
	//If this doesn't change pointer vals, just go back to searching manually
	//check if tile is in board? 
	if ( findTile(tile, rows, cols, board, targRow, targCol) == false ) {
		printf("error message?");
	}
	//save the value of the last element in target column before it's overwritten
	int temp = board[rows - 1][*targCol];
	//loop through the values in target column, moving each value "down" except the first (which
	//requires the last value which we have stored in temp).
	for ( int i = 1; i < rows; i++ ) {
		board[i][*targCol] = board[i - 1][*targCol];
	}
	//set the last value in the target column to temp
	board[0][*targCol] = temp;
}

bool moveLeft( int tile, int rows, int cols, int board[][ cols ] ) {
	int *targCol;
	int *targRow;
	
	//If this doesn't change pointer vals, just go back to searching manually
	//check if tile is in board? 
	if ( findTile(tile, rows, cols, board, targRow, targCol) == false ) {
		printf("error message?");
	}
	//save the value of the last element in target column before it's overwritten
	int temp = board[*targRow][cols - 1];
	//loop through the values in the target row, moving each value "left" except the last (which
	//requires the last value which we have stored in temp).
	for ( int i = 1; i < cols; i++ ) {
		board[*targRow][i] = board[*targRow][i - 1];
	}
	//set the first value in the target column to temp
	board[*targRow][0] = temp;
}

bool moveRight( int tile, int rows, int cols, int board[][ cols ] ) {
	int *targCol;
	int *targRow;
	
	//If this doesn't change pointer vals, just go back to searching manually
	//check if tile is in board? 
	if ( findTile(tile, rows, cols, board, targRow, targCol) == false ) {
		printf("error message?");
	}
	//save the value of the first element in target row before it's overwritten
	int temp = board[*targRow][0];
	//loop through the values in the target row, moving each value "up" until the last (which
	//requires the first value which we have stored in temp).
	for ( int i = 0; i < cols - 1; i++ ) {
		board[*targRow][i] = board[*targRow][i + 1];
	}
	//set the last value in the target row to temp
	board[*targRow][cols - 1] = temp;
}