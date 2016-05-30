/*
# proc: int_list_median - returns the median value from a list of
# proc:                   unsorted integers.
*/

#include <stdio.h>

/***************************************************************************/
/* int_list_median - locates the median value from a list of integers by   */
/* using a simple bubble sort.                                             */
/***************************************************************************/
int_list_median(list, num)
int *list, num;
{
   int median;
   int *slist;

   /* make a working copy of the integer list */
   malloc_int(&slist, num, "int_list_median : slist");
   memcpy(slist, list, num*sizeof(int));

   median = xkthi((num>>1), slist, num);
   free(slist);
   return(median);
}
