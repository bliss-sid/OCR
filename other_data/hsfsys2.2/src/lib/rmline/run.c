/*
# proc: next_n_run - determines the start and ending poinnts of the next contiguous
# proc:              run of elements in a list all equal to the specified value.
# proc: stddev_run_widths - computes the stddev on the legnth of black and white runs
# proc:              along a given line trajectory through an 8-bit binary pixel image.
*/

#include <stdio.h>
#include <defs.h>
#include <rmline.h>

/***************************************************************************/
/* next_n_run - determines the start and ending locations of the next      */
/* contiguous run of elements in a list all equal to a specified value,    */
/* given a starting point from which to begin the search. If no run is     */
/* found the routine returns FALSE.                                        */
/***************************************************************************/
next_n_run(rn, rs, re, s, list, n)
int rn, *rs, *re, s, *list, n;
{
   int i;

   i = s;
   /* while the specified value is not found */
   while((i < n) && (list[i] != rn))
      i++;
   /* if not found ... */
   if(i == n)
      return(FALSE);
   *rs = i;
   /* while the specified value is found */
   while((i < n) && (list[i] == rn))
      i++;
   /* back up one if off the run */
   /* or even off the list */
   i--;
   *re = i;
   return(TRUE);
}

/***************************************************************************/
/* stddev_run_widths - takes a line trajectory and computes the standard   */
/* deviation on the length of all black and white runs along the line. The */
/* more "regular" the lengths of the runs, the lower the stddev's.         */
/***************************************************************************/
stddev_run_widths(bstd, wstd, bpix, lx, ly, rx, ry, x1, y1, x2, y2, cdata, w, h)
float *bstd, *wstd;
int *bpix, *lx, *ly, *rx, *ry;
int x1, y1, x2, y2;
unsigned char *cdata;
int w, h;
{
   int i, ni, li, ri, npix, borw, max_i;
   int *xlist, *ylist, num, alloc;
   float *brun, *wrun, compute_stddev();
   int brn, wrn;

   if (!valid_point(x1, y1, w, h))
      fatalerr("stddev_run_widths","Point x1, y1","Invalid"); 
   if (!valid_point(x2, y2, w, h))
      fatalerr("stddev_run_widths","Point x2, y2","Invalid"); 
   /* interpolated points between the line's endpoints */
   alloc = 0;
   bres_line_alloc(x1, y1, x2, y2, &xlist, &ylist, &num, &alloc);

   /* find left edge of the line */
   max_i = num - 1;
   if((li = find_pix_fwd_on_line(BLACK_PIX, 0, max_i, xlist, ylist, num,
                                 cdata, w, h)) == NOT_FOUND){
      /* if not black pixels, then stddev's are 0.0 */
      *lx = NOT_FOUND;
      *ly = NOT_FOUND;
      *rx = NOT_FOUND;
      *ry = NOT_FOUND;
      *bstd = 0.0;
      *wstd = 0.0;
      *bpix = 0;
      free(xlist);
      free(ylist);
      return;
   }
   else{
      /* store location of left-most black pixel */
      *lx = xlist[li];
      *ly = ylist[li];
   }

   /* find right edge of the line */
   if((ri = find_pix_bwd_on_line(BLACK_PIX, max_i, li, xlist, ylist, num,
                                 cdata, w, h)) == NOT_FOUND)
      /* should never reach here, because fwd search would already have failed */
      fatalerr("stddev_run_widths", "search backward for a black pixel failed",
               "should never reach this point");
   else{
      /* store location of right-most black pixel */
      *rx = xlist[ri];
      *ry = ylist[ri];
   }

   malloc_flt(&brun, num, "stddev_run_widths : brun");
   malloc_flt(&wrun, num, "stddev_run_widths : wrun");

   /* start at left-most black pixel */
   i = li;
   borw = TRUE;
   brn = 0;
   wrn = 0;
   *bpix = 0;
   /* while right-most black pixel not reached ... */
   while(i <= ri){
      /* if on a black run, find next white pixel */
      if(borw){
         if((ni = find_pix_fwd_on_line(WHITE_PIX, i, ri, xlist, ylist, num,
                                       cdata, w, h)) == NOT_FOUND){
            /* final black run found, store run statistics and break from loop */
            npix = ri - i + 1;
            (*bpix) += npix;
            brun[brn++] = (float)npix;
            break;
         }
         else{
            /* compute and store black run statistics */
            npix = ni - i;
            (*bpix) += npix;
            brun[brn++] = (float)npix;
            borw = FALSE;
         }
      }
      /* else on a white run ... */
      else{
         if((ni = find_pix_fwd_on_line(BLACK_PIX, i, ri, xlist, ylist, num,
                                       cdata, w, h)) == NOT_FOUND){
            /* last run found is white, then an error has occurred */
            /* because search should end on a black run */
            fatalerr("stddev_run_widths", "search forward for a black pixel failed",
                     "should never reach this point");
         }
         else{
            /* compute and store white run statistics */
            npix = ni - i;
            wrun[wrn++] = (float)npix;
            borw = TRUE;
         }
      }

      /* move search forward to the beginning of the next run */
      i = ni;
   }

   if(brn > 1)
      *bstd = compute_stddev(brun, brn);
   else
      *bstd = 0.0;
   if(wrn > 1)
      *wstd = compute_stddev(wrun, wrn);
   else
      *wstd = 0.0;

   free(xlist);
   free(ylist);
   free(brun);
   free(wrun);
}
