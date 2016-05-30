/*
# proc: appendmfs - appends a specified string value to the end of an
# proc:             mfs structure.
*/

#include <stdio.h>
#include <string.h>
#include <mfs.h>
#include <defs.h>

void appendmfs(mfs,value)
MFS *mfs;
char *value;
{
   int increased, incr;

   if(mfs->num >= mfs->alloc)
   {
      incr      = mfs->alloc / 10;		/* add 10% or 10 which-	*/
      increased = mfs->alloc + max(10, incr); 	/* ever is larger	*/
      reallocmfs(mfs, increased);
   }
   mfs->values[mfs->num] = strdup(value);
   if(mfs->values[mfs->num] == NULL)
      syserr("appendmfs","strdup", "mfs->values[]");
   (mfs->num)++;
}
