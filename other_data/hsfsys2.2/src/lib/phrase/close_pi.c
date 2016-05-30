/*
# proc: get_closest_pi_list_to_right - determines which list of phrase indices
# proc:                        is closest to the right of another specified
# proc:                        list of phrase indices.
# proc: get_closest_pi_list_to_left - determines which list of phrase indices
# proc:                        is closest to the left of another specified
# proc:                        list of phrase indices.
*/

#include <ihead.h>

/***************************************************************************/
get_closest_pi_list_to_right(cur_list, cur_len, pi_lists, pi_lens,
                             mxs, mys, n, found, nfound)
int *cur_list, cur_len, *pi_lists[], *pi_lens;
int *mxs, *mys, n;
int *found, nfound;
{
   int i, closest, curlast, *pi_list;
   float mindist, dist, distance();

   if(nfound == 0)
      return(-1);
   closest = found[0];
   curlast = cur_len - 1;
   pi_list = pi_lists[found[0]];
   mindist = distance((float)mxs[cur_list[curlast]],
                      (float)mys[cur_list[curlast]],
                      (float)mxs[pi_list[0]],
                      (float)mys[pi_list[0]]);
   for(i = 1; i < nfound; i++){
      pi_list = pi_lists[found[i]];
      dist = distance((float)mxs[cur_list[curlast]],
                      (float)mys[cur_list[curlast]],
                      (float)mxs[pi_list[0]],
                      (float)mys[pi_list[0]]);
      if(dist < mindist){
         closest = found[i];
         mindist = dist;
      }
   }
   return(closest);
}

/***************************************************************************/
get_closest_pi_list_to_left(cur_list, cur_len, pi_lists, pi_lens,
                             mxs, mys, n, found, nfound)
int *cur_list, cur_len, *pi_lists[], *pi_lens;
int *mxs, *mys, n;
int *found, nfound;
{
   int i, closest, plast, *pi_list;
   float mindist, dist, distance();

   if(nfound == 0)
      return(-1);
   closest = found[0];
   pi_list = pi_lists[found[0]];
   plast = pi_lens[found[0]] - 1;
   mindist = distance((float)mxs[cur_list[0]], 
                      (float)mys[cur_list[0]],
                      (float)mxs[pi_list[plast]],
                      (float)mys[pi_list[plast]]);
   for(i = 1; i < nfound; i++){
      pi_list = pi_lists[found[i]];
      plast = pi_lens[found[i]] - 1;
      dist = distance((float)mxs[cur_list[0]], 
                      (float)mys[cur_list[0]],
                      (float)mxs[pi_list[plast]],
                      (float)mys[pi_list[plast]]);
      if(dist < mindist){
         closest = found[i];
         mindist = dist;
      }
   }
   return(closest);
}
