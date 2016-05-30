/*
# proc: allocfet - allocates and initialized an empty fet structure.
# proc:
# proc: reallocfet - reallocates an fet structure of a
# proc:              specified length.
*/

#include <stdio.h>
#include <fet.h>

FET *allocfet(numfeatures)
int numfeatures;
{
   FET *fet;

   if ((fet = (FET *)malloc(sizeof(FET))) == NULL)
      syserr("allocfet","malloc","fet");
   /* calloc here is required */
   if ((fet->names = (char **)calloc(numfeatures, sizeof(char *))) == NULL)
      syserr("allocfet","calloc","fet->names");
   if ((fet->values = (char **)calloc(numfeatures, sizeof(char *))) == NULL)
      syserr("allocfet","calloc","fet->values");
   fet->alloc = numfeatures;
   fet->num = 0;
   return(fet);
}

FET *reallocfet(fet, newlen)
FET *fet; int newlen;
{
   if (fet == NULL || fet->alloc == 0)
      return(allocfet(newlen));

   if ((fet->names = (char **)realloc(fet->names,
                       newlen * sizeof(char *))) == NULL)
      fatalerr("reallocfet", "realloc", "space for increased fet->names");
   if ((fet->values = (char **)realloc(fet->values,
                       newlen * sizeof(char *))) == NULL)
      fatalerr("reallocfet", "realloc", "space for increased fet->values");
   fet->alloc = newlen;

   return(fet);
}
