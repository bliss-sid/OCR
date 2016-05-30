/*
# proc: remove_from_list - removes an integer from the given list by shifting
# proc:                    down all integers following the point of removal.
# proc: remove_item - removes corresponding items from three lists of integers
# proc:               by shifting down all subsequent integers in the lists.
*/

#include <stdio.h>
#include <maxlist.h>

/************************************************************************/
remove_from_list(i, li, end)
int i, *li, end;
{
   int tlist[MAX_INDEX], cplen;

   if(i >= end)
      fatalerr("remove_from_list", "index off end of list", NULL);
   cplen = (end - i - 1) * sizeof(int);
   memcpy(tlist, li+i+1, cplen);
   memcpy(li+i, tlist, cplen);
}

/************************************************************************/
remove_item(i, li, lx, ly, end)
int i, *li, *lx, *ly, *end;
{
   remove_from_list(i, lx, (*end));
   remove_from_list(i, ly, (*end));
   remove_from_list(i, li, (*end));
   (*end)--;
}
