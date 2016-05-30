/*
# proc: rm_box_long_hori_sides - removes the relatively long top and bottom
# proc:                     sides of a box from within a binary char field image.
# proc: rm_box_long_vert_sides - carefully removes the  left and right vertical
# proc:                     sides of a box from within a binary char field image.
# proc: rm_box_short_vert_sides - removes the relatively short left and right vertical
# proc:                     sides of a box from within a binary char field image.
*/

#include <stdio.h>
#include <defs.h>

#define SHORT_VERT_WINDOW  0.125  /* 8th of an inch */
#define LONG_VERT_WINDOW   0.0625  /* 16th of an inch */
#define SHORT_PRCT_LENGTH  0.9
#define LONG_PRCT_LENGTH   0.75
#define SEARCH_WINDOW      0.5
/*******************************************************************/
rm_box_long_hori_lines(pts1x, pts1y, msws1, npts1, pts2x, pts2y, msws2, npts2,
                          cdata, bw, bh, bpi)
int **pts1x, **pts1y, **msws1, *npts1;
int **pts2x, **pts2y, **msws2, *npts2;
unsigned char *cdata;
int bw, bh, bpi;
{
   int sy2, sh1, sh2;
   sh1 = bh>>1;
   sy2 = sh1;
   sh2 = bh - sh1;
   if((rm_long_hori_line(pts1x, pts1y, msws1, npts1,
                     cdata, bw, sh1, bpi, None) == NOT_FOUND) ||
      (rm_long_hori_line(pts2x, pts2y, msws2, npts2,
                    (cdata + (sy2*bw)), bw, sh2, bpi, None) == NOT_FOUND))
      return(NOT_FOUND);
   else
      return(FOUND);
}

/***********************************************************************************/
rm_box_long_vert_sides(p1x, p1y, np1, p2x, p2y, np2, cdata, w, h, bpi)
int *p1x, *p1y, np1, *p2x, *p2y, np2;
unsigned char *cdata;
int w, h, bpi;
{
   int n1, dx, dw, sw, limit;
   int li, lx, ri, rx;
   int r2i;
   int lx1, ly1, lx2, lw, lh, llen, e1, e2;
   int rx1, ry1, rx2, rw, rh;
   unsigned char *sdata, *rdata, *mallocate_image();
   int *pts1x, *pts1y, *msws1, npts1;
   int *pts2x, *pts2y, *msws2, npts2;
   int *t1x, *t1y, tnp;

   /* 1/2 window size in pixels */
   dx = sround(bpi * LONG_VERT_WINDOW);

   sw = sround(bpi * SEARCH_WINDOW);

   n1 = min(np2, min(sw, np1 >> 1));

   /* list endpoints */
   e1 = np1 - 1;
   e2 = np2 - 1;

   /* line length threshold */
   limit = sround(LONG_PRCT_LENGTH * max(p2y[0]-p1y[0],p2y[e2]-p1y[e1]));

   /* allocate for the working space images */
   dw = (dx<<1)+1;
   sdata = mallocate_image(dw, h, 8);
   rdata = mallocate_image(h, dw, 8);

   /* locate leftmost black pixel along lines */
   if((li = find_pix_fwd_on_line(1, 0, n1, p1x, p1y, np1, cdata, w, h)) != NOT_FOUND)
      lx = p1x[li];
   else if((li = find_pix_fwd_on_line(1, 0, n1, p2x, p2y, np2, cdata, w, h)) != NOT_FOUND)
      lx = p2x[li];
   else{
      /* otherwise, split the difference */
      li = 0;
      lx = (p1x[0] + p2x[0])>>1;
   }
   lx1 = max(0, lx - dx);
   lx2 = min(w-1, lx + dx);
   ly1 = 0;
   lw = lx2 - lx1 + 1;
   lh = h;

   subimage_copy(cdata, w, h, sdata, lw, lh, lx1, ly1, lw, lh, 0, 0);
   grayscale_rotate_90(sdata, rdata, lw, lh);
   if(rm_long_hori_line(&pts1x, &pts1y, &msws1, &npts1,
                        rdata, lh, lw, bpi, limit) != NOT_FOUND){
      grayscale_rotate_n90(rdata, sdata, lh, lw);
      subimage_copy(sdata, lw, lh, cdata, w, h, 0, 0, lw, lh, lx1, ly1);
      free(pts1x);
      free(pts1y);
      free(msws1);
   }

   /* locate rightmost black pixel along lines */
   if((ri = find_pix_bwd_on_line(1, e1, max(0,e1-n1), p1x, p1y, np1,
                                 cdata, w, h)) != NOT_FOUND){
      rx = p1x[ri];
      t1x = p1x;
      t1y = p1y;
      tnp = np1;
   }
   else if((ri = find_pix_bwd_on_line(1, e2, max(0,e2-n1), p2x, p2y, np2,
                                      cdata, w, h)) != NOT_FOUND){
      rx = p2x[ri];
      t1x = p2x;
      t1y = p2y;
      tnp = np2;
   }
   else{
      ri = e1;
      rx = (p1x[e1]+p2x[e2])>>1;
      t1x = p1x;
      t1y = p1y;
      tnp = np1;
   }
   rx1 = max(0, rx - dx);
   rx2 = min(w-1, rx + dx);
   ry1 = 0;
   rw = rx2 - rx1 + 1;
   rh = h;

   subimage_copy(cdata, w, h, sdata, rw, rh, rx1, ry1, rw, rh, 0, 0);
   grayscale_rotate_90(sdata, rdata, rw, rh);
   if(rm_long_hori_line(&pts2x, &pts2y, &msws2, &npts2,
                        rdata, rh, rw, bpi, limit) != NOT_FOUND){
      grayscale_rotate_n90(rdata, sdata, rh, rw);
      subimage_copy(sdata, rw, rh, cdata, w, h, 0, 0, rw, rh, rx1, ry1);
      free(pts2x);
      free(pts2y);
      free(msws2);
   }

   /* if previous right window too close to edge of image ... */
   /* then maybe there is 2 right edges, 1 from current box and */
   /* one from right neighbor box */
   if(w - rx < dx){
      if((r2i = find_pix_bwd_on_line(1, max(0,ri-1), max(0,ri-n1), t1x, t1y, tnp,
                                    cdata, w, h)) != NOT_FOUND){
         rx = t1x[r2i];
         rx1 = max(0, rx - dx);
         rx2 = min(w-1, rx + dx);
         ry1 = 0;
         rw = rx2 - rx1 + 1;
         rh = h;

         subimage_copy(cdata, w, h, sdata, rw, rh, rx1, ry1, rw, rh, 0, 0);
         grayscale_rotate_90(sdata, rdata, rw, rh);
         if(rm_long_hori_line(&pts2x, &pts2y, &msws2, &npts2,
                              rdata, rh, rw, bpi, limit) != NOT_FOUND){
            grayscale_rotate_n90(rdata, sdata, rh, rw);
            subimage_copy(sdata, rw, rh, cdata, w, h, 0, 0, rw, rh, rx1, ry1);
            free(pts2x);
            free(pts2y);
            free(msws2);
         }
      }
   }

   free(sdata);
   free(rdata);
}

/***********************************************************************************/
rm_box_short_vert_sides(p1x, p1y, np1, p2x, p2y, np2, cdata, w, h, bpi)
int *p1x, *p1y, np1, *p2x, *p2y, np2;
unsigned char *cdata;
int w, h, bpi;
{
   int dx, lx1, ly1, lx2, ly2, lw, lh, llen, e1, e2;
   int rx1, ry1, rx2, ry2, rw, rh, rlen;
   int *bins, blen;
   int lthresh, rthresh;
   unsigned char *sdata, *mallocate_image();
   int mi, mv, fi, fv, ti, tv;

   dx = sround(bpi * SHORT_VERT_WINDOW);
   lx1 = max(0, min(p1x[0], p2x[0]) - dx);
   lx2 = min(w-1, max(p1x[0], p2x[0]) + dx);
   ly1 = p1y[0];
   ly2 = p2y[0];
   lw = lx2 - lx1 + 1;
   lh = ly2 - ly1 + 1;
   llen = lw * lh;
   e1 = np1-1;
   e2 = np2-1;
   rx1 = max(0, min(p1x[e1], p2x[e2]) - dx);
   rx2 = min(w-1, max(p1x[e1], p2x[e2]) + dx);
   ry1 = p1y[e1];
   ry2 = p2y[e2];
   rw = rx2 - rx1 + 1;
   rh = ry2 - ry1 + 1;
   rlen = rw * rh;

   /* allocate for the larger of the 2 regions */
   if(llen > rlen)
      sdata = mallocate_image(lw, lh, 8);
   else
      sdata = mallocate_image(rw, rh, 8);

   subimage_copy(cdata, w, h, sdata, lw, lh, lx1, ly1, lw, lh, 0, 0);
   chr_y_hist(sdata, lw, lh, &bins, &blen);
   lthresh = sround(SHORT_PRCT_LENGTH * lh);
   if(find_first_thresh_forward(bins, 0, blen, lthresh, &mi, &mv)){
     find_first_min_backward(bins, mi, 0, &fi, &fv);
     find_first_min_forward(bins, mi, blen, &ti, &tv);
     subimage_zeroes(cdata, w, h, lx1+fi, 0, ti-fi+1, h);
   }
   else{
     fprintf(stderr, "Warning: rm_box_short_vert_sides: left vertical edge not found\n");
   }
   free(bins);

   subimage_copy(cdata, w, h, sdata, rw, rh, rx1, ry1, rw, rh, 0, 0);
   chr_y_hist(sdata, rw, rh, &bins, &blen);
   rthresh = sround(SHORT_PRCT_LENGTH * rh);
   if(find_first_thresh_forward(bins, 0, blen, rthresh, &mi, &mv)){
     find_first_min_backward(bins, mi, 0, &fi, &fv);
     find_first_min_forward(bins, mi, blen, &ti, &tv);
     subimage_zeroes(cdata, w, h, rx1+fi, 0, ti-fi+1, h);
   }
   else{
     fprintf(stderr, "Warning: rm_box_short_vert_sides: right vertical edge not found\n");
   }
   free(bins);
   free(sdata);
}
