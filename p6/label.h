/**
  @file label.h
  @author

  Mapping from label names to lines of code.
*/

#ifndef _LABEL_H_
#define _LABEL_H_

#define INITIAL_CAPACITY 5
#define LABEL_LEN 21

//struct for the labels themselves
typedef struct Label {
  //label name (up to 20 chars)
  char name[LABEL_LEN];
  //location in the script
  int loc;
  
} Label;

/** Map from label names to locations in the code. */
typedef struct {
  // ...
  //size of map?
  int size;
  //capacity of map?
  int cap;
  //Head of map?
  Label *map;

} LabelMap;

/** Initialize the fields of the given labelMap structure.
    @param labelMap Addres of the structure to initialize.
*/
void initMap( LabelMap *labelMap );

/** Add a label to the given labelMap.  Print an error message and
    exit if the label is a duplicate.
    @param labelMap LabelMap to add a label to.
    @param name Name of the label to add.
    @param loc Location of the label in the code.
*/
void addLabel( LabelMap *labelMap, char *name, int loc );


int findLabel( LabelMap lmap, char *name );

void freeMap( LabelMap *lmap );

#endif
