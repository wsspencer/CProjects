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


//COMMAND STRUCTS:

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

//Representation for a set command? derived from Command.
typedef struct {
	int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
	int line;
	
	/** Arguments for setting variable */
	char *var;
	char *arg;
} SetCommand;

//Representation for a add command? derived from Command.
typedef struct {
	int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
	int line;
	
	/** Arguments for sum and two additives */
	char *sum;
	char *two;
	char *three;
} AddCommand;

//Representation for a sub command? derived from Command.
typedef struct {
	int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
	int line;
	
	/** Arguments for result and two args */
	char *res;
	char *two;
	char *three;
} SubCommand;

//Representation for a mult command? derived from Command.
typedef struct {
	int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
	int line;
	
	/** Arguments for product and two multiples */
	char *prod;
	char *two;
	char *three;
} MultCommand;

//Representation for a div command? derived from Command.
typedef struct {
	int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
	int line;
	
	/** Arguments for quotient and two dividends */
	char *quot;
	char *two;
	char *three;
} DivCommand;

//Representation for a modulo command? derived from Command.
typedef struct {
	int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
	int line;
	
	/** Arguments for modulo and two dividends */
	char *mod;
	char *two;
	char *three;
} ModCommand;

//Representation for an equality check command? derived from Command.
typedef struct {
	int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
	int line;
	
	/** Arguments for result and two args */
	char *res;
	char *two;
	char *three;
} EqCommand;

//Representation for a less than check command? derived from Command.
typedef struct {
	int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
	int line;
	
	/** Arguments for result and two args */
	char *res;
	char *two;
	char *three;
} LessCommand;

//Representation for goto command? derived from Command.
typedef struct {
	int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
	int line;
	
	/** Arguments for go to label */
	char *arg;
} GotoCommand;

//Representation for an if command? derived from Command.
typedef struct {
	int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
	int line;
	
	/** Arguments for conditional and jump label */
	char *cond;
	char *jmp;
} IfCommand;


//COMMAND EXECUTIONS:

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
  
  if (this->arg && strcmp( this->arg, " " ) != 0) {
  
  if ( isVarName( this->arg ) ) {
	  //check if the value has been set, if not, print error to stderr and exit
	  if ( !getenv(this->arg ) ) {
		  fprintf(stderr, "Undefined variable: %s (line %d)", this->arg, this->line);
		  exit(1);
	  }
	  else {
		//strip quotes?
		char *str = getenv(this->arg);
		//remove " if it's first char
		if (str[0] == '"') {
			str += 1;
		}
		//check that it is not an empty string
		if (strcmp( str, " " ) != 0) {
			printf( "%s", str );
		}
	  }
  }
  else {
	printf( "%s", this->arg + 1 );
  }

  }
  
  return pc + 1;
}

//execute set command
static int executeSet( Command *cmd, LabelMap *labelMap, int pc ) {
	SetCommand *this = (SetCommand *)cmd;
	
	//set a new environmental variable to this name and value
	//check if arg is a variable, if it is, set it to its value, not its
	//name.
	if ( isVarName( this->arg ) ) {
		setenv(this->var, getenv( this->arg ), 1);
	}
	else {
		//don't skip over " if it's all we've got because then we're left with nothing
		if ( strlen( this->arg ) > 1 ) {
			setenv(this->var, this->arg + 1, 1);
		}
		else {
			setenv(this->var, " ", 1);
		}
	}

	return pc + 1;
}

//execute add command
static int executeAdd( Command *cmd, LabelMap *labelMap, int pc ) {
	AddCommand *this = (AddCommand *)cmd;
	
	long add1 = 0;
	long add2 = 0;
	
	//if either of the two additive arguments are variables, carry out the addition with their environment values
	//be sure to exclude the opening " in scanning each literal but not variables
	if ( isVarName( this->two ) ) {
		sscanf( getenv( this->two ), "%ld", &add1 );
	}
	else {
		sscanf( this->two, "\"%ld", &add1 );
	}
	if (isVarName(this->three)) {
		sscanf( getenv( this->three ), "%ld", &add2 );
	}
	else {
		sscanf( this->three, "\"%ld", &add2 );
	}
	
	//set a new environmental variable to this name and value, then convert sum back to a string
	long total = add1 + add2;
	char sum[ MAX_TOKEN + 1 ];
	sprintf(sum, "%ld", total);
	setenv(this->sum, sum, 1);
	
	return pc + 1;
}

//execute sub command
static int executeSub( Command *cmd, LabelMap *labelMap, int pc ) {
	SubCommand *this = (SubCommand *)cmd;
	
	long sub1 = 0;
	long sub2 = 0;
	
	//if either of the two additive arguments are variables, carry out the equation with their environment values
	//be sure to exclude the opening " in scanning each literal but not variables
	if ( isVarName( this->two ) ) {
		sscanf( getenv( this->two ), "%ld", &sub1 );
	}
	else {
		sscanf( this->two, "\"%ld", &sub1 );
	}
	if (isVarName(this->three)) {
		sscanf( getenv( this->three ), "%ld", &sub2 );
	}
	else {
		sscanf( this->three, "\"%ld", &sub2 );
	}
	
	//set a new environmental variable to this name and value, then cast sum back to a string
	long total = sub1 - sub2;
	char res[ MAX_TOKEN + 1 ];
	sprintf(res, "%ld", total);
	setenv(this->res, res, 1);

	return pc + 1;
}

//execute mult command
static int executeMult( Command *cmd, LabelMap *labelMap, int pc ) {
	MultCommand *this = (MultCommand *)cmd;
	
	long mul1 = 0;
	long mul2 = 0;
	
	//if either of the two additive arguments are variables, carry out the equation with their environment values
	//be sure to exclude the opening " in scanning each literal but not variables
	if ( isVarName( this->two ) ) {
		sscanf( getenv( this->two ), "%ld", &mul1 );
	}
	else {
		sscanf( this->two, "\"%ld", &mul1 );
	}
	if (isVarName(this->three)) {
		sscanf( getenv( this->three ), "%ld", &mul2 );
	}
	else {
		sscanf( this->three, "\"%ld", &mul2 );
	}
	
	//set a new environmental variable to this name and value, then cast sum back to a string
	long total = mul1 * mul2;
	char prod[ MAX_TOKEN + 1 ];
	sprintf(prod, "%ld", total);
	setenv(this->prod, prod, 1);

	return pc + 1;
}

//execute div command
static int executeDiv( Command *cmd, LabelMap *labelMap, int pc ) {
	DivCommand *this = (DivCommand *)cmd;
	
	long div1 = 0;
	long div2 = 0;
	
	//if either of the two additive arguments are variables, carry out the equation with their environment values
	//be sure to exclude the opening " in scanning each literal but not variables
	if ( isVarName( this->two ) ) {
		sscanf( getenv( this->two ), "%ld", &div1 );
	}
	else {
		sscanf( this->two, "\"%ld", &div1 );
	}
	if (isVarName(this->three)) {
		sscanf( getenv( this->three ), "%ld", &div2 );
	}
	else {
		sscanf( this->three, "\"%ld", &div2 );
	}
	
	//set a new environmental variable to this name and value, then cast sum back to a string
	//first check for divide by zero error, if so, print error, otherwise carry out operation.
	long total = div1 / div2;
	char quot[ MAX_TOKEN + 1 ];
	sprintf(quot, "%ld", total);
	setenv(this->quot, quot, 1);

	return pc + 1;
}

//execute modulo command
static int executeMod( Command *cmd, LabelMap *labelMap, int pc ) {
	ModCommand *this = (ModCommand *)cmd;
	
	long div1 = 0;
	long div2 = 0;
	
	//if either of the two additive arguments are variables, carry out the equation with their environment values
	//be sure to exclude the opening " in scanning each literal but not variables
	if ( isVarName( this->two ) ) {
		sscanf( getenv( this->two ), "%ld", &div1 );
	}
	else {
		sscanf( this->two, "\"%ld", &div1 );
	}
	if (isVarName(this->three)) {
		sscanf( getenv( this->three ), "%ld", &div2 );
	}
	else {
		sscanf( this->three, "\"%ld", &div2 );
	}
	
	//set a new environmental variable to this name and value, then cast sum back to a string
	long remainder = div1 % div2;
	char mod[ MAX_TOKEN + 1 ];
	sprintf(mod, "%ld", remainder);
	setenv(this->mod, mod, 1);

	return pc + 1;
}

//execute equality check command
static int executeEq( Command *cmd, LabelMap *labelMap, int pc ) {
	EqCommand *this = (EqCommand *)cmd;
	
	long arg1 = 0;
	long arg2 = 0;
	
	//if either of the two additive arguments are variables, carry out the equation with their environment values
	//be sure to exclude the opening " in scanning each literal but not variables
	if ( isVarName( this->two ) ) {
		sscanf( getenv( this->two ), "%ld", &arg1 );
	}
	else {
		sscanf( this->two, "\"%ld", &arg1 );
	}
	if (isVarName(this->three)) {
		sscanf( getenv( this->three ), "%ld", &arg2 );
	}
	else {
		sscanf( this->three, "\"%ld", &arg2 );
	}
	
	//check if the two args are equal,  if they are store 1 in result, if not store empty string (null).
	char res[ 2 ] = " ";
	
	if ( arg1 == arg2 ) {
		strcpy(res, "1");
	}
	
	setenv(this->res, res, 1);

	return pc + 1;
}

//execute Less command
static int executeLess( Command *cmd, LabelMap *labelMap, int pc ) {
	LessCommand *this = (LessCommand *)cmd;
	
	long arg1 = 0;
	long arg2 = 0;
	
	//if either of the arguments are variables, carry out the equation with their environment values
	//be sure to exclude the opening " in scanning each literal but not variables
	if ( isVarName( this->two ) ) {
		sscanf( getenv( this->two ), "%ld", &arg1 );
	}
	else {
		sscanf( this->two, "\"%ld", &arg1 );
	}
	if (isVarName(this->three)) {
		sscanf( getenv( this->three ), "%ld", &arg2 );
	}
	else {
		sscanf( this->three, "\"%ld", &arg2 );
	}
	
	//check if second arg is less than third arg,  if they are store 1 (true) in result, if not store empty string (false).
	char res[ 2 ] = " ";
	
	if ( arg1 < arg2 ) {
		strcpy(res, "1");
	}
	else {
		strcpy(res, " ");
	}
	
	setenv(this->res, res, 1);

	return pc + 1;
}

//execute Goto label command
static int executeGoto( Command *cmd, LabelMap *labelMap, int pc ) {
	GotoCommand *this = (GotoCommand *)cmd;
	
	int jump = findLabel( *labelMap, this->arg );
	
	return jump;
}

//execute If command (go to label if conditional is true, otherwise continue as normal
static int executeIf( Command *cmd, LabelMap *labelMap, int pc ) {
	IfCommand *this = (IfCommand *)cmd;
	
	long conditional = 0;
	int jump = pc + 1;
	
	//whether the conditional is variable or literal, make its value a long so we can check it
	//be sure to exclude the opening " in scanning each literal but not variables
	if ( isVarName( this->cond ) ) {
		sscanf( getenv( this->cond ), "%ld", &conditional );

	}
	else {
		sscanf( this->cond, "\"%ld", &conditional );
	}
	
	//check if conditional is true, if not jump's int value will be unchanged and processing will
	//continue as normal.
	if ( conditional ) {
		jump = findLabel( *labelMap, this->jmp );
	}

	return jump;
}

//COMMAND MAKES:

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
  
  //WORK NEEDED HERE?:  THIS IS WHERE YOU ADDED VARIABLE COMPATIBILITY FOR PRINTING
  //Check if token is a variable (environmental or otherwise since we set normal variables as
  //environmental ones?) name, if it is, change its arg's value to the value of the variable?
  
  this->arg = copyString( arg );  
  
  // Return the result, as an instance of the Command interface.
  return (Command *) this;
}

static Command *makeSet( char const *var, char const *arg ) {
	SetCommand *this = (SetCommand *) malloc( sizeof( SetCommand ) );
	
	this->execute = executeSet;
	this->line = getLineNumber();
	
	//set variable and argument
	this->var = copyString( var );
	this->arg = copyString( arg );

	return (Command *) this;
}

static Command *makeAdd( char const *sum, char const *two, char const *three ) {
	AddCommand *this = (AddCommand *) malloc( sizeof( AddCommand ) );
	
	this->execute = executeAdd;
	this->line = getLineNumber();
	
	//set variable and argument
	this->sum = copyString( sum );
	this->two = copyString( two );
	this->three = copyString( three );
	
	return (Command *) this;
}

static Command *makeSub( char const *res, char const *two, char const *three ) {
	SubCommand *this = (SubCommand *) malloc( sizeof( SubCommand ) );
	
	this->execute = executeSub;
	this->line = getLineNumber();
	
	//set variable and argument
	this->res = copyString( res );
	this->two = copyString( two );
	this->three = copyString( three );
	
	return (Command *) this;
}

static Command *makeMult( char const *prod, char const *two, char const *three ) {
	MultCommand *this = (MultCommand *) malloc( sizeof( MultCommand ) );
	
	this->execute = executeMult;
	this->line = getLineNumber();
	
	//set variable and argument
	this->prod = copyString( prod );
	this->two = copyString( two );
	this->three = copyString( three );
	
	return (Command *) this;
}

static Command *makeDiv( char const *quot, char const *two, char const *three ) {
	DivCommand *this = (DivCommand *) malloc( sizeof( DivCommand ) );
	
	this->execute = executeDiv;
	this->line = getLineNumber();
	
	//set variable and argument
	this->quot = copyString( quot );
	this->two = copyString( two );
	this->three = copyString( three );
	
	return (Command *) this;
}

//make function for initializing a modulus command struct
static Command *makeMod( char const *mod, char const *two, char const *three ) {
	ModCommand *this = (ModCommand *) malloc( sizeof( ModCommand ) );
	
	this->execute = executeMod;
	this->line = getLineNumber();
	
	//set variable and argument
	this->mod = copyString( mod );
	this->two = copyString( two );
	this->three = copyString( three );
	
	return (Command *) this;
}

//make function for initializing an equality check command struct
static Command *makeEq( char const *res, char const *two, char const *three ) {
	EqCommand *this = (EqCommand *) malloc( sizeof( EqCommand ) );
	
	this->execute = executeEq;
	this->line = getLineNumber();
	
	//set variable and argument
	this->res = copyString( res );
	this->two = copyString( two );
	this->three = copyString( three );
	
	return (Command *) this;
}

//make function for initializing a less command struct
static Command *makeLess( char const *res, char const *two, char const *three ) {
	LessCommand *this = (LessCommand *) malloc( sizeof( LessCommand ) );
	
	this->execute = executeLess;
	this->line = getLineNumber();
	
	//set variable and argument
	this->res = copyString( res );
	this->two = copyString( two );
	this->three = copyString( three );
	
	return (Command *) this;
}

//make function for going to the label given
static Command *makeGoto( char const *arg ) {
	GotoCommand *this = (GotoCommand *) malloc( sizeof( GotoCommand ) );
	
	this->execute = executeGoto;
	this->line = getLineNumber();
	
	//set argument
	this->arg = copyString( arg );
	
	return (Command *) this;
}

//make function for going to the label given if the first argument is true
static Command *makeIf( char const *cond, char const *jmp ) {
	IfCommand *this = (IfCommand *) malloc( sizeof( IfCommand ) );
	
	this->execute = executeIf;
	this->line = getLineNumber();
	
	//set conditional and jump location
	this->cond = copyString( cond );
	this->jmp = copyString( jmp );
	
	return (Command *) this;
}

//COMMAND PARSE:

Command *parseCommand( char *cmdName, FILE *fp )
{
  // Read the first token.
  char tok[ MAX_TOKEN + 1 ];

  // Figure out what kind of command it is:
  
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
	  char var[ MAX_TOKEN + 1 ];

	  expectVariable( var, fp );
	  expectToken( tok, fp );

	  requireToken(";", fp );

	  return makeSet( var, tok );
  }
  //If command is Add
  else if ( strcmp( cmdName, "add" ) == 0 ) {
	  //three args, add second and third, store value in first
	  char two[ MAX_TOKEN + 1 ];
	  char three[ MAX_TOKEN + 1 ];

	  expectVariable( tok, fp );
	  expectToken( two, fp );
	  expectToken( three, fp );

	  requireToken( ";", fp );

	  return makeAdd( tok, two, three );
  }
  //If command is Sub
  else if ( strcmp( cmdName, "sub" ) == 0 ) {
	  //three args, subtract third FROM second, store value in first
	  char two[ MAX_TOKEN + 1 ];
	  char three[ MAX_TOKEN + 1 ];
	  
	  expectVariable( tok, fp );
	  expectToken( two, fp );
	  expectToken( three, fp );
	  
	  requireToken( ";", fp );
	  
	  return makeSub( tok, two, three );
  }
  //If command is Mult
  else if ( strcmp( cmdName, "mult" ) == 0 ) {
	  //three args, multiply second and third, store value in first
	  char two[ MAX_TOKEN + 1 ];
	  char three[ MAX_TOKEN + 1 ];
	  
	  expectVariable( tok, fp );
	  expectToken( two, fp );
	  expectToken( three, fp );
	  
	  requireToken( ";", fp );
	  
	  return makeMult( tok, two, three );
  }
  //If command is Div
  else if ( strcmp( cmdName, "div" ) == 0 ) {
	  //three args, divide second BY the third, store value in first (discard any remainder
	  //using long)
	  char two[ MAX_TOKEN + 1 ];
	  char three[ MAX_TOKEN + 1 ];
	  
	  expectVariable( tok, fp );
	  expectToken( two, fp );
	  expectToken( three, fp );
	  
	  requireToken( ";", fp );
	  
	  return makeDiv( tok, two, three );
  }
  //If command is Mod
  else if ( strcmp( cmdName, "mod" ) == 0 ) {
	  //three args, divide second BY the third, store value of REMAINDER in first
	  char two[ MAX_TOKEN + 1 ];
	  char three[ MAX_TOKEN + 1 ];
	  
	  expectVariable( tok, fp );
	  expectToken( two, fp );
	  expectToken( three, fp );
	  
	  requireToken( ";", fp );
	  
	  return makeMod( tok, two, three );
  }
  //If command is Eq
  else if ( strcmp( cmdName, "eq" ) == 0 ) {
	  //three args, compare numeric value of the second arg and third and store the result in first
	  //(if equal, first is set to 1 (true) if not equal, it will be set to an empty string (false)
	  char two[ MAX_TOKEN + 1 ];
	  char three[ MAX_TOKEN + 1 ];
	  
	  expectVariable( tok, fp );
	  expectToken( two, fp );
	  expectToken( three, fp );
	  
	  requireToken( ";", fp );
	  
	  return makeEq( tok, two, three );
  }
  //If command is Less
  else if ( strcmp( cmdName, "less" ) == 0 ) {
	  //three args, set first to true when second is less than third
	  char two[ MAX_TOKEN + 1 ];
	  char three[ MAX_TOKEN + 1 ];
	  
	  expectVariable( tok, fp );
	  expectToken( two, fp );
	  expectToken( three, fp );
	  
	  requireToken( ";", fp );
	  
	  return makeLess( tok, two, three );
  }
  //If command is Goto
  else if ( strcmp( cmdName, "goto" ) == 0 ) {
	  //one arg, jump to the label with the name given as the argument.
      expectToken( tok, fp );
      requireToken( ";", fp );
	
      return makeGoto( tok );
  }
  //If command is If
  else if ( strcmp( cmdName, "if" ) == 0 ) {
	  //two args, if first is true, jump to label given as second argument, otherwise execution
	  //continues with the next command.
	  char var[ MAX_TOKEN + 1 ];

	  expectVariable( var, fp );
	  expectToken( tok, fp );

	  requireToken(";", fp );

	  return makeIf( var, tok );
  }
  //Otherwise, command is invalid so give syntax error
  else {
    syntaxError();
  }
  // Never reached.
  return NULL;
}
