/*
# proc: box_overlap_hori - determines if two boxes overlap horizontally.
# proc:
# proc: is_box_inside - determines if one box is inside another.
# proc:
*/

#include <defs.h>

/*************************************************************************/
box_overlap_hori(cx1, cy1, cx2, cy2, nx1, ny1, nx2, ny2)
int cx1, cy1, cx2, cy2, nx1, ny1, nx2, ny2;
{
   if(((nx1 <= cx1) && (cx1 <= nx2)) ||
      ((cx1 <= nx1) && (nx1 <= cx2)))
      return(TRUE);
   else
      return(FALSE);
}

/************************************************************/
is_box_inside(bix, biy, bix2, biy2, box, boy, box2, boy2)
int bix, biy, bix2, biy2, box, boy, box2, boy2;
{
   if((bix >= box) && (bix2 <= box2) &&
      (biy >= boy) && (biy2 <= boy2))
      return(TRUE);
   else
      return(FALSE);
}
