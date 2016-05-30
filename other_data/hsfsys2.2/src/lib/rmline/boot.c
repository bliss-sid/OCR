/*
# proc: is_boot - determines if the character piece touches along the bottom of
# proc:           the line.
# proc: process_boot - removes the line data from the "boot" charater stroke.
# proc:
# proc: boot_left_right_steps - determines the ratio of left to right steps
# proc:                based on projected cut slopes on both sides of the boot.
# proc:
*/

#include <stdio.h>
#include <values.h>
#include <math.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* is_boot - determines if character data touches above the line on both   */
/* left and right sides of the run of slices.                              */
/***************************************************************************/
is_boot(lb, la, rb, ra)
int lb, la, rb, ra;
{
      /* character data sitting on top of the line */
/*
                      \           /
                       \  char   /
                     ---------------
                       |  line   |
                       |<- run ->|
                     ---------------
*/
   if((la > lb) && (ra > rb) && (lb < SMALL_NOISE) && (rb < SMALL_NOISE))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
/* process_boot - shapes the identified character stroke that touches      */
/* along the top of the line on both sides of the run of unerased slices.  */
/***************************************************************************/
process_boot(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int lx, ly, rx, ry;
   float lm, rm, compute_y();
   int i, r_per_l, clx, crx, max_y;
   int slimit, step_right, step_left;

   /* find slopes to cut along on both sides of the run of slices */
   bl_corner_cut_slope(&lm, &lx, &ly, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h);
   br_corner_cut_slope(&rm, &rx, &ry, rs, re, msw, ebin, fsy, tsy, nsw, cdata, w, h);

   /* determine the rate at which the left and right cuts are to be conducted with */
   /* respect to their associated slopes. */
   boot_left_right_steps(&r_per_l, &step_right, &step_left, lm, rm);
   if(!step_left && !step_right){
      my_trace("no cutting on left or right sides\n");
      return;
   }

   my_trace3("left slope = %f, right slope = %f, ratio of r/l = %f\n", lm, rm, r_per_l);

   /* start cuts at nbring white pixels below */
   max_y = h - 1;
   ry = min(ry+1, max_y);
   ly = min(ly+1, max_y);
   clx = lx;
   crx = rx;
   slimit = step_limit(msw);
   /* while left and right steps do not intersect */
   /* and other stopping conditions not met ... */
   while((clx < crx) && (step_right || step_left)){
      /* take 0 or more steps on right, for each step on left */
      /* always test for stopping conditions */
      for(i = 0; (clx < crx)&&(step_right)&&(i < r_per_l); i++){
         step_right = process_b_corner_slice(crx, rm, rx, ry, rs, re, slimit,
                                             ebin, sw, fsy, tsy, nsw, cdata, w, h);
         crx--;
      }
      /* take step on left */
      /* always test for stopping conditions */
      if((clx < crx) && (step_left)){
         step_left = process_b_corner_slice(clx, lm, lx, ly, rs, re, slimit,
                                            ebin, sw, fsy, tsy, nsw, cdata, w, h);
         clx++;
      }
   }

   /* take final step as long as stopping conditions not met */
   if((r_per_l != 0) && (clx == crx) && (step_right))
      step_right = process_b_corner_slice(crx, rm, rx, ry, rs, re, slimit,
                                          ebin, sw, fsy, tsy, nsw, cdata, w, h);
   else if((r_per_l == 0) && (clx == crx) && (step_left))
      step_left = process_b_corner_slice(clx, lm, lx, ly, rs, re, slimit,
                                         ebin, sw, fsy, tsy, nsw, cdata, w, h);
   /* otherwise finished */
}

/***************************************************************************/
/* boot_left_right_steps - given the left and right slopes for a run of    */
/* unerased slices identified to be a boot, determines the rate at which   */
/* the left and right cuts are to be conducted.                            */
/***************************************************************************/
boot_left_right_steps(r_per_l, step_right, step_left, lm, rm)
int *r_per_l, *step_right, *step_left;
float lm, rm;
{
   *step_left = TRUE;
   *step_right = TRUE;

   if((lm == MAXFLOAT) && (rm == MAXFLOAT)){
      /* if both slopes are vertical then there is not cutting */
      my_trace("left and right slopes are both vertical\n");
      *step_left = FALSE;
      *step_right = FALSE;
      *r_per_l = NOT_FOUND;
   }
   else if (lm == MAXFLOAT) {
      /* if only left slope is vertical, then take all steps on the right */
      my_trace("left slope is vertical\n");
      *step_left = FALSE;
      *r_per_l = MAXINT;
   }
   else if (rm == MAXFLOAT) {
      /* if only right slope is vertical, then take all steps on the left */
      my_trace("right slope is vertical\n");
      *step_right = FALSE;
      *r_per_l = 0;
   }
   else if (lm < 0.0) {
      /* can't have left side with negative slope if really a boot */
      /* take all steps on the right */
      *step_left = FALSE;
      *r_per_l = MAXINT;
   }
   else if (rm > 0.0) {
      /* can't have right side with positive slope if really a boot */
      /* take all steps on the left */
      *step_right = FALSE;
      *r_per_l = 0;
   }
   else if (rm != 0.0) {
      /* for every step on left, you need r_per_l steps on the right */
      *r_per_l = sround(fabs(lm) /fabs(rm));
      if(*r_per_l == 0)
         /* if the left slope is 0, then r_per_l will be 0 */
         /* take all steps on the left */
         *step_right = FALSE;
   }
   else{
      /* if the right slope is 0, take all steps on the right */
      *step_left = FALSE;
      *r_per_l = MAXINT;
   }
}
