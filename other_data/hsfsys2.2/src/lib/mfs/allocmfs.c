/*
# proc: allocmfs - allocates and initializes an mfs structure of a
# proc:            specified length.
# proc: reallocmfs - reallocates an mfs structure of a
# proc:            specified length.
*/

#include <stdio.h>
#include <mfs.h>

MFS *allocmfs(mfslen)
int mfslen;
{
   MFS *mfs;

   if((mfs = (MFS *)malloc(sizeof(MFS))) == NULL)
      syserr("allocmfs","malloc","mfs");
   if((mfs->values = (char **)calloc(mfslen, sizeof(char *))) == NULL)
      syserr("allocmfs","calloc","mfs->values");
   mfs->alloc = mfslen;
   mfs->num = 0;
   return(mfs);
}


MFS *reallocmfs(mfs, newlen)
MFS *mfs; int newlen;
{
   if (mfs == NULL || mfs->alloc == 0)
      return allocmfs(newlen);

   if ((mfs->values = (char **)realloc(mfs->values,
                       newlen * sizeof(char *))) == NULL)
      fatalerr("reallocmfs", "realloc", "space for increased mfs->values");
   mfs->alloc = newlen;

   return(mfs);
}
