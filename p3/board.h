//NOTE:  Functions that are defined in one component and used in another should be prototyped
//(and documented) in the header. Functions that are for internal use only by a component (e.g.,
//findTile()) should be given internal linkage, so they won't be visible to other components.

#define DEFAULT_ROWS 5
#define DEFAULT_COLS 7

//initBoard prototyped
void initBoard( int rows, int cols, int board[][ cols ] );

//printBoard prototyped
void printBoard( int rows, int cols, int board[][ cols ] );

//moveUp prototyped
bool moveUp( int tile, int rows, int cols, int board[][ cols ] );

//moveDown prototyped
bool moveDown( int tile, int rows, int cols, int board[][ cols ] );

//moveLeft prototyped
bool moveRight( int tile, int rows, int cols, int board[][ cols ] );

//moveRight prototyped
bool moveLeft( int tile, int rows, int cols, int [][ cols ] );