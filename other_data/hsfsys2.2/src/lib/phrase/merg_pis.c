/*
# proc: merge_pi_lists - collapses lists of phrase indices into complete lines
# proc:                  of text by applying two merge heuristics controlled
# proc:                  by the provided parameters.
# proc: merge_pi_pair - appends the contents of two lists of phrase indices,
# proc:                 removes the merged list from the input lists, and
# proc:                 sorts the appended list's members on blob center x's.
# proc: merge_pi_lists_Rel2 - collapses lists of phrase indices into complete
# proc:                  lines of text by applying two merge heuristics controlled
# proc:                  by the provided parameters using dymanic allocation.
# proc: merge_pi_pair_Rel2 - appends the contents of two lists of phrase indices,
# proc:      removes the merged list from the input lists, and sorts the appended
# proc:      list's members on blob center x's using dynamic allocation.
*/

#include <stdio.h>
#include <defs.h>
#include <phrase.h>
#include <maxlist.h>

/***************************************************************************/
merge_pi_lists(pi_lists, pi_lens, nphrases, mxs, mys, n, hmean,
              mrg1factor, mrg2factor, runfactor)
int *pi_lists[], *pi_lens;
int *nphrases, *mxs, *mys, n;
float hmean, mrg1factor, mrg2factor, runfactor;
{
   int phrases_merged, i, j;
   int *cur_list, cur_len;
   float m, b;
   int *found, nfound, closest;

   malloc_int(&found, *nphrases, "merge_pi_lists : found");
   do{
      phrases_merged = FALSE;
      sort_pi_lists_by_length(pi_lists, pi_lens, (*nphrases));
      for(i = 0; i < (*nphrases); i++){
         cur_list = pi_lists[i];
         cur_len = pi_lens[i];
         nfound = 0;
         for(j = i+1; j < (*nphrases); j++){
            if(pi_list_to_right(cur_list, cur_len, pi_lists[j], pi_lens[j],
                                mxs, mys, n, hmean, mrg1factor))
               found[nfound++] = j;
         }
         if(nfound != 0){
            closest = get_closest_pi_list_to_right(cur_list, cur_len,
                               pi_lists, pi_lens, mxs, mys, n, found, nfound);
            merge_pi_pair(pi_lists, pi_lens, i, closest, nphrases,
                           mxs, mys, n);
            phrases_merged = TRUE;
            break;
         }
         nfound = 0;
         for(j = i+1; j < (*nphrases); j++){
            if(pi_list_to_right(pi_lists[j], pi_lens[j], cur_list, cur_len,
                                mxs, mys, n, hmean, mrg1factor))
               found[nfound++] = j;
         }
         if(nfound != 0){
            closest = get_closest_pi_list_to_left(cur_list, cur_len,
                               pi_lists, pi_lens, mxs, mys, n, found, nfound);
            merge_pi_pair(pi_lists, pi_lens, i, closest, nphrases,
                           mxs, mys, n);
            phrases_merged = TRUE;
            break;
         }
         if(cur_len > 1){
            lsq_ind_line_params(&m, &b, cur_list, cur_len, mxs, mys, n);
            for(j = i+1; j < (*nphrases); j++){
               if(pi_list_along_line(m, b, pi_lists[j], pi_lens[j],
                                mxs, mys, n, hmean, mrg2factor, runfactor)){
                  merge_pi_pair(pi_lists, pi_lens, i, j, nphrases,
                                 mxs, mys, n);
                  phrases_merged = TRUE;
                  break;
               }
            }
         }
         if(phrases_merged == TRUE)
            break;
      }
   }while(phrases_merged);
   free(found);
}

/***************************************************************************/
merge_pi_pair(pi_lists, pi_lens, to_i, from_i, nphrases, mxs, mys, n)
int *pi_lists[], *pi_lens;
int to_i, from_i, *nphrases;
int *mxs, *mys, n;
{
   int *from, *to;

   from = pi_lists[from_i];
   to = pi_lists[to_i];

   append_pi_list(to, &(pi_lens[to_i]), from, pi_lens[from_i]);
   remove_from_pi_lists(from_i, pi_lists, pi_lens, (*nphrases));
   (*nphrases)--;
   sort_pi_list_on_x(to, pi_lens[to_i], mxs, n);
}

/***************************************************************************/
merge_pi_lists_Rel2(pi_lists, pi_lens, nphrases, mxs, mys, n, hmean,
              mrg1factor, mrg2factor, runfactor)
int **pi_lists, *pi_lens;
int *nphrases, *mxs, *mys, n;
float hmean, mrg1factor, mrg2factor, runfactor;
{
   int phrases_merged, i, j;
   int *cur_list, cur_len;
   float m, b;
   int *found, nfound, closest;

   malloc_int(&found, *nphrases, "merge_pi_lists : found");
   do{
      phrases_merged = FALSE;
      sort_pi_lists_by_length(pi_lists, pi_lens, (*nphrases));
      for(i = 0; i < (*nphrases); i++){
         cur_list = pi_lists[i];
         cur_len = pi_lens[i];
         nfound = 0;
         for(j = i+1; j < (*nphrases); j++){
            if(pi_list_to_right(cur_list, cur_len, pi_lists[j], pi_lens[j],
                                mxs, mys, n, hmean, mrg1factor))
               found[nfound++] = j;
         }
         if(nfound != 0){
            closest = get_closest_pi_list_to_right(cur_list, cur_len,
                               pi_lists, pi_lens, mxs, mys, n, found, nfound);
            merge_pi_pair_Rel2(pi_lists, pi_lens, i, closest, nphrases,
                           mxs, mys, n);
            phrases_merged = TRUE;
            break;
         }
         nfound = 0;
         for(j = i+1; j < (*nphrases); j++){
            if(pi_list_to_right(pi_lists[j], pi_lens[j], cur_list, cur_len,
                                mxs, mys, n, hmean, mrg1factor))
               found[nfound++] = j;
         }
         if(nfound != 0){
            closest = get_closest_pi_list_to_left(cur_list, cur_len,
                               pi_lists, pi_lens, mxs, mys, n, found, nfound);
            merge_pi_pair_Rel2(pi_lists, pi_lens, i, closest, nphrases,
                           mxs, mys, n);
            phrases_merged = TRUE;
            break;
         }
         if(cur_len > 1){
            lsq_ind_line_params(&m, &b, cur_list, cur_len, mxs, mys, n);
            for(j = i+1; j < (*nphrases); j++){
               if(pi_list_along_line(m, b, pi_lists[j], pi_lens[j],
                                mxs, mys, n, hmean, mrg2factor, runfactor)){
                  merge_pi_pair_Rel2(pi_lists, pi_lens, i, j, nphrases,
                                 mxs, mys, n);
                  phrases_merged = TRUE;
                  break;
               }
            }
         }
         if(phrases_merged == TRUE)
            break;
      }
   }while(phrases_merged);
   free(found);
}

/***************************************************************************/
merge_pi_pair_Rel2(pi_lists, pi_lens, to_i, from_i, nphrases, mxs, mys, n)
int **pi_lists, *pi_lens;
int to_i, from_i, *nphrases;
int *mxs, *mys, n;
{
   append_pi_list_Rel2(&(pi_lists[to_i]), &(pi_lens[to_i]),
                   pi_lists[from_i], pi_lens[from_i]);
   remove_from_pi_lists(from_i, pi_lists, pi_lens, (*nphrases));
   (*nphrases)--;
   sort_pi_list_on_x(pi_lists[to_i], pi_lens[to_i], mxs, n);
}
