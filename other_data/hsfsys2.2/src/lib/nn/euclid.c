/*
# proc: squared_euclid_dist - calculate the euclidean distances of many
# proc:                       unknown feature vectors to many known ones
# proc: one_squared_euclid_dist - calculate the euclidean distances of one
# proc:                       unknown feature vectors to many known ones
*/

#include <stdio.h>

/* for U unknown examples produce the squared euclidean distance 	*/
/* to each of the K known prototype examples 				*/
/* the floating vector "dists" should be regarded as a row major U*K 	*/
/* matrix (U rows, K cols) such that the (u,k) entry is the distance 	*/
/* of the uth unknown to the kth prototype 				*/
/* enough space must be pointed to by dists to hold U*K floats 		*/


squared_euclid_dist(known, nPats_known, found, nPats_found, nInps, dists)
float *known, *found, *dists;
int   nPats_known, nPats_found, nInps;
{
float diff, dist, *dptr, *fptr, *mptr, *kptr;
int i, j, k;
 
   for (i = 0, dptr=dists, fptr = found ; i < nPats_found ; i++, fptr += nInps)
      for (j = 0, kptr = known ; j < nPats_known ; j++ )
      {
         for (k = 0, mptr = fptr, dist = 0.0 ; k < nInps ; k++)
            diff  = *mptr++ - *kptr++,
            dist += diff * diff;
         *dptr++ = dist;
      }
}

one_squared_euclid_dist(known, nPats_known, found, nInps, dists)
float *known, *found, *dists;
int   nPats_known, nInps;
{
    squared_euclid_dist(known, nPats_known, found, 1, nInps, dists);
}
