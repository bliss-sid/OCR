/*
# proc: fill_line_breaks - takes a sparse set of line points and extends the last point
# proc:                    to the left of each void through the break.
# proc: interpolate_line_breaks - takes a sparse set of line points and interpolates
# proc:                    across the breaks.
*/

#include <stdio.h>

/***************************************************************************/
fill_line_breaks(pts2x, pts2y, npts2, ptsx, ptsy, npts)
int **pts2x, **pts2y, *npts2;
int *ptsx, *ptsy, npts;
{
   int i, x, si, ei, ox, oy;

   /* allocate new line point lists */
   si = 0;
   ei = npts-1;
   malloc_int(pts2x, ptsx[ei] - ptsx[si] + 1, "fill_line_breaks: pts2x");
   malloc_int(pts2y, ptsx[ei] - ptsx[si] + 1, "fill_line_breaks: pts2y");

   i = si;
   *npts2 = 0;
   ox = ptsx[i];
   oy = ptsy[i++];
   (*pts2x)[*npts2] = ox;
   (*pts2y)[(*npts2)++] = oy;

   while(i <= ei){
      if((ptsx[i] - ox) > 1){
         for(x = ox+1; x < ptsx[i]; x++){
            (*pts2x)[*npts2] = x;
            (*pts2y)[(*npts2)++] = oy;
         }
      }
      ox = ptsx[i];
      oy = ptsy[i++];
      (*pts2x)[*npts2] = ox;
      (*pts2y)[(*npts2)++] = oy;
   }
}

/***************************************************************************/
interpolate_line_breaks(pts2x, pts2y, npts2, ptsx, ptsy, npts)
int **pts2x, **pts2y, *npts2;
int *ptsx, *ptsy, npts;
{
   int i, k, si, ei, ox, oy;
   int nalloc;
   int *xlist, *ylist, num;

   /* mark off initial run of pruned points */
   i = 0;
   while((i < npts) && (ptsy[i] == -1))
      i++;
   if(i == npts)
      fatalerr("interpolate_line_breaks",
               "all points on line have been pruned", NULL);
   si = i;

   /* mark off final run of pruned points */
   i = npts-1;
   while((i >= 0) && (ptsy[i] == -1))
      i--;
   if(i < 0)
      fatalerr("interpolate_line_breaks",
               "all points on line have been pruned", NULL);
   ei = i;

   /* allocate new line point lists */
   malloc_int(pts2x, ptsx[ei] - ptsx[si] + 1, "interpolate_line_breaks: pts2x");
   malloc_int(pts2y, ptsx[ei] - ptsx[si] + 1, "interpolate_line_breaks: pts2y");
   *npts2 = 0;
   nalloc = 0;

   i = si;
   *npts2 = 0;
   ox = ptsx[i];
   oy = ptsy[i++];
   (*pts2x)[*npts2] = ox;
   (*pts2y)[(*npts2)++] = oy;
   while(i <= ei){
      while((i <= ei) && (ptsy[i] == -1))
         i++;
      if(i > ei)
         break;
      if((ptsx[i] - ox) == 1){
         ox = ptsx[i];
         oy = ptsy[i++];
         (*pts2x)[*npts2] = ox;
         (*pts2y)[(*npts2)++] = oy;
      }
      else{
         dx_line_alloc(ox+1, oy, ptsx[i], ptsy[i], &xlist, &ylist, &num, &nalloc);
         for(k = 0; k < num; k++){
            (*pts2x)[*npts2] = xlist[k];
            (*pts2y)[(*npts2)++] = ylist[k];
         }
         ox = ptsx[i];
         oy = ptsy[i];
         i++;
      }
   }

   if(nalloc != 0){
      free(xlist);
      free(ylist);
   }
}
