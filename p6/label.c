/**
  @file label.c
  @author William S Spencer

  This object is used by our script interpreter program to handle labels within the script given
  to us by our user.  As nonde.c reads through our script and parse.c parses its contents, we will
  inevitably run into labels intended to be jumping and reference points for the script.  This
  object is responsible for keeping track of a data structure that holds the labels called
  "labelmap" and adding more labels to it, as well as finding labels within it.  Once we are done
  using the labelmap, this object will be responsible for freeing the memory used by the map
  (labelmap's memory is allocated dynamically so we can grow it beyond a predefined size).
*/

/** Header for standard input/output function calls */
#include <stdio.h>
/** Header for standard library function calls */
#include <stdlib.h>
/** Header for string associated function calls */
#include <string.h>
/** Header for label function calls */
#include "label.h"

void initMap( LabelMap *labelMap )
{
  // ...
  //allocate memory for the map portion of labelMap separately
  labelMap->map = (Label *) malloc( INITIAL_CAPACITY * sizeof( Label ) );

  //initiate size of map
  labelMap->size = 0;
  //intitate capacity of map
  labelMap->cap = INITIAL_CAPACITY;
}

void addLabel( LabelMap *labelMap, char *name, int loc )
{
  // ...
  //check if array needs to be resized
  if (labelMap->size + 1 >= labelMap->cap) {
    //increase capacity
    labelMap->cap += INITIAL_CAPACITY;
    //reallocate the map memory
    labelMap->map = realloc(labelMap->map, labelMap->cap * sizeof(Label));
  }

  //check that the label does not already exist in the labelMap, if it does,
  //print error and exit
  if ( findLabel( *labelMap, name ) != -1 ) {
    fprintf(stderr, "Duplicate label: %s\n", name);
    exit(1);
  }

  //add the label name to the next available location
  strcpy( labelMap->map[labelMap->size].name, name);
  //add the label location to next available index
  labelMap->map[labelMap->size].loc = loc;
  //increment size
  labelMap->size++;
}

int findLabel( LabelMap lmap, char *name )
{
  // ...
  //iterate through map until we find the label in question
  int index = 0;
  while ( strcmp(lmap.map[index].name, name) != 0 ) {
      index++;
      //check if we're at the end of the map
      if ( index > lmap.size ) {
          return -1;
      }
  }
  //if label was not found, return -1

  //return the location in the script of the label at index
  return lmap.map[index].loc;
}

void freeMap( LabelMap *lmap )
{
  // ...
  // Free the memory of out labelmap's map first, then the memory containing the labelmap struct
  //itself
  free(lmap->map);
  free(lmap);
}
