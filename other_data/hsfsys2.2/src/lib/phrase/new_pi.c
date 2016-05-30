/*
# proc: new_pi_list - allocates and initializes a new list of phrase indices,
# proc:               storing the top-leftmost blob's index into it.
# proc: new_pi_list_Rel2 - allocates and initializes a new list of phrase indices,
# proc:               storing the top-leftmost blob's index into it using dynamic
# proc:               allocation.
*/

#include <stdio.h>
#include <defs.h>
#include <phrase.h>
#include <maxlist.h>

/*************************************************************************/
new_pi_list(pi, px, py, prevdelta, prevflag,
            pi_lists, pi_lens, nphrases, pis, mxs, mys, end, abm, a1, b1,
            abegin, alimit, ylimit)
int *pi_lists[], *pi_lens;
int *nphrases;
float *prevdelta;
int *prevflag;
int *pi, *px, *py, *pis, *mxs, *mys, *end;
float abm, a1, b1, abegin, alimit, ylimit;
{
   int tl, left_i, dist;
   int *pi_list;

   find_top_left(&tl, mxs, mys, (*end));
   look_left(&left_i, &dist, tl, mxs, mys, (*end),
             abm, a1, b1, abegin, alimit, ylimit);
   if(left_i == -1)
      left_i = tl;
   malloc_int(&pi_list, MAX_INDEX, "new_pi_list : pi_list");
   pi_list[0] = pis[left_i];

   (*px) = mxs[left_i];
   (*py) = mys[left_i];
   (*pi) = left_i;
   *(prevdelta) = 0.0;
   *(prevflag) = FALSE;
   remove_item(left_i, pis, mxs, mys, end);

   pi_lists[*nphrases] = pi_list;
   pi_lens[*nphrases] = 1;
   (*nphrases)++;
}

/*************************************************************************/
new_pi_list_Rel2(pi, px, py, prevdelta, prevflag,
            pi_lists, pi_lens, pi_alens, nphrases, aphrases,
            pis, mxs, mys, end, abm, a1, b1, abegin, alimit, ylimit)
int ***pi_lists, **pi_lens, **pi_alens;
int *nphrases, *aphrases;
float *prevdelta;
int *prevflag;
int *pi, *px, *py, *pis, *mxs, *mys, *end;
float abm, a1, b1, abegin, alimit, ylimit;
{
   int tl, left_i, dist;
   int *pi_list, pi_alen;

   find_top_left(&tl, mxs, mys, (*end));
   look_left(&left_i, &dist, tl, mxs, mys, (*end),
             abm, a1, b1, abegin, alimit, ylimit);
   if(left_i == -1)
      left_i = tl;

   pi_alen = PHRASE_LEN_CHUNKS;
   malloc_int(&pi_list, pi_alen, "new_pi_list_Rel2 : pi_list");
   pi_list[0] = pis[left_i];

   (*px) = mxs[left_i];
   (*py) = mys[left_i];
   (*pi) = left_i;
   *(prevdelta) = 0.0;
   *(prevflag) = FALSE;
   remove_item(left_i, pis, mxs, mys, end);

   if(*nphrases >= *aphrases){
      *aphrases += PHRASE_NUM_CHUNKS;
      realloc_dbl_int_l1(pi_lists, *aphrases, "new_pi_list_Rel2 : pi_lists");
      realloc_int(pi_lens, *aphrases, "new_pi_list_Rel2 : pi_lens");
      realloc_int(pi_alens, *aphrases, "new_pi_list_Rel2 : pi_alens");
   }
   (*pi_lists)[*nphrases] = pi_list;
   (*pi_alens)[*nphrases] = pi_alen;
   (*pi_lens)[*nphrases] = 1;
   (*nphrases)++;
}
