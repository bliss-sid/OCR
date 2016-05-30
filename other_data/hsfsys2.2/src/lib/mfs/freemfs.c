/*
# proc: freemfs - deallocates an mfs structure and its values.
# proc:
*/

#include <mfs.h>

void freemfs(mfsptr)
MFS *mfsptr;
{
  int item;
  for (item=0;item<mfsptr->num;item++)
      free(mfsptr->values[item]);
  free((char *)mfsptr->values);
  free((char *)mfsptr);
}
