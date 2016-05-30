/*
# proc: load_phrase_dictionary - loads the contents of the given filename into
# proc:                          a phrase dictionary (mfs structure).
*/

#include <stdio.h>
#include <mfs.h>

#define PHRASES 20000

MFS *load_phrase_dictionary(filename)
char *filename;
{
   MFS *phrases;

   phrases = readmfsfile(filename);
   if(phrases->num > PHRASES)
      fatalerr("digraph_encode_file", "phrase dictionary too long",
               "length must equal PHRASES");
   return(phrases);
}
