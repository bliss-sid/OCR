/*
# proc: is_thru_dl - determines if the character piece passes through the line
# proc:           on its left side and touches the top of the line on its right side.
# proc: process_thru_dl - removes the line data from the "thru_dl" charater stroke.
# proc:
*/

#include <stdio.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* is_thru_dl - determines if character data touches on the left side      */
/* above and below and the right side above the line.                      */
/***************************************************************************/
is_thru_dl(lb, la, rb, ra)
int lb, la, rb, ra;
{
      /* character stoke passing through and protruding on the */
      /* bottom left of the line */
/*
                       /        /
                      |  char  /
                   ----------------
                      |  line  |
                      |<-run ->|
                   ----------------
                      | /
               char ->|/
*/
   if((lb > 0) && (la > 0) && (ra > rb) && (rb < SMALL_NOISE))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
/* process_thru_dl - shapes the identified character stroke by locating    */
/* a bottom-right corner of line data and cutting it off.                  */
/***************************************************************************/
process_thru_dl(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int elbow_x, rlen;

   rlen = re - rs;
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
