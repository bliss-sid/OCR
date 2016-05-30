/*
# proc: bres_line_alloc - same as bres_line except if nalloc == 0 allocates new
# proc:                   memory, or reallocs if nalloc is too small.
*/


#include <stdio.h>
#include <math.h>
#include <defs.h>

/***************************************************************************/
/* At call nalloc is how much memory is currently allocated for x_list and */
/* y_list.  If nalloc == 0 routine will allocate needed memory based on:   */
/*                                                                         */
/*         asize = max(abs(x2-x1)+2,abs(y2-y1)+2))                         */
/*                                                                         */
/* If nalloc < asize will realloc memory for x_list and y_list to asize.   */
/***************************************************************************/
bres_line_alloc(x1,y1,x2,y2,x_list,y_list,num,nalloc)
int x1,y1,x2,y2,**x_list,**y_list,*num,*nalloc;
{
   int x, y, i, sign;
   int dx, dy, dy2, dx2, dymdx2;
   int x_end, y_end;
   int x_incr, y_incr;
   int p;
   int *xptr, *yptr;
   int asize;

   asize = max(abs(x2-x1)+2, abs(y2-y1)+2);
   if(*nalloc == 0) {
      *nalloc = asize;
      malloc_int(x_list, *nalloc, "bres_line_alloc x_list");
      malloc_int(y_list, *nalloc, "bres_line_alloc y_list");
   }
   else if(*nalloc < asize) {
      *nalloc = asize;
      realloc_int(x_list, *nalloc, "bres_line_alloc x_list");
      realloc_int(y_list, *nalloc, "bres_line_alloc y_list");
   }


   i = 0;
   dx = abs(x1 - x2);
   dy = abs(y1 - y2);
   dx2 = dx<<1;
   dy2 = dy<<1;
   dymdx2 = (dy - dx)<<1;
   p = dy2 - dx;

   xptr = *x_list;
   yptr = *y_list;
   if(dx == 0){
      x = x1;
      y = y1;
      y_end = y2;
      xptr[i] = x;
      yptr[i++] = y;
      if(y1 > y2){
         while(y > y_end){
            xptr[i] = x;
            yptr[i++] = --y;
         }
      }
      else{
         while(y < y_end){
            xptr[i] = x;
            yptr[i++] = ++y;
         }
      }
      *num = i;
      return;
   }
   if(dy == 0){
      x = x1;
      y = y1;
      x_end = x2;
      xptr[i] = x;
      yptr[i++] = y;
      if(x1 > x2){
         while(x > x_end){
            xptr[i] = --x;
            yptr[i++] = y;
         }
      }
      else{
         while(x < x_end){
            xptr[i] = ++x;
            yptr[i++] = y;
         }
      }
      *num = i;
      return;
   }

   if(dx > dy){
      if(y1 < y2)
         y_incr = 1;
      else
         y_incr = -1;

      x = x1;
      y = y1;
      x_end = x2;
      xptr[i] = x;
      yptr[i++] = y;

      if(x1 <= x2) {
         while(x < x_end){
            ++x;
            if(p < 0)
               p = p + dy2;
            else{
               y += y_incr;
               p = p + dymdx2;
            }
            xptr[i] = x;
            yptr[i++] = y;
         }
         if(x != x2 || y != y2) {
            xptr[i] = x2;
            yptr[i++] = y2;
         }
      }
      else {
         while(x > x_end){
            --x;
            if(p < 0)
               p = p + dy2;
            else{
               y += y_incr;
               p = p + dymdx2;
            }
            xptr[i] = x;
            yptr[i++] = y;
         }
         if(x != x2 || y != y2) {
            xptr[i] = x2;
            yptr[i++] = y2;
         }
      }
      *num = i;
      return;
   }
   else {
      if(x1 < x2)
         x_incr = 1;
      else
         x_incr = -1;

      x = x1;
      y = y1;
      y_end = y2;
      xptr[i] = x;
      yptr[i++] = y;

      if(y1 <= y2) {
         while(y < y_end){
            ++y;
            if(p > 0)
               p = p - dx2;
            else{
               x += x_incr;
               p = p + dymdx2;
            }
            xptr[i] = x;
            yptr[i++] = y;
         }
         if(x != x2 || y != y2) {
            xptr[i] = x2;
            yptr[i++] = y2;
         }
      }
      else {
         while(y > y_end){
            --y;
            if(p > 0)
               p = p - dx2;
            else{
               x += x_incr;
               p = p + dymdx2;
            }
            xptr[i] = x;
            yptr[i++] = y;
         }

         if(x != x2 || y != y2) {
            xptr[i] = x2;
            yptr[i++] = y2;
         }
      }
      *num = i;
      return;
   }
}

