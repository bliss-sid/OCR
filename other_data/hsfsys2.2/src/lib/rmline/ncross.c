/*
# proc: is_ncross - determines if the character piece passes through the line
# proc:           top-to-bottom and right-to-left.
# proc: process_ncross - removes the line data from the "ncross" charater stroke.
# proc:
*/

#include <stdio.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* is_ncross - determines if character data touches on the righ side above */
/* and the left side below the line.                                       */
/***************************************************************************/
is_ncross(lb, la, rb, ra)
int lb, la, rb, ra;
{
      /* character stroke with negative slope (downward right to left) */
      /* passing through the line */
/*
                          /       /
                         / char  /
                   -----------------
                     |   line    |
                     |<-- run -->|
                   -----------------
                     / char  /
                    /       /
*/
   if((lb > la) && (ra > rb) && (la < SMALL_NOISE) && (rb < SMALL_NOISE))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
/* process_ncross - shapes the identified character stroke that crosses    */
/* all the way through the line on both sides with negative slope.         */
/***************************************************************************/
process_ncross(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int elbow_x, rlen;

   /* do left side first */
   rlen = re - rs;
   /* try to find the elbow for the top-left conrner using 2nd derivative info */
   if((elbow_x = der2_change_right_up(rs, rlen, SD2_THRESH, fsy, nsw)) == NOT_FOUND){
      /* if the elbow is not found, then use 1-sided slope removal */
      my_trace("tl corner's elbow not found, using 1-side slope removal\n");
      rm_tl_corner_1_sided(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h);
   }
   else{ 
      /* if elbow is found, then use 2-sided point removal */
      my_trace2("tl corner's elbow at (%d, %d), using 2-sided point removal\n",
                elbow_x, fsy[elbow_x]);
      rm_tl_corner_2_sided(elbow_x, rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h);
   }

   /* now do right side */
   rlen = re - max(rs,elbow_x);
   /* try to find the elbow for the bottom-right conrner using 2nd derivative info */
   if((elbow_x = der2_change_left_down(re, rlen, SD2_THRESH, tsy, nsw)) == NOT_FOUND){
      /* if the elbow is not found, then use 1-sided slope removal */
      my_trace("br corner's elbow not found, using 1-side slope removal\n");
      rm_br_corner_1_sided(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h);
   }
   else{
      /* if elbow is found, then use 2-sided point removal */
      my_trace2("br corner's elbow at (%d, %d), using 2-sided point removal\n",
                elbow_x, tsy[elbow_x]);
      rm_br_corner_2_sided(elbow_x, rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h);
   }
}
