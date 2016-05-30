/*
# proc: sort_index_dec_on_int - sorts an integer list into descending order.
# proc:
# proc: sort_index_on_int - sorts an integer list into ascending order.
# proc:
# proc: sort_index_on_int_2id - sorts two integer lists the first in ascending the
# proc:                       second in descending order using the first list as a
# proc:                       primary key and the second list as a secondary key.
# proc: sort_index_on_int_2ii - sort a double ranked list of integers in
# proc:                       ascending order returning the sorted order of the
# proc:                       ranked pairs.
*/

#include <stdio.h>
#include <multsort.h>
#include <maxlist.h>

/**********************************************************/
int *sort_index_dec_on_int(i1, num)
int *i1, num;
{
   int i, *s;

   malloc_int(&s, num, "sort_index_dec_on_int : s");
   with_1_INDEX_alloc(istruct, num, INTTYPE)
      for(i = 0; i < num; i++)
         add_to_1_INDEX(istruct, i, i1[i]);
      multisort_1_INDEX(istruct, DEC);
      i = 0;
      foreach_in_INDEX(istruct)
         s[i++] = this_item_int;
      endfor
   end_with_INDEX_alloc(istruct)
   return(s);
}

/**********************************************************/
int *sort_index_on_int(i1, num)
int *i1, num;
{
   int i, *s;

   malloc_int(&s, num, "sort_index_on_int : s");
   with_1_INDEX_alloc(istruct, num, INTTYPE)
      for(i = 0; i < num; i++)
         add_to_1_INDEX(istruct, i, i1[i]);
      multisort_1_INDEX(istruct, INC);
      i = 0;
      foreach_in_INDEX(istruct)
         s[i++] = this_item_int;
      endfor
   end_with_INDEX_alloc(istruct)
   return(s);
}

/**********************************************************/
int *sort_index_on_int_2id(i1, i2, num)
int *i1, *i2, num;
{
   int i, *s;

   malloc_int(&s, num, "sort_index_on_int_2id : s");
   with_2_INDEX_alloc(istruct, num, INTTYPE)
      for(i = 0; i < num; i++)
         add_to_2_INDEX(istruct, i, i1[i], i2[i]);
      multisort_2_INDEX(istruct, INC, DEC);
      i = 0;
      foreach_in_INDEX(istruct)
         s[i++] = this_item_int;
      endfor
   end_with_INDEX_alloc(istruct)
   return(s);
}

/**********************************************************/
int *sort_index_on_int_2ii(i1, i2, num)
int *i1, *i2, num;
{
   int i, *s;

   malloc_int(&s, num, "sort_index_on_int_2ii : s");
   with_2_INDEX_alloc(istruct, num, INTTYPE)
      for(i = 0; i < num; i++)
         add_to_2_INDEX(istruct, i, i1[i], i2[i]);
      multisort_2_INDEX(istruct, INC, INC);
      i = 0;
      foreach_in_INDEX(istruct)
         s[i++] = this_item_int;
      endfor
   end_with_INDEX_alloc(istruct)
   return(s);
}
