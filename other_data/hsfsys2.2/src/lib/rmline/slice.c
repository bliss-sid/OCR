/*
# proc: slice_widths_hori_nb - traverses a predefined set of horizontal line points and
# proc:              locates the top and bottom of the intersecting vertical run of
# proc:              black pixels (slice).
# proc: slice_width_hori - locates the top and bottom of a vertical run of contiguously
# proc:                    black pixels that overlap (or are very close) to the specified
# proc:                    starting point.
# proc: erase_slices_hori_ml - erases vertical slices along a predefined set of horizontal
# proc:              line points based on a vector of locally dynamic median line widths.
# proc: erase_slice_hori - erases the pixels along a specified vertical slice.
# proc:
# proc: draw_slice_hori - writes a given 8-bit pixel value along a specified vertical slice.
# proc:
# proc: erase_fuzzy_slices_hori_ml - erases vertical slices along a predefined set of
# proc:              of horizontal line points based on "fuzzy" line conditions.
*/

#include <stdio.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* slice_widths_hori - traverses the line spanning two specified endpoints */
/* and locates the top and bottom of the intersecting (or very close)      */
/* vertical run of black pixels (called a slice). A slice is located and   */
/* its statistics stored (width, top, and bottom) at each point along the  */
/* line. These slices are used to get line width statistics and to         */
/* identify places where charcter data intersect with the line.            */
/***************************************************************************/
/***************************************************************************/
slice_widths_hori_nb(sw, fsy, tsy, nsw, ptsx, ptsy, npts, cdata, w, h)
int **sw, **fsy, **tsy, *nsw, w, h;
int *ptsx, *ptsy, npts;
unsigned char *cdata;
{
   int i;
   int tsw, nfsy, ntsy, j;

   /* the resulting line cannot be longer than the width of the image */
   /* however, the line can be shorter than the width of the image    */
   /* if it is sufficiently sloped to intersect with the top and / or */
   /* bottom of the image, rather than the sides.                     */
   if(npts > w)
      fatalerr("slice_widths_hori_nb",
               "line projection longer than image width", NULL); 
   /* allocate memory for slice statistics the width of the image */
   calloc_int(sw, w, "slice_widths_hori_nb : sw");
   calloc_int(fsy, w, "slice_widths_hori_nb : fsy");
   calloc_int(tsy, w, "slice_widths_hori_nb : tsy");
   /* for each point along the line, locate and compute slice statistics. */
   for(i = 0, j = ptsx[0]; i < npts; i++, j++){
      slice_width_hori(&tsw, &nfsy, &ntsy, ptsx[i], ptsy[i], cdata, w, h);
      /* the slices statistics are stored according to their column raster location */
      /* in the image, NOT according to their index along the interpolated line.    */
      (*sw)[j] = tsw;
      (*fsy)[j] = nfsy;
      (*tsy)[j] = ntsy;
   }
   /* this way the number of slices will always equal the width of the image. */
   /* locations along the line that fall off the image have slice widths of 0 */
   /* which (by convention) will be ignored and flagged as being erased       */
   *nsw = w;
}

/***************************************************************************/
/* slice_width_hori - locates top and bottom of a vertical run of conti-   */
/* guously black pixels that either overlap (or are very close) to the     */
/* specified starting point.                                               */
/***************************************************************************/
slice_width_hori(sw, fsy, tsy, x, y, cdata, w, h)
int *sw, *fsy, *tsy;
int x, y, w, h;
unsigned char *cdata;
{
   unsigned char *sptr;
   int cy, sy, s1, n1, max_y;
   int nfound, ndist, sfound, sdist;

   sy = y;
   sptr = cdata + (sy*w) + x;
   /* if starting search on a white pixel ... */
   if(*sptr == 0){

      /* this part is necessary when there is sufficient warping of */
      /* the line in the image that it does not hold true to the    */
      /* detected theoretical line that is perfectly straight in    */
      /* raster space with line width of exactly one pixel. this    */
      /* deviation can be introduced by printing, page, and scanner */
      /* annomalies. this occurs more frequently along lines of     */
      /* longer length where the likihood of deviations and their   */
      /* propogation is increase.                                   */
 
      /* look south and north for a black pixel */
      /* was a black pixel found to the south? */
      if((s1 = find_pix_south(BLACK_PIX, x, y, cdata, w, h)) == NOT_FOUND)
         sfound = FALSE;
      else
         sfound = TRUE;

      /* was a black pixel found to the north? */
      if((n1 = find_pix_north(BLACK_PIX, x, y, cdata, w, h)) == NOT_FOUND)
         nfound = FALSE;
      else
         nfound = TRUE;

      /* if black not found in both directions */
      if(!nfound && !sfound){
         *fsy = y;
         *tsy = y;
         *sw = 0;
         return;
      }
      /* if north found, but not south, use north */
      else if(nfound && !sfound)
         sy = n1;
      /* if south found, but not north, use south */
      else if(!nfound && sfound)
         sy = s1;
      /* if black found in both directions ... */
      else{
         ndist = y - n1;
         sdist = s1 - y;
         /* if distance to north and south points equal ... */
         if(ndist == sdist){
            /* if point on line is closer to top then use north */
            if(y < (h - y))
               sy = n1;
            /* if point on line is closer to bottom then use south */
            else
               sy = s1;
         }
         /* otherwise choose the closer black pixel */
         else if(ndist < sdist)
            sy = n1;
         else
            sy = s1;
      }

      /* if distance to new y is too big ... */
      if(abs(y - sy) > SMALL_NOISE){
         *fsy = y;
         *tsy = y;
         *sw = 0;
         return;
      }
   }

   max_y = h - 1;

   /* at this point, sy should be pointing to a black pixel */
   /* find first white pixel to the north */
   if((cy = find_pix_north(WHITE_PIX, x, sy, cdata, w, h)) == NOT_FOUND)
      *fsy = 0;
   else
      /* if white pixel found, then move back one pixel to black */
      *fsy = min(cy+1, max_y);

   /* find first white pixel to the south */
   if((cy = find_pix_south(WHITE_PIX, x, sy, cdata, w, h)) == NOT_FOUND)
      *tsy = max_y;
   else
      /* if white pixel found, then move back one pixel to black */
      *tsy = max(0, cy-1);

   /* compute the slice width */
   *sw = (*tsy) - (*fsy) + 1;
}

/***************************************************************************/
/* erase_slices_hori - erases vetical slices along a horizontal line       */
/* based on a specified threshold.                                         */
/***************************************************************************/
/***************************************************************************/
erase_slices_hori_ml(ebin, msws, sw, fsy, tsy, nsw, cdata, w, h)
int **ebin, *msws, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int i;

   /* allocate a logical vector for keeping track of which slices */
   /* have been erased */
   calloc_int(ebin, nsw, "erase_slices_hori_ml : ebin");
   for(i = 0; i < nsw; i++){
      /* if slice width is <= threshold, then erase the slice */
      if(sw[i] <= msws[i]+SMALLER_NOISE)
         erase_slice_hori((*ebin), sw, i, fsy[i], tsy[i], cdata, w, h);
   }
}

/***************************************************************************/
/* erase_slice_hori - erases the pixels along a specified vetical slice.   */
/***************************************************************************/
erase_slice_hori(ebin, sw, x, fsy, tsy, cdata, w, h)
int *ebin, *sw, x, fsy, tsy, w, h;
unsigned char *cdata;
{
   if(sw[x] != 0)
   /* fill in the vertical slice with zeros */
      draw_slice_hori(0, x, fsy, tsy, cdata, w, h);
   ebin[x] = ERASED;
}

/***************************************************************************/
/* draw_slice_hori - writes a given pixel value along a specified vertical */
/* slice.                                                                  */
/***************************************************************************/
draw_slice_hori(pix, x, fsy, tsy, cdata, w, h)
int pix, x, fsy, tsy, w, h;
unsigned char *cdata;
{
   int y, max_y;
   unsigned char *cptr;

   if(!(is_in_range(pix, 0, 255)))
      fatalerr("draw_slice_hori", "pixel values must be [0..255]", NULL);
   max_y = h - 1;
   if((!is_in_range(x, 0, w-1)) ||
      (!is_in_range(fsy, 0, max_y)) ||
      (!is_in_range(tsy, 0, max_y)))
      fatalerr("draw_slice_hori", "coordinates exceed image dimensions", NULL);
   cptr = cdata + (fsy*w) + x;
   for(y = fsy; y <= tsy; y++){
      *cptr = pix;
      cptr += w;
   }
}

/***************************************************************************/
/* erase_fuzzy_slices_hori - erases vetical slices along a horizontal line */
/* based on fuzzy line conditions to account for scanner jitter and line   */
/* width variations, all the while preserving pieces of intersecting       */
/* characters.                                                             */
/***************************************************************************/
/***************************************************************************/
erase_fuzzy_slices_hori_ml(ebin, smsw, msws, sw, fsy, tsy, nsw, cdata, w, h)
int *ebin, smsw, *msws, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int i, j, rs, omsw;
   int slimit, sw1_limit, sw2_limit, longer_run;
   int turned_on, from_erase;
   int min_sw, max_sw;

   my_trace("entered erase_fuzzy_slices_hori_ml\n");
   /* softer limit based on median line width */
   sw1_limit = step_limit(smsw);
   /* harder limit based on median line width */
   sw2_limit = step_limit2(smsw);
   /* longer limit on the number of slices to be erased in a single run */
   longer_run = SLICE_FCTR*smsw;
   omsw = smsw;

   /* flag to determine if current run of slices are to be erased */
   turned_on = FALSE;
   /* flag to determine if the current run of slices to be erased */
   /* began from a point of erasure */
   from_erase = TRUE;
   /* initialize to the softer limit */
   slimit = sw1_limit;
   i = 0;
   /* search the list of line slices ... */
   while(i < nsw){
      /* if median slice width changes, then update thresholds */
      if(omsw != msws[i]){
         /* softer limit based on median line width */
         sw1_limit = step_limit(msws[i]);
         /* harder limit based on median line width */
         sw2_limit = step_limit2(msws[i]);
         /* longer limit on the number of slices to be erased in a single run */
         longer_run = SLICE_FCTR*msws[i];
         omsw = msws[i];
      }
      /* if the current slice has been erased ... */
      if (ebin[i] == ERASED){
         /* if there is a run of slices to be potentially erased ... */
         if(turned_on){
            /* if the run begins and ends at points of erasure and not from char pieces ... */
            if((from_erase) ||
               /* OR the run is sufficiently long to get some line statistics (history) */
               ((i - rs) > SLICES_FOR_LINE)){
               my_trace("hit point of erasure\n");
               my_trace2("erasing slices from %d to %d\n", rs, i-1);
               /* erase slices from rs to i-1 */
               for(j = rs; j < i; j++)
                  erase_slice_hori(ebin, sw, j, fsy[j], tsy[j], cdata, w, h);
            }
            /* turn the erase flag to off */
            turned_on = FALSE;
         }
         /* else there is nothing to erase, so continue to the next slice */
      }
      /* if the current slice is unerased and too long to be a part of the line ... */
      else if (sw[i] >= slimit){
         /* if there is a run of slices to be potentially erased ... */
         if(turned_on){
            /* don't erase the run unless VERY sure: */
            /* if the run did not start from a point of erasure, but from a char peice ... */
            if((!from_erase) &&
               /* AND if the run is more sufficiently long ... */
               ((i - rs) > longer_run) &&
               /* AND if variation of run's slices shows only a small amount of jitter ... */
               ((max_sw - min_sw) <= SMALLER_NOISE)){
               /* then erase slices from rs to i-1 */
               my_trace("hit point where slice is too big\n");
               my_trace2("erasing slices from %d to %d\n", rs, i-1);
               for(j = rs; j < i; j++)
                  erase_slice_hori(ebin, sw, j, fsy[j], tsy[j], cdata, w, h);
            }
            /* turn the erase flag to off */
            turned_on = FALSE;
         }
         /* there is nothing to erase, so continue to the next slice */
      }
      /* if the current slice is unerased and short enough to be a part of the line ... */
      else {
         /* if a run of slices to be erased is not in process, then initialize a run */
         if(!turned_on){
            /* keep track of min and max slice widths in the current run */
            min_sw = sw[i];
            max_sw = sw[i];
            /* turn the erase flag on */
            turned_on = TRUE;
            /* mark the start of the run */
            rs = i;
            /* treat the first line slice a starting from a point of erasure */
            if((i == 0) ||
               /* OR if starting the run from a point of erasure ... */
               ((i > 0) && (ebin[i-1] == ERASED))){
               /* set flag to TURE */
               from_erase = TRUE;
               /* use the softer limit */
               slimit = sw1_limit;
            }
            /* if starting the run from a piece of character ... */
            else{
               /* set flag to FALSE */
               from_erase = FALSE;
               /* use the harder limit */ 
               slimit = sw2_limit;
            }
         }
         /* if the current slice is short enough and a run is in process ... */
         else{
            /* update run's slice statistics */
            if(min_sw > sw[i])
               min_sw = sw[i];
            if(max_sw < sw[i])
               max_sw = sw[i];
            /* if the run fluctuates too much ... */
            if((max_sw - min_sw) > SMALL_NOISE){
               my_trace("hit point where slice variation is too big\n");
               my_trace2("skipping run from %d to %d\n", rs, i-1);
               /* then don't erase slices to this point and */
               /* restart the run from the current slice */
               min_sw = sw[i];
               max_sw = sw[i];
               rs = i;
               /* restarting the run from a piece of character, so ... */
               /* set flag to FALSE */
               from_erase = FALSE;
               /* use the harder limit */ 
               slimit = sw2_limit;
            }
         }
      }
      /* next slice */
      i++;
   }
   my_trace("finished erase_fuzzy_slices_hori_ml\n");
}
