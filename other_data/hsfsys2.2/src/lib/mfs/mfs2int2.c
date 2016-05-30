/*
# proc: mfs2int2 - converts the mfs structure values containing two integers
# proc:            into two separate arrays of integers.
*/

#include <stdio.h>
#include <mfs.h>

void mfs2int2(mfs, i1, i2)
MFS *mfs;
int **i1, **i2;
{
   int i, *i1ptr, *i2ptr;

   malloc_int(i1, mfs->num, "mfs2int2 : i1");
   malloc_int(i2, mfs->num, "mfs2int2 : i2");
   i1ptr = (*i1);
   i2ptr = (*i2);
   for(i = 0; i < mfs->num; i++)
      sscanf(mfs->values[i], "%d %d", i1ptr++, i2ptr++);
}
