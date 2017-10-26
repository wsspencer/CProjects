#include "item.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define LINE_MAX 10

//struct Item {
//    int id;            //id for referencing in list
//    char store[STORE_MAX + 1];    //12 char length store name
//    double price;    //double price
//    char *name;        //arbitrary length name
//}

Item *readItem( char *str ) {
    //create dynamically allocated item from string.  Store, price, name.  If successful, return pointer to new item.  If not, return null.
    Item *it = (Item*) malloc(sizeof(Item));
    it->name = (char*) malloc(LINE_MAX * sizeof(char));

    int stringPlace = 0;

    //check if we can get the store name and price and last index of them from the given line
    if ( sscanf(str, "%s %lf %n", it->store, &it->price, &stringPlace) == 2 ) {
        //malloc new item, set its values, and return a pointer to it.
        //be sure to malloc name separately
		
		//check if store name is too long
		if (strlen(it->store) > STORE_MAX) {
			return NULL;
		}

        int numChars = 0;
        int nameCap = 1;
		
		it->name[0] = 0;
		
		while (str[stringPlace] != '\n') {
            if (numChars >= nameCap) {
                nameCap += LINE_MAX;
                it->name = realloc(it->name, nameCap * sizeof(char));
			}
            it->name[numChars] = str[stringPlace];
            numChars++;
			stringPlace++;
        }
		if (numChars != 0) {
			//null terminate the string before returning the item
			//for (int i = numChars; i < (strlen(str) - stringPlace); i++) {
			it->name[numChars] = '\0';
			//}
			return it;
		}
        else {
			freeItem(it);
            return NULL;
        }
    }
    else {
        //if the store name was too long, if the price didn't parse as a floating point number or if the item name was empty
        freeItem(it);
        return NULL;
    }
}

void freeItem( Item *it ) {
    //free items name separately
    free(it->name);
    //free item
    free(it);
}