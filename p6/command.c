#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label.h"
#include "parse.h"

#define SHORTSTR 2

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
    char *A;
    char *B;
} AddCommand;

//Representation for a sub command? derived from Command.
typedef struct {
    int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
    int line;
    
    /** Arguments for result and two args */
    char *res;
    char *A;
    char *B;
} SubCommand;

//Representation for a mult command? derived from Command.
typedef struct {
    int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
    int line;
    
    /** Arguments for product and two multiples */
    char *prod;
    char *A;
    char *B;
} MultCommand;

//Representation for a div command? derived from Command.
typedef struct {
    int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
    int line;
    
    /** Arguments for quotient and two dividends */
    char *quot;
    char *A;
    char *B;
} DivCommand;

//Representation for a modulo command? derived from Command.
typedef struct {
    int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
    int line;
    
    /** Arguments for modulo and two dividends */
    char *mod;
    char *A;
    char *B;
} ModCommand;

//Representation for an equality check command? derived from Command.
typedef struct {
    int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
    int line;
    
    /** Arguments for result and two args */
    char *res;
    char *A;
    char *B;
} EqCommand;

//Representation for a less than check command? derived from Command.
typedef struct {
    int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
    int line;
    
    /** Arguments for result and two args */
    char *res;
    char *A;
    char *B;
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
          fprintf(stderr, "Undefined variable: %s (line %d)\n", this->arg, this->line);
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
static int executeSet( Command *cmd, LabelMap *labelMap, int pc )
{
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
static int executeAdd( Command *cmd, LabelMap *labelMap, int pc )
{
    AddCommand *this = (AddCommand *)cmd;
    
    long addA = 0;
    long addB = 0;
    
    int checkerA;
    int checkerB;
    
    //if either of the two additive arguments are variables, carry out the addition with their
    //environment values be sure to exclude the opening " in scanning each literal but not
    //variables
    if ( isVarName( this->A ) ) {
        checkerA = sscanf( getenv( this->A ), "%ld", &addA );
    }
    else {
        checkerA = sscanf( this->A, "\"%ld", &addA );
    }
    if (isVarName(this->B)) {
        checkerB = sscanf( getenv( this->B ), "%ld", &addB );
    }
    else {
        checkerB = sscanf( this->B, "\"%ld", &addB );
    }
    
    //set a new environmental variable to this name and value, then convert sum back to a string
    //check that the values we are about to operate with parsed as longs, if not report error and
    //exit
    if ( !checkerA || !checkerB ) {
        fprintf(stderr, "Invalid number (line %d)\n", this->line);
        exit(1);
    }
    long total = addA + addB;
    char sum[ MAX_TOKEN + 1 ];
    sprintf(sum, "%ld", total);
    setenv(this->sum, sum, 1);

    return pc + 1;
}

//execute sub command
static int executeSub( Command *cmd, LabelMap *labelMap, int pc )
{
    SubCommand *this = (SubCommand *)cmd;

    long subA = 0;
    long subB = 0;

    int checkerA;
    int checkerB;

    //if either of the two additive arguments are variables, carry out the equation with their
    //environment values be sure to exclude the opening " in scanning each literal but not
    //variables
    if ( isVarName( this->A ) ) {
        checkerA = sscanf( getenv( this->A ), "%ld", &subA );
    }
    else {
        checkerA = sscanf( this->A, "\"%ld", &subA );
    }
    if (isVarName(this->B)) {
        checkerB = sscanf( getenv( this->B ), "%ld", &subB );
    }
    else {
        checkerB = sscanf( this->B, "\"%ld", &subB );
    }

    //set a new environmental variable to this name and value, then cast sum back to a string
    //check that the values we are about to operate with parsed as longs, if not report error and
    //exit
    if ( !checkerA || !checkerB ) {
        fprintf(stderr, "Invalid number (line %d)\n", this->line);
        exit(1);
    }
    long total = subA - subB;
    char res[ MAX_TOKEN + 1 ];
    sprintf(res, "%ld", total);
    setenv(this->res, res, 1);

    return pc + 1;
}

//execute mult command
static int executeMult( Command *cmd, LabelMap *labelMap, int pc )
{
    MultCommand *this = (MultCommand *)cmd;

    long mulA = 0;
    long mulB = 0;

    int checkerA;
    int checkerB;

    //if either of the two additive arguments are variables, carry out the equation with their
    //environment values be sure to exclude the opening " in scanning each literal but not
    //variables
    if ( isVarName( this->A ) ) {
        checkerA = sscanf( getenv( this->A ), "%ld", &mulA );
    }
    else {
        checkerA = sscanf( this->A, "\"%ld", &mulA );
    }
    if (isVarName(this->B)) {
        checkerB = sscanf( getenv( this->B ), "%ld", &mulB );
    }
    else {
        checkerB = sscanf( this->B, "\"%ld", &mulB );
    }

    //set a new environmental variable to this name and value, then cast sum back to a string
    //check that the values we are about to operate with parsed as longs, if not report error and
    //exit
    if ( !checkerA || !checkerB ) {
        fprintf(stderr, "Invalid number (line %d)\n", this->line);
        exit(1);
    }
    long total = mulA * mulB;
    char prod[ MAX_TOKEN + 1 ];
    sprintf(prod, "%ld", total);
    setenv(this->prod, prod, 1);

    return pc + 1;
}

//execute div command
static int executeDiv( Command *cmd, LabelMap *labelMap, int pc )
{
    DivCommand *this = (DivCommand *)cmd;

    long divA = 0;
    long divB = 0;

    int checkerA;
    int checkerB;

    //if either of the two additive arguments are variables, carry out the equation with their
    //environment values be sure to exclude the opening " in scanning each literal but not
    //variables
    if ( isVarName( this->A ) ) {
        checkerA = sscanf( getenv( this->A ), "%ld", &divA );
    }
    else {
        checkerA = sscanf( this->A, "\"%ld", &divA );
    }
    if (isVarName(this->B)) {
        checkerB = sscanf( getenv( this->B ), "%ld", &divB );
    }
    else {
        checkerB = sscanf( this->B, "\"%ld", &divB );
    }

    //set a new environmental variable to this name and value, then cast sum back to a string

    //first check that the values we are about to operate with parsed as longs, if not report
    //error and exit
    if ( !checkerA || !checkerB ) {
        fprintf(stderr, "Invalid number (line %d)\n", this->line);
        exit(1);
    }
    //then check for divide by zero error, if so, print error, otherwise carry out operation.
    //do this by checking that all values parsed as longs in their scan.
    if ( divA == 0 || divB == 0 ) {
        fprintf(stderr, "Divide by zero (line %d)\n", this->line);
        exit(1);
    }
    long total = divA / divB;
    char quot[ MAX_TOKEN + 1 ];
    sprintf(quot, "%ld", total);
    setenv(this->quot, quot, 1);

    return pc + 1;
}

//execute modulo command
static int executeMod( Command *cmd, LabelMap *labelMap, int pc )
{
    ModCommand *this = (ModCommand *)cmd;

    long divA = 0;
    long divB = 0;

    int checkerA;
    int checkerB;

    //if either of the two additive arguments are variables, carry out the equation with their
    //environment values be sure to exclude the opening " in scanning each literal but not
    //variables
    if ( isVarName( this->A ) ) {
        checkerA = sscanf( getenv( this->A ), "%ld", &divA );
    }
    else {
        checkerA = sscanf( this->A, "\"%ld", &divA );
    }
    if (isVarName(this->B)) {
        checkerB = sscanf( getenv( this->B ), "%ld", &divB );
    }
    else {
        checkerB = sscanf( this->B, "\"%ld", &divB );
    }

    //set a new environmental variable to this name and value, then cast sum back to a string

    //check that the values we are about to operate with parsed as longs, if not report error and
    //exit
    if ( !checkerA || !checkerB ) {
        fprintf(stderr, "Invalid number (line %d)\n", this->line);
        exit(1);
    }
    //then (as with div) check that we aren't dividing by zero
    if ( divA == 0 || divB == 0 ) {
        fprintf(stderr, "Divide by zero (line %d)\n", this->line);
        exit(1);
    }
    long remainder = divA % divB;
    char mod[ MAX_TOKEN + 1 ];
    sprintf(mod, "%ld", remainder);
    setenv(this->mod, mod, 1);

    return pc + 1;
}

//execute equality check command
static int executeEq( Command *cmd, LabelMap *labelMap, int pc )
{
    EqCommand *this = (EqCommand *)cmd;

    long argA = 0;
    long argB = 0;

    int checkerA;
    int checkerB;

    //if either of the two additive arguments are variables, carry out the equation with their
    //environment values be sure to exclude the opening " in scanning each literal but not
    //variables
    if ( isVarName( this->A ) ) {
        checkerA = sscanf( getenv( this->A ), "%ld", &argA );
    }
    else {
        checkerA = sscanf( this->A, "\"%ld", &argA );
    }
    if (isVarName(this->B)) {
        checkerB = sscanf( getenv( this->B ), "%ld", &argB );
    }
    else {
        checkerB = sscanf( this->B, "\"%ld", &argB );
    }

    //check if the two args are equal,  if they are store 1 in result, if not store empty string
    //(null).  check that the values we are about to operate with parsed as longs, if not report
    //error and exit
    if ( !checkerA || !checkerB ) {
        fprintf(stderr, "Invalid number (line %d)\n", this->line);
        exit(1);
    }
    char res[ SHORTSTR ] = " ";

    if ( argA == argB ) {
        strcpy(res, "1");
    }

    setenv(this->res, res, 1);

    return pc + 1;
}

//execute Less command
static int executeLess( Command *cmd, LabelMap *labelMap, int pc )
{
    LessCommand *this = (LessCommand *)cmd;

    long argA = 0;
    long argB = 0;

    int checkerA;
    int checkerB;

    //if one of the arguments are variables, carry out the equation with their environment values
    //be sure to exclude the opening " in scanning each literal but not variables
    if ( isVarName( this->A ) ) {
        checkerA = sscanf( getenv( this->A ), "%ld", &argA );
    }
    else {
        checkerA = sscanf( this->A, "\"%ld", &argA );
    }
    if (isVarName(this->B)) {
        checkerB = sscanf( getenv( this->B ), "%ld", &argB );
    }
    else {
        checkerB = sscanf( this->B, "\"%ld", &argB );
    }

    //check that the values we are about to operate with parsed as longs, if not report error and
    //exit
    if ( !checkerA || !checkerB ) {
        fprintf(stderr, "Invalid number (line %d)\n", this->line);
        exit(1);
    }

    //check if second arg is less than third arg,  if they are store 1 (true) in result, if not
    //store empty string (false).
    char res[ SHORTSTR ] = " ";
    
    if ( argA < argB ) {
        strcpy(res, "1");
    }
    else {
        strcpy(res, " ");
    }

    setenv(this->res, res, 1);

    return pc + 1;
}

//execute Goto label command
static int executeGoto( Command *cmd, LabelMap *labelMap, int pc )
{
    GotoCommand *this = (GotoCommand *)cmd;

    int jump = findLabel( *labelMap, this->arg );

    return jump;
}

//execute If command (go to label if conditional is true, otherwise continue as normal
static int executeIf( Command *cmd, LabelMap *labelMap, int pc )
{
    IfCommand *this = (IfCommand *)cmd;

    long conditional = 0;
    int jump = pc + 1;

    int checker;

    //whether the conditional is variable or literal, make its value a long so we can check it
    //be sure to exclude the opening " in scanning each literal but not variables
    if ( isVarName( this->cond ) ) {
        checker = sscanf( getenv( this->cond ), "%ld", &conditional );

    }
    else {
        checker = sscanf( this->cond, "\"%ld", &conditional );
    }

    //check that the value we are about to operate with parsed as long, if not report error and
    //exit
    if ( !checker ) {
        fprintf(stderr, "Invalid number (line %d)\n", this->line);
        exit(1);
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

    //Check if token is a variable (environmental or otherwise since we set normal variables as
    //environmental ones?) name, if it is, change its arg's value to the value of the variable?

    this->arg = copyString( arg );

    // Return the result, as an instance of the Command interface.
    return (Command *) this;
}

static Command *makeSet( char const *var, char const *arg )
{
    SetCommand *this = (SetCommand *) malloc( sizeof( SetCommand ) );

    this->execute = executeSet;
    this->line = getLineNumber();

    //set variable and argument
    this->var = copyString( var );
    this->arg = copyString( arg );

    return (Command *) this;
}

static Command *makeAdd( char const *sum, char const *A, char const *B )
{
    AddCommand *this = (AddCommand *) malloc( sizeof( AddCommand ) );

    this->execute = executeAdd;
    this->line = getLineNumber();

    //set variable and argument
    this->sum = copyString( sum );
    this->A = copyString( A );
    this->B = copyString( B );
    
    return (Command *) this;
}

static Command *makeSub( char const *res, char const *A, char const *B )
{
    SubCommand *this = (SubCommand *) malloc( sizeof( SubCommand ) );

    this->execute = executeSub;
    this->line = getLineNumber();

    //set variable and argument
    this->res = copyString( res );
    this->A = copyString( A );
    this->B = copyString( B );

    return (Command *) this;
}

static Command *makeMult( char const *prod, char const *A, char const *B )
{
    MultCommand *this = (MultCommand *) malloc( sizeof( MultCommand ) );

    this->execute = executeMult;
    this->line = getLineNumber();

    //set variable and argument
    this->prod = copyString( prod );
    this->A = copyString( A );
    this->B = copyString( B );

    return (Command *) this;
}

static Command *makeDiv( char const *quot, char const *A, char const *B )
{
    DivCommand *this = (DivCommand *) malloc( sizeof( DivCommand ) );

    this->execute = executeDiv;
    this->line = getLineNumber();

    //set variable and argument
    this->quot = copyString( quot );
    this->A = copyString( A );
    this->B = copyString( B );

    return (Command *) this;
}

//make function for initializing a modulus command struct
static Command *makeMod( char const *mod, char const *A, char const *B )
{
    ModCommand *this = (ModCommand *) malloc( sizeof( ModCommand ) );

    this->execute = executeMod;
    this->line = getLineNumber();

    //set variable and argument
    this->mod = copyString( mod );
    this->A = copyString( A );
    this->B = copyString( B );

    return (Command *) this;
}

//make function for initializing an equality check command struct
static Command *makeEq( char const *res, char const *A, char const *B )
{
    EqCommand *this = (EqCommand *) malloc( sizeof( EqCommand ) );

    this->execute = executeEq;
    this->line = getLineNumber();

    //set variable and argument
    this->res = copyString( res );
    this->A = copyString( A );
    this->B = copyString( B );

    return (Command *) this;
}

//make function for initializing a less command struct
static Command *makeLess( char const *res, char const *A, char const *B )
{
    LessCommand *this = (LessCommand *) malloc( sizeof( LessCommand ) );

    this->execute = executeLess;
    this->line = getLineNumber();

    //set variable and argument
    this->res = copyString( res );
    this->A = copyString( A );
    this->B = copyString( B );

    return (Command *) this;
}

//make function for going to the label given
static Command *makeGoto( char const *arg )
{
    GotoCommand *this = (GotoCommand *) malloc( sizeof( GotoCommand ) );

    this->execute = executeGoto;
    this->line = getLineNumber();

    //set argument
    this->arg = copyString( arg );

    return (Command *) this;
}

//make function for going to the label given if the first argument is true
static Command *makeIf( char const *cond, char const *jmp )
{
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
        char A[ MAX_TOKEN + 1 ];
        char B[ MAX_TOKEN + 1 ];

        expectVariable( tok, fp );
        expectToken( A, fp );
        expectToken( B, fp );

        requireToken( ";", fp );

        return makeAdd( tok, A, B );
    }
    //If command is Sub
    else if ( strcmp( cmdName, "sub" ) == 0 ) {
        //three args, subtract third FROM second, store value in first
        char A[ MAX_TOKEN + 1 ];
        char B[ MAX_TOKEN + 1 ];

        expectVariable( tok, fp );
        expectToken( A, fp );
        expectToken( B, fp );

        requireToken( ";", fp );

        return makeSub( tok, A, B );
    }
    //If command is Mult
    else if ( strcmp( cmdName, "mult" ) == 0 ) {
        //three args, multiply second and third, store value in first
        char A[ MAX_TOKEN + 1 ];
        char B[ MAX_TOKEN + 1 ];

        expectVariable( tok, fp );
        expectToken( A, fp );
        expectToken( B, fp );

        requireToken( ";", fp );

        return makeMult( tok, A, B );
    }
    //If command is Div
    else if ( strcmp( cmdName, "div" ) == 0 ) {
        //three args, divide second BY the third, store value in first (discard any remainder
        //using long)
        char A[ MAX_TOKEN + 1 ];
        char B[ MAX_TOKEN + 1 ];

        expectVariable( tok, fp );
        expectToken( A, fp );
        expectToken( B, fp );

        requireToken( ";", fp );

        return makeDiv( tok, A, B );
    }
    //If command is Mod
    else if ( strcmp( cmdName, "mod" ) == 0 ) {
        //three args, divide second BY the third, store value of REMAINDER in first
        char A[ MAX_TOKEN + 1 ];
        char B[ MAX_TOKEN + 1 ];

        expectVariable( tok, fp );
        expectToken( A, fp );
        expectToken( B, fp );

        requireToken( ";", fp );

        return makeMod( tok, A, B );
    }
    //If command is Eq
    else if ( strcmp( cmdName, "eq" ) == 0 ) {
        //three args, compare numeric value of the second arg and third and store the result in
        //first.
        //(if equal, first is set to 1 (true) if not equal, it will be set to an empty string
        //(false).
        char A[ MAX_TOKEN + 1 ];
        char B[ MAX_TOKEN + 1 ];

        expectVariable( tok, fp );
        expectToken( A, fp );
        expectToken( B, fp );

        requireToken( ";", fp );

        return makeEq( tok, A, B );
    }
    //If command is Less
    else if ( strcmp( cmdName, "less" ) == 0 ) {
        //three args, set first to true when second is less than third
        char A[ MAX_TOKEN + 1 ];
        char B[ MAX_TOKEN + 1 ];

        expectVariable( tok, fp );
        expectToken( A, fp );
        expectToken( B, fp );

        requireToken( ";", fp );

        return makeLess( tok, A, B );
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
