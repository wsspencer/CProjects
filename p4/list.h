/**
    @file list.h
    @author W. Scott Spencer

    This file defines all the function prototypes for list.c as well as its struct definition
    so they can be called and used by other functions.
*/

/** Header file containing boolean operations we will use. */
#include <stdbool.h>

/** Representation for a pping list, an arbitrary-length list of Items. */
typedef struct {
  /** Resizable array of pointers to Items. */
  Item **items;

  /** Current number of items on the list. */
  int length;

  /** Current capacity of the list, how many pointers we have room for. */
  int capacity;
} ShoppingList;

/**
    This function instantiates a the list of items and allocates its initial block of memory.
    @return ShoppingList * pointer to shoppinglist instance our user will work with.
*/
ShoppingList *makeShoppingList();

/**
    This function frees the memory storing an instance of shoppinglist so it may be used elsewhere.
    @param *list ShoppingList the instance of shoppinglist we want to free
    @return void
*/
void freeShoppingList( ShoppingList *list );

/**
    This function takes an instance of shoppinglist and of item and adds the item to the shopping
    list (if it is valid).  It will alter the instance of list so it returns nothing.
    @param *it Item instance we want to add to a shoppinglist
    @param *list the instance of shoppinglist we want to add to
    @return void
*/
void shoppingListAdd( ShoppingList *list, Item *it );

/**
    This function removes an item with a given unique identifier from an instance of shoppinglist.
    @param *list ShoppingList instance we want to alter
    @param id int representing an item's unique identifier that we want to remove
    @return bool telling us if the remove was successful or not
*/
bool shoppingListRemove( ShoppingList *list, int id );

/**
    This function generates and prints a report of items in the shopping list given certain
    constraints. These can be only report items greater than a given price, only report items
    less than a certain price, only report items from a certain store name, or report all
    items in the shoppinglist.
    @param *list ShoppingList an instance of shoppinglist we want the report to work from
    @param test (Item, void) function pointer for testing an item against report constraints
    @param *arg void anything we want, in this case it will be the report type and constraints
    @return char* array of chars representing the line we want to retrieve.
*/
void shoppingListReport( ShoppingList *list, bool test( Item *it, void *arg ), void *arg );
