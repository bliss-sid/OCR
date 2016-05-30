/*
# proc: max_overlap_run_along_x - analyzes a system of horizontal runs and
# proc:                assigns a vector of y values accoriding to the maximum
# proc:                length run at each x posistion.
# proc: init_markers - initializes markers to a specifiec x position to keep
# proc:                track of processing within the system of horizontal runs.
*/

#include <defs.h>

#define DONE -1
/***************************************************************************/
max_overlap_run_along_x(ptsx, ptsy, npts, sx, ex, rx, ry, rlen, nruns, w, h)
int **ptsx, **ptsy, *npts;
int sx, ex;
int *rx, *ry, *rlen, nruns;
int w, h;
{
   int ri, xlen, px, py;
   int maxri, maxrlen;
   int *rx2;
   int *rankyx;
   int *sort_index_on_int_2ii();
   int *markers, test_length;
   int mark;

   /* sort runs on y, then x */
   rankyx = sort_index_on_int_2ii(ry, rx, nruns);

   xlen = ex-sx+1;
   malloc_int(ptsx, xlen, "max_overlap_run_along_x: ptsx");
   malloc_int(ptsy, xlen, "max_overlap_run_along_x: ptsy");
   *npts = 0;

   /* compute run endpoints x-coordinate up front */
   malloc_int(&rx2, nruns, "max_overlap_run_along_x: rx2");
   for(ri = 0; ri < nruns; ri++)
      rx2[ri] = rx[rankyx[ri]] + rlen[rankyx[ri]];

   /* set up markers to point to most relevant runs on each scanline */
   malloc_int(&markers, h, "max_overlap_run_along_x: markers");
   init_markers(sx, markers, h, rx, rx2, ry, rankyx, nruns);

   for(px = sx; px <= ex; px++){
      /* reset max overlapping run for current x */
      maxri = NOT_FOUND;
      maxrlen = -1;
      /* for each scanline ... */
      for(py = 0; py < h; py++){
         mark = markers[py];
         /* if marker set to DONE OR x left of marked run, then continue */
         if((mark != DONE) && (px >= rx[rankyx[mark]])){
            test_length = FALSE;
            /* if x right of marked run, then advance marker */
            if(px >= rx2[rankyx[mark]]){
               (markers[py])++;
               mark = markers[py];
               /* if no more runs on current scanline ... */
               if((mark >= nruns) || (ry[rankyx[mark]] != py))
                  markers[py] = DONE;
               else{
               /* otherwise if marked run overlaps with current x ... */
                  if(rx[rankyx[mark]] == px)
                     /* turn test flag ON */
                     test_length = TRUE;
               }
            }
            /* otherwise marked run intersects with current x */
            else
               /* turn test flag ON */
               test_length = TRUE;
            if(test_length){
               /* test for max length run */
               if(rlen[rankyx[mark]] > maxrlen){
                  /* store current run */
                  maxri = mark;
                  maxrlen = rlen[rankyx[mark]];
               }
            }
         }
      }
      if(maxri != NOT_FOUND){
         (*ptsx)[*npts] = px;
         (*ptsy)[(*npts)++] = ry[rankyx[maxri]];
      }
   }
   free(rankyx);
   free(rx2);
   free(markers);
}

/***************************************************************************/
init_markers(sx, markers, nmarks, rx, rx2, ry, rankyx, nruns)
int sx, *markers, nmarks;
int *rx, *rx2, *ry, *rankyx, nruns;
{
   int i, cy, ri;

   /* by default all scanlines are marked DONE */
   for(i = 0; i < nmarks; i++)
      markers[i] = DONE;

   ri = 0;
   while(ri < nruns){
      /* get current y of runs */
      cy = ry[rankyx[ri]];
      /* mark first run to overlap with sx or first run to the right */
      while((ri < nruns) && (ry[rankyx[ri]] == cy) && (sx > rx2[rankyx[ri]]))
         ri++;
      /* if no more runs, then quit */
      if(ri >= nruns)
         break;
      /* if current run is on same scanline ... */
      if(ry[rankyx[ri]] == cy)
         markers[cy] = ri;
      /* skip to next scanline */
      while((ri < nruns) && (ry[rankyx[ri]] == cy))
         ri++;
   }
}
