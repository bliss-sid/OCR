/*
# proc: blob_cut_x_as_pair - use multiple character detector to select an optimal
# proc:                      vertical cut point in an image of two touching characters.
# proc: blob_cut_x_fr_left - use multiple character detector to select an optimal
# proc:                      vertical cut point working from left-to-right in an image
# proc:                      containing multiple touching characters.
*/

#include <stdio.h>
#include <blobls.h>
#include <segchars.h>
#include <defs.h>

/*******************************************************************/
blob_cut_x_as_pair(cut_x, lx, ld, ln, la, start_x, esw, charh, dlm, dlx, dly, blob)
int *cut_x, start_x;
int **lx, *ln, *la;
int esw, charh;
float **ld, dlm, dlx, dly;
BLOB *blob;
{
   int x, xlimit;
   int lblack, rblack, col_black;
   float l_hvb_fet, l_hvw_fet, l_thresh_y, l_dist;
   float r_hvb_fet, r_hvw_fet, r_thresh_y, r_dist;
   float perp_dist(), compute_y();
   int *cx, cn, ca;
   float *cd, min_v;
   int min_i;
   unsigned char *cdata;
   int w, h;
   static int oesw = -1, ocharh = -1, odlx = -1, std_stroke, xmax;

   if((oesw != esw) || (ocharh != charh) || (odlx != dlx)){
      oesw = esw;
      ocharh = charh;
      odlx = dlx;
      std_stroke = esw * charh;
      xmax = sround(dlx*charh);
   }

   *lx = (int *)NULL;
   *ld = (float *)NULL;
   *ln = 0;

   cdata = blob->data;
   w = blob->w;
   h = blob->h;

   /* preempt checking for cut point in really wide images */
   xlimit = max(1, min(blob->w-esw, xmax));

   /* initialize left and right pixel counts at starting position */
   if((start_x > 0) && (start_x < xlimit)){
      lblack = subpixelcnt8(cdata, w, h, 0, 0, start_x-1, h);
      rblack = subpixelcnt8(cdata, w, h, start_x-1, 0, w-start_x+1, h);
   }
   else
      /* otherwise nothing to cut */
      return(FALSE);

   /* initialize and compute cut statistics */
   malloc_int(&cx, CHUNKS, "blob_cut_x_as_pair : malloc : cx");
   malloc_flt(&cd, CHUNKS, "blob_cut_x_as_pair : malloc : cd");
   cn = 0;
   ca = CHUNKS;

   malloc_int(lx, CHUNKS, "blob_cut_x_as_pair : malloc : lx");
   malloc_flt(ld, CHUNKS, "blob_cut_x_as_pair : malloc : ld");
   *ln = 0;
   *la = CHUNKS;

   for(x = start_x; x < xlimit; x++){

      /* count black pixels along current cut column */
      col_black = subpixelcnt8(cdata, w, h, x, 0, 1, h);

      /* compute left subimage features */
      lblack += col_black;
      l_hvb_fet = lblack/(float)std_stroke;
      l_hvw_fet = x/(float)charh;
      l_thresh_y = compute_y(l_hvw_fet, dlm, dlx, dly);

      /* if left piece is exceeds threshold, then stop */
      if(l_hvb_fet >= l_thresh_y)
         break;
      else{
         /* compute and store l_dist for every x, this will be used later */
         /* if this routine fails to produce a cut point */
         l_dist = perp_dist(l_hvw_fet, l_hvb_fet, dlx, dly, dlm);
         if(*ln >= *la){
            (*la) += CHUNKS;
            realloc_int(lx, *la, "blob_cut_x_as_pair : realloc : lx");
            realloc_flt(ld, *la, "blob_cut_x_as_pair : realloc : ld");
         }
         (*lx)[*ln] = x;
         (*ld)[*ln] = l_dist;
         (*ln)++;

         /* compute right subimage features */
         rblack -= col_black;
         r_hvb_fet = rblack/(float)std_stroke;
         r_hvw_fet = (w-x)/(float)charh;
         r_thresh_y = compute_y(r_hvw_fet, dlm, dlx, dly);

         /* is right peice also less than its threshold? */
         if(r_hvb_fet < r_thresh_y){
            r_dist = perp_dist(r_hvw_fet, r_hvb_fet, dlx, dly, dlm);
            if(cn >= ca){
               ca += CHUNKS;
               realloc_int(&cx, ca, "blob_cut_x_as_pair : realloc : cx");
               realloc_flt(&cd, ca, "blob_cut_x_as_pair : realloc : cd");
            }
            cx[cn] = x;
            cd[cn] = (l_dist > r_dist)?l_dist:r_dist;
            cn++;
         }
      }
   }
   if(cn == 0){
      free(cx);
      free(cd);
      return(FALSE);
   }
   else{
      minpv(cd, cn, &min_v, &min_i);
      *cut_x = cx[min_i];
      free(cx);
      free(cd);
      return(TRUE);
   }
}

/*******************************************************************/
blob_cut_x_fr_left(cut_x, start_x, esw, charh, dlm, dlx, dly, blob)
BLOB *blob;
int *cut_x, start_x;
int esw, charh;
float dlm, dlx, dly;
{
   int x, xlimit, min_i;
   int lblack, col_black;
   float l_hvb_fet, l_hvw_fet, l_thresh_y, l_dist;
   float perp_dist(), compute_y();
   int *cx, cn, ca;
   float *cd, min_v;
   unsigned char *cdata;
   int w, h;
   static int oesw = -1, ocharh = -1, odlx = -1, std_stroke, xmax;

   if((oesw != esw) || (ocharh != charh) || (odlx != dlx)){
      oesw = esw;
      ocharh = charh;
      odlx = dlx;
      std_stroke = esw * charh;
      xmax = sround(dlx*charh);
   }

   cdata = blob->data;
   w = blob->w;
   h = blob->h;

   /* preempt checking for cut point in really wide images */
   xlimit = max(1, min(blob->w-esw, xmax));

   /* count black pixels on both sides of current column */
   if((start_x > 0) && (start_x < xlimit))
      lblack = subpixelcnt8(cdata, w, h, 0, 0, start_x-1, h);
   else
      return(FALSE);

   /* initialize and compute cut statistics */
   malloc_int(&cx, CHUNKS, "blob_cut_x_fr_left : malloc : cx");
   malloc_flt(&cd, CHUNKS, "blob_cut_x_fr_left : malloc : cd");
   cn = 0;
   ca = CHUNKS;

   for(x = start_x; x < xlimit; x++){

      /* count black pixels along current cut column */
      col_black = subpixelcnt8(cdata, w, h, x, 0, 1, h);
      lblack += col_black;

      /* compute left subimage features */
      l_hvb_fet = lblack/(float)std_stroke;
      l_hvw_fet = x/(float)charh;

      l_thresh_y = compute_y(l_hvw_fet, dlm, dlx, dly);

      if(l_hvb_fet >= l_thresh_y)
         break;
      else{
         l_dist = perp_dist(l_hvw_fet, l_hvb_fet, dlx, dly, dlm);
         if(cn >= ca){
            ca += CHUNKS;
            realloc_int(&cx, ca, "blob_cut_x_fr_left : realloc : cx");
            realloc_flt(&cd, ca, "blob_cut_x_fr_left : realloc : cd");
         }
         cx[cn] = x;
         cd[cn] = l_dist;
         cn++;
      }
   }

   if(cn == 0){
      free(cx);
      free(cd);
      return(FALSE);
   }
   else{
      minpv(cd, cn, &min_v, &min_i);
      *cut_x = cx[min_i];
      free(cx);
      free(cd);
      return(TRUE);
   }
}
