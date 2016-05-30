/*
# proc: pi_list_to_right - merge heuristic which determines if one list of
# proc:                    phrase indices is to the right of another list.
# proc: pi_list_along_line - merge heuristic which determines if a list of
# proc:                      phrase indices lies sufficiently close to a line
# proc:                      projection defined by the given parameters.
*/

#include <ihead.h>
#include <defs.h>

/***************************************************************************/
pi_list_to_right(cur_list, cur_len, new_list, new_len, mxs, mys, n,
                 hmean, factor)
int *cur_list, cur_len, *new_list, new_len;
int *mxs, *mys, n;
float hmean, factor;
{
   int curlast;
   float hf, llimit, ulimit;

   curlast = cur_len - 1;
   if(mxs[cur_list[curlast]] > mxs[new_list[0]])
      return(FALSE);
   hf = hmean * factor;
   llimit = mys[cur_list[curlast]] - hf;
   ulimit = mys[cur_list[curlast]] + hf;
   if((mys[new_list[0]] < ulimit) &&
      (mys[new_list[0]] > llimit))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
pi_list_along_line(m, b, pi_list, pi_len, mxs, mys, n,
                   hmean, factor, runfactor)
float m, b;
int *pi_list, pi_len;
int *mxs, *mys, n;
float hmean, factor, runfactor;
{
   int i;
   float dist, perp_dist(), limit;
   int sum;

   limit = hmean * factor;
   sum = 0;
   for(i = 0; i < pi_len; i++){
      dist = perp_dist((float)mxs[pi_list[i]], (float)mys[pi_list[i]],
                        0.0, b, m);
      if(dist < limit)
         sum++;
   }
   if(sum > (pi_len * runfactor))
      return(TRUE);
   else
      return(FALSE);
}
