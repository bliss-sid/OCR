/*
# proc: ramp_int_runs - takes a list of integers, thresholds them into
# proc:                 binary values and assigns incrementally accumulated
# proc:                 run length vales into a new list.
*/

#include <stdio.h>

ramp_int_runs(bins, rlist, blen, thresh)
int *bins, **rlist;
int blen, thresh;
{
   int i, count;

   calloc_int(rlist, blen, "ramp_int_runs : rlist");

   count = 0;
   for(i = 0; i < blen; i++){
      if(bins[i] > thresh)
         (*rlist)[i] = ++count;
      else
         count = 0;
   }
}
