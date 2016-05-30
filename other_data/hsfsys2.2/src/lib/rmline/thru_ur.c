/*
# proc: is_thru_ur - determines if the character piece passes through the line
# proc:           on its right side and touches the bottom of the line on its left side.
# proc: process_thru_ur - removes the line data from the "thru_ur" charater stroke.
# proc:
*/

#include <stdio.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* is_thru_ur - determines if character data touches on the right side     */
/* above and below and the left side below the line.                       */
/***************************************************************************/
is_thru_ur(lb, la, rb, ra)
int lb, la, rb, ra;
{
      /* character stoke passing through and protruding on the */
      /* top right of the line */
/*
                              /|<- char
                             / |
                   ----------------
                      |  line  |
                      |<-run ->|
                   ----------------
                      /  char  |
                     /        /
*/
   if((rb > 0) && (ra > 0) && (lb > la) && (la < SMALL_NOISE))
      return(TRUE);
   else
      return(FALSE);
}

/***************************************************************************/
/* process_thru_ul - shapes the identified character stroke by locating    */
/* a top-left corner of line data and cutting it off.                      */
/***************************************************************************/
process_thru_ur(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int elbow_x, rlen;

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
}