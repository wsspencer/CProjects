#include "label.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
  //add the label name to the next available location
  strcpy( labelMap->map[labelMap->size].name, name);
  //add the label location to next available index
  labelMap->map[labelMap->size].loc = loc;
  //increment size
  labelMap->size++;
}

int findLabel( LabelMap lmap, char *name ) {
  // ...
  //iterate through map until we find the label in question
  int index = 0;
  while ( strcmp(lmap.map[index].name, name) != 0 ) {
	  index++;
	  //check if we're at the end of the map
	  if ( index > lmap.size ) {
		  printf("label not found!\n");
	  }
  }
  //return the location in the script of the label at index
  return lmap.map[index].loc;
}

void freeMap( LabelMap *lmap ) {
  // ...
  // Free the memory of out labelmap's map first, then the memory containing the labelmap struct
  //itself
  free(lmap->map);
  free(lmap);
}