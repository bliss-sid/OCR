/*
# proc: nearest_nbor - returns the location of the point from a list of points
# proc:                that is closest to another specified point and returns
# proc:                the closest distance.
# proc: find_closest_pts - locates the closest pair of points between two
# proc:                    specified lists of points.
*/

#include <stdio.h>
#include <values.h>

/***********************************************************************/
/* nearest_nbor nearest neighbor routine */
/* Darrin L. Dimmick 5-21-93 */
/* Returns the point from esnx and esny which is closest to point_x, point_y */
/* in nearest_x, nearest_y */
/* esnc is the number of values in ensx. It is also the number of values */
/* in esny */  

int nearest_nbor(nindex, ndistance, point_x, point_y, esnx, esny, esnc)
int *nindex, *ndistance, point_x, point_y, *esnx, *esny, esnc;
{
  int i, success, ni, nd, dist;

  ni = -1;
  nd = MAXINT;
  for(i = 0; i < esnc; i++){
     dist = diff_sq_distance(point_x, point_y, esnx[i], esny[i]);
     if (dist < nd){
         ni = i;
         nd = dist;
     } /* if */
   } /* for */
  *nindex = ni;
  *ndistance  = nd;
  if (ni < 0)
     success = 0;
  else
     success = 1;
  return(success);
}

/*******************************************************************/
find_closest_pts(dcx, dcy, dci, ucx, ucy, uci, dx, dy, dn, ux, uy, un)
int *dcx, *dcy, *dci, *ucx, *ucy, *uci;
int *dx, *dy, dn;
int *ux, *uy, un;
{
   int di, dist, ui, mindist, min_ui, min_di;

   mindist = MAXINT;
   for(di = 0; di < dn; di++){
      if(!nearest_nbor(&ui, &dist, dx[di], dy[di], ux, uy, un))
         fatalerr("find_closest_pts", "nearest nbor not found", NULL);
      if(dist < mindist){
         mindist = dist;
         min_ui = ui;
         min_di = di;
      }
   }
   *dcx = dx[min_di];
   *dcy = dy[min_di];
   *dci = min_di;
   *ucx = ux[min_ui];
   *ucy = uy[min_ui];
   *uci = min_ui;
}
