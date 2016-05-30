/*
# proc: is_pcross - determines if the character piece passes through the line
# proc:           top-to-bottom and left-to-right.
# proc: process_pcross - removes the line data from the "pcross" charater stroke.
# proc:
*/

#include <stdio.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* is_pcross - determines if character data touches on the left side above */
/* and the right side below the line.                                      */
/***************************************************************************/
is_pcross(lb, la, rb, ra)
int lb, la, rb, ra;
{
      /* character stroke with positive slope (downward left to right) */
      /* passing through the line */
/*
                    \       \
                     \ char  \
                   -----------------
                     |   line    |
                     |<-- run -->|
                   -----------------
                         \ char  \
                          \       \
*/
   if((la > lb) && (rb > ra) && (lb < SMALL_NOISE) && (ra < SMALL_NOISE))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
/* process_pcross - shapes the identified character stroke that crosses    */
/* all the way through the line on both sides with posititve slope.        */
/***************************************************************************/
process_pcross(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int elbow_x, rlen;

   /* do left side first */
   rlen = re - rs;
   /* try to find the elbow for the bottom-left conrner using 2nd derivative info */
   if((elbow_x = der2_change_right_down(rs, rlen, SD2_THRESH, tsy, nsw)) == NOT_FOUND){
      /* if the elbow is not found, then use 1-sided slope removal */
      my_trace("bl corner's elbow not found, using 1-side slope removal\n");
      rm_bl_corner_1_sided(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h);
   }
   else{
      /* if elbow is found, then use 2-sided point removal */
      my_trace2("bl corner's elbow at (%d, %d), using 2-sided point removal\n",
                elbow_x, tsy[elbow_x]);
      rm_bl_corner_2_sided(elbow_x, rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h);
   }

   /* now do right side */
   rlen = re - max(rs,elbow_x);
   /* try to find the elbow for the top-right conrner using 2nd derivative info */
   if((elbow_x = der2_change_left_up(re, rlen, SD2_THRESH, fsy, nsw)) == NOT_FOUND){
      /* if the elbow is not found, then use 1-sided slope removal */
      my_trace("tr corner's elbow not found, using 1-side slope removal\n");
      rm_tr_corner_1_sided(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h);
   }
   else{
      /* if elbow is found, then use 2-sided point removal */
      my_trace2("tr corner's elbow at (%d, %d), using 2-sided point removal\n",
                elbow_x, fsy[elbow_x]);
      rm_tr_corner_2_sided(elbow_x, rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h);
   }
}
