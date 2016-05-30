/*
# proc: dominant_hori_line - chooses the most "dominant" solid or dashed
# proc:          horizontal line from the hough transform matrices passed.
# proc: pick_max_pix_line - selects the most "dominant" solid horizontal line
# proc:          from the hough transform matrices passed.
# proc: pick_dash_line - selects the most "dominant" dashed horizontal line
# proc:          from the the hough transform matrices passed.
*/

#include <stdio.h>
#include <math.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* dominant_hori_line - takes the output from two hough line transforms    */
/* (one for [0..+n] degrees and the other for [0..-n] degrees) and locates */
/* the next "dominant" line. The routine returns the endpoints of the      */
/* actual detected line (lx, ly) to (rx, ry), the endpoints of the line    */
/* projected to the edges of the field image, and the black pixels summed  */
/* along this trajectory.                                                  */
/***************************************************************************/
dominant_hori_line(npix, x1, y1, x2, y2, lx, ly, rx, ry, solid_flag,
                   pacc, pax, pay, pmin, pmax, nacc, nax, nay, nmin, nmax, td,
                   cdata, w, h, prct_len, dist_limit)
int *npix, *x1, *y1, *x2, *y2, *lx, *ly, *rx, *ry;
int *pacc, pax, pay;
float pmin, pmax;
int *nacc, nax, nay;
float nmin, nmax, td;
unsigned char *cdata;
int w, h, dist_limit;
float prct_len;
{
   int i, line_found, pix_limit;
   float dist, distance();

   my_trace("entered dominant_hori_line\n");
   i = 0;
   do{
      ++i;
      if(solid_flag)
         /* locate a dominant solid line */
         line_found = pick_max_pix_line(npix, x1, y1, x2, y2, lx, ly, rx, ry,
                 TOPN, pacc, pax, pay, pmin, pmax, nacc, nax, nay, nmin, nmax, td,
                 cdata, w, h, dist_limit,(i==1?1:0));
      else
         /* locate a dominant dashed line */
         line_found = pick_dash_line(npix, x1, y1, x2, y2, lx, ly, rx, ry,
                 TOPN, pacc, pax, pay, pmin, pmax, nacc, nax, nay, nmin, nmax, td,
                        cdata, w, h, dist_limit,(i==1?1:0));
      if((!line_found) || ((*npix) == 0)){
         my_trace("did not find next hori line\n");
         my_trace("finished dominant_hori_line\n");
         return(FALSE);
      }
      dist = distance((float)(*lx), (float)(*ly), (float)(*rx), (float)(*ry));
      pix_limit = prct_len * dist;
      my_trace2("black pixels = %d, pixel limit = %d\n", *npix, pix_limit);
      my_trace2("dist = %f, dist limit = %d\n", dist, dist_limit);
   /* look for another line if either the number of black pixels is too few */
   /* or the distance is too short (these help define "dominant") */
   }while(((*npix) < pix_limit) || (dist < dist_limit));
   my_trace("found next hori line\n");
   my_trace("finished dominant_hori_line\n");
   return(TRUE);
}

/***************************************************************************/
/* pick_max_pix_line - takes the output from two hough line transforms     */
/* (one for [0..+n] degrees and the other for [0..-n] degrees) and locates */
/* the next "dominant" solid line. The routine returns the endpoints of    */
/* the detected line (lx, ly) to (rx, ry), the endpoints of the line       */
/* projected to the edges of the field image, and the black pixels summed  */
/* along this trajectory.                                                  */
/***************************************************************************/
pick_max_pix_line(max_pix, mx1, my1, mx2, my2, mlx, mly, mrx, mry,
               topn, pacc, pax, pay, pmin, pmax, nacc, nax, nay, nmin, nmax, td,
               cdata, w, h, dist_limit, reset)
int *max_pix, *mx1, *my1, *mx2, *my2, *mlx, *mly, *mrx, *mry;
int topn, *pacc, pax, pay;
float pmin, pmax;
int *nacc, nax, nay;
float nmin, nmax, td;
unsigned char *cdata;
int w, h, dist_limit, reset;
{
   int i, need_pacc, need_nacc;
   int npix, x1, y1, x2, y2, lx, ly, rx, ry;
   int pmax_v, pmx, pmy;
   int nmax_v, nmx, nmy;
   int mmx, mmy;
   float slope;
   int nx, ny;
   int *tpacc, plen, *tnacc, nlen;
   int last, which, top_i;
   static int last_x1, last_y1, last_x2, last_y2, last_npix;

   my_trace("entered pick_max_pix_line\n");

   if(reset){
      last_x1 = NOT_FOUND;
      last_y1 = NOT_FOUND;
      last_x2 = NOT_FOUND;
      last_y2 = NOT_FOUND;
      last_npix = NOT_FOUND;
   }

   /* allocate working copies of hough line transform matrices */
   plen = pax * pay;
   malloc_int(&tpacc, plen, "pick_max_pix_line : tpacc");
   memcpy(tpacc, pacc, plen*sizeof(int));
   nlen = nax * nay;
   malloc_int(&tnacc, nlen, "pick_max_pix_line : tnacc");
   memcpy(tnacc, nacc, nlen*sizeof(int));

   need_pacc = 1;
   need_nacc = 1;
   *max_pix = 0;
   *mx1 = NOT_FOUND;
   *my1 = NOT_FOUND;
   *mx2 = NOT_FOUND;
   *my2 = NOT_FOUND;
   *mlx = NOT_FOUND;
   *mly = NOT_FOUND;
   *mrx = NOT_FOUND;
   *mry = NOT_FOUND;

   /* choose the line with the most black pixels from the set of "top_n" max lines */
   /* in the hough line transform matrices */
   for(i = 0; i < topn; i++){
      /* if needed, get the next max from the [0..+n] degrees matrix */
      if(need_pacc){
         max_2d_addr(tpacc, pax, pay, &pmax_v, &pmx, &pmy);
         need_pacc = 0;
      }
      /* if needed, get the next max from the [0..-n] degrees matrix */
      if(need_nacc){
         max_2d_addr(tnacc, nax, nay, &nmax_v, &nmx, &nmy);
         need_nacc = 0;
      }
      my_trace3("line %d: hough max = %d, limit = %d\n", i, max(pmax_v, nmax_v),
                dist_limit);
      /* if the next max is not "dominant" enough then stop */
      if(max(pmax_v, nmax_v) < dist_limit)
         break;

      /* if max from [0..+n] matrix ... */
      if(pmax_v >= nmax_v){
         /* erase max point from the working copy matrix, so not to be picked again */
         *(tpacc + (pmy * pax) + pmx) = 0;
         /* remember, hough matrix based on radial distance along "normal" vector */
         /* to the line */
         /* compute point and slope of the detected line based on location of max */
         /* in hough matrix */
         gen_slope_point(&slope, &nx, &ny, pmx, pmy, pmin, pmax, td, (pax>>1));
         /* need a new max from [0..+n] matrix */
         need_pacc = 1;
         /* set flag to [0..+n] matrix */
         last = 1;
      }
      /* if max from [0..-n] matrix ... */
      else{
         /* erase max point from the working copy matrix, so not to be picked again */
         *(tnacc + (nmy * nax) + nmx) = 0;
         /* remember, hough matrix based on radial distance along "normal" vector */
         /* to the line. */
         /* compute point and slope of the detected line based on location of max */
         /* in hough matrix */
         gen_slope_point(&slope, &nx, &ny, nmx, nmy, nmin, nmax, td, (nax>>1));
         /* need a new max from [0..-n] matrix */
         need_nacc = 1;
         /* set flag to [0..-n] matrix */
         last = 0;
      }
      /* project the detected line to the edges of the image */
      intersect_sides(&x1, &y1, &x2, &y2, nx, ny, slope, w, h);

      /* sum up the number of black pixels along this line projection */
      /* this black pixel count is much more accurate than the accumulated pixels */
      /* in the hough matrices, and should be used to choose actual lines */
      n_pixels_on_line(&npix, &lx, &ly, &rx, &ry, BLACK_PIX, x1, y1, x2, y2, cdata, w, h);

      /* if npix == 0, then line must have been already erased when processing */
      /* a previous line, so erase cell from appropriate global acc so it won't be */
      /* chosen again and go back and pick a new line */
      if(npix == 0){
         if(last)
            *(pacc + (pmy * pax) + pmx) = 0;
         else
            *(nacc + (nmy * nax) + nmx) = 0;
         /* pick a new line it this line's place */
         i--;
         my_trace("line was previously erased, so ignored\n");
      }
      /* update maximum black pixel line statistics */
      else if(npix > *max_pix){
         top_i = i;
         *mx1 = x1;
         *my1 = y1;
         *mx2 = x2;
         *my2 = y2;
         *mlx = lx;
         *mly = ly;
         *mrx = rx;
         *mry = ry;
         *max_pix = npix;
         which = last;
         /* store the actual locations in the hough matrices as well */
         if(last == 1){
            mmx = pmx;
            mmy = pmy;
         }
         else{
            mmx = nmx;
            mmy = nmy;
         }
      }
   }

   /* free the working copies */
   free(tpacc);
   free(tnacc);

   /* if the selected line has no black pixels, then the line must have been erased       */
   /* when another selected line was processed. this can happen because the lines in      */
   /* an image frequently have line widths greater than 1. so, for each line in an image, */
   /* there are multiple lines of width 1 overlaying the same line. therefore, there      */
   /* can be multple points in the hough line transform matrices representing the same    */
   /* line in the image. by selecting and processing one hough detected line, other lines */
   /* in the hough matrices may become obsolete. upon experiments, it was found to be     */
   /* easier and more accurate to detect these obsolete lines by checking for 0 black     */
   /* pixels after selection, rather than deleteing cells from hough matrices up front.   */
   if(*max_pix <= 0){
      /* it is possible for all "top_n" max hough lines to be obsolete with no black */
      /* pixels in them. possibly something could be done about this, but for now    */
      /* line processing will simply stop */
      /* this point is reached also when no more "dominant" lines are found in the */
      /* hough matrices */
      my_trace("no more dominant lines found in hough matrices\n");
      my_trace("finished pick_max_pix_line\n");
      return(FALSE);
   }

   /* if the same line is selected between subsequent calls to this routine, */
   /* then line searching should stop so as not to fall into an infinite loop */
   if((last_x1 == *mx1) &&
      (last_y1 == *my1) &&
      (last_x2 == *mx2) &&
      (last_y2 == *my2) &&
      (last_npix == *max_pix)){
      my_trace("uh-oh, selected same line as last time\n");
      my_trace("finished pick_max_pix_line\n");
      return(FALSE);
   }
   /* store the selected line into static variables */
   my_trace2("selected line %d, black pixels = %d\n", top_i, *max_pix);
   my_trace4("line projected from (%d, %d) to (%d, %d)\n", *mx1, *my1, *mx2, *my2);
   last_x1 = *mx1;
   last_y1 = *my1;
   last_x2 = *mx2;
   last_y2 = *my2;
   last_npix = *max_pix;

   /* must erase the chosen line from the appropriate master hough matrix */
   /* so it can't be selected again */
   if(which)
      *(pacc + (mmy * pax) + mmx) = 0;
   else
      *(nacc + (mmy * nax) + mmx) = 0;

   my_trace("finished pick_max_pix_line\n");
   return(TRUE);
}

/***************************************************************************/
/* pick_dash_line - takes the output from two hough line transforms        */
/* (one for [0..+n] degrees and the other for [0..-n] degrees) and locates */
/* the next "dominant" dashed line. The routine returns the endpoints of   */
/* the detected line (lx, ly) to (rx, ry), the endpoints of the line       */
/* projected to the edges of the field image, and the black pixels summed  */
/* along this trajectory.                                                  */
/***************************************************************************/
pick_dash_line(sum_pix, mx1, my1, mx2, my2, mlx, mly, mrx, mry,
                  topn, pacc, pax, pay, pmin, pmax, nacc, nax, nay, nmin, nmax, td,
                  cdata, w, h, dist_limit, reset)
int *sum_pix, *mx1, *my1, *mx2, *my2, *mlx, *mly, *mrx, *mry;
int topn, *pacc, pax, pay;
float pmin, pmax;
int *nacc, nax, nay;
float nmin, nmax, td;
unsigned char *cdata;
int w, h, dist_limit, reset;
{
   int i, need_pacc, need_nacc;
   int x1[TOPN], y1[TOPN], x2[TOPN], y2[TOPN];
   int lx[TOPN], ly[TOPN], rx[TOPN], ry[TOPN];
   int pmax_v, pmx, pmy;
   int nmax_v, nmx, nmy;
   int mmx, mmy, mxs[TOPN], mys[TOPN];
   float slope;
   int nx, ny;
   int *tpacc, plen, *tnacc, nlen;
   int which[TOPN];
   float bstd[TOPN], wstd[TOPN], max_stddev, tmax;
   int nstd;
   int npix[TOPN];
   float how_small, how_close;
   float max_score, score;
   int  max_score_i;
   static int last_x1, last_y1, last_x2, last_y2, last_npix;
   static float last_bstd, last_wstd;

   my_trace("entered pick_dash_line\n");
   if(reset){
      last_x1 = NOT_FOUND;
      last_y1 = NOT_FOUND;
      last_x2 = NOT_FOUND;
      last_y2 = NOT_FOUND;
      last_npix = NOT_FOUND;
      last_bstd = 0.0;
      last_wstd = 0.0;
   }

   /* allocate working copies of hough line transform matrices */
   plen = pax * pay;
   malloc_int(&tpacc, plen, "pick_dash_line : tpacc");
   memcpy(tpacc, pacc, plen*sizeof(int));
   nlen = nax * nay;
   malloc_int(&tnacc, nlen, "pick_dash_line : tnacc");
   memcpy(tnacc, nacc, nlen*sizeof(int));

   need_pacc = 1;
   need_nacc = 1;
   /* instead of looking for max black pixels, to find a dashed line am going to      */
   /* look for lowest stddev in both black and white pixel runs along a line          */
   /* trajectory. dashed lines should have very uniform black and white run durations */
   nstd = 0;
   max_stddev = 0;
   /* get the "top_n" selections based on summed pixels in the hough matrices */
   for(i = 0; i < topn; i++){
      /* if needed, get the next max from the [0..+n] matrix */
      if(need_pacc){
         max_2d_addr(tpacc, pax, pay, &pmax_v, &pmx, &pmy);
         need_pacc = 0;
      }
      /* if needed, get the next max from the [0..-n] matrix */
      if(need_nacc){
         max_2d_addr(tnacc, nax, nay, &nmax_v, &nmx, &nmy);
         need_nacc = 0;
      }
      my_trace3("line %d: hough max = %d, limit = %d\n",
                nstd, max(pmax_v, nmax_v), dist_limit);
      /* if the next max is not "dominant" enough then stop */
      /* dist_limit must be set much lower for dashed lines than with solid lines */
      if(max(pmax_v, nmax_v) < dist_limit)
         break;
   
      /* if max from [0..+n] matrix ... */
      if(pmax_v >= nmax_v){
         /* erase max point from the working copy matrix, so not to be picked again */
         *(tpacc + (pmy * pax) + pmx) = 0;
         /* remember, hough matrix based on radial distance along "normal" vector */
         /* to the line */
         /* compute point and slope of the detected line based on location of max */
         /* in hough matrix */
         gen_slope_point(&slope, &nx, &ny, pmx, pmy, pmin, pmax, td, (pax>>1));
         /* need a new max from [0..+n] matrix */
         need_pacc = 1;
         /* store which matrix the current line was selected from along with location */
         which[nstd] = 1;
         mxs[nstd] = pmx;
         mys[nstd] = pmy;
      }
      else{
         /* erase max point from the working copy matrix, so not to be picked again */
         *(tnacc + (nmy * nax) + nmx) = 0;
         /* remember, hough matrix based on radial distance along "normal" vector */
         /* to the line */
         /* compute point and slope of the detected line based on location of max */
         /* in hough matrix */
         gen_slope_point(&slope, &nx, &ny, nmx, nmy, nmin, nmax, td, (nax>>1));
         /* need a new max from [0..-n] matrix */
         need_nacc = 1;
         /* store which matrix the current line was selected from along with location */
         which[nstd] = 0;
         mxs[nstd] = nmx;
         mys[nstd] = nmy;
      }
      /* project the detected line to the edges of the image */
      intersect_sides(&(x1[nstd]), &(y1[nstd]), &(x2[nstd]), &(y2[nstd]),
                      nx, ny, slope, w, h);
      /* compute the stddev of black and white runs (and sum the black pixels) along */
      /* the selected line projection */
      stddev_run_widths(&(bstd[nstd]), &(wstd[nstd]), &(npix[nstd]),
                       &(lx[nstd]), &(ly[nstd]), &(rx[nstd]), &(ry[nstd]),
                       x1[nstd], y1[nstd], x2[nstd], y2[nstd], cdata, w, h);
      /* if npix == 0, then line must have been already erased when processing */
      /* a previous line, so erase cell from appropriate global acc so it won't be */
      /* chosen again and go back and pick a new line */
      if(npix[nstd] == 0){
         if(which[nstd])
            *(pacc + (pmy * pax) + pmx) = 0;
         else
            *(nacc + (nmy * nax) + nmx) = 0;
         /* pick a new line it this line's place */
         i--;
         my_trace("line was previously erased, so ignored\n");
      }
      /* if the selected line is not empty ... */
      else{
         /* keep track of the max stddev so far */
         tmax = max(bstd[nstd], wstd[nstd]);
         if(max_stddev < tmax)
            max_stddev = tmax;
        nstd++;
      }
   }

   /* free the working copies */
   free(tpacc);
   free(tnacc);

   /* if no lines were selected from the hough matrices then stop processing lines */
   if(nstd == 0){
      /* this point is reached when no more "dominant" lines are found in the */
      /* hough matrices */
      my_trace("no more dominant lines found in hough matrices\n");
      my_trace("finished pick_dash_line\n");
      *sum_pix = 0;
      return(FALSE);
   }

   /* pick the best dashed line from the "top_n" (more correctly, nstd) choices */
   max_score = NOT_FOUND;
   max_score_i = NOT_FOUND;
   /* score each line based on its black and white run stddevs. scores are computed */
   /* in two parts (how small, and how close). 1. the smaller the black and white run  */
   /* stddevs, the more likely a dashed line. 2. the closer the black and white runs */
   /* are to each other, the more likely a dashed line. */
   for(i = 0; i < nstd; i++){
      tmax = max(bstd[i], wstd[i]);
      /* compute how small the stddevs are to each other with respect max stddev */
      /* incurred in the "topn" lines */
      if(max_stddev != 0)
         how_small = (max_stddev - tmax) / max_stddev;
      else
         how_small = 0.0;
      /* compute how close the stddevs are to each other with respect to the */
      /* pairs max stddev */
      if(tmax != 0)
         how_close = 1.0 - (fabs(bstd[i] - wstd[i]) / tmax);
      else
         how_close = 0.0;
      /* average the two components together to get the line's score */
      score = (how_small + how_close) / 2.0;
      /* keep track of the max score */
      if(score > max_score){
         max_score = score;
         max_score_i = i;
      }
   }

   /* store selected dashed line statistics */
   *mx1 = x1[max_score_i];
   *my1 = y1[max_score_i];
   *mx2 = x2[max_score_i];
   *my2 = y2[max_score_i];
   *mlx = lx[max_score_i];
   *mly = ly[max_score_i];
   *mrx = rx[max_score_i];
   *mry = ry[max_score_i];

   *sum_pix = npix[max_score_i];
   mmx = mxs[max_score_i];
   mmy = mys[max_score_i];

   /* if the same line is selected between subsequent calls to this routine, */
   /* then line searching should stop so as not to fall into an infinite loop */
   if((last_bstd == bstd[max_score_i]) &&
      (last_wstd == wstd[max_score_i]) &&
      (last_x1 == *mx1) &&
      (last_y1 == *my1) &&
      (last_x2 == *mx2) &&
      (last_y2 == *my2) &&
      (last_npix == *sum_pix)){
      my_trace("uh-oh, selected same line as last time\n");
      my_trace("finished pick_max_pix_line\n");
      return(FALSE);
   }
   /* store the selected line into static variables */
   last_bstd = bstd[max_score_i];
   last_wstd = wstd[max_score_i];
   last_x1 = *mx1;
   last_y1 = *my1;
   last_x2 = *mx2;
   last_y2 = *my2;
   last_npix = *sum_pix;

   /* if the selected line has no black pixels, then the line must have been erased       */
   /* when another selected line was processed. this can happen because the lines in      */
   /* an image frequently have line widths greater than 1. so, for each line in an image, */
   /* there are multiple lines of width 1 overlaying the same line. therefore, there      */
   /* can be multple points in the hough line transform matrices representing the same    */
   /* line in the image. by selecting and processing one hough detected line, other lines */
   /* in the hough matrices may become obsolete. upon experiments, it was found to be     */
   /* easier and more accurate to detect these obsolete lines by checking for 0 black     */
   /* pixels after selection, rather than deleteing cells from hough matrices up front.   */
   if(*sum_pix <= 0){
      /* it is possible for all "top_n" max hough lines to be obsolete with no black */
      /* pixels in them. possibly something could be done about this, but for now  */
      /* line processing will simply stop */
      my_trace("line selected has no black pixels\n");
      my_trace("finished pick_dash_line\n");
      return(FALSE);
   }

   my_trace4("selected line %d, black stddev = %f, white stddev = %f, score = %f\n",
             max_score_i, bstd[max_score_i], wstd[max_score_i], max_score);
   my_trace4("line projected from (%d, %d) to (%d, %d)\n", *mx1, *my1, *mx2, *my2);
   /* must erase the chosen line from the appropriate master hough matrix */
   /* so it can't be selected again */
   if(which[max_score_i])
      *(pacc + (mmy * pax) + mmx) = 0;
   else
      *(nacc + (mmy * nax) + mmx) = 0;
   my_trace("finished pick_dash_line\n");
   return(TRUE);
}
