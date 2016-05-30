/*
# proc: segblobs8 - takes a char image and returns all the connected components in a list.
# proc:
# proc: segblobs8rlen - takes a char image and returns all the connected components and the
# proc:                 list of run lengths for the entire image.
*/

#include <stdio.h>
#include <findblob.h>
#include <blobls.h>
#include <defs.h>

/************************************************************/
segblobs8(blobls, bflags, cdata, w, h)
BLOBLS **blobls;
unsigned char *cdata;
int w, h, bflags;
{
   int ret;
   unsigned char *bdata;
   int bx, by, bw, bh;
   int strt_x1, strt_y1;
   int blob_found;
   int pix, tflags;
   BLOB *blob;
   RUN *runs, *run_t, *run_off, *rptr;
   static int sflags = BLOB_XY1S | BLOB_WHS;

   if((sflags & bflags) != sflags)
      fatalerr("segblobs8",
               "must have at least (x1, y1) and (w, h) flags specified for blobls", NULL);

   /* compute and store blob statistics according to bflags */
   build_blobls(blobls, BLOB_CHUNKS, bflags, 8, BLOB_BIN);

   strt_x1 = 0;
   strt_y1 = 0;
   blob_found = TRUE;
   while (blob_found){
      ret = findblobnruns8(cdata, w, h, ERASE, ALLOC, BOUND_BLOB, &strt_x1, &strt_y1,
                          &bdata, &bx, &by, &bw, &bh, &runs, &run_t, &run_off);
      switch (ret){
      case 0:
         blob_found = FALSE;
         break;
      case 1:
         /* if pixelcnt flag set, then count runlengths instead of scanning pixels */
         if(bflags & BLOB_PIXCNTS){
            pix = 0;
            for(rptr = runs; rptr < run_t; rptr++)
               pix += (rptr->e_off - rptr->w_on);
            tflags = bflags & ~BLOB_PIXCNTS;
         }
         else
            tflags = bflags;
         /* allocate and initialize blob structure */
         build_blob(&blob, tflags, bdata, bx, by, bw, bh);
         if(bflags & BLOB_PIXCNTS)
            blob->pixcnt = pix;
         /* append new blob to the list */
         append_blobls((*blobls), blob);
         break;
      default:
         fatalerr("segblobs8", "received illegal return code", "findblob");
      break;
      }
   }
   /* free static global memory allocated in findblobnruns() */
   end_findblobs();
}

/************************************************************/
segblobs8rlen(blobls, bflags, cdata, w, h, rlen, rn, ra)
BLOBLS **blobls;
unsigned char *cdata;
int w, h, bflags;
int **rlen, *rn, *ra;
{
   int i, ret;
   unsigned char *bdata;
   int bx, by, bw, bh;
   int strt_x1, strt_y1;
   int blob_found;
   BLOB *blob;
   RUN *runs, *run_t, *run_off, *rptr;
   int pix, tflags, sn;
   static int sflags = BLOB_XY1S | BLOB_WHS;

   if((sflags & bflags) != sflags)
      fatalerr("segblobs8rlen",
               "must have at least (x1, y1) and (w, h) flags specified for blobls", NULL);

   /* compute and store the blob statistics according to bflag */
   build_blobls(blobls, BLOB_CHUNKS, bflags, 8, BLOB_BIN);

   /* initialize run length statistics */
   malloc_int(rlen, CHUNKS, "segblobs8rlen : malloc : rlen");
   *rn = 0;
   *ra = CHUNKS;

   strt_x1 = 0;
   strt_y1 = 0;
   blob_found = TRUE;
   while (blob_found){
      ret = findblobnruns8(cdata, w, h, ERASE, ALLOC, BOUND_BLOB, &strt_x1, &strt_y1,
                          &bdata, &bx, &by, &bw, &bh, &runs, &run_t, &run_off);
      switch (ret){
      case 0:
         blob_found = FALSE;
         break;
      case 1:
         /* compute and store run lengths */
         sn = *rn;
         for(rptr = runs; rptr < run_t; rptr++){
            if(*rn >= *ra){
               (*ra) += CHUNKS;
               realloc_int(rlen, *ra, "segblobs8rlen : realloc : rlen");
            }
            (*rlen)[*rn] = rptr->e_off - rptr->w_on;
            (*rn)++;
         }
         /* if pixelcnt flag set, then count runlengths instead of scanning pixels */
         if(bflags & BLOB_PIXCNTS){
            pix = 0;
            for(i = sn; i < *rn; i++)
               pix += (*rlen)[i];
            tflags = bflags & ~BLOB_PIXCNTS;
         }
         else
            tflags = bflags;
         /* allocate and initialize blob structure */
         build_blob(&blob, tflags, bdata, bx, by, bw, bh);
         if(bflags & BLOB_PIXCNTS)
            blob->pixcnt = pix;
         /* append new blob to the list */
         append_blobls((*blobls), blob);
         break;
      default:
         fatalerr("segblobs8rlen", "received illegal return code", "findblob");
         break;
      }
   }
   /* free static global memory allocated in findblobnruns() */
   end_findblobs();
}
