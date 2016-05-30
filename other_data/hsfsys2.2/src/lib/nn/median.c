/*
# proc: featsclassmedian - make the by-class median vectors of
# proc:                    a feature set.
*/ 

#include <stdio.h>
#include <malloc.h>
#include <math.h>

void featsclassmedian(feats, class, nPats, nInps, nClasses, meds)
float *feats, **meds;
int   *class, nPats, nInps, nClasses;
{
int   i, j, k, n;
float *vals, *m, *f;
extern float xkthf();

   malloc_flt(&vals, nPats, "featclassmedian vals");
   malloc_flt( meds, nClasses * nInps, "featclassmedian meds");

   for ( k = 0, m = *meds ; k < nClasses ; k++ )
      for ( i = 0 ; i < nInps ; i++ )
      {
         for ( j = n = 0, f = feats ; j < nPats ; j++, f += nInps )
            if (class[j] == k)
               vals[n++] = f[i];

         *m++ = (n > 0) ? xkthf((n-1)/2, vals, n) : 0.0;
      }

   free(vals);
}
