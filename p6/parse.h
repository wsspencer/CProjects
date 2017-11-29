/**
  @file parse.h
  @author

  Tokenizer and parser functions for our language.
*/

#ifndef _PARSE_H_
#define _PARSE_H_

#include <stdio.h>
#include <stdbool.h>

/** Maximum length of a token in the source file. */
#define MAX_TOKEN 1023

/** Maximum length of a variable name or a label. */
#define MAX_VARNAME 20

/** Return the current line number in the input (for error messages).
    @return Line number in the input.
*/
int getLineNumber();

/** Print a syntax error message, with a line number and exit. */
void syntaxError();

/** Return true if the given string is a legal variable name.
    @return True if it's a legal variable name.
*/
bool isVarName( char const *str );

/** Read the next token from the given file, a space-delimtied word, a
    double quoted string or closing parentheses, crly brackets or a
    semi-colon.  For double-quoted strings, it removes the final
    double quote, but leaves the one at the start.
    @param tok storage for the token, with room for a string of up to
     MAX_TOKEN characters.
    @param fp file to read tokens from.
    @return true if the token is successfully read.
*/
bool parseToken( char *token, FILE *fp );

/** Called when we expect another token on the input.  This function
    parses the token and exits with an error if there isn't one.
    @param storage for the next token, with capacity for at least
    MAX_TOKEN characters.
    @param fp file tokens should be read from.
*/
void expectToken( char *tok, FILE *fp );

/** Called when there needs to be a next token, and it needs to be a legal
    variable name.  Prints the syntax error message if it's not.
    @param storage for the next token, with capacity for at least
    MAX_TOKEN characters.
    @param fp file tokens should be read from.
*/
void expectVariable( char *tok, FILE *fp );

/** Called when the next token, must be a particular value,
    target.  Prints an error message and exits if it's not.
    @param target string that the next token should match.
    @param fp file tokens should be read from.
*/
void requireToken( char const *target, FILE *fp );

#endif
