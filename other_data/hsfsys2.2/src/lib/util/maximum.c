/*
# proc: maxpv - returns the first position and value of maximum in a vector
# proc:         of floats.
*/

#include <stdio.h>
#include <malloc.h>
#include <values.h>


/* returns first position and value of maximum of a vector of floats */
maxpv(avec, n, maxv, maxp)
float *avec, *maxv;
int   n,     *maxp;
{
float *ptr, maxval;
int   j,    maxpos;

      ptr    = avec;
      maxval = *ptr++;
      maxpos = 0;
      for (j = 1; j < n; j++)
      {
         if (*ptr > maxval)
         {
            maxval = *ptr;
            maxpos = j;
         }   
         ptr++;
      }
      *maxv = maxval;
      *maxp = maxpos;
}
