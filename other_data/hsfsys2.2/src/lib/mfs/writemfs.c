/*
# proc: writemfsfile - takes an mfs structure and writes its contents to the
# proc:                specified file.
*/

#include <stdio.h>
#include <mfs.h>

void writemfsfile(file,mfsptr)
char *file;
MFS *mfsptr;
{
  FILE *mfs = NULL;
  int item;

  if ((mfs = fopen(file,"wb")) == NULL)
     syserr("writemfsfile","fopen",file);
  fprintf(mfs,"%d\n",mfsptr->num);
  for (item=0;item < mfsptr->num;item++)
      fprintf(mfs,"%s\n",mfsptr->values[item]);
  fclose(mfs);
}
