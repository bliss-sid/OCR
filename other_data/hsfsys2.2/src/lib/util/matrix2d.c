/*
# proc: max_2d_addr - locates the maximum integer in a 2d matrix of specified
# proc:               dimensions.
*/

#include <values.h>
#include <defs.h>

/***************************************************************************/
/* max_2d_addr - locates the maximum within a 2d matrix of integers,       */
/* returning the maximum value and its location within the matrix.         */
/***************************************************************************/
max_2d_addr(mat_2d, w, h, max_v, mx, my)
int *mat_2d, w, h;
int *max_v, *mx, *my;
{
   int x, y;
   int *mptr;

   mptr = mat_2d;
   *max_v = -MAXINT;
   *mx = NOT_FOUND;
   *my = NOT_FOUND;

   for(y = 0; y < h; y++){
      for(x = 0; x < w; x++){
         if(*mptr > *max_v){
            *mx = x;
            *my = y;
            *max_v = *mptr;
         }
         mptr++;
      }
   }
}
