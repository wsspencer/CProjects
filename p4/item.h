/**
    @file item.h
    @author W. Scott Spencer

    This file handles all the function prototypes for the functions in item.c and its struct
    definition so it can be used by other files.
*/

/** Constant representing maximum length of a store name */
#define STORE_MAX 12

/** Representation for an item to be purchased. */
typedef struct {
  /** Unique ID for this particular item.  */
  int id;

  /** Store where we're supposed to buy the item.  */
  char store[ STORE_MAX + 1 ];

  /** Price of this item in dollars (but you might normally store this
      as an integer number of cents). */
  double price;

  /** Name of this item.  Pointer to a string of arbitrary length. */
  char *name;
} Item;

/**
    Function prototype for read Item which takes a pointer to a string of item info
    and returns an instance of item.  If the item info is invalid it will return null.

    @param *str char array that contains all the info needed to read an item.
    @return Item pointer which points to all the information of this instance of item.
*/
Item *readItem( char *str );

/**
    Function prototype for freeItem which takes a pointer to an item and frees up all the
    memory dynamically allocated to that item so it may be used by other operations.

    @param *it Item pointer that points to all the info contained in an instance of item.
    @return void
*/
void freeItem( Item *it );
