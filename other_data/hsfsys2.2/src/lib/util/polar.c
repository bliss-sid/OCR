/*
# proc: to_rel_polar2 - converts a list of cartesian points indirectly 
# proc:                 referenced through an index list to polar coordinates
# proc:                 relative to a specified center point.
*/

#include <math.h>

/***************************************************************************/
to_rel_polar2(lr, lt, px, py, lx, ly, end, inbox, inbox_i)
float *lr, *lt;
int px, py, *lx, *ly, end, *inbox, inbox_i;
{
   float x, y, t;
   int i;

   for(i = 0; i < inbox_i; i++){
      x = (float)(px - lx[inbox[i]]);
      y = (float)(py - ly[inbox[i]]);
      lr[i] = (float)sqrt((double)((x*x)+(y*y)));
      if(x != 0.0)
         t = (float)atan2((double)(y), (double)(x));
      else{
         if(ly[inbox[i]] > py)
            t = M_PI/2.0;
         else
            t = M_PI/-2.0;
      }
      if(t > (M_PI/2.0))
         lt[i] = t - M_PI;
      else{
         if(t < (M_PI/-2.0))
            lt[i] = t + M_PI;
         else
            lt[i] = t;
      }
   }      
}
