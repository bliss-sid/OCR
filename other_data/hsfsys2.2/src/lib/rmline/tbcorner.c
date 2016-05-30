/*
# proc: process_t_corner_slice - erases a portion of a slice for either a top-left
# proc:                          or top-right corner of line data based on a specifed
# proc:                          cut slope.
# proc: process_b_corner_slice - erases a portion of a slice for either a bottom-left
# proc:                          or bottom-right corner of line data based on a specifed
# proc:                          cut slope.
*/

#include <stdio.h>
#include <defs.h>
#include <rmline.h>

/***************************************************************************/
/* process_t_corner_slice - erases a portion of a slice for either a top-  */
/* left or top-right corner by computing a new bottom y coordinate for the */
/* slice according to a specified cut slope. The routine does not erase    */
/* any pixels (and returns FALSE) if certain stopping conditions are met.  */
/***************************************************************************/
process_t_corner_slice(cx, m, x, y,
                       rs, re, slimit, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int cx, x, y;
float m;
int rs, re, slimit, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int ntsy;
   float compute_y();


   /* compute the new y coordinate */
   ntsy = min(h-1, max(0, sround(compute_y((float)cx, (float)m,
                                           (float)x, (float)y))));
   /* if the new slice width is too big */
   /* OR the new y coordinate crosses over the top y coordinate ... */
   if(((ntsy - fsy[cx] + 1) >= slimit) || (ntsy < fsy[cx])){
      /* stop taking steps */
      return(FALSE);
   }
   /* if the slice has not already been altered ... */
   if(ebin[cx] == UNERASED){
      tsy[cx] = ntsy;
      /* erase the portion of the slice that is part of the line */
      erase_slice_hori(ebin, sw, cx, fsy[cx], tsy[cx], cdata, w, h);
   }
   return(TRUE);
}

/***************************************************************************/
/* process_b_corner_slice - erases a portion of a slice for either a       */
/* bottom-left or bottom-right corner by computing a new top y coordinate  */
/* for the slice according to a specified cut slope. The routine does not  */
/* erase any pixels (and returns FALSE) if certain stopping conditions are */
/* met.                                                                    */
/***************************************************************************/
process_b_corner_slice(cx, m, x, y,
                        rs, re, slimit, ebin, sw, fsy, tsy, nsw, cdata, w, h)
int cx, x, y;
float m;
int rs, re, slimit, *ebin, *sw, *fsy, *tsy, nsw;
unsigned char *cdata;
int w, h;
{
   int nfsy;
   float compute_y();

   /* compute the new y coordinate */
   nfsy = min(h-1, max(0, sround(compute_y((float)cx, (float)m,
                                           (float)x, (float)y))));
   /* if the new slice width is too big */
   /* OR the new y coordinate crosses over the bottom y coordinate ... */
   if(((tsy[cx] - nfsy + 1) >= slimit) || (tsy[cx] < nfsy)){
      /* stop taking steps */
      return(FALSE);
   }
   /* if the slice has not already been altered ... */
   if(ebin[cx] == UNERASED){
      fsy[cx] = nfsy;
      /* erase the portion of the slice that is part of the line */
      erase_slice_hori(ebin, sw, cx, fsy[cx], tsy[cx], cdata, w, h);
   }
   return(TRUE);
}
