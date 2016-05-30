/*
# proc: current_pi_list - determines if the next blob should be added to the
# proc:                   current phrase index list or be added to a new
# proc:                   phrase index list.
# proc: current_pi_list_Rel2 - determines if the next blob should be added to the
# proc:                   current phrase index list or be added to a new
# proc:                   phrase index list using dynamic allocation.
*/

#include <math.h>
#include <defs.h>
#include <phrase.h>

/***************************************************************************/
current_pi_list(na, nb, near_i, curdelta, px, py, 
               prevx, prevy, prevdelta, prevflag, succlimit, 
               pi_lists, pi_lens, nphrases,
               hmean, meanfactor, slopethresh,
               pis, mxs, mys, end, abm, a1, b1, abegin, alimit, ylimit)
float na, nb;
int *pi_lists[], *pi_lens;
int *nphrases;
int near_i, *px, *py, *prevx, *prevy, *prevflag, *pis, *mxs, *mys, *end;
float curdelta, *prevdelta, succlimit, abm, a1, b1;
float hmean, meanfactor, slopethresh, abegin, alimit, ylimit;
{
   int pi;

   if(too_big((*prevdelta), curdelta, succlimit)){
      new_pi_list(&pi, px, py, prevdelta, prevflag,
                  pi_lists, pi_lens, nphrases,
                  pis, mxs, mys, end, abm, a1, b1,
                  abegin, alimit, ylimit);
      (*prevdelta) = 0.0;
      (*prevflag) = FALSE;
   }
   else{
      process_nearest_pi(near_i, px, py, prevx, prevy, prevflag,
                         pi_lists, pi_lens, (*nphrases),
                         hmean, meanfactor, slopethresh,
                         pis, mxs, mys, end);
      (*prevdelta) = curdelta;
   }
}

/***************************************************************************/
current_pi_list_Rel2(na, nb, near_i, curdelta, px, py, 
               prevx, prevy, prevdelta, prevflag, succlimit, 
               pi_lists, pi_lens, pi_alens, nphrases, aphrases,
               problems, nprob, aprob,
               hmean, meanfactor, slopethresh,
               pis, mxs, mys, omys, end, abm, a1, b1, abegin, alimit, ylimit)
float na, nb;
int ***pi_lists, **pi_lens, **pi_alens, **problems;
int *nphrases, *aphrases, *nprob, *aprob;
int near_i, *px, *py, *prevx, *prevy, *prevflag, *pis, *mxs, *mys, *omys, *end;
float curdelta, *prevdelta, succlimit, abm, a1, b1;
float hmean, meanfactor, slopethresh, abegin, alimit, ylimit;
{
   int pi, lastp, delta;

   if((fabs(curdelta) > hmean) || too_big((*prevdelta), curdelta, succlimit)){
      /* add current point problem list */
      if((*nprob) >= (*aprob)){
         (*aprob) += PHRASE_LEN_CHUNKS;
         realloc_int(problems, *aprob, "current_pi_list_Rel2 : problems");
      }
      (*problems)[*nprob] = pis[near_i];
      (*nprob)++;
      remove_item(near_i, pis, mxs, mys, end);

      if(*end > 0){
         new_pi_list_Rel2(&pi, px, py, prevdelta, prevflag,
                     pi_lists, pi_lens, pi_alens, nphrases, aphrases,
                     pis, mxs, mys, end, abm, a1, b1, abegin, alimit, ylimit);
         (*prevdelta) = 0.0;
         (*prevflag) = FALSE;
      }
   }
   else{
      process_nearest_pi_Rel2(near_i, px, py, prevx, prevy, prevflag,
                         pi_lists, pi_lens, pi_alens, nphrases, aphrases,
                         hmean, meanfactor, slopethresh, pis, mxs, mys, end);
      (*prevdelta) = curdelta;
   }
}
