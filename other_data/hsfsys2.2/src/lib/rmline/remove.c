/*
# proc: rm_long_hori_line - intelligently removes a long horizontal line from a
# proc:                     binary char image while preserving data that
# proc:                     intersects the line.
*/

#include <stdio.h>
#include <rmline.h>
#include <mytrace.h>
#include <defs.h>

#define TOPN_RUNS      3

/***********************************************************************************/
rm_long_hori_line(optsx, optsy, omsws, onpts, cdata, w, h, bpi, limit)
int **optsx, **optsy, **omsws, *onpts;
int limit;
unsigned char *cdata;
int w, h, bpi;
{
   int *rx, *ry, *rlen, nruns, ralloc;
   int *bins, nbins, *rankyl;
   int ly, lv, slx, srx, thresh, pw;
   int *ptsx, *ptsy, npts, *mptsy;
   int *pts2x, *pts2y, npts2;
   int *ebin, *msws, *sw, *fsy, *tsy, nsw, winw;
   int *modefilteri(), *modesubfilteri();
   static float slope = ((PRCT2*PIX2) - (PRCT1*PIX1))/(PIX2 - PIX1);
   static float sy1 = PRCT1*PIX1;
   float compute_y();

   /* get horizontal runs in field */
   get_runs8(&rx, &ry, &rlen, &nruns, &ralloc, cdata, w, h);

   /* for each x along the entire width of the image */
   /* find the y corresponding to the longest overlapping run */
   max_overlap_run_along_x(&ptsx, &ptsy, &npts, 0, w-1, rx, ry, rlen, nruns, w, h);

   /* compute histogram from runs */
   hist_nruns_hori2(TOPN_RUNS, rx, ry, rlen, nruns, w, h, &bins, &nbins, &rankyl);
   free(rx);
   free(ry);
   free(rlen);

   /* search for histogram bin exceeding threshold */
   if(limit == None)
      thresh = sround(compute_y((float)w, slope, PIX1, sy1));
   else
      thresh = limit;

   if(!find_first_thresh_forward(bins, 0, nbins, thresh, &ly, &lv)){
      find_first_max_forward(bins, 0, nbins, &ly, &lv);
      free(rankyl);
      free(bins);
      fprintf(stderr, "Warning: rm_long_hori_line: line to be removed not found\n");
      return(NOT_FOUND);
   }
   free(rankyl);
   free(bins);

   /* smooth the line trajectory */
   winw = ((int)(sround(bpi * FILTER_WIN_1)/2)<<1)+1;
   if(winw > npts){
      if(npts % 2 == 0)
         winw = npts - 1;
      else
         winw = npts;
   }
   mptsy = modefilteri(ptsy, npts, winw);
   free(ptsy);
   ptsy = mptsy;

   /* fill in discontinuities in x along trajectory */
   fill_line_breaks(&pts2x, &pts2y, &npts2, ptsx, ptsy, npts);
   free(ptsx);
   free(ptsy);

   /* intelligently erase line along computed trajectory */
   slice_widths_hori_nb(&sw, &fsy, &tsy, &nsw, pts2x, pts2y, npts2, cdata, w, h);
   if(!find_first_thresh_forward(sw, 0, nsw, 0, &slx, &lv) ||
      !find_first_thresh_backward(sw, 0, nsw, 0, &srx, &lv))
      fatalerr("rm_long_hori_line", "no slices found for detected line", NULL);

   winw = ((int)(sround(bpi * FILTER_WIN_2)/2)<<1)+1;
   pw = srx - slx + 1;
   if(winw > pw){
      if(pw % 2 == 0)
         winw = pw - 1;
      else
         winw = pw;
   }
   msws = modesubfilteri(slx, srx, sw, nsw, winw);
   erase_slices_hori_ml(&ebin, msws, sw, fsy, tsy, nsw, cdata, w, h);
   erase_fuzzy_slices_hori_ml(ebin, msws[slx], msws, sw, fsy, tsy, nsw, cdata, w, h);
   unerase_short_voids_ml(ebin, msws, sw, fsy, tsy, nsw, cdata, w, h);
   unerase_curved_voids_ml(ebin, msws, sw, fsy, tsy, nsw, cdata, w, h);
   process_char_pieces_ml(ebin, msws, sw, fsy, tsy, nsw, cdata, w, h);
   free(ebin);
   free(sw);
   free(fsy);
   free(tsy);

   *optsx = pts2x;
   *optsy = pts2y;
   *omsws = msws;
   *onpts = npts2;
   return(FOUND);
}
