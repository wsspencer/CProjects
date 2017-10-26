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

//Function prototypes
ShoppingList *makeShoppingList();

void freeShoppingList( ShoppingList *list );

void shoppingListAdd( ShoppingList *list, Item *it );

bool shoppingListRemove( ShoppingList *list, int id );

void shoppingListReport( ShoppingList *list, bool test( Item *it, void *arg ), void *arg );
