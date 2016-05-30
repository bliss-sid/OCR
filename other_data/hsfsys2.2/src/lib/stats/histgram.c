/*
# proc: int_histogram - takes a list of integers and returns a list of
# proc:                 histogram bins (old).
*/

#include <stdio.h>

int_histogram(bins, n_bins, intlist, listlen)
int **bins, n_bins;
int *intlist, listlen;
{
   int i;

   calloc_int(bins, n_bins, "int_histogram : bins");
   for(i = 0; i < listlen; i++)
      (*bins)[intlist[i]]++;
}
