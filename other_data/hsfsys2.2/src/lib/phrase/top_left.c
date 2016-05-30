/*
# proc: find_top_left - determines the top-leftmost blob of all the blobs not
# proc:                 yet assigned any phrase structure.
*/

#include <stats.h>

find_top_left(tl, xs, ys, end)
int *tl, *xs, *ys, end;
{
   int i, d, min_dist;

   (*tl) = 0;
   min_dist = diff_sq_distance(0, 0, xs[0], ys[0]);

   for(i = 1; i < end; i++){
      d = diff_sq_distance(0, 0, xs[i], ys[i]);
      if(d < min_dist){
         (*tl) = i;
         min_dist = d;
      }
   }
}
