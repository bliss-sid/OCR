/*
# proc: dx_line_alloc - interpolates a line between two endpoints by incrementing
# proc:                 x by single units and computing a resulting y.
*/

#include <defs.h>

/***************************************************************************/
/* At call nalloc is how much memory is currently allocated for x_list and */
/* y_list.  If nalloc == 0 routine will allocate needed memory based on:   */
/*                                                                         */
/*         asize = abs(x2-x1)+2                                            */
/*                                                                         */
/* If nalloc < asize will realloc memory for x_list and y_list to asize.   */
/***************************************************************************/
dx_line_alloc(x1,y1,x2,y2,x_list,y_list,num,nalloc)
int x1,y1,x2,y2,**x_list,**y_list,*num,*nalloc;
{
   int x, y, i;
   int dx, dy;
   int x_end;
   int x_incr;
   float slope, fy, fy_incr;
   int *xptr, *yptr;
   int asize;

   asize = abs(x2-x1)+2;
   if(*nalloc == 0) {
      *nalloc = asize;
      malloc_int(x_list, *nalloc, "dx_line_alloc x_list");
      malloc_int(y_list, *nalloc, "dx_line_alloc y_list");
   }
   else if(*nalloc < asize) {
      *nalloc = asize;
      realloc_int(x_list, *nalloc, "dx_line_alloc x_list");
      realloc_int(y_list, *nalloc, "dx_line_alloc y_list");
   }

   i = 0;
   dx = (x1 - x2);
   dy = (y1 - y2);
   xptr = *x_list;
   yptr = *y_list;

   /* if line is vertical, then store midpoint of line */
   if(dx == 0){
      x = x1;
      y = y1;
      xptr[i] = x1;
      yptr[i++] = sround((y1+y2)/2.0);
      *num = i;
      return;
   }
   /* if line is horizontal, then store same y value along line */
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

   /* otherwise increment in x and use slope to update y */
   slope = dy/(float)dx;
   if(x1 < x2){
      x_incr = 1;
      fy_incr = slope;
   }
   else{
      x_incr = -1;
      fy_incr = -slope;
   }

   x = x1;
   fy = (float)y1;
   y = y1;
   x_end = x2;
   while(x <= x_end){
      xptr[i] = x;
      yptr[i++] = y;
      x += x_incr;
      fy += fy_incr;
      y = sround(fy);
   }
   *num = i;
   return;
}
