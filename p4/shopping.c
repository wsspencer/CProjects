/**
    @file shopping.c
    @author W. Scott Spencer

    This file contains the main method of our program and interacts with the user in order to
    create lists, load lists from files, save lists to files, add and remove items from lists,
    and generate reports of lists by greater/less than prices or store name.
*/

/** Header file containing the function prototypes for item functions. */
#include "item.h"
/** Header file containing the function prototypes for list functions. */
#include "list.h"
/** Header file containing standard input/output functions we will use. */
#include <stdio.h>
/** Header file containing string functions we will use. */
#include <string.h>
/** Header file containing standard library functions we will use. */
#include <stdlib.h>
/** Header file containing boolean operations we will use. */
#include <stdbool.h>

/** Constant int representing the color the initial size of a line */
#define LINE_MAX 20
/** Constant int representing the color the initial size of a file line */
#define FILE_LINE_MAX 400
/** Constant int representing the color the length of a command */
#define CMD_LEN 7
/** Constant int representing the size of the check string */
#define CHECK_SIZE 8

/** Function prototype for a shopping list  */
ShoppingList *makeShoppingList();


/**
    This function gets a line from a file stream (or stdin if it's passed to it).  It returns
    an array of characters that represent that line of input.  It can also load items into a
    shoppinglist from a file, and save shoppinglist items to a new file.
    @param *stream FILE the file stream you want to read a line of input from.
    @return char* array of chars representing the line we want to retrieve.
*/
char *getLine( FILE *stream ) {

    char *buffer = (char*) malloc(LINE_MAX * sizeof(char));
    int numChars = 0;
    int buffSize = LINE_MAX;
    char ch;

    //do a loop for as long as we can read 200 chars into our buffer string (until newline char is
    //scanned or stdin yields an EOF)?
    do {
        if (numChars >= buffSize) {
            buffSize += LINE_MAX;
            buffer = realloc(buffer, buffSize);
        }
        ch = fgetc(stdin);
        buffer[numChars] = ch;
        numChars++;
    } while (ch != '\n' && ch != EOF);

    if (ch == EOF) {
        return NULL;
    }

    return buffer;
}

/**
    This function tests whether or not an item should be printed given a certain type of report.
    @param *it Item instance that we want to check
    @param *arg void anything we want to pass, in this case it's the string of info for the report
    @return bool representing whether or not the item should be printed to the report
*/
bool test (Item *it, void *arg) {
    //convert arg pointer back into a string (void *arg is a pointer to anything we want, but we
    //still need to convert it to the desired type before we use it in an operation.
    char *cast = (char *)arg;
    //create a new variable to work with (add values to so we can change index) without changing
    //original
    char *extra = cast;
    char *type = (char*) malloc(LINE_MAX * sizeof(char));

    int stringPlace = 0;
    //scan extra to save the report type and also keep track of where we left off scanning
    if (sscanf(extra, "%s %n", type, &stringPlace) != 1) {
        free(type);
        return true;
    }
    else {

        //try to scan in the item's price
        double price;

        //is the report a store report?
        if (strcmp(type, "store") == 0) {
            char *storeName = (char*) malloc(LINE_MAX * sizeof(char));
            //scan the rest of extra for store name
            sscanf(extra + stringPlace, "%s", storeName);

            //if item's store name matches the given store name, return true
            if (strcmp(storeName, it->store) == 0) {
                free(type);
                free(storeName);
                return true;
            }
        }

        //is the report a greater than report?
        else if (strcmp(type, "greater") == 0) {
            //scan the rest of extra for price
            sscanf(extra + stringPlace, "%lf", &price);

            if (it->price > price) {
                free(type);
                return true;
            }
        }

        //is the report a less than report?
        else if (strcmp(type, "less") == 0) {
            //scan the rest of extra for price
            sscanf(extra + stringPlace, "%lf", &price);

            if (it->price < price) {
                free(type);
                return true;
            }
        }
    }
    free(type);
    return false;

}

/**
    This is the main function which handles all the user interaction and delegates processes
    to other functions and files in order to facilitate changes to lists.
    @param argc number of command line arguments passed at runtime
    @param *argv[] command line arguments array
    @return int exit status
*/
int main(int argc, char *argv[]) {
    int promptcount = 0;


    //NOTE:  May be easier to read line of user input with sscanf and parse out the command name...
    //but that means restructuring to scan each "if" starting with the expected command


    //pointer position in string
    int linePos;

    ShoppingList *list = makeShoppingList();
    //input command
    char input[CMD_LEN];
    //input line
    char *line = NULL;

    //flag for when there's a bad file load so we don't use the line terminator...it
    //said to in the project design
    bool lineTermFlag = true;

    do {
        promptcount++;
        printf("%d> ", promptcount);

        line = getLine(stdin);
        if (line == NULL) {
            return 0;
        }

        //scan first command out of line, saving the last position read
        sscanf(line, "%s %n", input, &linePos);

        //testing
        char *newline = line;
        newline += linePos;


        //check for load, save, add, remove, report, help, and quit
        if ( strcmp(input, "load") == 0) {
            //read in a filename of unknown size
            char *filename = (char*) malloc(LINE_MAX * sizeof(char));
            int numChars = 0;
            int filenameCap = LINE_MAX;
            //loop through user input to build filename
            while (newline[numChars] != '\n' && newline[numChars] != ' ' &&
                        newline[numChars] != EOF) {
                if (numChars >= filenameCap) {
                    filenameCap += LINE_MAX;
                    filename = realloc(filename, filenameCap * sizeof(char));
                }
                filename[numChars] = newline[numChars];
                numChars++;
            }
            //null terminate filename
            filename[numChars] = '\0';
            //initialize file pointer, open it at the given filename
            FILE *fp;
            fp = fopen(filename, "r");

            //check if file exists
            if (fp != NULL) {
                //iterate through the file one line at a time.  Each line should yield an item
                //which we will add to our instance of a list.  BE SURE NOT TO ALTER LINE SO
                //IT CAN BE FREED
                char *fileLine = (char*) malloc(FILE_LINE_MAX * sizeof(char));
                int lineCount = 1;
                while (fgets(fileLine, FILE_LINE_MAX, fp) != NULL) {
                    Item *it = readItem(fileLine);
                    //if it is null, we got a bad line, so let the user know which line it was on
                    if (it == NULL) {
                        printf("\nInvalid item, line %d", lineCount);
                    }
                    else {
                        shoppingListAdd(list, it);
                    }
                    lineCount++;
                }
                fclose(fp);
                free(fileLine);
                free(filename);
            }
            else {
                //Print to STANDARD OUTPUT
                printf("\nCan't open file");
                lineTermFlag = false;
            }
        }
        else if ( strcmp(input, "save") == 0) {
            //save the current list to a file
            //read in a filename of unknown size
            char *filename = (char*) malloc(LINE_MAX * sizeof(char));
            int numChars = 0;
            int filenameCap = LINE_MAX;
            //loop through user input to build filename
            while (newline[numChars] != '\n' && newline[numChars] != ' ' &&
                        newline[numChars] != EOF) {
                if (numChars >= filenameCap) {
                    filenameCap += LINE_MAX;
                    filename = realloc(filename, filenameCap * sizeof(char));
                }
                filename[numChars] = newline[numChars];
                numChars++;
            }
            //null terminate filename
            filename[numChars] = '\0';

            //open the file
            FILE *fp;
            fp = fopen (filename, "w");
            //check if file exists
            if (fp == NULL) {
                //print error to standard output
                printf("\nCan't open file");
            }
            else {
                for (int i = 0; i < list->length; i++) {
                    //check if item is null, do not print it if it is
                    if (list->items[i] != NULL) {
                        //write each individual item to the file one line at a time
                        fprintf(fp, "%s ", list->items[i]->store);
                        fprintf(fp, "%.2lf ", list->items[i]->price);
                        fprintf(fp, "%s\n", list->items[i]->name);
                    }
                }
            }
            fclose(fp);
            free(filename);
        }
        else if ( strcmp(input, "add") == 0) {
            //NOTE: sscanf from earlier already parsed out "add" from line, so just pass it to make
            //an item
            Item *it = NULL;
            it = readItem(newline);

            if (it != NULL) {
                shoppingListAdd(list, it);
            }
            else {
                printf("\nInvalid command");
            }
        }
        else if ( strcmp(input, "remove") == 0) {
            //remove the item in our instance of list corresponding to the given id
            int rId;
            sscanf(newline, "%d", &rId);
            shoppingListRemove(list, rId);
        }
        else if ( strcmp(input, "report") == 0) {
            //run tests to see if user specified store or less or greater report.
            //check if valid report command
            char checker[CHECK_SIZE] = "";
            sscanf(newline, "%s", checker);
            if (strcmp(checker, "store") == 0 || strcmp(checker, "greater") == 0 ||
                    strcmp(checker, "less") == 0 || strcmp(checker, "") == 0) {
                shoppingListReport(list, test, newline);
            }
            else {
                printf("\nInvalid command");
            }
        }
        else if ( strcmp(input, "help") == 0) {
            //When the user enters the help command, the program will respond with a report of the
            //valid commands. It will print the following message to STANDARD OUTPUT, then prompt
            //for another command:

           printf("\nload <file>\nsave <file>\nadd <store> <price> <name>\nremove <id>\nreport\n");
           printf("report store <store>\nreport less <price>\nreport greater <price>\nhelp\nquit");
        }
        else if (strcmp(input, "quit") != 0) {
            //print to standard output
            printf("\nInvalid command");
        }
        if (lineTermFlag) {
            printf("\n");
        }
        lineTermFlag = true;
        free(line);

    } while ( strcmp(input, "quit") != 0);
    //free the list
    freeShoppingList(list);

    return 0;
}
