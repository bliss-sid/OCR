/*
# proc: unerase_short_voids_ml - refills short voids between character pices based on one
# proc:               of the pieces abutting to the void with a slice width "close" to
# proc:               the locally dynamic median line width.
# proc: unerase_curved_voids_ml - refills voids based on the intersection of slope
# proc:                trajectories using a vector of locally dynamic median line widths.
# proc: join_nbrs_on_top - refills voids based on the intersection of slope trajectories
# proc:                    measured from 2 nbring character pieces sitting on top the line.
# proc: join_nbrs_below - refills voids based on the intersection of slope trajectories
# proc:                   measured from 2 nbring character pieces touching along the bottom
# proc:                   of the line.
# proc: right_vert_clear - checks if right edge is vertically clear of character data above
# proc:                    and below.
# proc: left_vert_clear - checks if left edge is vertically clear of character data above
# proc:                   and below.
# proc: top_left_hori_clear - checks if top-left edge is horizontally clear of character data.
# proc:
# proc: top_right_hori_clear - checks if top-right edge is horizontally clear of
# proc:                        character data.
# proc:
# proc: bottom_left_hori_clear - checks if bot-left edge is horizontally clear of
# proc:                          character data.
# proc: bottom_right_hori_clear - checks if bot-right edge is horizontally clear of
# proc:                          character data.
*/

#include <stdio.h>
#include <values.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* unserase_short_voids - refills short voids between character pieces     */
/* based on one of the pieces abutting to the void with a slice width      */
/* "close" to the median line width. This routine is useful to join left   */
/* and right pices of horizontal stokes separated by a short void where    */
/* the original stroke overlapped close enough with the line to be         */
/* partially erased.                                                       */
/***************************************************************************/
/***************************************************************************/
unerase_short_voids_ml(ebin, msws, sw, fsy, tsy, nsw, cdata, w, h)
int *ebin, *msws, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int i, rs, re, nextbin;
   int limit, rlen;

   my_trace("entered unerase_short_voids_ml\n");
   nextbin = 0;
   while(next_n_run(ERASED, &rs, &re, nextbin, ebin, nsw)){
      limit = max(MIN_SHORT_VOID, sround(SHORT_VOID_FCTR*msws[rs]));
      my_trace2("next void from %d to %d\n", rs, re);
      if((rs > 0) && (re < nsw-1)){
         rlen = re - rs + 1;
         /* if void is short enough ... */
         if(rlen < limit){
            /* test nbring character pieces */
            if((sw[rs-1] <= msws[rs]+SMALL_NOISE) ||
               (sw[re+1] <= msws[re]+SMALL_NOISE)){
               my_trace("unerase void\n");
               for(i = rs; i <= re; i++){
                  if(sw[i] != 0)
                     draw_slice_hori(1, i, fsy[i], tsy[i], cdata, w, h);
                  ebin[i] = UNERASED;
               }
            }
         }
      }
      nextbin = re+1;
   }
   my_trace("finished unerase_short_voids_ml\n");
}

/***************************************************************************/
/* unerase_curved_voids - refills erased voids based on the intersection   */
/* of slope trajectories measured from two neighboring character pieces.   */
/* This routine is useful to join left and right pieces of curvatures,     */
/* such as a hole in the top or bottom of a zero.                          */
/***************************************************************************/
/***************************************************************************/
unerase_curved_voids_ml(ebin, msws, sw, fsy, tsy, nsw, cdata, w, h)
int *ebin, *msws, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int nextbin;
   int lrs, lre, rrs, rre;
   float lm, rm;
   float slope_left_above(), slope_right_above();
   float slope_left_below(), slope_right_below();
   int slimit, x, max_x;

   my_trace("entered unerase_curved_voids_ml\n");
   max_x = w-1;
   nextbin = 0;
   /* slices not erased to this point are assumed to be pieces of characters */
   /* find the left character piece */
   if(next_n_run(0, &lrs, &lre, nextbin, ebin, nsw)){
      slimit = step_limit(msws[lre]);
      nextbin = lre+1;
      /* find the next right character piece ... */
      while(next_n_run(0, &rrs, &rre, nextbin, ebin, nsw)){
         nextbin = rre+1;
         my_trace2("left char piece from %d to %d\n", lrs, lre);
         my_trace2("right char piece from %d to %d\n", rrs, rre);
         /* check if left and right edges of the void are vertically clear - */
         /* we are looking for places severed completely through a horizontal */
         /* stroke at the top or the bottom of a potentially curved character */
         if((right_vert_clear(lrs,lre,fsy,tsy,msws[lre],nsw,cdata,w,h)) &&
            (left_vert_clear(rrs,rre,fsy,tsy,msws[rrs],nsw,cdata,w,h))){
            my_trace("void has vertically clear edges\n");
            /* is only character data potentially touching the top of the line? */
            if((bottom_right_hori_clear(lrs, lre, fsy, tsy,msws[lre],nsw, cdata, w, h)) &&
               (bottom_left_hori_clear(rrs, rre, fsy, tsy,msws[rrs],nsw, cdata, w, h))){
               my_trace("character data maybe touching top of line\n");
               /* get slope from left char piece's right edge */
               x = min(lre+1, max_x);
               lm = slope_right_above(x, fsy[x], slimit, RUN_DYS, cdata, w, h);
               my_trace1("left nbr slope = %f\n", lm);
               /* get slope from right char piece's left edge */
               x = max(0, rrs-1);
               rm = slope_left_above(x, fsy[x], slimit, RUN_DYS, cdata, w, h);
               my_trace1("right nbr slope = %f\n", rm);
               /* test to join nboring char pieces based on intersection of slopes */
               join_nbrs_on_top(lm, lrs, lre, rm, rrs, rre, fsy, tsy, ebin, sw, nsw,
                                msws[lre], cdata, w, h);
            /* is only character data potentially touching the bottom of the line? */
            }else if((top_right_hori_clear(lrs,lre,fsy,tsy,msws[lre],nsw,cdata,w,h)) &&
               (top_left_hori_clear(rrs, rre, fsy, tsy, msws[rrs], nsw, cdata, w, h))){
               my_trace("character data maybe touching bottom of line\n");
               /* get slope from left char piece's right edge */
               x = min(lre+1, max_x);
               lm = slope_right_below(x, tsy[x], slimit, RUN_DYS, cdata, w, h);
               my_trace1("left nbr slope = %f\n", lm);
               /* get slope from right char piece's left edge */
               x = max(0, rrs-1);
               rm = slope_left_below(x, tsy[x], slimit, RUN_DYS, cdata, w, h);
               my_trace1("right nbr slope = %f\n", rm);
               /* test to join nboring char pieces based on intersection of slopes */
               join_nbrs_below(lm, lrs, lre, rm, rrs, rre, fsy, tsy, ebin, sw, nsw,
                               msws[lre], cdata, w, h);
            }
         }
         /* right char piece becomes left char piece */
         lrs = rrs;
         lre = rre;
      }
   }
   my_trace("finished unerase_curved_voids_ml\n");
}

/***************************************************************************/
/* join_nbrs_on_top - refills erased voids based on the intersection of    */
/* slope trajectories measured from two neighboring character pieces       */
/* sitting on top of the line.                                             */
/***************************************************************************/
join_nbrs_on_top(lm, lrs, lre, rm, rrs, rre, fsy, tsy, ebin, sw, nsw, msw, cdata, w, h)
float lm, rm;
int lrs, lre, rrs, rre;
int *fsy, *tsy, *ebin, *sw, nsw, msw;
unsigned char *cdata;
int w, h;
{
   int i, ix, iy, ty, by;
   int lx, rx, limit;

   my_trace("entered join_nbrs_on_top\n");
   /* form the bounding box within which intersections will be accepted */
   lx = min(lre+1, nsw-1);
   rx = max(0, rrs-1);
   my_trace2("void from %d to %d\n", lx, rx);
   ty = min(fsy[lx], fsy[rx]);
   by = max(tsy[lx], tsy[rx]);

   /* set a limit on the length of the void to avoid rediculous unerasures */
   limit = max(MIN_CURVE_VOID, sround(CURVE_VOID_FCTR*msw));

   /* to be compatible: */
   /* the left nbr slope must be positive and right nbr slope must be negative */
   if((lm >= 0) && ((rm == MAXFLOAT) || (rm <= 0)) &&
      /* to account for annomalies in the measured slopes: */
      /* one side is not permitted to be "perfectly" vertical with the other */
      /* side "perfectly" horizontal */
      ((lm != MAXFLOAT) || (rm != 0.0)) &&
      ((rm != MAXFLOAT) || (lm != 0.0)) &&
      /* the length of the void must not exceed the limit */
      ((rx - lx + 1) < limit) &&
      /* an intersection between the two slopes must exist (ie. slopes not parallel) */
      (pt_intersect(&ix, &iy, lx, fsy[lx], lm, rx, fsy[rx], rm))){

      my_trace3("left line: pt = (%d, %d) slope = %f\n", lx, fsy[lx], lm);
      my_trace3("right line: pt = (%d, %d) slope = %f\n", rx, fsy[rx], rm);
      my_trace2("intersection at (%d, %d)\n", ix, iy);
      /* if intersection point is within the rectangular region of acceptance ... */
      if((is_in_range(ix, lx, rx) && (is_in_range(iy, ty, by)))){
         /* unerase the void */
         my_trace("unerase void\n");
         for(i = lx; i <= rx; i++){
            if(sw[i] != 0)
               draw_slice_hori(1, i, fsy[i], tsy[i], cdata, w, h);
            ebin[i] = UNERASED;
         }
      }
   }
   my_trace("finisned join_nbrs_on_top\n");
}

/***************************************************************************/
/* join_nbrs_below - refills erased voids based on the intersection of     */
/* slope trajectories measured from two neighboring character pieces       */
/* that touch the line from below.                                         */
/***************************************************************************/
join_nbrs_below(lm, lrs, lre, rm, rrs, rre, fsy, tsy, ebin, sw, nsw, msw, cdata, w, h)
float lm, rm;
int lrs, lre, rrs, rre;
int *fsy, *tsy, *ebin, *sw, nsw, msw;
unsigned char *cdata;
int w, h;
{
   int i, ix, iy, ty, by;
   int lx, rx, limit;

   my_trace("entered join_nbrs_below\n");
   /* form the bounding box within which intersections will be accepted */
   lx = min(lre+1, nsw-1);
   rx = max(0, rrs-1);
   my_trace2("void from %d to %d\n", lx, rx);
   ty = min(fsy[lx], fsy[rx]);
   by = max(tsy[lx], tsy[rx]);

   /* set a limit on the length of the void to avoid rediculous unerasures */
   limit = max(sround(CURVE_VOID_FCTR*msw), MIN_CURVE_VOID);

   /* to be compatible: */
   /* the left nbr slope must be negative and right nbr slope must be positive */
   if(((lm == MAXFLOAT) || (lm <= 0)) && (rm >= 0) &&
      /* to account for annomalies in the measured slopes: */
      /* one side is not permitted to be "perfectly" vertical with the other */
      /* side "perfectly" horizontal */
      ((lm != MAXFLOAT) || (rm != 0.0)) &&
      ((rm != MAXFLOAT) || (lm != 0.0)) &&
      /* the length of the void must not exceed the limit */
      ((rx - lx + 1) < limit) &&
      /* an intersection between the two slopes must exist (ie. slopes not parallel) */
      (pt_intersect(&ix, &iy, lx, tsy[lx], lm, rx, tsy[rx], rm))){

      my_trace3("left line: pt = (%d, %d) slope = %f\n", lx, fsy[lx], lm);
      my_trace3("right line: pt = (%d, %d) slope = %f\n", rx, fsy[rx], rm);
      my_trace2("intersection at (%d, %d)\n", ix, iy);
      /* if intersection point is within the rectangular region of acceptance ... */
      if((is_in_range(ix, lx, rx) && (is_in_range(iy, ty, by)))){
         /* unerase the void */
         my_trace("unerase void\n");
         for(i = lx; i <= rx; i++){
            if(sw[i] != 0)
               draw_slice_hori(1, i, fsy[i], tsy[i], cdata, w, h);
            ebin[i] = UNERASED;
         }
      }
   }
   my_trace("finished join_nbrs_below\n");
}

/***************************************************************************/
/* right_vert_clear - checks to see if right edge is vertically clear of   */
/* character data above and below.                                         */
/***************************************************************************/
right_vert_clear(rs, re, fsy, tsy, msw, nsw, cdata, w, h)
int rs, re, *fsy, *tsy, msw, nsw;
unsigned char *cdata;
int w, h;
{
   int x, sy, ey, slimit;

   slimit = step_limit(msw);
   x = re+1;
   if(x >= w)
      return(FALSE);
   sy = max(0, fsy[x] - slimit);
   ey = min(h-1, tsy[x] + slimit);
   if(sub_column_eq(0, x, sy, ey, cdata, w, h))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
/* left_vert_clear - checks to see if left edge is vertically clear of     */
/* character data above and below.                                         */
/***************************************************************************/
left_vert_clear(rs, re, fsy, tsy, msw, nsw, cdata, w, h)
int rs, re, *fsy, *tsy, msw, nsw;
unsigned char *cdata;
int w, h;
{
   int x, sy, ey, slimit;

   slimit = step_limit(msw);
   x = rs-1;
   if(x < 0)
      return(FALSE);
   sy = max(0, fsy[x] - slimit);
   ey = min(h-1, tsy[x] + slimit);
   if(sub_column_eq(0, x, sy, ey, cdata, w, h))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
/* top_left_hori_clear - checks to see if top-left edge of character piece */
/* is horizontally clear of character data.                                */
/***************************************************************************/
top_left_hori_clear(rs, re, fsy, tsy, msw, nsw, cdata, w, h)
int rs, re, *fsy, *tsy, msw, nsw;
unsigned char *cdata;
int w, h;
{
   int x, ex, y, slimit, rlen;

   rlen = re - rs + 1;
   slimit = min(rlen, step_limit(msw));
   x = rs;
   y = fsy[x]-1;
   if(y < 0)
      return(FALSE);
   ex = min(x+slimit, w-1);
   if(sub_row_eq(0, x, ex, y, cdata, w, h))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
/* bottom_left_hori_clear - checks to see if bottom-left edge of character */
/* piece is horizontally clear of character data.                          */
/***************************************************************************/
bottom_left_hori_clear(rs, re, fsy, tsy, msw, nsw, cdata, w, h)
int rs, re, *fsy, *tsy, msw, nsw;
unsigned char *cdata;
int w, h;
{
   int x, ex, y, slimit, rlen;

   rlen = re - rs + 1;
   slimit = min(rlen, step_limit(msw));
   x = rs;
   y = tsy[x]+1;
   if(y >= h)
      return(FALSE);
   ex = min(x+slimit, w-1);
   if(sub_row_eq(0, x, ex, y, cdata, w, h))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
/* bottom_right_hori_clear - checks to see if bottom-right edge of char    */
/* is horizontally clear of character data.                                */
/***************************************************************************/
bottom_right_hori_clear(rs, re, fsy, tsy, msw, nsw, cdata, w, h)
int rs, re, *fsy, *tsy, msw, nsw;
unsigned char *cdata;
int w, h;
{
   int x, sx, y, slimit, rlen;

   rlen = re - rs + 1;
   slimit = min(rlen, step_limit(msw));
   x = re;
   y = tsy[x]+1;
   if(y >= h)
      return(FALSE);
   sx = max(0, x-slimit);
   if(sub_row_eq(0, sx, x, y, cdata, w, h))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
/* top_right_hori_clear - checks to see if top-right edge of character     */
/* piece is horizontally clear of character data.                          */
/***************************************************************************/
top_right_hori_clear(rs, re, fsy, tsy, msw, nsw, cdata, w, h)
int rs, re, *fsy, *tsy, msw, nsw;
unsigned char *cdata;
int w, h;
{
   int x, sx, y, slimit, rlen;

   rlen = re - rs + 1;
   slimit = min(rlen, step_limit(msw));
   x = re;
   y = fsy[x]-1;
   if(y < 0)
      return(FALSE);
   sx = max(0, x-slimit);
   if(sub_row_eq(0, sx, x, y, cdata, w, h))
      return(TRUE);
   else
      return(FALSE);
}
