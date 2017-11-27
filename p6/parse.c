#include "parse.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Current line we're parsing, starting from 1 like most editors. */
static int lineCount = 1;

int getLineNumber()
{
  return lineCount;
}

void syntaxError()
{
  fprintf( stderr, "Syntax error (line %d)\n", lineCount );
  exit( EXIT_FAILURE );
}

// check if string is a variable name
bool isVarName( char const *str )
{
  for ( int i = 0; str[ i ]; i++ ) {
    // Character better be a letter, an underscore or a digit (and not
    // the first character).
    if ( ! ( isalpha( str[ i ] ) || str[ i ] == '_' ||
             ( i > 0 && isdigit( str[ i ] ) ) ) )
      return false;
    if ( i >= MAX_VARNAME )
      return false;
  }

  return true;
}

bool parseToken( char *token, FILE *fp )
{
  int ch;

  // Skip whitespace and comments.
  while ( isspace( ch = fgetc( fp ) ) || ch == '#' ) {
    // If we hit the comment characer, skip the whole line.
    if ( ch == '#' )
      while ( ( ch = fgetc( fp ) ) != EOF && ch != '\n' )
        ;

    if ( ch == '\n' )
      lineCount++;
  }

  //if we've reached the end of the file, there is no token
  if ( ch == EOF )
    return false;

  // Record the character we've read and keep up with the token length.
  int len = 0;
  token[ len++ ] = ch;

  // Was that a command terminator?  If so, we're done.
  if ( ch == ';' ) {
    token[ len++ ] = '\0';
    return true;
  }

  // Handle non-quoted words.
  if ( ch != '"' ) {
    while ( ( ch = fgetc( fp ) ) != EOF && !isspace( ch )
            && ch != '"' && ch != '#' && ch != ';' ) {
      // Complain if the token is too long.
      if ( len >= MAX_TOKEN )
        syntaxError( lineCount );

      token[ len++ ] = ch;
    }

    // We had to read one character too far to find the end of the token.
    // So put it back.
    if ( ch != EOF )
      ungetc( ch, fp );

    token[ len ] = '\0';
    return true;
  }

  // Most interesting case, handle strings.

  // Is the next character escaped.
  bool escape = false;

  // Keep reading until we hit the matching close quote.
  while ( ( ch = fgetc( fp ) ) != '"' || escape ) {
    // Error conditions
    if ( ch == EOF || ch == '\n' )
      syntaxError( lineCount );

    // On a backslash, we just enable escape mode.
    if ( !escape && ch == '\\' ) {
      escape = true;
    } else {
      // Interpret escape sequences if we're in escape mode.
      if ( escape ) {
        switch ( ch ) {
        case 'n':
          ch = '\n';
          break;
        case 't':
          ch = '\t';
          break;
        case '"':
          ch = '"';
          break;
        case '\\':
          ch = '\\';
          break;
        default:
          syntaxError( lineCount );
        }
        escape = false;
      }

      // Complain if this string, with the eventual close quote, is too long.
      if ( len + 1 >= MAX_TOKEN )
        syntaxError( lineCount );

      token[ len++ ] = ch;
    }
  }

  // We leave off the closing quote, so it's easier to use just the content of a quoted string.
  token[ len ] = '\0';
  return token;
}

void expectToken( char *tok, FILE *fp )
{ 
  if ( !parseToken( tok, fp ) ) {
    syntaxError();
  }
}

//WHEN DOES THIS GET CALLED?
void expectVariable( char *tok, FILE *fp )
{
  if ( !parseToken( tok, fp ) || !isVarName( tok ) ) {
    syntaxError();
  }
}

void requireToken( char const *target, FILE *fp )
{
  char tok[ MAX_TOKEN + 1 ];
  expectToken( tok, fp );
  if ( strcmp( tok, target ) != 0 )
    syntaxError();
}

