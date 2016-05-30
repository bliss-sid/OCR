/*
# proc: rm_tr_corner_1_sided - removes the top-right corner of line data
# proc:                using a slope projection from one side.
# proc: rm_tr_corner_2_sided - removes the top-right corner of line data
# proc:                by projecting a cut line between two points.
# proc: tr_corner_cut_slope - determines the slope for cutting off the
# proc:                top-right corner of line data.
# proc: sum_dys_tr_corner - computes changes in y used to estimate the
# proc:                slope along which the top-right corner will be cut.
*/

#include <stdio.h>
#include <values.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* rm_tr_corner_1_sided - removes the top-right corner of line data using  */
/* a slope projection from the character data on the bottom side of the    */
/* corner.                                                                 */
/***************************************************************************/
rm_tr_corner_1_sided(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int rx, ry;
   float rm;
   int crx, slimit;
   int take_step;


   /* determine the slope of the cut, from the character data on the */
   /* bottom-right side of the corner */
   tr_corner_cut_slope(&rm, &rx, &ry, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h);

   /* can't have cut with negative slope if really tr corner */
   if((rm == MAXFLOAT) || (rm < 0.0)) {
      my_trace("tr corner not found\n");
      return;
   }
   /* project cut from the white pixel above ry */
   ry = max(0, ry-1);
   crx = rx;
   slimit = step_limit2(msw);
   take_step = TRUE;
   /* while right steps do not exceed the left edge of the run of unerased slices */
   /* and other stopping conditions not met ... */
   while((crx >= rs) && (take_step)){
      /* take next step */
      take_step = process_t_corner_slice(crx, rm, rx, ry, rs, re, slimit,
                                          ebin, sw, fsy, tsy, nsw, cdata, w, h);
      crx--;
   }
}

/***************************************************************************/
/* rm_tr_corner_2_sided - removes the top-right corner of line data by     */
/* projecting a cut line connecting the bottom-right of the run of         */
/* unerased slices to the top-left point where the 2nd derivative elbow    */
/* (or inflection) was detected.                                           */
/***************************************************************************/
rm_tr_corner_2_sided(elbow_x, rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
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
   ry = tsy[min(w-1, re+1)];
   slimit = step_limit(msw);

   /* compute the slope of the cut line projection */
   rm = compute_slope((float)rx, (float)ry, (float)elbow_x, (float)fsy[elbow_x]);
   take_step = TRUE;
   /* while right steps do not exceed the elbow */
   /* and other stopping conditions not met ... */
   while((crx >= elbow_x) && (take_step)){
      /* take step */
      take_step = process_t_corner_slice(crx, rm, rx, ry, rs, re, slimit,
                                          ebin, sw, fsy, tsy, nsw, cdata, w, h);
      crx--;
   }
}

/***************************************************************************/
/* tr_corner_cut_slope - determines the slope of the cut for a top-right   */
/* corner by measuring first derivative changes in y between the bottom of */
/* the line to the character data on the bottom-right side of the corner   */
/* for a specified duration.                                               */
/***************************************************************************/
tr_corner_cut_slope(rm, rx, ry, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h)
float *rm;
int *rx, *ry;
int rs, re, msw, *ebin, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int rdys, nrdys;

   /* measure the first derivative changes in y between the bottom of the line */
   /* and the character data on the bottom-right side of a top-right corner. */
/*
                   ----------------
                      |    xxxx|
                      |      xx|
                   ----------------
                      |        \  ^
                                \ | delta ys
                                 \v
                                  \
*/
   sum_dys_tr_corner(&rdys, &nrdys, rx, ry, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h);
   /* if charater data was found to compute delta ys ... */
   if(nrdys != 0){
      /* calculate the slope from the delta y's */
      *rm = rdys /(float)nrdys;
   }
   /* otherwise the slope is vetical */
   else
      *rm = MAXFLOAT;
   my_trace1("tr corner cut slope = %f\n", *rm);
}

/***************************************************************************/
/* sum_dys_tr_corner - measures the first derivative changes in y between  */
/* the bottom of the line and the character data on the bottom-right side  */
/* of a top-right corner for a specified duration. The resulting delta ys  */
/* are used to compute a slope along which the top-right corner will be    */
/* cut.                                                                    */
/***************************************************************************/
sum_dys_tr_corner(rdys, nrdys, rx, ry, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h)
int *rdys, *nrdys, *rx, *ry;
int rs, re, msw, *ebin, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int i, x, sum = 0, c = 0, diff, max_y;
   int oy, ny, slimit;

   /* if there is no data to the right of the run of unerased slices ... */
   if(re == (nsw - 1)){
      /* return 0 delta dys ==> vertical slope */
      *rdys = 0;
      *nrdys = 0;
      *rx = re;
      *ry = tsy[re];
      return;
   }

   /* start dys to the right of the run of unerased slices */
   x = re + 1;
   /* locate the first black pixel downward */
   /* this black pixel "should" be part of the character data */
   max_y = h - 1;
   if((oy = find_pix_south(BLACK_PIX, x, min(tsy[x]+1,max_y), cdata, w, h)) != NOT_FOUND){
      slimit = step_limit(msw);
      /* compute the distance between the bottom of the line and the */
      /* character data */
     diff = oy - tsy[x];
   }
   /* if character data not found */
   /* OR the delta is too big ... */
   if((oy == NOT_FOUND) || (diff > slimit)){
      /* return 0 delta dys ==> vertical slope */
      *rdys = 0;
      *nrdys = 0;
      *rx = x;
      *ry = tsy[x];
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
      /* find the edge of the character data downward */
      if((ny = find_pix_south(BLACK_PIX, x, min(tsy[x]+1,max_y), cdata, w, h)) != NOT_FOUND)
         /* compute change in previous and current slices' character data ys */
         diff = (ny - oy);
      /* if character data not found */
      /* OR the delta is too big, then stop computing delta ys */
      if((ny == NOT_FOUND) || (diff > slimit))
         break;
      /* otherwise accumulate new delta y */
      sum += diff;
      oy = ny;
      c++;
   }

   *rdys = sum;
   *nrdys = c;
}
