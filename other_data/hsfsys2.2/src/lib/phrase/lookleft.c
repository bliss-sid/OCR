/*
# proc: look_left - initializes the position of a new phrase by using the
# proc:             bubbles to look left of the specified position, attempting
# proc:             to begin the new phrase at the beginning of a line.
*/

#include <stdio.h>

/***************************************************************************/
look_left(left_i, dist, pi, lx, ly, end, m, a1, b1, abegin, alimit, ylimit)
int *left_i, *dist, pi, *lx, *ly, end;
float m, a1, b1, abegin, alimit, ylimit;
{
   int i, near_i, last_i;
   int px, py;
   int ndist, ldist;
   int *tli, *tlx, *tly, tend, cplen;

   cplen = end * sizeof(int);
   malloc_int(&tli, end, "look_left : tli");
   for(i = 0; i < end; i++)
      tli[i] = i;
   malloc_int(&tlx, end, "look_left : tlx");
   memcpy(tlx, lx, cplen);
   malloc_int(&tly, end, "look_left : tly");
   memcpy(tly, ly, cplen);
   tend = end;
   px = tlx[pi];
   py = tly[pi];
   remove_item(pi, tli, tlx, tly, &tend);

   nearest_nbor_in_left_bubble(&near_i, &ndist, px, py, tlx, tly,
                                tend, m, a1, b1, abegin, alimit, ylimit);
   last_i = -1;
   ldist = 0;
   while(near_i != -1){
      last_i = tli[near_i];
      ldist = ndist;
      px = tlx[near_i];
      py = tly[near_i];
      remove_item(near_i, tli, tlx, tly, &tend);
      nearest_nbor_in_left_bubble(&near_i, &ndist, px, py, tlx, tly,
                                   tend, m, a1, b1, abegin, alimit, ylimit);
   }
   (*left_i) = last_i;
   (*dist) = ldist;
   free(tli);
   free(tlx);
   free(tly);
}
