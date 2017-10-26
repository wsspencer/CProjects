#include "item.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define LINE_MAX 20

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
    
    //check if we can get the store name and price and following space from the given line
    if ( sscanf(str, "%s %lf %[^\n]\n", it->store, &it->price, it->name) == 3 ) {
        //malloc new item, set its values, and return a pointer to it.
        //be sure to malloc name separately
            
        return it;
        //}
        //else {
            //return NULL;
        //}
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