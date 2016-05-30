/*
# proc: kl_premult - does necessary premultiplication ahead of the KL_transform
# proc:              call. Gives efficiency.
# proc: kl_transform - calculate the KL transform of 32x32 binary images
# proc:
*/

#include <stdio.h>
#include <stdlib.h>

#define ES2 	1024

/* performs the KL transform of binary 32 x 32 mis images  	*/
/* from +1,-1 binary images a mean vector is subtracted 	*/
/* the result is projected onto an eigenvector set  		*/
/* the code is a little more cryptic than would be the case for */
/* for grey level images since large efficiency gains are 	*/
/* available for binary data 					*/

/* speed is data dependent - faster for fewer true ink pixels 	*/
/* and faster for processing many mis files since some 		*/
/* pre processing is done to the eigenvector and mean 		*/

/* The KL transform is v = \Psi^{T} (u - \mu) 			*/
/*                       = \Psi^{T} u  - \Psi^{T} \mu 		*/
/* the second product is all float point stuff 			*/
/* the first is the binary add or subtract dot product bit 	*/




/* Projects the 1 byte per pixel two level image "data" onto the KL 	*/
/* basis "evts" then subtracts the KL transform of the "mean" image  	*/
/* does this for nPats images onto a subspace basis of dimension nInps 	*/
/* the feature dimensionality should not change between calls 		*/

kl_premult(nInps, mean, evts, klmu, sume)
float   *mean, *evts, **klmu, **sume;
int     nInps;
{
float *kptr, *eptr, *mptr, *sptr;
int    j, k;

   /* do some preliminary work ahead of time and only do it once */
   /* ------------------------------------------------------------ 	*/
   /* premulptiply the mean vector by the eigenvector basis 		*/
   calloc_flt(klmu, nInps, "kl_transform : klmu");
    for ( j = 0, kptr = (*klmu), eptr = evts ; j < nInps ; j++, kptr++ )
      for ( k = 0, mptr = mean ; k <  ES2 ; k++ )
         *kptr += *mptr++  *  *eptr++;
   /* ------------------------------------------------------------ 	*/
   /* ------------------------------------------------------------ 	*/
   calloc_flt(sume, nInps, "kl_transform : sume");
    for ( j = 0, sptr = (*sume), eptr = evts ; j < nInps ; j++, sptr++ )
      for ( k = 0 ; k <  ES2 ; k++ )
         *sptr -= *eptr++;
   /* ------------------------------------------------------------ 	*/
   /* ------------------------------------------------------------ 	*/
   for ( j = 0, eptr = evts ; j < nInps ; j++ )
      for ( k = 0 ; k <  ES2 ; k++ )
         *eptr++ *= 2.0;
   /* ------------------------------------------------------------ 	*/

   transpose_rect_matrix_s(evts, ES2, nInps);
}

kl_transform(data, nPats, nInps, evts, klmu, sume, klts)
float   *evts, **klts, *klmu, *sume;
unsigned char  *data;
int     nPats, nInps;
{
float *kptr, *eptr, *mptr;
int    n, j, k;
unsigned char  *base;

   /* ---------------------------------------------------------------	*/
   /* allocate and initialize space for the KL transform result		*/
   malloc_flt(klts, nPats * nInps, "kl_transform : klts");

   n = nInps * sizeof(float);
   for ( j = 0, kptr = *klts ; j < nPats ; j++, kptr += nInps )
      memcpy(kptr, sume, n);
   /* ---------------------------------------------------------------	*/


   /* ---------------------------------------------------------------	*/
   /* all the time goes in this loop. Quite quick though 		*/
   for ( n = 0, base = data, kptr = *klts ; n < nPats ; n++, kptr += nInps )
   {
      /* it is possible, although messily so, to speed this up 		*/
      /* further by '1' terminating the image vector, thereby avoiding	*/
      /* the need to test k < ES2 on the white space skip loop. need to	*/
      /* fix up afterwards though and that is the messy bit		*/
      for ( k = 0, eptr = evts ; k < ES2 ; )
      {
         /* skip white */
         for (  ; k < ES2 && !*base ; base++, k++, eptr += nInps );

         for (  ; k < ES2 &&  *base ; base++, k++ )
            for ( j = 0 ; j < nInps ; j++ )
               kptr[j] += *eptr++;
      }
   }
   /* --------------------------------------------------------------- */


   /* --------------------------------------------------------------- */
   /* finally subtract the transformed mean from all the projections  */
   for ( n = 0, kptr = *klts ; n < nPats ; n++ )
      for ( j = 0, mptr = klmu ; j < nInps ; j++ )
         *kptr++ -= *mptr++;
   /* --------------------------------------------------------------- */
}
