//NOTE:  Functions that are defined in one component and used in another should be prototyped
//(and documented) in the header. Functions that are for internal use only by a component (e.g.,
//findTile()) should be given internal linkage, so they won't be visible to other components.

#define NUMCOMMANDS 10
#define COMMANDLEN 30

//This is an array of strings (so, really, a two-dimensional array of characters), with room for 
//the 10 most recent commands. Remember, valid commands may be up to 30 characters long.
extern char history[COMMANDLEN][NUMCOMMANDS];

//This records the number of commands in the history. Initially, this will be zero, but it will
//increase as the user enters commands.
extern int histLen;


//skipline prototyped

//getCommand prototyped