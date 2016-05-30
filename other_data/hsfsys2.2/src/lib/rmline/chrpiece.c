/*
# proc: process_char_pieces_ml - shapes and smooths stems and descenders of character
# proc:              strokes that intersect the lines using a locally dynamic vector
# proc:              of median line widths.
# proc: identify_char_piece - categorizes the character piece by examining
# proc:              its left and right edges.
# proc: extract_shape_features - computes 4 features used to represent the
# proc:              character piece.
# proc: process_char_piece - main switch for shaping and smoothing a character
# proc:              piece based on its identified type.
*/

#include <stdio.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* process_char_pieces - cleans up the portions of the characters around   */
/* where they intersected the removed line. This routine shapes and        */
/* smooths stems and descenders.                                           */
/***************************************************************************/
/***************************************************************************/
process_char_pieces_ml(ebin, msws, sw, fsy, tsy, nsw, cdata, w, h)
int *ebin, *msws, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int nextbin, chartype;
   int rs, re;

   nextbin = 0;
   /* any slice not erased to this point is assumed to belong to a character */
   while(next_n_run(0, &rs, &re, nextbin, ebin, nsw)){
      nextbin = re+1;
      /* identify the shape of the piece */
      chartype = identify_char_piece(rs, re, fsy, tsy, nsw, w, h);
      /* process the piece of character */
      process_char_piece(chartype, rs, re, msws[rs], ebin, sw, fsy, tsy, nsw, cdata, w, h);
   }
}

/***************************************************************************/
/* identify_char_piece - attempts to categorize the shape of a contiguous  */
/* run of slices potentially containing both character and line data. It   */
/* does this by simply looking at the left and right edges of the run, and */
/* determining if data exists above or below the the line being removed.   */
/***************************************************************************/
identify_char_piece(rs, re, fsy, tsy, nsw, w, h)
int rs, re, *fsy, *tsy, nsw;
int w, h;
{
   int lb, la;
   int rb, ra;

   /* if only one slice, then really don't know what it is */
   if(nsw == 1)
      return(RUN_UNKNOWN);

   /* identification is conducted based on four extracted features. */
   /* these features represent whether character data touch above   */
   /* and/or below the line on the left and right sides of the      */
   /* current run of slices. to measure this, the position of the   */
   /* last nbring slice previously erased is subtracted from the    */
   /* corresponding edge of the run of current slices. if the delta */
   /* is positive then character data is "assumed" to touch in that */
   /* area. this measurement works because the previously erased    */
   /* slice was removed because it was determined to "completely"   */
   /* part of the line. this provides a reference to the relative   */
   /* location of the line, from which the features can be measured.*/

   /* extract the four features */
   extract_shape_features(&la, &lb, &ra, &rb, rs, re, fsy, tsy, nsw);

   /* now try to identify the run of slices based on the four features */
   if(is_boot(lb, la, rb, ra)){
      my_trace2("run [%d .. %d] boot\n", rs, re);
      return(RUN_BOOT);
   }
   if(is_hat(lb, la, rb, ra)){
      my_trace2("run [%d .. %d] hat\n", rs, re);
      return(RUN_HAT);
   }
   if(is_ncross(lb, la, rb, ra)){
      my_trace2("run [%d .. %d] negative crossover\n", rs, re);
      return(RUN_NCROSS);
   }
   if(is_pcross(lb, la, rb, ra)){
      my_trace2("run [%d .. %d] positive crossover\n", rs, re);
      return(RUN_PCROSS);
   }
   if(is_thru_dl(lb, la, rb, ra)){
      my_trace2("run [%d .. %d] thru down and left\n", rs, re);
      return(RUN_THRU_DL);
   }
   if(is_thru_dr(lb, la, rb, ra)){
      my_trace2("run [%d .. %d] thru down and right\n", rs, re);
      return(RUN_THRU_DR);
   }
   if(is_thru_ul(lb, la, rb, ra)){
      my_trace2("run [%d .. %d] thru up and left\n", rs, re);
      return(RUN_THRU_UL);
   }
   if(is_thru_ur(lb, la, rb, ra)){
      my_trace2("run [%d .. %d] thru up and right\n", rs, re);
      return(RUN_THRU_UR);
   }
   /* the run may not fit into one of these 8 categories */
   /* in this case the run is simply ignored */
   my_trace2("run [%d .. %d] unidentified\n", rs, re);
   return(RUN_UNKNOWN);
}

/***************************************************************************/
/* extract_shape_features - computes the four features used to represent   */
/* whether character data touch above and/or below the line on the left    */
/* and right sides of the current run of unerased slices. To measure this, */
/* the position of the closest nbring erased slice is subtracted from the  */
/* corresponding edge of the run of unerased slices. If the delta is       */
/* positive then character data is "assumed" to touch in that area.        */
/***************************************************************************/
extract_shape_features(la, lb, ra, rb, rs, re, fsy, tsy, nsw)
int *la, *lb, *ra, *rb;
int rs, re, *fsy, *tsy, nsw;
{
   int lx, rx, max_x;

   /* extract the four features */
   lx = rs - 1;
   rx = re + 1;
   max_x = nsw - 1;
   *lb = tsy[rs] - tsy[max(0,lx)];
   *la = fsy[max(0,lx)] - fsy[rs];
   *rb = tsy[re] - tsy[min(rx, max_x)];
   *ra = fsy[min(rx, max_x)] - fsy[re];

   if(*lb > 0)
      my_trace("left edge: character data below line\n");
   if(*la > 0)
      my_trace("left edge: character data above line\n");
   if(*lb <= 0 && *la <= 0)
      my_trace("left edge: no character data found\n");

   if(*rb > 0)
      my_trace("right edge: character data below line\n");
   if(*ra > 0)
      my_trace("right edge: character data above line\n");
   if(*rb <= 0 && *ra <= 0)
      my_trace("right edge: no character data found\n");
}

/***************************************************************************/
/* process_char_piece - main switch for shaping and smoothing various      */
/* categories of character descenders and strokes that touch the line.     */
/***************************************************************************/
process_char_piece(chartype, rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, fw, fh)
int chartype, rs, re, msw, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int fw, fh;
{
   switch(chartype){
   case RUN_BOOT:
      /* character data sitting on top of the line */
/*
                      \           /
                       \         /
                     ---------------
                       |x       x|
                       |xx     xx|
                     ---------------
*/
      process_boot(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, fw, fh);
   break;
   case RUN_HAT:
      /* character data touching the bottom of the line */
/*
                    ---------------
                      |xx     xx|
                      |x       x|
                    ---------------
                      /         \
                     /           \
*/
      process_hat(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, fw, fh);
   break;
   case RUN_NCROSS:
      /* character stroke with negative slope (downward right to left) */
      /* passing through the line */
/*
                          /       /
                         /       /
                   -----------------
                     |xx        x|
                     |x        xx|
                   -----------------
                     /       /
                    /       /
*/
      process_ncross(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, fw, fh);
   break;
   case RUN_PCROSS:
      /* character stroke with positive slope (downward left to right) */
      /* passing through the line */
/*
                    \       \
                     \       \
                   -----------------
                     |x        xx|
                     |xx        x|
                   -----------------
                         \       \
                          \       \
*/
      process_pcross(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, fw, fh);
   break;
   case RUN_THRU_DL:
      /* character stoke passing through and protruding on the */
      /* bottom left of the line */
/*
                       /        /
                      |        /
                   ----------------
                      |      xx|
                      |    xxxx|
                   ----------------
                      | /
                      |/
*/
      process_thru_dl(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, fw, fh);
   break;
   case RUN_THRU_DR:
      /* character stoke passing through and protruding on the */
      /* bottom right of the line */
/*
                     \        \
                      \        |
                   ----------------
                      |xx      |
                      |xxxx    |
                   ----------------
                             \ |
                              \|
*/
      process_thru_dr(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, fw, fh);
   break;
   case RUN_THRU_UL:
      /* character stoke passing through and protruding on the */
      /* top left of the line */
/*
                      |\
                      | \
                   ----------------
                      |    xxxx|
                      |      xx|
                   ----------------
                      |        \
                       \        \
*/
      process_thru_ul(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, fw, fh);
   break;
   case RUN_THRU_UR:
      /* character stoke passing through and protruding on the */
      /* top right of the line */
/*
                              /|
                             / |
                   ----------------
                      |xxxx    |
                      |xx      |
                   ----------------
                      /        |
                     /        /
*/
      process_thru_ur(rs, re, msw, ebin, sw, fsy, tsy, nsw, cdata, fw, fh);
   break;
   default:
      /* if character piece not identified, just ignore it and continue */
      my_trace("category of character piece not identified\n");
   break;
   }
}
