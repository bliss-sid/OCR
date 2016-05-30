/*
# proc: sort_pi_lists_on_y - sorts lists of phrase indices based on the
# proc:                      y coordinate of their first blob's center in
# proc:                      ascending order.
# proc: sort_pi_list_on_x - sorts the elements of a list of phrase indices
# proc:                     based on the x coordinate of each blob's center in
# proc:                     ascending order.
# proc: sort_pi_lists_by_length - sorts lists of phrase indices based on the
# proc:                           number of elements (blobs) in each list in
# proc:                           decending order.
*/

#include <stdio.h>
#include <maxlist.h>

/***************************************************************************/
sort_pi_lists_on_y(pi_lists, pi_lens, nphrases, mys, n)
int *pi_lists[], *pi_lens;
int nphrases, *mys, n;
{
   int i;
   int **tlists, *tlens, *pi_mys, *pi_list;
   int *spis, *sort_index_on_int();

   malloc_int(&pi_mys, nphrases, "sort_pi_list_on_y : pi_mys");
   malloc_dbl_int_l1(&tlists, nphrases, "sort_pi_lists_on_y : tlists");
   malloc_int(&tlens, nphrases, "sort_pi_lists_on_y : tlens");

   memcpy(tlens, pi_lens, nphrases * sizeof(int));
   for(i = 0; i < nphrases; i++){
      pi_list = pi_lists[i];
      tlists[i] = pi_list;
      pi_mys[i] = mys[pi_list[0]];
   }

   spis = sort_index_on_int(pi_mys, nphrases);
   for(i = 0; i < nphrases; i++){
      pi_lists[i] = tlists[spis[i]];
      pi_lens[i] = tlens[spis[i]];
   }

   free(spis);
   free(tlists);
   free(tlens);
   free(pi_mys);
}

/***************************************************************************/
sort_pi_list_on_x(pi_list, pi_len, mxs, n)
int *pi_list, pi_len;
int *mxs, n;
{
   int i, *spis, *sort_index_on_int();
   int *tlist, *pi_mxs;

   malloc_int(&pi_mxs, pi_len, "sort_pi_list_on_x : pi_mxs");
   for(i = 0; i < pi_len; i++)
      pi_mxs[i] = mxs[pi_list[i]];

   malloc_int(&tlist, pi_len, "sort_pi_list_on_x : tlist");
   memcpy(tlist, pi_list, pi_len * sizeof(int));

   spis = sort_index_on_int(pi_mxs, pi_len);

   for(i = 0; i < pi_len; i++){
      pi_list[i] = tlist[spis[i]];
   }
   free(spis);
   free(pi_mxs);
   free(tlist);
}

/***************************************************************************/
sort_pi_lists_by_length(pi_lists, pi_lens, nphrases)
int *pi_lists[], *pi_lens;
int nphrases;
{
   int i;
   int **tlists, *tlens, *pi_list;
   int *spis, *sort_index_dec_on_int();

   malloc_int(&tlens, nphrases, "sort_pi_lists_by_length : tlens");
   memcpy(tlens, pi_lens, nphrases * sizeof(int));

   malloc_dbl_int_l1(&tlists, nphrases,  "sort_pi_lists_by_length : tlists");
   for(i = 0; i < nphrases; i++){
      tlists[i] = pi_lists[i];
   }

   spis = sort_index_dec_on_int(pi_lens, nphrases);
   for(i = 0; i < nphrases; i++){
      pi_lists[i] = tlists[spis[i]];
      pi_lens[i] = tlens[spis[i]];
   }

   free(spis);
   free(tlists);
   free(tlens);
}
