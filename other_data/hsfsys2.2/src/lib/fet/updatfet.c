/*
# proc: updatefet - replaces a feature entry in an fet structure, or creates
# proc:             a new entry if the feature does not already exist.
*/

#include <stdio.h>
#include <string.h>
#include <fet.h>
#include <defs.h>

void updatefet(feature,value,fet)
char *feature,*value;
FET *fet;
{
  int item;
  int increased, incr;

  for (item = 0;
       (item < fet->num) && (strcmp(fet->names[item],feature) != 0);
       item++);
  if (item < fet->num){
     if(fet->values[item] != NULL){
        free(fet->values[item]);
        fet->values[item] = NULL;
     }
     if(value != NULL){
        fet->values[item] = strdup(value);
        if(fet->values[item] == NULL)
           syserr("updatefet","strdup","fet->values[]");
     }
  }
  else{
     if(fet->num >= fet->alloc){
        incr      = fet->alloc / 10;		/* add 10% or 10 which-	*/
        increased = fet->alloc + max(10, incr);	/* ever is larger	*/
        reallocfet(fet, increased);
     }
     fet->names[fet->num] = strdup(feature);
     if(fet->names[fet->num] == NULL)
        syserr("updatefet","strdup","fet->names[]");
     if(value != NULL){
        fet->values[fet->num] = strdup(value);
        if(fet->values[fet->num] == NULL)
           syserr("updatefet","strdup","fet->values[]");
     }
     (fet->num)++;
  }
}
