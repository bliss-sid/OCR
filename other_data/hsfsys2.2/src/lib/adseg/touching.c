/*
# proc: split_touching_chars - separates multiple touching characters into individual
# proc:                        characters.
# proc: split_touching_alphas - separates multiple touching characters into individual
# proc:                        alphabetic characters.
# proc: segment_blob_as_pair - attempts to split touching character image based on
# proc:                        assumption that only two charcters are in the image.
# proc: segment_blob_fr_left - attempts to split off a character from within the image
# proc:                        of touching charcters by working from left-to-right.
*/

#include <stdio.h>
#include <blobls.h>
#include <segchars.h>
#include <defs.h>

/*******************************************************************/
split_touching_chars(blob, esw, charh, dlm, dlx, dly, blobls, noisels)
BLOB *blob;
BLOBLS **blobls, *noisels;
int esw, charh;
float dlm, dlx, dly;
{
   BLOB *sblob, *lblob, *rblob, *dup_blob();
   int *px, *py, pn, pa;
   int *lx, ln, la;
   float *ld;
   int segflag, newsblob;
   unsigned char *mallocate_image();

   /* create working copy of input blob */
   sblob = dup_blob(blob, (*blobls)->bpp);
   newsblob = 0;

   do{
         /* try to get cut point as a character pair */
      if(segment_blob_as_pair(&lblob, &rblob, &px, &py, &pn, &pa, &lx, &ld, &ln, &la,
                              esw, charh, dlm, dlx, dly, sblob) ||
         /* try to get cut point from left piece of image */
         segment_blob_fr_left(&lblob, &rblob, &px, &py, &pn, &pa, lx, ld, ln,
                              esw, charh, dlm, dlx, dly, sblob)){
         segflag = TRUE;

         /* compute absolute origin */
         lblob->x1 += sblob->x1;
         lblob->y1 += sblob->y1;
         /* compute new blob stats, area already computed */
         compute_blob_stats(lblob, (*blobls)->flags);
         /* add left blob to the global list */
         append_blobls(*blobls, lblob);
         free(px);
         free(py);

         /* update right piece cut x, y */
         rblob->x1 += sblob->x1;
         rblob->y1 += sblob->y1;
         /* manually compute pixel count in right blob */
         rblob->pixcnt = sblob->pixcnt - lblob->pixcnt;
         free_blob(sblob);
         /* assign to working blob */
         sblob = rblob;
         newsblob = 1;
      }
      else
         segflag = FALSE;
      if(lx != (int *)NULL)
         free(lx);
      if(ld != (float *)NULL)
         free(ld);
   } while(segflag && detect_multi_char_blob(sblob, esw, charh, dlm, dlx, dly));

   /* if working blob changed, compute new blob stats */
   if(newsblob)
      compute_blob_stats(sblob, (*blobls)->flags);

   /* if last blob too small, add to noise list */
   if(is_noise_blob(sblob, esw))
      append_blobls(noisels, sblob);
   else
      append_blobls(*blobls, sblob);
}

/*******************************************************************/
split_touching_alphas(blob, esw, charh, dlm, dlx, dly, blobls, noisels)
BLOB *blob;
BLOBLS **blobls, *noisels;
int esw, charh;
float dlm, dlx, dly;
{
   BLOB *sblob, *lblob, *rblob, *dup_blob();
   int *px, *py, pn, pa;
   int *lx, ln, la;
   float *ld;
   int segflag, newsblob;
   unsigned char *mallocate_image();

   /* create working copy of input blob */
   sblob = dup_blob(blob, (*blobls)->bpp);
   newsblob = 0;

   do{
         /* try to get cut point as a character pair */
      if(segment_blob_as_pair(&lblob, &rblob, &px, &py, &pn, &pa, &lx, &ld, &ln, &la,
                              esw, charh, dlm, dlx, dly, sblob) ||
         /* try to get cut point from left piece of image */
         segment_blob_fr_left(&lblob, &rblob, &px, &py, &pn, &pa, lx, ld, ln,
                              esw, charh, dlm, dlx, dly, sblob)){
         segflag = TRUE;

         /* compute absolute origin */
         lblob->x1 += sblob->x1;
         lblob->y1 += sblob->y1;
         /* compute new blob stats, area already computed */
         compute_blob_stats(lblob, (*blobls)->flags);
         /* add left blob to the global list */
         append_blobls(*blobls, lblob);
         free(px);
         free(py);

         /* update right piece cut x, y */
         rblob->x1 += sblob->x1;
         rblob->y1 += sblob->y1;
         /* manually compute pixel count in right blob */
         rblob->pixcnt = sblob->pixcnt - lblob->pixcnt;
         free_blob(sblob);
         /* assign to working blob */
         sblob = rblob;
         newsblob = 1;
      }
      else
         segflag = FALSE;
      if(lx != (int *)NULL)
         free(lx);
      if(ld != (float *)NULL)
         free(ld);
   } while(segflag && detect_multi_alpha_blob(sblob, esw, charh, dlm, dlx, dly));

   /* if working blob changed, compute new blob stats */
   if(newsblob)
      compute_blob_stats(sblob, (*blobls)->flags);

   /* if last blob too small, add to noise list */
   if(is_noise_blob(sblob, esw))
      append_blobls(noisels, sblob);
   else
      append_blobls(*blobls, sblob);
}

/************************************************************/
segment_blob_as_pair(lblob, rblob, px, py, pn, pa, lx, ld, ln, la,
                     esw, charh, dlm, dlx, dly, sblob)
BLOB **lblob, **rblob, *sblob;
int **px, **py, *pn, *pa;
int **lx, *ln, *la;
int esw, charh;
float **ld, dlm, dlx, dly;
{
   int cut_x;

   /* compute cut starting points and split blob image */
   if(blob_cut_x_as_pair(&cut_x, lx, ld, ln, la,
                            esw, esw, charh, dlm, dlx, dly, sblob) &&
      blob_split_n_test(lblob, rblob, px, py, pn, pa, cut_x, esw, sblob))
      return(TRUE);
   else
      return(FALSE);
}

/*******************************************************************/
segment_blob_fr_left(lblob, rblob, px, py, pn, pa, lx, ld, ln,
                     esw, charh, dlm, dlx, dly, sblob)
BLOB **lblob, **rblob, *sblob;
int **px, **py, *pn, *pa;
int *lx, ln;
int esw, charh;
float *ld, dlm, dlx, dly;
{
   BLOB *tblob;
   int segflag, cut_x, min_i;
   float min_v, *td;
   int *tx, tn, ta;
   unsigned char *mallocate_image();
   static int flags = BLOB_XY1S| BLOB_WHS | BLOB_AS;

   /* if no cut stats from image, then nothing to segment */
   if(ln == 0)
      segflag = FALSE;
   /* otherwise cut off a piece of the image and treat it like a character pair */
   else{
      /* find minimum distance point from cut stats */
      minpv(ld, ln, &min_v, &min_i);
      /* get piece from larger image approx. 2 chars in size */
      cut_x = min(lx[min_i]*CUT_FACTOR, sblob->w-esw);
      /* create a temporary blob of the smaller image */
      build_blob(&tblob, flags, mallocate_image(cut_x, sblob->h, 8),
                 sblob->x1, sblob->y1, cut_x, sblob->h);
      subimage_copy(sblob->data, sblob->w, sblob->h, tblob->data, tblob->w, tblob->h,
                    0, 0, tblob->w, tblob->h, 0, 0);

      if(segflag = segment_blob_as_pair(lblob, rblob, px, py, pn, pa, &tx, &td, &tn, &ta,
                                        esw, charh, dlm, dlx, dly, tblob)){
         free_blob(*lblob);
         free_blob(*rblob);
         /* make same left/right cut from larger image */
         split_blob_along_path(lblob, rblob, *px, *py, *pn, sblob);
      }
      if(tx != (int *)NULL)
         free(tx);
      if(td != (float *)NULL)
         free(td);
      free_blob(tblob);
   }
   return(segflag);
}
