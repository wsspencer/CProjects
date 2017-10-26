
#include "item.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define LINE_MAX 20
#define CMD_LEN 7

//function prototypes
ShoppingList *makeShoppingList();

//IMPORTANT NOTE: add one free at a time...it seems like everytime I free something I get an error

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

//function to pass by pointer for shoppinglist report
bool test (Item *it, void *arg) {
    //convert arg pointer back into a string (void *arg is a pointer to anything we want, but we
    //still need to convert it to the desired type before we use it in an operation.
    char *extra = (char *)arg;
    char *type = (char*) malloc(LINE_MAX * sizeof(char));

    if (sscanf(extra, "%s", type) != 1) {
        free(type);
        return true;
    }
    else {

        //try to scan in the item's price
        double price = 0.0;

        //is the report a store report?
        if (strcmp(type, "store") == 0) {
            char *storeName = (char*) malloc(LINE_MAX * sizeof(char));
            //scan store name
            sscanf(extra, "%s", storeName);

            printf(storeName);

            //if item's store name matches the given store name, return true
            if (strcmp(storeName, it->store) == 0) {
                //free(type);
                return true;
            }
        }

        //is the report a greater than report?
        if (strcmp(type, "greater") == 0) {    
            sscanf(extra, "%lf", &price);
            if (it->price > price) {
                free(type);
                return true;
            }
        }

        //is the report a less than report?
        if (strcmp(type, "less") == 0) {
            sscanf(extra, "%lf", &price);
            if (it->price < price) {
                free(type);
                return true;
            }
        }
    }
    free(type);
    return false;

}

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

    do {
        promptcount++;
        printf("%d> ", promptcount);

        line = getLine(stdin);

        //scan first command out of line
        sscanf(line, "%s %n", input, &linePos);

        //testing
        char *newline = line;
        newline += linePos;


        //check for load, save, add, remove, report, help, and quit
        if ( strcmp(input, "load") == 0) {
            char *filename = "";
            scanf("%s", filename);
            FILE *fp;
            fp = fopen(filename, "r");
            //check if file exists
            if (fp == NULL) {
                //Print to STANDARD OUTPUT
                printf("Can't open file\n");
            }
            else {
                //iterate through the file one line at a time.  Each line should yield an item which we will
                //add to our instance of a list.  BE SURE NOT TO ALTER LINE SO IT CAN BE FREED
                char *fileLine = getLine(fp);
                do {
                    Item *it = readItem(fileLine);
                    shoppingListAdd(list, it);
                    fileLine = getLine(fp);
                } while (fileLine != NULL);
                
                //!!!Remember that the input files do not have "add" at the beginning of each line.
            }
        }
        if ( strcmp(input, "save") == 0) {
            //save the current list to a file
            //scan the filename
            FILE *fp;
            char *filename = "";
            scanf("%s", filename);
            fp = fopen (filename, "w");
            //check if file exists
            if (fp == NULL) {
                //print error to standard output
                printf("Can't open file\n");
            }
            else {
                for (int i = 0; i < list->length; i++) {
                    //write each individual item to the file one line at a time
                    fprintf(fp, "%s", list->items[i]->store);
                    fprintf(fp, "%lf", list->items[i]->price);
                    fprintf(fp, "%s", list->items[i]->name);
                }
            }
        }
        else if ( strcmp(input, "add") == 0) {
            //NOTE: sscanf from earlier already parsed out "add" from line, so just pass it to make an item
            Item *it = NULL;
            //printf(initline);
            it = readItem(newline);

            shoppingListAdd(list, it);
        }
        else if ( strcmp(input, "remove") == 0) {
            //remove the item in our instance of list corresponding to the given id 
            int rId;
            sscanf(newline, "%d", &rId);
            shoppingListRemove(list, rId);
        }
        else if ( strcmp(input, "report") == 0) {
            //run tests to see if user specified store or less or greater report.
            shoppingListReport(list, test, newline);
        }
        else if ( strcmp(input, "help") == 0) {
            //When the user enters the help command, the program will respond with a report of the valid commands. It will
            //print the following message to STANDARD OUTPUT, then prompt for another command:

            printf("load <file>\nsave <file>\nadd <store> <price> <name>\nremove <id>\nreport\n");
            printf("report store <store>\nreport less <price>\nreport greater <price>\nhelp\nquit");
        }
        else if (strcmp(input, "quit") != 0) {
            //print to standard output
            printf("Invalid command\n");
        }
        printf("\n");
        free(line);
        //free(newline);  //MAY NOT NEED THIS

    } while ( strcmp(input, "quit") != 0 /* && not EOF*/);

    //free(line);

    //don't think I'm properly freeing up memory here...
    //freeShoppingList(list);

    return 0;
}
