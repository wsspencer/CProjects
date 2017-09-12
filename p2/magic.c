//use getchar to read input and putchar or printf to write output.  (you'll also appreciate the ungetc() function.  it lets you put the most recently read character back onto a stream so it can be read again.  e.g. you read a decimal digit in main and notices it's the start of an integer, put it back on the stream, then calls highlightNumber to print the whole thing in red).  
//Main will do most of the input and output work but the other two functions will help carry out tasks as well.
//

#include <stdio.h>
#include <stdbool.h>

//colors (033 is octal representation of ascii decimal 27)
#define RED "\033[31m"
#define RESET "\033[0m"

void highlightNumber() {
	//if token is an integer, highlight it
	char ch = getchar(); 
	
	//change print color to red
	printf( RED );
	
	//print the digit (since it's back on the stream)
	while (ch != ' ' && ch != EOF && ch != '\n') {
		printf( "%c", ch );
		ch = getchar();
	}
	//put the space, EOF, or newline char back on the stream
	ungetc( ch, stdin );
	
	//change color back to default
	printf( RESET );
}

void skipIdentifier() {
	//if token is identifier, skip highlighting any digits in this token (move stream to next char after token)
	char ch = getchar();
	
	//loop, printing the chars of the token, until the end of the token 
	//(excluding tabs since Jenkins tells me that's wrong)
	while ( ch != ' ' && ch != EOF && ch != '\n') {
		printf( "%c", ch );
		ch = getchar();
	}
	
	//print the instance of whitespace too
	if ( ch != EOF && ch != '\n') {
		printf( "%c", ch );
	}
}

bool isDigit( char ch ) {
	//check if param is a digit
	if ( ch >= '0' && ch <= '9' ) {
		return true;
	}
	
	return false;
}

bool isAlpha( char ch ) {
	//check if param is a letter
	if ( ch >= 'a' && ch <= 'z' ) {
		return true;
	}
	else if ( ch >= 'A' && ch <= 'Z' ) {
		return true;
	}
	return false;
}

int main() {
	char ch;
	
	//read user input one char at a time
	ch = getchar();
	while ( ch != EOF ) {
		//check if ch is the beginning of an identifier (letter or underscore) 
		if ( isAlpha( ch ) || ch == '_' ) {
			//iterate through token
			ungetc( ch, stdin );
			skipIdentifier();
		}
		
		//check if ch is a digit (that isn't part of an identifier)
		else if ( isDigit( ch ) ) {
			//put ch back on stream to be read again, then highlight it
			ungetc( ch, stdin );
			highlightNumber();
		}
		
		else if ( ch != '\n' && ch != EOF ) {
			//print the character
			printf( "%c", ch );
		}
		ch = getchar();
	}
	
	return 0;
}