//This is the main component, it will open and read from the input file, detect errors and use the
//codepoints component to print the names of valid codepoints.

#include "codepoints.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	//If the user tries to run the program with invalid command-line arguments (anything other
	//than a single filename and the program call), it will print an error message to standard
	//error and exit with an exit status of 1.
	if (argc != 2) {
		fprintf(stderr, "usage: un-utf8 <input-file>");
		exit(1);
	}
	
	//Try to open the file whose name the user input in binary mode (input will contain bytes
	//that aren't ordinary 7-bit ASCII characters.
	FILE *fp;
	fp = fopen(argv[1], "rb");
	
	//if the file cannot be opened, print an 2 messages to standard error and exit with an exit
	//status of 1.
	if (fp == NULL) {
		fprintf(stderr, "Can't open file: ");
		fprintf(stderr, argv[1]);
		fprintf(stderr, "\nusage: un-utf8 <input-file>");
		exit(1);
	}
	
	
	//Read in a table of unicode characters, mapping each character's code to a name for that
	//character.  Use that table to read each character code in the user's input file and print
	//out the name for each character.
	loadTable();
	
	//Now we iterate through the binary file, trying to match each value's code to a code in the
	//table we just loaded by calling "reportCode" which will also print the code's associated name
	//to the standard output file.
	
	//NOTE:
	//If code is 1-7 bits, it will be represented by one byte beginning with "0."
	//If code is 8-11 bits, it will  be represented by two bytes.  The first beginning with "110"
	//and the second with "10."
	//If code is 12-16 bits, it will  be represented by three bytes.  The first beginning with
	//"1110" and the second and third with "10."
	//If the code is 17 to 21 bits, it will be represented by four bytes.  The first beginning with
	//11110 and the second, third, and fourth with "10."
	
	//(I think I'll need to use fseek and constants like SEEK_END to create a variable to contain
	//the size of the file, then read in one byte at a time and check to see how many bytes this
	//particular unicode has and read in the rest of it accordingly.)
	
	//ALSO, read the values in as values, remove the formatting bits through masking/bitshifting,
	//then search codepoint table for that value. 
	
	//okay so start by scanning a byte, then check its high order bits to determine if we need to
	//scan in more bytes (reference lecture 17).  Once we have the bits we need, compare them to
	//the codes in the codepoints table.

	
	//fseek(fp, 0, SEEK_END); //set pointer to end
	//long fileLength;
	//fileLength = ftell(fp); //set fileLength to size in bytes
	//rewind(fp); //set pointer back to beginning of file
	
	//loop for every byte in the file
	unsigned int input;
	//read into "input" 1 element of size 1 byte of memory from fp
	
	while (fread(&input, 1, 1, fp) == 1) {
		
		//case of 1 byte read (bin 0XXXXXXX), let i increment (note, this will also handle null)
		if ( (input & 0x80) == 0x00) {
			//mask off all but the last
			input = input & 0xFF;
			//report code
			reportCode(input);
		}
		
		//case of 2 bytes read (bin 110XXXXX), increment i by 2
		else if ( (input & 0xE0) == 0xC0) {
			//new int that we can add to the end of input
			unsigned int concat;			
			//read another 2 bytes into input
			fread(&concat, 1, 1, fp);
			//we know concat's byte will begin with 10 so go ahead and remove the first 2 bits
			concat = concat & 0x3F;
			//shift input over and add concat to its end (only shift 6 since we've removed the
			//first 2 bits)
			input = input << 6;
			//bitwise OR to concatenate the two bitfields
			input = input | concat;

			//mask off the first 3 bits since we know they're 110
			input = input & 0xFFF;
			
			//call reportCode
			reportCode(input);
		}
		
		//case of 3 bytes read (bin 1110XXXX), increment i by 3
		else if ( (input & 0xF0) == 0xE0) {
			//new int bitfields that we can add to the end of input
			unsigned int firstByte;
			unsigned int secondByte;
			//read another 2 bytes into input
			fread(&firstByte, 1, 1, fp);
			fread(&secondByte, 1, 1, fp);
			//we know concat's 2 bytes will begin with 10 so go ahead and remove the first two bits
			//from both bytes
			firstByte = firstByte & 0x3F;
			secondByte = secondByte & 0x3F;
			
			//shift input over and add the two bytes minus their 2 highest bits 
			input = input << 6;
			input = input | firstByte;
			input = input << 6;
			input = input | secondByte;
			
			//mask off all but the last
			input = input & 0xFFFF;
			//call reportCode
			reportCode(input);
		}
		
		//case of 4 bytes read (bin 11110XXX), increment i by 4
		else if ( (input & 0xF8) == 0xF0) {
			//new int bitfields that we can add to the end of input
			unsigned int firstByte;
			unsigned int secondByte;
			unsigned int thirdByte;
			//read another 3 bytes into input
			fread(&firstByte, 1, 1, fp);
			fread(&secondByte, 1, 1, fp);
			fread(&thirdByte, 1, 1, fp);
			//we know concat's 2 bytes will begin with 10 so go ahead and remove the first two bits
			//from all three bytes
			firstByte = firstByte & 0x3F;
			secondByte = secondByte & 0x3F;
			thirdByte = thirdByte & 0x3F;
			//shift input over and add the three bytes minus their 2 highest bits 
			input = input << 6;
			input = input | firstByte;
			input = input << 6;
			input = input | secondByte;
			input = input << 6;
			input = input | thirdByte;
			
			//mask off all but the last
			input = input & 0xFFFFF;
			//call reportCode
			reportCode(input);
		}
	}
	
	//Once we are done with the table we should free its memory by calling freeTable()
	freeTable();
}