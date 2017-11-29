/**
  @file label.h
  @author William S Spencer

  Mapping from label names to lines of code.
*/

#ifndef _LABEL_H_
#define _LABEL_H_

/** The initial capacity of our label map */
#define INITIAL_CAPACITY 5
/** The maximum length of a label */
#define LABEL_LEN 21

/** struct for the labels themselves */
typedef struct Label {
  //label name (up to twenty chars)
  char name[LABEL_LEN];
  //location in the script
  int loc;
  
} Label;

/** Map from label names to locations in the code. */
typedef struct {
  // ...
  //size of map
  int size;
  //capacity of map
  int cap;
  //Head of map
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

/** Find where the label is in the labelmap, return its location.
    @param lmap instance of our labelmap
    @param name the name of the label we're looking for.
    @return int location of the label we found
 */
int findLabel( LabelMap lmap, char *name );


/** Free the dynamically allocated memory storing our label map.
    @param lmap instance of our labelmap
    @return void
 */
void freeMap( LabelMap *lmap );

#endif
