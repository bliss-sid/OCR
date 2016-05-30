/*
# proc: remove_from_pi_lists - removes the specified list of phrase indices
# proc:                        from the given lists of indices.
*/

#include <stdio.h>

/***************************************************************************/
remove_from_pi_lists(i, pi_lists, pi_lens, end)
int i, end;
int *pi_lists[], *pi_lens;
{
   int j;

   if(i >= end)
      fatalerr("remove_from_pi_lists", "index off end of lists", NULL);
   free(pi_lists[i]);
   for(j = i+1; j < end; j++){
      pi_lists[j-1] = pi_lists[j];
      pi_lens[j-1] = pi_lens[j];
   }
}
