#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct CodePoint{
	//numeric code
	unsigned int code;
	
	//string name (won't be longer than 90 chars, but make room for null terminator)
	char name[91];
	
} CodePoint;

int compareCP( const void * a, const void * b );

void loadTable();

void freeTable();

bool reportCode( unsigned int code );