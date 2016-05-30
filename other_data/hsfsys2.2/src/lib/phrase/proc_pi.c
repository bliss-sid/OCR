/*
# proc: process_nearest_pi - adds the next blob to the current phrase index
# proc:                      list and adjusts the bubble location statistics.
# proc: process_nearest_pi_Rel2 - adds the next blob to the current phrase index
# proc:                      list and adjusts the bubble location statistics
# proc:                      using dynamic allocation.
*/

#include <math.h>
#include <defs.h>
#include <phrase.h>

/*************************************************************/
process_nearest_pi(near_i, px, py, prevx, prevy, prevflag,
        pi_lists, pi_lens, nphrases, hmean, meanfactor, slopethresh,
        pis, mxs, mys, end)
int *pi_lists[], *pi_lens;
int nphrases;
int near_i, *px, *py, *prevx, *prevy, *prevflag, *pis, *mxs, *mys, *end;
float hmean, meanfactor, slopethresh;
{
   float curdelta, delta, slope, compute_slope();
   int *pi_list, dst_i;

   dst_i = nphrases-1;
   pi_list = pi_lists[dst_i];
   pi_list[pi_lens[dst_i]] = pis[near_i];
   pi_lens[dst_i]++;

   curdelta = (float)((*py) - mys[near_i]);
   delta = (float)fabs(curdelta);
   slope = (float)fabs(compute_slope((float)*px, (float)*py,
                         (float)mxs[near_i], (float)mys[near_i]));
   if(within_mean_slope(delta, slope, hmean, meanfactor, slopethresh)){
      *px = mxs[near_i];
      *py = mys[near_i];
      *prevx = -1;
      *prevy = -1;
      *prevflag = FALSE;
   }
   else{
      *prevx = mxs[near_i];
      *prevy = mys[near_i];
      *prevflag = TRUE;
   }
   remove_item(near_i, pis, mxs, mys, end);
}

/*************************************************************/
process_nearest_pi_Rel2(near_i, px, py, prevx, prevy, prevflag,
        pi_lists, pi_lens, pi_alens, nphrases, aphrases,
        hmean, meanfactor, slopethresh, pis, mxs, mys, end)
int ***pi_lists, **pi_lens, **pi_alens;
int *nphrases, *aphrases;
int near_i, *px, *py, *prevx, *prevy, *prevflag, *pis, *mxs, *mys, *end;
float hmean, meanfactor, slopethresh;
{
   float curdelta, delta, slope, compute_slope();
   int dst_i;

   dst_i = (*nphrases)-1;
   if((*pi_lens)[dst_i] >= (*pi_alens)[dst_i]){
      (*pi_alens)[dst_i] += PHRASE_LEN_CHUNKS;
      realloc_int(&((*pi_lists)[dst_i]), (*pi_alens)[dst_i],
                  "process_nearest_pi_Rel2 : pi_list");
   }
   ((*pi_lists)[dst_i])[(*pi_lens)[dst_i]] = pis[near_i];
   (*pi_lens)[dst_i]++;

   curdelta = (float)((*py) - mys[near_i]);
   delta = (float)fabs(curdelta);
   slope = (float)fabs(compute_slope((float)*px, (float)*py,
                         (float)mxs[near_i], (float)mys[near_i]));
   if(within_mean_slope(delta, slope, hmean, meanfactor, slopethresh)){
      *px = mxs[near_i];
      *py = mys[near_i];
      *prevx = -1;
      *prevy = -1;
      *prevflag = FALSE;
   }
   else{
      *prevx = mxs[near_i];
      *prevy = mys[near_i];
      *prevflag = TRUE;
   }
   remove_item(near_i, pis, mxs, mys, end);
}
