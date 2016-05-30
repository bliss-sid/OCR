/*
# proc: iso_handprint_in_box - isolates the handprint written within the given
# proc:                   box-field subimage and preserves character strokes that
# proc:                   intersect the box.
# proc: lift_chars_fr_zone - returns a composit image and a list of connected
# proc:                   components extracted from the specified subimage.
# proc: next_pix_in_subimage - locates the next black pixel in a subimage search.
# proc:
*/

#include <stdio.h>
#include <blobls.h>
#include <findblob.h>
#include <rmline.h>
#include <defs.h>

#define FIELD_PAD 0.125 /* 8th of an inch */

/*********************************************************************/
iso_handprint_in_box(blobls, fcdata, fx, fy, fw, fh, rlen, rn, ra,
                     icdata, bpi, iw, ih, x1, y1, x2, y2)
BLOBLS **blobls;
unsigned char **fcdata;
int *fx, *fy, *fw, *fh;
int **rlen, *rn, *ra;
unsigned char *icdata;
int bpi, iw, ih, x1, y1, x2, y2;
{
   unsigned char *ocdata, *scdata, *rcdata, *mallocate_image();
   int pad;
   int sx1, sw1, sh1;
   int sx2, sy2, sw2, sh2;
   int *pts1x, *pts1y, *msws1, npts1;
   int *pts2x, *pts2y, *msws2, npts2;
   int tw, th;
   int zx1, zy1, zx2, zy2;
   int mi;
   int limit;
   static float slope = ((PRCT2*PIX2) - (PRCT1*PIX1))/(PIX2 - PIX1);
   static float ty1 = PRCT1*PIX1;
   float compute_y();

   /* extract padded field subimage */
   pad = sround(FIELD_PAD * bpi);
   *fx = max(0, x1 - pad);
   *fy = max(0, y1 - pad);
   *fw = min(iw-1, x2 - *fx + 1 + pad);
   *fh = min(ih-1, y2 - *fy + 1 + pad);
   *fcdata = mallocate_image(*fw, *fh, 8);
   subimage_copy(icdata, iw, ih, *fcdata, *fw, *fh, *fx, *fy, *fw, *fh, 0, 0);

   tw = x2 - x1 + 1;
   limit = sround(compute_y((float)tw, slope, PIX1, ty1));

   /* find and remove top edge of box */
   sh1 = min(3*pad, (*fh)>>1);
   if(rm_long_hori_line(&pts1x, &pts1y, &msws1, &npts1,
                        *fcdata, (*fw), sh1, bpi, limit) == NOT_FOUND)
      return(FALSE);
   /* compute top of zone to lift characters */
   mi = npts1>>1;
   zy1 = pts1y[mi];
   free(pts1x);
   free(pts1y);
   free(msws1);

   /* find and remove bottom edge of box */
   sy2 = (*fh) - sh1;
   sh2 = sh1;
   if(rm_long_hori_line(&pts2x, &pts2y, &msws2, &npts2,
              ((*fcdata) + (sy2*(*fw))), (*fw), sh2, bpi, limit) == NOT_FOUND)
      return(FALSE);
   /* compute bottom of zone to lift characters */
   mi = npts2>>1;
   zy2 = sy2 + pts2y[mi];
   free(pts2x);
   free(pts2y);
   free(msws2);

   th = y2 - y1 + 1;
   limit = sround(compute_y((float)th, slope, PIX1, ty1));

   /* find and remove left edge of box */
   sx1 = 0;
   sw1 = min(3*pad, (*fw)>>1);
   scdata = mallocate_image(sw1, *fh, 8);
   rcdata = mallocate_image(*fh, sw1, 8);
   subimage_copy(*fcdata, *fw, *fh, scdata, sw1, *fh, sx1, 0, sw1, *fh, 0, 0);
   grayscale_rotate_90(scdata, rcdata, sw1, *fh);
   if(rm_long_hori_line(&pts1x, &pts1y, &msws1, &npts1,
                        rcdata, (*fh), sw1, bpi, limit) == NOT_FOUND){
      free(scdata);
      free(rcdata);
      return(FALSE);
   }
   grayscale_rotate_n90(rcdata, scdata, *fh, sw1);
   subimage_copy(scdata, sw1, *fh, *fcdata, *fw, *fh, 0, 0, sw1, *fh, sx1, 0);
   /* compute left of zone to lift characters */
   mi = npts1>>1;
   zx1 = pts1y[mi];
   free(pts1x);
   free(pts1y);
   free(msws1);

   /* find and remove right edge of box */
   sx2 = (*fw) - sw1;
   sw2 = sw1;
   subimage_copy(*fcdata, *fw, *fh, scdata, sw2, *fh, sx2, 0, sw2, *fh, 0, 0);
   grayscale_rotate_90(scdata, rcdata, sw2, *fh);
   if(rm_long_hori_line(&pts2x, &pts2y, &msws2, &npts2,
                        rcdata, (*fh), sw2, bpi, limit) == NOT_FOUND){
      free(scdata);
      free(rcdata);
      return(FALSE);
   }
   grayscale_rotate_n90(rcdata, scdata, *fh, sw2);
   subimage_copy(scdata, sw2, *fh, *fcdata, *fw, *fh, 0, 0, sw2, *fh, sx2, 0);
   free(rcdata);
   free(scdata);
   /* compute right of zone to lift characters */
   mi = npts2>>1;
   zx2 = sx2 + pts2y[mi];
   free(pts2x);
   free(pts2y);
   free(msws2);

   /* this routine is destructive to fcdata */
   lift_chars_fr_zone(blobls, &ocdata, rlen, rn, ra,
                      *fcdata, *fw, *fh, zx1, zy1, zx2, zy2);

   free(*fcdata);
   *fcdata = ocdata;

   return(TRUE);
}


/****************************************************************************/
lift_chars_fr_zone(blobls, ocdata, rlen, rn, ra, fcdata, fw, fh,
                   zx1, zy1, zx2, zy2)
BLOBLS **blobls;
unsigned char **ocdata, *fcdata;
int **rlen, *rn, *ra;
int fw, fh;
int zx1, zy1, zx2, zy2;
{
   int i, px, py;
   unsigned char *bcdata, *allocate_image();
   int ret, bx, by, bw, bh;
   RUN *runs, *run_t, *run_off, *rptr;
   int npixels, sn, tflags, bflags;
   BLOB *blob;

   *ocdata = allocate_image(fw, fh, 8);

   /* allocate blob list */
   bflags = BLOB_ALLS;
   build_blobls(blobls, BLOB_CHUNKS, bflags, 8, BLOB_BIN);

   /* initialize run length statistics */
   malloc_int(rlen, CHUNKS, "lift_chars_fr_zone : malloc : rlen");
   *rn = 0;
   *ra = CHUNKS;

   /* initialize black pixel search */
   px = zx1;
   py = zy1;
   while(next_pix_in_subimage(1, &px, &py, zx1, zy1, zx2, zy2,
                              fcdata, fw, fh) == FOUND){
      ret = findblobnruns8(fcdata, fw, fh, ERASE, ALLOC, BOUND_BLOB, &px, &py,
                           &bcdata, &bx, &by, &bw, &bh, &runs, &run_t, &run_off);
      switch (ret){
      case 0:
         fatalerr("lift_chars_fr_zone",
                   "no blob found when black pixel detected", NULL);
         break;
      case 1:
         /* compute and store run lengths */
         sn = *rn;
         for(rptr = runs; rptr < run_t; rptr++){
            if(*rn >= *ra){
               (*ra) += CHUNKS;
               realloc_int(rlen, *ra, "lift_chars_fr_zone : realloc : rlen");
            }
            (*rlen)[*rn] = rptr->e_off - rptr->w_on;
            (*rn)++;
         }
         /* count black pixels from runlengths */
         npixels = 0;
         for(i = sn; i < *rn; i++)
            npixels += (*rlen)[i];

         /* or the blob into output field image */
         subimage_or(bcdata, bw, bh, (*ocdata), fw, fh, 0, 0, bw, bh, bx, by);

         /* already have pixel count, so don't compute inside build_blob() */
         tflags = bflags & ~BLOB_PIXCNTS;
         /* allocate and initialize blob structure */
         build_blob(&blob, tflags, bcdata, bx, by, bw, bh);
         blob->pixcnt = npixels;
         /* append new blob to the list */
         append_blobls((*blobls), blob);
         break;
      default:
         fatalerr("lift_chars_fr_zone",
                  "received illegal return code", "findblob");
         break;
      }
   }

   end_findblobs();
}

/****************************************************************************/
next_pix_in_subimage(pix, px, py, x1, y1, x2, y2, cdata, w, h)
int pix;
int *px, *py;
int x1, y1, x2, y2;
unsigned char *cdata;
int w, h;
{
   unsigned char *cptr;
   int cx, cy;

   cx = *px;
   cy = *py;

   cptr = cdata + (cy * w) + cx;

   while(cx <= x2){
      while(cy <= y2){
         if(*cptr == pix){
            *px = cx;
            *py = cy;
            return(FOUND);
         }
         cy++;
         cptr += w;
      }
      cy = y1;
      cx++;
      cptr = cdata + (cy * w) + cx;
   }
   return(NOT_FOUND);
}
