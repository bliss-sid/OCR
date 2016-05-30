/*
# proc: compute_slope - computes the slope of the line between two points.
# proc:
# proc: compute_y - given the x coordinate, slope, and another point on the
# proc:             line, computes the corresponding y coordinate.
# proc: compute_x - computes the x coordinate for a specified y coordinate,
# proc:             given a non-zero slope and a point on the line.
# proc: pt_intersect - computes the point of intersection between two
# proc:                specified lines.
# proc: intersect_sides - projects a line within a specified rectangle and
# proc:                determines the enpoints of where the line intersects the
# proc:                sides.
*/

/************************************************************/
/*         File Name: LINE.C                                */
/*         Package:   Fundamental Line Utilities            */
/*         Author:    Michael D. Garris                     */
/*         Date:      03/05/91                              */
/*                                                          */
/*         Contents:  compute_slope()                       */
/*                    compute_y()                           */
/************************************************************/
#include <stdio.h>
#include <math.h>
#include <values.h>
#include <defs.h>

/************************************************************/
/*         Routine:   compute_slope()                       */
/*                                                          */
/* Compute_slope(), given two coordinate points, returns    */
/* the slope of a line connecting the two points.           */
/* Note: This routine returns the system's MAXFLOAT for     */
/* points vertically aligned.                               */
/************************************************************/
/* x1, y1 - first coordinate point.                         */
/* x2, y2 - second coordinate point.                        */
/************************************************************/
float compute_slope(x1, y1, x2, y2)
float x1, y1, x2, y2;
{
   float f_slope, dx, dy;

   dx = (x2 - x1);
   if(dx == 0.0)
      return(MAXFLOAT);
   dy = (y2 - y1);
   f_slope = dy/dx;
   return(f_slope);
}

/************************************************************/
float compute_y(x, m, x1, y1)
float x, m, x1, y1;
{
   float y;

   y = (m * x) - (m * x1) + y1;
   return(y);
}

/***************************************************************************/
/* compute_x - computes the x coordinate for a specified y coordinate,     */
/* given a none-zero slope and a point on the line.                        */
/***************************************************************************/
float compute_x(y, m, x1, y1)
float y, m, x1, y1;
{
   float x;

   if(m == 0.0)
      fatalerr("compute_x", "slope is 0.0, so x coordinate cannot be computed",
               NULL);

   x = ((y - y1)/m) + x1;
   return(x);
}

/***************************************************************************/
/* pt_intersect - given a point and slope of two lines, computes their     */
/* point of intersection. If the lines are parallel, then the routine      */
/* retruns FALSE.                                                          */
/***************************************************************************/
pt_intersect(ix, iy, x1, y1, m1, x2, y2, m2)
int *ix, *iy;
int x1, y1, x2, y2;
float m1, m2;
{
   *ix = NOT_FOUND;
   *iy = NOT_FOUND;
   if(m1 == m2)
      return(FALSE);
   *ix = sround((y2-y1+(m1*x1)-(m2*x2))/(m1-m2));
   *iy = sround(y1 + (m1 * ((*ix) - x1)));
   return(TRUE);
}

/***************************************************************************/
/* intersect_sides - given a point and a slope for a line, projects the    */
/* line to the edge of a rectangular region and determines endpoints where */
/* the line intersects the edge of the rectangle.                          */
/***************************************************************************/
intersect_sides(x1, y1, x2, y2, x, y, slope, fw, fh)
int *x1, *y1, *x2, *y2;
int x, y, fw, fh;
float slope;
{
   float compute_y(), compute_x();
   int ty, tx, max_x, max_y;

   *x1 = NOT_FOUND;
   *y1 = NOT_FOUND;
   *x2 = NOT_FOUND;
   *y2 = NOT_FOUND;

   /* special case, if slope is exactly 0.0 */
   if(slope == 0.0){
      *x1 = 0;
      *y1 = y;
      *x2 = fw - 1;
      *y2 = y;
      return;
   }

   /* compute y coordinate with x at left edge of image */
   ty = sround(compute_y(0.0, slope, (float)x, (float)y));

   /* if y coordinate is within image dimensions ... */
   max_y = fh - 1;
   if(is_in_range(ty, 0, max_y)){
      /* store as the 1st point */
      *x1 = 0;
      *y1 = ty;
   }

   /* compute x coordinate with y at top edge of image */
   tx = sround(compute_x(0.0, slope, (float)x, (float)y));

   /* if x coordinate is within image dimensions ... */
   max_x = fw - 1;
   if(is_in_range(tx, 0, max_x)){
      /* if 1st point not yet found ... */
      if((*x1 == NOT_FOUND) || (*y1 == NOT_FOUND)){
         /* store as 1st point */
         *x1 = tx;
         *y1 = 0;
      }
      else{
         /* otherwise store as 2nd point */
         *x2 = tx;
         *y2 = 0;
      }
   }

   /* compute x coordinate with y at bottom edge of image */
   tx = sround(compute_x((float)max_y, slope, (float)x, (float)y));

   /* if x coordinate is within image dimensions ... */
   if(is_in_range(tx, 0, max_x)){
      /* if 1st point not yet found ... */
      if((*x1 == NOT_FOUND) || (*y1 == NOT_FOUND)){
         /* store as 1st point */
         *x1 = tx;
         *y1 = max_y;
      }
      else{
         /* otherwise store as 2nd point */
         *x2 = tx;
         *y2 = max_y;
      }
   }

   /* compute y coordinate with x at right edge of image */
   ty = sround(compute_y((float)max_x, slope, (float)x, (float)y));

   /* if y coordinate is within image dimensions ... */
   if(is_in_range(ty, 0, max_y)){
      /* store as 2nd point */
      *x2 = max_x;
      *y2 = ty;
   }

   /* double check to make sure points are filled in */
   if((*x1 == NOT_FOUND) || (*y1 == NOT_FOUND))
      fatalerr("intersect_sides", "first intersection point not found", NULL);
   if((*x2 == NOT_FOUND) || (*y2 == NOT_FOUND))
      fatalerr("intersect_sides", "second intersection point not found", NULL);
}
