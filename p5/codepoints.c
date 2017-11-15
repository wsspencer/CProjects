//Each code point will be represented by an instance of a CodePoint struct, which you get to
// design. The code point table will be a dynamically allocated array of of instances of this
// struct.

//The code point table will be stored as a static global variables in the codepoints component.
// All of the functions in this component will be able to access it, but, by making it static,
// code elsewhere in the program won't be able to access it directly (and it won't pollute the
// global namespace).

//As you read code point descriptions from the unicode.txt file, you'll need to implement resizable
// array behavior for the code point table, since you won't know how many code points you'll need
// to read. Once you've read the whole table, you won't need to resize it again, so the current
// capacity (the capacity variable that's normally part of a resizable array implementation) won't
// need to be global; you just need it inside the loadTable() function.

//The reportCode() function will use binary search to quickly find the record for a given code
// point. There's a generic binary search function in the standard library, but I'd like you to
// write your own for this assignment. It's not difficult.

//Before you can use binary search, you'll need to sort the code point table. Do this before you
// return from loadTable(). For the sort, use the standard library qsort() function. To use
// qsort(), you have to define your own comparison function and pass in the function's address as
// the last parameter to qsort(). The type of this comparison function has to be generic, taking
// two const void pointers as parameters and returning int. Inside the function, you'll know what
// types of values you're really sorting. You'll need to cast these void pointers to pointers to
// the type of value that's really stored in your array. So, if your sorting an array of CodePoint
// structures, you'll cast the void pointers to pointers to CodePoints. Alternatively, if you've
// implemented your code point table as an array of pointers to structs, then you're really sorting
// an array of pointers. So, inside your comparison function, you'll need to cast the parameters
// to pointers to pointers to CodePoint structures (since you're sorting an array of pointers to
// CodePoints).

#include "codepoints.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER 100

static struct CodePoint *table;
static int size;

int compareCP( const void * a, const void * b ) {
	//cast params to CodePoints
	CodePoint *cpA = (CodePoint *) a;
	CodePoint *cpB = (CodePoint *) b;
	//return the difference between codepoint a and codepoint b's codes
	return ( cpA->code - cpB->code );
}

void loadTable() {
	// load the table defined in unicode.txt into memory by way of our static list of Codepoint
	// structs.  It's static in visibility but dynamically allocated in memory.  (Even though
	// the size of the table is not arbitrary we still must allocate the table in memory
	// dynamically since there is not enough stack memory to hold it in its entirety, therefore
	// we will use heap memory to store it.
	
	//try to open unicode.txt for reading (not a binary file so just read it as ASCII plain text)
	FILE *fp = fopen("unicode.txt", "r");
	
	//if we fail to open the file, print error message to standard error and exit with status of 1
	if ( fp == NULL ) {
		fprintf( stderr, "Invalid unicode table file" );
		exit(1);
	}
	
	//read each line of the unicode file.  Each line is organized as such:
	//numeric value for a code point | tab char | name of the code point | end of line char
	
	//malloc initial memory for table and set its initial size and capacity
	int cap = 10;
	size = 0;
	table = (struct CodePoint *) malloc(cap * sizeof(CodePoint));


	//scan the code and name for each codepoint struct until EOF
	//%x because codes are in hex
	char string[BUFFER];
	while ( fgets(string, BUFFER, fp) ) {
		unsigned int nextCode;
		char nextName[91];
		sscanf(string, "%x %[^\n]", &nextCode, nextName);
		
		if ( size + 1 >= cap ) {
			cap *= 2;
			table = realloc(table, cap * sizeof(CodePoint));
		}
		
		table[size].code = nextCode;
		strcpy( table[size].name, nextName );
		
		//increment the size of the table since scan was successful
		size++;
		
	}

	//use qsort to sort the codepoints table given its #elements, sizeof a codepoint,
	//and the comparison function we defined above
	qsort( table, size, sizeof(CodePoint), compareCP );
	
	//close the file
	fclose(fp);
}

void freeTable() {
	
	//NOTE: DOUBLE CHECK THAT THIS WORKS AND FREES THE TABLE PROPERLY
	
	//free the dynamically allocated table of codepoints so we don't get a memory leak
	//struct CodePoint *temp;
	free(table);
	//while (table != NULL) {
	//	temp = table;
	//	table = table->next;
	//	free(temp);
	//}
}

bool reportCode( unsigned int code ) {
	//BINARY SEARCH OF THE ARRAY SINCE IT HAS BEEN QSORTED
	
	int min = 0;
	int max = size - 1;
	int mid = ( min + max ) / 2;
	
	while ( min <= max ) {
		if ( table[mid].code < code ) {
			min = mid + 1;
		}
		else if ( table[mid].code == code ) {
			printf( table[mid].name );
			printf("\n");
			return true;			
		}
		else {
			max = mid - 1;
		}
		
		mid = ( min + max ) / 2;
	}
	return false;	
}