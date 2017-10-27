/**
    @file list.c
    @author W. Scott Spencer

    This file handles all the functions for the List struct.  It will handle the initial creation
    of a list of Items, adding to the list, removing from the list, generating specific reports
    for the list, and eventually freeing the dynamically allocated list for our user.
*/

/** Header file containing the function prototypes for item functions. */
#include "item.h"
/** Header file containing the function prototypes for list functions. */
#include "list.h"
/** Header file containing standard library functions we will use. */
#include <stdlib.h>
/** Header file containing string functions we will use. */
#include <string.h>
/** Header file containing standard input/output functions we will use. */
#include <stdio.h>

/** Constant int representing the color the initial capacity of a list of items */
#define INITIAL_CAP 50

/**
    This function is documented in list.h.
*/
ShoppingList *makeShoppingList() {
    ShoppingList *list = (ShoppingList*) malloc(sizeof(ShoppingList));
    list->length = 0;
    list->capacity = INITIAL_CAP;
    list->items = (Item**) malloc(INITIAL_CAP * sizeof(Item*));

    return list;
}

/**
    This function is documented in list.h.
*/
void freeShoppingList( ShoppingList *list ) {
    for(int i = 0; i <list->length; i++) {
        if (list->items[i] != NULL) {
            freeItem(list->items[i]);
        }
    }
    //free the list of items
    free(list->items);
    //free the rest of list info
    free(list);
}

/**
    This function is documented in list.h.
*/
void shoppingListAdd( ShoppingList *list, Item *it ) {
    if (list->length >= list->capacity) {
        //double capacity
        list->capacity *= 2;

        //reallocate memory for list with double space for capacity
        list->items = realloc(list->items, list->capacity * sizeof(Item*));
    }

    //add it to end of list, with new unique id (not = size because size may change but id cannot,
    //and increment length.)
    list->length++;
    it->id = list->length;
    list->items[list->length - 1] = it;

}

/**
    This function is documented in list.h.
*/
bool shoppingListRemove( ShoppingList *list, int id ) {
    //remove item at id from the array...return true if successful, false if not
    //if an item in the middle of the array is used, do not print it, and do not re-use that item's
    //id.
    int index = id - 1;
    if (list->items[index] != NULL) {
        //free(list->items[index]);
        list->items[index] = NULL;
        return true;
    }
    return false;

}

/**
    This function is documented in list.h.
*/
void shoppingListReport( ShoppingList *list, bool test( Item *it, void *arg ), void *arg ) {

            //iterate through list, printing the items that fit our criteria and incrementing total
            //cost.  You can tell if an item should be printed based on the function pointer passed
            //as a paramter.  Call it with the desired argument.

            double total = 0.0;
            printf("\n");
            for (int i = 0; i < list->length; i++) {
                if (list->items[i] != NULL && test(list->items[i], arg)) {
                    //print list->items[i] in report format
                    //printf("test");
                    printf("%4d ", list->items[i]->id);
                    printf("%-12s ", list->items[i]->store);
                    printf("%7.2lf ", list->items[i]->price);
                    printf("%s\n", list->items[i]->name);
                    //add list->items[i]->price to total
                    total += list->items[i]->price;
                }
            }
            printf("                  %7.2lf", total); 
            //right justified 7 character field with 2 fractional digits (and alligning spaces)
}