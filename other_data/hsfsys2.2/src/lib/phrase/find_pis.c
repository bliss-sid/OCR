/*
# proc: find_pi_lists - given a list of segmented character centers, sorts the
# proc:                 blob indices into phrases according to proper reading
# proc:                 order.
# proc: find_pi_lists_Rel2 - given a list of segmented character centers, sorts
# proc:                 the blob indices into phrases according to proper reading
# proc:                 order using dynamic allocation.
*/

#include <stdio.h>
#include <phrase.h>

/***************************************************************************/
find_pi_lists(pi_lists, pi_lens, nphrases,
             hmean, prevmeanfactor, meanfactor, slopethresh,
             omxs, omys, all, pa1, b1, pa2, b2,  abegin, alimit)
int *pi_lists[], *pi_lens;
int *nphrases, *omxs, *omys, all;
float hmean, prevmeanfactor, meanfactor, slopethresh, abegin, alimit;
float pa1, b1, pa2, b2;
{
   int i;
   float succlimit;
   float curdelta, a1, a2, prevdelta;
   float abm, ylimit, compute_slope(), find_bubble_max_y();
   int near_i, dist, px, py, pi, end;
   int prevx, prevy, prevflag;
   float na, nb;
   int *mxs, *mys, *pis;

   end = all;

   malloc_int(&mxs, all, "find_pi_lists : mxs");
   malloc_int(&mys, all, "find_pi_lists : mys");
   malloc_int(&pis, all, "find_pi_lists : pis");
   memcpy(mxs, omxs, all*sizeof(int));
   memcpy(mys, omys, all*sizeof(int));
   for(i = 0; i < all; i++){
      pis[i] = i;
   }

   succlimit = prevmeanfactor * hmean;
   a1 = pa1 * hmean;
   a2 = pa2 * hmean;
   abm = compute_slope(a1, b1, a2, b2);

   ylimit = find_bubble_max_y(alimit, a1, b1, abm);

   new_pi_list(&pi, &px, &py, &prevdelta, &prevflag,
               pi_lists, pi_lens, nphrases, pis, mxs, mys, &end,
               abm, a1, b1, abegin, alimit, ylimit);
   all--;

   while (all > 0){
      near_i = -1;
      nearest_nbor_in_right_bubble(&near_i, &dist, &na, &nb,
                       px, py, mxs, mys, end, abm, a1, b1,
                       abegin, alimit, ylimit);
      if(near_i == -1){
         new_pi_list(&pi, &px, &py, &prevdelta, &prevflag, 
                     pi_lists, pi_lens, nphrases, pis, mxs, mys, &end,
                     abm, a1, b1, abegin, alimit, ylimit);
         all--;
      }
      else{
         if(prevflag){
            curdelta = (float)(prevy - mys[near_i]);
            current_pi_list(na, nb, near_i, curdelta,
                  &px, &py, &prevx, &prevy,
                  &prevdelta, &prevflag, succlimit,
                  pi_lists, pi_lens, nphrases,
                  hmean, meanfactor, slopethresh,
                  pis, mxs, mys, &end,
                  abm, a1, b1, abegin, alimit, ylimit);
            all--;
         }
         else{
            curdelta = (float)(py - mys[near_i]);
            current_pi_list(na, nb, near_i, curdelta,
                  &px, &py, &prevx, &prevy,
                  &prevdelta, &prevflag, succlimit,
                  pi_lists, pi_lens, nphrases,
                  hmean, meanfactor, slopethresh,
                  pis, mxs, mys, &end,
                  abm, a1, b1, abegin, alimit, ylimit);
            all--;
         }
      }

   }
   free(mxs);
   free(mys);
   free(pis);
}

/***************************************************************************/
find_pi_lists_Rel2(pi_lists, pi_lens, pi_alens, nphrases, aphrases,
               problems, nprob, aprob, hmean,
               prevmeanfactor, meanfactor, slopethresh,
               omxs, omys, all, pa1, b1, pa2, b2,  abegin, alimit)
int ***pi_lists, **pi_lens, **pi_alens, **problems;
int *nphrases, *aphrases, *nprob, *aprob;
int *omxs, *omys, all;
float hmean, prevmeanfactor, meanfactor, slopethresh, abegin, alimit;
float pa1, b1, pa2, b2;
{
   int i, j, k;
   float succlimit;
   float curdelta, a1, a2, prevdelta;
   float abm, ylimit, compute_slope(), find_bubble_max_y();
   int near_i, dist, px, py, pi, end;
   int prevx, prevy, prevflag;
   float na, nb;
   int *mxs, *mys, *pis;

   end = all;

   malloc_int(&mxs, all, "find_pi_lists_Rel2 : mxs");
   memcpy(mxs, omxs, all*sizeof(int));
   malloc_int(&mys, all, "find_pi_lists_Rel2 : mys");
   memcpy(mys, omys, all*sizeof(int));
   malloc_int(&pis, all, "find_pi_lists_Rel2 : pis");

   for(i = 0; i < all; i++){
      pis[i] = i;
   }

   succlimit = prevmeanfactor * hmean;
   a1 = pa1 * hmean;
   a2 = pa2 * hmean;
   abm = compute_slope(a1, b1, a2, b2);

   ylimit = find_bubble_max_y(alimit, a1, b1, abm);

   new_pi_list_Rel2(&pi, &px, &py, &prevdelta, &prevflag,
               pi_lists, pi_lens, pi_alens, nphrases, aphrases,
               pis, mxs, mys, &end, abm, a1, b1, abegin, alimit, ylimit);

   while (end > 0){
      near_i = -1;
      nearest_nbor_in_right_bubble(&near_i, &dist, &na, &nb,
                       px, py, mxs, mys, end, abm, a1, b1,
                       abegin, alimit, ylimit);
      if(near_i == -1){
         new_pi_list_Rel2(&pi, &px, &py, &prevdelta, &prevflag, 
                     pi_lists, pi_lens, pi_alens, nphrases, aphrases,
                     pis, mxs, mys, &end, abm, a1, b1, abegin, alimit, ylimit);
      }
      else{
         if(prevflag){
            curdelta = (float)(prevy - mys[near_i]);
            current_pi_list_Rel2(na, nb, near_i, curdelta,
                  &px, &py, &prevx, &prevy,
                  &prevdelta, &prevflag, succlimit,
                  pi_lists, pi_lens, pi_alens, nphrases, aphrases,
                  problems, nprob, aprob,
                  hmean, meanfactor, slopethresh,
                  pis, mxs, mys, omys, &end,
                  abm, a1, b1, abegin, alimit, ylimit);
         }
         else{
            curdelta = (float)(py - mys[near_i]);
            current_pi_list_Rel2(na, nb, near_i, curdelta,
                  &px, &py, &prevx, &prevy,
                  &prevdelta, &prevflag, succlimit,
                  pi_lists, pi_lens, pi_alens, nphrases, aphrases,
                  problems, nprob, aprob,
                  hmean, meanfactor, slopethresh,
                  pis, mxs, mys, omys, &end,
                  abm, a1, b1, abegin, alimit, ylimit);
         }
      }

   }

   /* make sure points are in increasing order on mxs */
   for(i = 0; i < *nphrases; i++){
      for(j = 0, k = 1; j < (*pi_lens)[i]-1; j++, k++){
         if(omxs[((*pi_lists)[i])[k]] < omxs[((*pi_lists)[i])[j]]){
            sort_pi_list_on_x((*pi_lists)[i], (*pi_lens)[i], omxs, (*pi_lens)[i]);
         }
      }
   }

   free(mxs);
   free(mys);
   free(pis);
}
