#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label.h"
#include "parse.h"

/** Copy the given string to a dynamically allocated character array.
    @param str the string to copy.
    @return the dynamically allocated copy of the string.
*/
static char *copyString( char const *str )
{
  char *cpy = (char *) malloc( strlen( str ) + 1 );
  return strcpy( cpy, str );
}

//////////////////////////////////////////////////////////////////////
// Print Command

// Representation for a print command, derived from Command.
typedef struct {
  // Documented in the superclass.
  int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
  int line;

  /** Argument we're supposed to print. */
  char *arg;
} PrintCommand;

// execute function for the print command
static int executePrint( Command *cmd, LabelMap *labelMap, int pc )
{
  // Cast the this pointer to the struct type it really points to.
  PrintCommand *this = (PrintCommand *)cmd;

  // Right now, we're assuming the argument is a literal value, but
  // you'll need to add support for variables.
  // WORK NEEDED HERE: add compatibility for variables as well as literals
  // Environment variables are stored in the standard library,
  // so we should use getenv and setenv to work with them...
  printf( "%s", this->arg + 1 );

  return pc + 1;
}

/** Make a command that prints the given argument to the terminal.
    @param arg The argument to print, either a string literal or the
    name of an environment variable.
    @return a new Command that implements print.
 */
static Command *makePrint( char const *arg )
{
  // Allocate space for the PrintCommand object
  PrintCommand *this = (PrintCommand *) malloc( sizeof( PrintCommand ) );
  
  // Remember pointers to our overridable methods and line number.
  this->execute = executePrint;
  this->line = getLineNumber();
  
  //WORK NEEDED HERE:  THIS IS WHERE YOU ADDED VARIABLE COMPATIBILITY FOR PRINTING
  //Check if token is a variable (environmental or otherwise since we set normal variables as
  //environmental ones?) name, if it is, change its arg's value to the value of the variable?
  if ( isVarName( arg ) ) {
    this->arg = copyString( getenv( arg ) ); 
  }  
  
  // Make a copy of the argument.
  else {
	this->arg = copyString( arg );  
  }
  
  // Return the result, as an instance of the Command interface.
  return (Command *) this;
}

Command *parseCommand( char *cmdName, FILE *fp )
{
  // Read the first token.
  char tok[ MAX_TOKEN + 1 ];

  // Figure out what kind of command it is.
  // SCOTT'S WORK: Is this how we figure out variables?
  
  //If command is print
  if ( strcmp( cmdName, "print" ) == 0 ) {
    // Parse the one argument to print.
    expectToken( tok, fp );
    requireToken( ";", fp );
	
    return makePrint( tok );
  } 
  //If command is Set
  else if ( strcmp( cmdName, "set" ) == 0 ) {
	  //two args: set first to second's value
  }
  //If command is Add
  else if ( strcmp( cmdName, "add" ) == 0 ) {
	  //three args, add second and third, store value in first
  }
  //If command is Sub
  else if ( strcmp( cmdName, "sub" ) == 0 ) {
	  //three args, subtract third FROM second, store value in first
  }
  //If command is Mult
  else if ( strcmp( cmdName, "mult" ) == 0 ) {
	  //three args, multiply second and third, store value in first
  }
  //If command is Div
  else if ( strcmp( cmdName, "div" ) == 0 ) {
	  //three args, divide second BY the third, store value in first (discard any remainder
	  //using long)
  }
  //If command is Mod
  else if ( strcmp( cmdName, "mod" ) == 0 ) {
	  //three args, divide second BY the third, store value of REMAINDER in first
  }
  //If command is Eq
  else if ( strcmp( cmdName, "eq" ) == 0 ) {
	  //three args, compare numeric value of the second arg and third and store the result in first
	  //(if equal, first is set to 1 (true) if not equal, it will be set to an empty string (false)
  }
  //If command is Less
  else if ( strcmp( cmdName, "less" ) == 0 ) {
	  //three args, set first to true when second is less than third
  }
  //If command is Goto
  else if ( strcmp( cmdName, "goto" ) == 0 ) {
	  //one arg, jump to the label with the name given as the argument.
  }
  //If command is If
  else if ( strcmp( cmdName, "if" ) == 0 ) {
	  //two args, if first is true, jump to label given as second argument, otherwise execution
	  //continues with the next command.
  }
  //Otherwise, command is invalid so give syntax error
  else {
    syntaxError();
  }
  // Never reached.
  return NULL;
}
