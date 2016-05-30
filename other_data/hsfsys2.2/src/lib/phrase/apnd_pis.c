/*
# proc: append_pi_list - appends the contents of the second list of phrase
# proc:                  indices to the members of the first phrase index list.
# proc: append_pi_list_Rel2 - appends the contents of the second list of phrase
# proc:                   indices to the members of the first phrase index
# proc:                   list using dynamic allocation.
*/

#include <stdio.h>
#include <phrase.h>
#include <maxlist.h>

/***************************************************************************/
append_pi_list(to, to_len, from, from_len)
int *to, *to_len, *from, from_len;
{
   int newnum, cplen;

   newnum = (*to_len) + from_len;
   if(newnum > MAX_INDEX)
      fatalerr("append_pi_list", "pi_list copy larger than MAX_INDEX", NULL);
   cplen = from_len * sizeof(int);
   memcpy(&(to[(*to_len)]), from, cplen);
   *to_len = newnum;
}

/***************************************************************************/
append_pi_list_Rel2(to, to_len, from, from_len)
int **to, *to_len, *from, from_len;
{
   int newnum, *nto;

   newnum = (*to_len) + from_len;
   malloc_int(&nto, newnum, "append_pi_list_Rel2 : nto");
   memcpy(nto, *to, (*to_len)*sizeof(int));
   memcpy(&(nto[(*to_len)]), from, from_len * sizeof(int));
   free(*to);
   *to = nto;
   *to_len = newnum;
}

