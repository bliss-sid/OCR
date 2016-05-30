/*
# proc: calc_trans - computes the disparity in x and y between "dominant"
# proc:              structures in two images.
# proc: find_trans_reg_points - locates dominant structures in an image.
# proc:
# proc: chk_trans_reg_points - computes and checks the dispairty in x and y.
# proc:
*/

#include <stdio.h>
#include <regform.h>
#include <defs.h>

/*****************************************************************************/
calc_trans(tx, ty, cdata, w, h, refs, nref, errlimit, rejlimit)
int *tx, *ty;
unsigned char *cdata;
int w, h;
int *refs, nref;
int errlimit, rejlimit;
{
   int hyps[NUM_REG_PTS];

   if(nref != NUM_REG_PTS)
      fatalerr("calc_trans", "must have 4 registrations points (l,r,t,b)",
               NULL);
   find_trans_reg_points(&(hyps[0]), &(hyps[1]), &(hyps[2]), &(hyps[3]),
                         cdata, w, h);

   chk_trans_reg_points(tx, ty, hyps, refs, nref, errlimit);

   if(rejlimit != -1){
      if((abs(*tx) > rejlimit) || (abs(*ty) > rejlimit))
         return(FALSE);
   }
   return(TRUE);
}

/*****************************************************************************/
find_trans_reg_points(left, right, top, bot, cdata, w, h)
int *left, *right, *top, *bot;
unsigned char *cdata;
int w, h;
{
   int *rx, *ry, *rlen, nruns, ralloc;
   int *bins, nbins;
   unsigned char *rdata, *mallocate_image();
   int mi, mv, tv, thresh;

   /* get all horizontal runs in the image */
   get_runs8(&rx, &ry, &rlen, &nruns, &ralloc, cdata, w, h);
   hist_nruns_hori(TOPN_RUNS, rx, ry, rlen, nruns, w, h, &bins, &nbins);
   free(rx);
   free(ry);
   free(rlen);
   find_first_max_forward(bins, 0, nbins, &mi, &mv);
   thresh = (int)(mv*MAX_BIN_PRCT);
   find_first_thresh_forward(bins, 0, nbins, thresh, top, &tv);
   find_first_thresh_backward(bins, 0, nbins, thresh, bot, &tv);
   free(bins);

   /* get all vertical runs in the image */
   rdata = mallocate_image(h, w, 8);
   grayscale_rotate_90(cdata, rdata, w, h);
   get_runs8(&rx, &ry, &rlen, &nruns, &ralloc, rdata, h, w);
   free(rdata);
   hist_nruns_hori(TOPN_RUNS, rx, ry, rlen, nruns, h, w, &bins, &nbins);
   free(rx);
   free(ry);
   free(rlen);
   find_first_max_forward(bins, 0, nbins, &mi, &mv);
   thresh = (int)(mv*MAX_BIN_PRCT);
   find_first_thresh_forward(bins, 0, nbins, thresh, left, &tv);
   find_first_thresh_backward(bins, 0, nbins, thresh, right, &tv);
   free(bins);
}

/*****************************************************************************/
chk_trans_reg_points(tx, ty, hyps, refs, npts, errlimit)
int *tx, *ty;
int *hyps, *refs, npts;
int errlimit;
{
   int i, err[NUM_REG_PTS], aerr[NUM_REG_PTS];
   int llimit, ulimit;
   int minx, miny;

   if(npts != NUM_REG_PTS)
      fatalerr("chk_trans_reg_points",
               "must have 4 registrations points (l,r,t,b)", NULL);

   /* compute singed and unsigned error distances */
   for(i = 0; i < npts; i++){
      err[i] = hyps[i] - refs[i];
      aerr[i] = abs(err[i]);
   }

   /* choose smallest unsigned error in both x and y */
   minx = (aerr[0] < aerr[1]) ? 0 : 1;
   miny = (aerr[2] < aerr[3]) ? 2 : 3;

   /* compute error tolerances about smallest error in x */
   llimit = err[minx] - errlimit;
   ulimit = err[minx] + errlimit;
   /* test left and right hypotheses */
   if(is_in_range(err[0] , llimit, ulimit)){
      if(is_in_range(err[1], llimit, ulimit)){
         *tx = sround((err[0]+err[1])/2.0);
      }
      else{
         *tx = err[0];
      }
   }
   else{
      if(is_in_range(err[1], llimit,ulimit)){
         *tx = err[1];
      }
      else
         fatalerr("chk_trans_reg_points", "form registration failed",
               "left and right registration errors too large");
   }

   /* compute error tolerances about smallest error in y */
   llimit = err[miny] - errlimit;
   ulimit = err[miny] + errlimit;
   /* test top and bottom hypotheses */
   if(is_in_range(err[2], llimit, ulimit)){
      if(is_in_range(err[3], llimit, ulimit)){
         *ty = sround((err[2]+err[3])/2.0);
      }
      else{
         *ty = err[2];
      }
   }
   else{
      if(is_in_range(err[3], llimit, ulimit)){
         *ty = err[3];
      }
      else
         fatalerr("chk_trans_reg_points", "form registration failed",
               "top and bottom registration errors too large");
   }
}
