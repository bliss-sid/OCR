/*
# proc: within_mean_slope - heuristic used in constreader, tests both a
# proc:                     distance within a factor of the given mean and
# proc:                     a slope within a given tolerance.
*/

#include <defs.h>

within_mean_slope(delta, slope, mean, mfactor, sthresh)
float delta, slope, mean, mfactor, sthresh;
{
   if((delta > (mean * mfactor)) && (slope > sthresh))
      return(FALSE);
   else
      return(TRUE);
}
