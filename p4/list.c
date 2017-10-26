#include "item.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_CAP 2

//struct ShoppingList {
//    Item *items;        //pointer to first item in list
//    int length;            //current list length
//    int capacity;        //current list capacity
//}

ShoppingList *makeShoppingList() {
    ShoppingList *list = (ShoppingList*) malloc(sizeof(ShoppingList));
    list->length = 0;
    list->capacity = INITIAL_CAP;
    list->items = (Item**) malloc(INITIAL_CAP * sizeof(Item*));

    return list;
}

void freeShoppingList( ShoppingList *list ) {
    for(int i = list->length - 1; i >= 0; i--) {
        freeItem(list->items[i]);
    }
    free(list);
    //do I need to call freeitem method in a loop here?  I don't know why freeing the list itself won't work...
    //unless I'm not suppose to be allocating memory for shopping list in the first place...
    //because isn't that just allocating memory twice for the same thing?  But then what's the significance 
    //of resizing the array.
}

void shoppingListAdd( ShoppingList *list, Item *it ) {
    if (list->length >= list->capacity) {
        //double capacity
        list->capacity *= 2;
		
		printf("%d\n", list->capacity);
		
        //reallocate memory for list with double space for capacity
        list->items = realloc(list->items, list->capacity * sizeof(Item*));
    }

    //add it to end of list, with new unique id (not = size because size may change but id cannot, and increment length.)
    list->length++;
    it->id = list->length;
    list->items[list->length - 1] = it;


    //testing
    //so here's the deal.  We can't access some members of "it for some reason..idk why.
    //printf("%s", it->store);
    //printf("%lf", it->price);
    //printf("%s", it->name);
    //printf("%d", it->id);
}

bool shoppingListRemove( ShoppingList *list, int id ) {
    //remove item at id from the array...return true if successful, false if not.
    //if an item in the middle of the array is used, do not print it, and do not re-use that item's id.
    int index = id - 1;
    if (list->items[index] != NULL) {
        //free(list->items[index]);
        list->items[index] = NULL;
        return true;
    }
    return false;

}

void shoppingListReport( ShoppingList *list, bool test( Item *it, void *arg ), void *arg ) {

            //iterate through list, printing the items that fit our criteria and incrementing total cost.  You can tell
            //if an item should be printed based on the function pointer passed as a paramter.  Call it with the desired
            //argument.

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


            //PRINT REPORT TO STANDARD OUTPUT
            //DON'T PRINT THE EMPTY INDICES
            //generate and print a report.  User can specify whether they want the full list, or just some items.
            //if just "report" is entered, print the full report to standard output, in order by ID
            //the user can also request "report store _____", "report greater ____", "report less ____"
            //(meaning only reporting items form a certain store, greater than a given price , or less than a given price)
            //Each item is printed with the ID right-justified in a 4-character field, then a space, then the store name
            //left-justified in a 12-character field, then a space, then the price right-justified in a 7-character field,
            //with two fractional digits of precision, then a space, then the item name filling the rest of the output line.
            //the last line will be the price totals justified to
            
            //Just like the individual prices, the total price will be printed in a 7-column field, rounded to two-fractional
            //digits. As a result, if some of the prices are really high, or if they total is too larger, the price column may
            //not line up properly. The 7-column field width is a minimum field width, so if a price is too large to fit in
            //seven characters, it will take more. It's OK if the prices or the total don't line up when some of these numbers
            //are too large.

            //If the shopping list is empty, or if none of the items match the user's selection (see below), the report won't
            //contain any items, but it'll still contain a total (the total will just be 0.00 in this case). This behavior is
            //kind of silly, but it'll be easy to implement.
}