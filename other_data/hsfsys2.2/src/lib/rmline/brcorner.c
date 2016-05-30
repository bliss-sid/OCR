/*
# proc: rm_br_corner_1_sided - removes the bot-right corner of line data
# proc:                using a slope projection from one side.
# proc: rm_br_corner_2_sided - removes the bot-right corner of line data
# proc:                by projecting a cut line between two points.
# proc: br_corner_cut_slope - determines the slope for cutting off the
# proc:                bot-right corner of line data.
# proc: sum_dys_br_corner - computes changes in y used to estimate the
# proc:                slope along which the bot-right corner will be cut.
*/

#include <stdio.h>
#include <values.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* rm_br_corner_1_sided - removes the bottom-right corner of line data     */
/* using a slope projection from the character data on the top side of the */
/* corner.                                                                 */
/***************************************************************************/
rm_br_corner_1_sided(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int rx, ry;
   float rm;
   int crx, slimit;
   int take_step;

   /* determine the slope of the cut, from the character data on the */
   /* top-right side of the corner */
   br_corner_cut_slope(&rm, &rx, &ry, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h);

   /* can't have cut with positive slope if really br corner */
   if((rm == MAXFLOAT) || (rm > 0.0)) {
      my_trace("br corner not found\n");
      return;
   }
   /* project cut from the white pixel below ry */
   ry = min(h-1, ry+1);
   crx = rx;
   slimit = step_limit2(msw);
   take_step = TRUE;
   /* while right steps do not exceed the left edge of the run of unerased slices */
   /* and other stopping conditions not met ... */
   while((crx >= rs) && (take_step)){
      /* take next step */
      take_step = process_b_corner_slice(crx, rm, rx, ry, rs, re, slimit,
                                          ebin, sw, fsy, tsy, nsw, cdata, w, h);
      crx--;
   }
}

/***************************************************************************/
/* rm_br_corner_2_sided - removes the bottom-right corner of line data by  */
/* projecting a cut line connecting the top-right of the run of unerased   */
/* slices to the top-right point where the 2nd derivative elbow (or        */
/* inflection) was detected.                                               */
/***************************************************************************/
rm_br_corner_2_sided(elbow_x, rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int elbow_x, rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int rx, ry;
   float rm, compute_slope();
   int crx, slimit;
   int take_step;

   rx = re;
   crx = rx;
   ry = fsy[min(w-1, re+1)];
   slimit = step_limit(msw);

   /* compute the slope of the cut line projection */
   rm = compute_slope((float)rx, (float)ry, (float)elbow_x, (float)tsy[elbow_x]);
   take_step = TRUE;
   /* while right steps do not exceed the elbow */
   /* and other stopping conditions not met ... */
   while((crx >= elbow_x) && (take_step)){
      /* take next step */
      take_step = process_b_corner_slice(crx, rm, rx, ry, rs, re, slimit,
                                          ebin, sw, fsy, tsy, nsw, cdata, w, h);
      crx--;
   }
}

/***************************************************************************/
/* br_corner_cut_slope - determines the slope of the cut for a bottom-right*/
/* corner by measuring first derivative changes in y between the top of    */
/* the line to the character data on the top-right side of the corner for  */
/* a specified duration.                                                   */
/***************************************************************************/
br_corner_cut_slope(rm, rx, ry, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h)
float *rm;
int *rx, *ry;
int rs, re, msw, *ebin, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int rdys, nrdys;

   /* measure the first derivative changes in y between the top of the line */
   /* and the character data on the top-right side of a bottom-right corner. */
/*
                                  /
                                 /^
                                / | delta ys
                      |        /  v
                   ----------------
                      |      xx|
                      |    xxxx|
                   ----------------
*/
   sum_dys_br_corner(&rdys, &nrdys, rx, ry, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h);
   /* if charater data was found to compute delta ys ... */
   if(nrdys != 0){
      /* calculate the slope from the delta y's */
      *rm = rdys /(float)nrdys;
   }
   /* otherwise the slope is vetical */
   else
      *rm = MAXFLOAT;
   my_trace1("br corner cut slope = %f\n", *rm);
}

/***************************************************************************/
/* sum_dys_br_corner - measures the first derivative changes in y between  */
/* the top of the line and the character data on the top-right side of a   */
/* bottom-right corner for a specified duration. The resulting delta ys    */
/* are used to compute a slope along which the bottom-right corner will be */
/* cut.                                                                    */
/***************************************************************************/
sum_dys_br_corner(rdys, nrdys, rx, ry, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h)
int *rdys, *nrdys, *rx, *ry;
int rs, re, msw, *ebin, *tsy, *fsy, nsw;
unsigned char *cdata;
int w, h;
{
   int i, x, sum = 0, c = 0, diff;
   int oy, ny, slimit;

   /* if there is no data to the right of the run of unerased slices ... */
   if(re == (nsw - 1)){
      /* return 0 delta dys ==> vertical slope */
      *rdys = 0;
      *nrdys = 0;
      *rx = re;
      *ry = fsy[re];
      return;
   }

   /* start dys to the right of the run of unerased slices */
   x = re + 1;
   /* locate the first black pixel upward */
   /* this black pixel "should" be part of the character data */
   if((oy = find_pix_north(BLACK_PIX, x, max(0,fsy[x]-1), cdata, w, h)) != NOT_FOUND){
      slimit = step_limit(msw);
      /* compute the distance between the top of the line and the */
      /* character data */
      diff = oy - fsy[x];
   }
   /* if character data not found */
   /* OR the delta is too big ... */
   if((oy == NOT_FOUND) || (-diff > slimit)){
      /* return 0 delta dys ==> vertical slope */
      *rdys = 0;
      *nrdys = 0;
      *rx = x;
      *ry = fsy[x];
      return;
   }
   /* store the very first point of the character data */
   /* to the right of the run of unerased slices */
   *rx = x;
   *ry = oy;
   /* bump to the next slice on right */
   x++;
   /* for the specified duration */
   /* AND while slices to the right */
   /* AND while curent slice has been erased ... */
   for(i = 1; (i < RUN_DYS)&&(x<w)&&(ebin[x] == ERASED); i++,x++){
      /* find the edge of the character data upward */
      if((ny = find_pix_north(BLACK_PIX, x, max(0,fsy[x]-1), cdata, w, h)) != NOT_FOUND)
         /* compute change in previous and current slices' character data ys */
         diff = ny - oy;
      /* if character data not found */
      /* OR the delta is too big, then stop computing delta ys */
      if((ny == NOT_FOUND) || (-diff > slimit))
         break;
      /* otherwise accumulate new delta y */
      sum += diff;
      oy = ny;
      c++;
   }

   *rdys = sum;
   *nrdys = c;
}
