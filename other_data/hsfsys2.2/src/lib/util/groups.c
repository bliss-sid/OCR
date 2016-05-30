/*
# proc: int_groups - determines the number of contiguous groupings of
# proc:              characters belonging to a given set within another string.
*/

int_groups(alignment, alen, set)
int *alignment, alen;
char *set;
{
   int i, count;

   count = 0;
   i = 0;
   while(i < alen){
      while((i < alen) && (!is_in_int_set(alignment[i], set)))
         i++;
      if(i < alen)
         count++;
      while((i < alen) && (is_in_int_set(alignment[i], set)))
         i++;
   }
   return(count);
}
