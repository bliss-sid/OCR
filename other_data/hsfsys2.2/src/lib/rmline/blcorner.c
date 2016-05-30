/*
# proc: rm_bl_corner_1_sided - removes the bot-left corner of line data
# proc:                using a slope projection from one side.
# proc: rm_bl_corner_2_sided - removes the bot-left corner of line data
# proc:                by projecting a cut line between two points.
# proc: bl_corner_cut_slope - determines the slope for cutting off the
# proc:                bot-left corner of line data.
# proc: sum_dys_bl_corner - computes changes in y used to estimate the
# proc:                slope along which the bot-left corner will be cut.
*/

#include <stdio.h>
#include <values.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* rm_bl_corner_1_sided - removes the bottom-left corner of line data      */
/* using a slope projection from the character data on the top side of the */
/* corner.                                                                 */
/***************************************************************************/
rm_bl_corner_1_sided(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int lx, ly;
   float lm;
   int clx, slimit;
   int take_step;

   /* determine the slope of the cut, from the character data on the */
   /* top-left side of the corner */
   bl_corner_cut_slope(&lm, &lx, &ly, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h);

   /* can't have cut with negative slope if really bl corner */
   if((lm == MAXFLOAT) || (lm < 0.0)) {
      my_trace("bl corner not found\n");
      return;
   }

   ly = min(h-1, ly+1);
   clx = lx;
   slimit = step_limit2(msw);
   take_step = TRUE;
   /* while left steps do not exceed the right edge of the run of unerased slices */
   /* and other stopping conditions not met ... */
   while((clx <= re) && (take_step)){
      /* take next step */
      take_step = process_b_corner_slice(clx, lm, lx, ly, rs, re, slimit,
                                          ebin, sw, fsy, tsy, nsw, cdata, w, h);
      clx++;
   }
}

/***************************************************************************/
/* rm_bl_corner_2_sided - removes the bottom-left corner of line data by   */
/* projecting a cut line connecting the top-left of the run of unerased    */
/* slices to the bottom-right point where the 2nd derivative elbow (or     */
/* inflection) was detected.                                               */
/***************************************************************************/
rm_bl_corner_2_sided(elbow_x, rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int elbow_x, rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int lx, ly;
   float lm, compute_slope();
   int clx, slimit;
   int take_step;

   lx = max(0, rs-1);
   clx = lx;
   ly = fsy[lx];
   slimit = step_limit(msw);

   /* compute the slope of the cut line projection */
   lm = compute_slope((float)lx, (float)ly, (float)elbow_x, (float)tsy[elbow_x]);
   take_step = TRUE;
   /* while left steps do not exceed the elbow */
   /* and other stopping conditions not met ... */
   while((clx <= elbow_x) && (take_step)){
      /* take next step */
      take_step = process_b_corner_slice(clx, lm, lx, ly, rs, re, slimit,
                                          ebin, sw, fsy, tsy, nsw, cdata, w, h);
      clx++;
   }
}

/***************************************************************************/
/* bl_corner_cut_slope - determines the slope of the cut for a bottom-left */
/* corner by measuring first derivative changes in y between the top of    */
/* the line to the character data on the top-left side of the corner for a */
/* specified duration.                                                     */
/***************************************************************************/
bl_corner_cut_slope(lm, lx, ly, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h)
float *lm;
int *lx, *ly;
int rs, re, msw, *ebin, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int ldys, nldys;

   /* measure the first derivative changes in y between the top of the line */
   /* and the character data on the top-left side of a bottom-left corner. */
/*
                   \
                   ^\
          delta ys | \
                   v  \        |
                   ----------------
                      |xx      |
                      |xxxx    |
                   ----------------
*/
   sum_dys_bl_corner(&ldys, &nldys, lx, ly, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h);
   /* if charater data was found to compute delta ys ... */
   if(nldys != 0){
      /* calculate the slope from the delta y's */
      *lm = ldys /(float)nldys;
   }
   /* otherwise the slope is vetical */
   else
      *lm = MAXFLOAT;
   my_trace1("bl corner cut slope = %f\n", *lm);
}

/***************************************************************************/
/* sum_dys_bl_corner - measures the first derivative changes in y between  */
/* the top of the line and the character data on the top-left side of a    */
/* bottom-left corner for a specified duration. The resulting delta ys are */
/* used to compute a slope along which the bottom-left corner will be cut. */
/***************************************************************************/
sum_dys_bl_corner(ldys, nldys, lx, ly, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h)
int *ldys, *nldys, *lx, *ly;
int rs, re, msw, *ebin, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int i, x, sum = 0, c = 0, diff;
   int oy, ny, slimit;

   /* if there is no data to the left of the run of unerased slices ... */
   if(rs == 0){
      /* return 0 delta dys ==> vertical slope */
      *ldys = 0;
      *nldys = 0;
      *lx = rs;
      *ly = fsy[rs];
      return;
   }

   /* start dys to the left of the run of unerased slices */
   x = rs - 1;
   /* locate the first black pixel upward */
   /* this black pixel "should" be part of the character data */
   if((oy = find_pix_north(BLACK_PIX, x, max(0,fsy[x]-1), cdata, w, h)) != NOT_FOUND){
      slimit = step_limit(msw);
      /* compute the distance between the top of the line and the */
      /* character data */
      diff = fsy[x] - oy;
   }
   /* if character data not found */
   /* OR the delta is too big ... */
   if((oy == NOT_FOUND) || (diff > slimit)){
      /* return 0 delta dys ==> vertical slope */
      *ldys = 0;
      *nldys = 0;
      *lx = x;
      *ly = fsy[x];
      return;
   }
   /* store the very first point of the character data */
   /* to the left of the run of unerased slices */
   *lx = x;
   *ly = oy;
   /* bump to the next slice on left */
   x--;
   /* for the specified duration */
   /* AND while slices to the left */
   /* AND while curent slice has been erased ... */
   for(i = 1; (i < RUN_DYS)&&(x>=0)&&(ebin[x] == ERASED); i++,x--){
      /* find the edge of the character data upward */
      if((ny = find_pix_north(BLACK_PIX, x, max(0,fsy[x]-1), cdata, w, h)) != NOT_FOUND)
         /* compute change in previous and current slices' character data ys */
         diff = oy - ny;
      /* if character data not found */
      /* OR the delta is too big, then stop computing delta ys */
      if((ny == NOT_FOUND) || (diff > slimit))
         break;
      /* otherwise accumulate new delta y */
      sum += diff;
      oy = ny;
      c++;
   }

   *ldys = sum;
   *nldys = c;
}
