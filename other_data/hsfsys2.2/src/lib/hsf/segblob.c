/*
# proc: segbinblob - segments the given binary bitmap into blobs, storing the
# proc:              results in an mis memory and returning stats.
# proc: segbinblobdigits - segments the given binary bitmap containing a
# proc:                    single line of handprinted digits, storing the
# proc:                    results in an mis memory and returning stats.
# proc: paste_digit_blobs - pastes certain blobs back together in an attempt
# proc:                     to put disjoint hats back on fives.
*/

#include <stdio.h>
#include <math.h>
#include <mfs.h>
#include <ihead.h>
#include <mis.h>
#include <findblob.h>
#include <defs.h>
#include <segment.h>

/************************************************************/
segbinblob(mis, par_x, par_y, prisp_w, prisp_h, bdata, w, h)
MIS **mis;
int **par_x, **par_y, **prisp_w, **prisp_h;
unsigned char *bdata;
int w, h;
{
   int mw, mh, ow, oh, bin_w, bin_h;
   int ret, blkpix;
   unsigned char *cdata, *binblob, *allocate_image();
   int box_x, box_y, box_w, box_h;
   int strt_x, strt_y;
   int mcount, blob_found;

   mw = ENTW;
   mh = ENTH;
   cdata = allocate_image(w, h, 8);
   bits2bytes(bdata, cdata, w*h);

   /* mw is not necessarily a multiple of 8, so the output binary width */
   /* ow may need to be byte aligned */
   ow = (int)(ceil(mw / 8.0))<<3;
   oh = mh;
   (*mis) = allocmis(NULL, 1, BPI, ow, oh, MAX_BLOBS);
   malloc_int(par_x, MAX_BLOBS, "segbinblob : par_x");
   malloc_int(par_y, MAX_BLOBS, "segbinblob : par_y");
   malloc_int(prisp_w, MAX_BLOBS, "segbinblob : prisp_w");
   malloc_int(prisp_h, MAX_BLOBS, "segbinblob : prisp_h");
   strt_x = 0;
   strt_y = 0;
   blob_found = TRUE;
   mcount = 0;
   while (blob_found){
      box_w = mw;
      box_h = mh;
      ret = findbinblob(cdata, w, h, ERASE, W_H_BLOB, &strt_x, &strt_y,
                     &binblob, &bin_w, &bin_h, &box_x, &box_y, &box_w, &box_h);
      switch (ret){
         case 0:
            blob_found = FALSE;
            break;
         case 1:
            if(bin_w != ow)
               fatalerr("segbinblob",
                        "width of blob raster not equal to mis entry", NULL);
            if(mcount < MAX_BLOBS){
               blkpix = black_pixel_count(binblob, bin_w, bin_h);
               if((blkpix > MINPIXELS) && (blkpix < MAXPIXELS)){
                  appendmis(binblob, bin_w, bin_h, (*mis));
                  (*par_x)[mcount] = box_x;
                  (*par_y)[mcount] = box_y;
                  (*prisp_w)[mcount] = box_w;
                  (*prisp_h)[mcount] = box_h;
                  mcount++;
               }
               /* if the blob has too few or too many black pixels, */
               /* then ignore it */
            }
            else
               fatalerr("segbinblob", "mis overflow", "MAX_BLOBS exceeded");
            free(binblob);
            break;
         case 2:
            /* if blob spatially too big, then ignore it */
            free(binblob);
            break;
         default:
            fatalerr("segbinblob", "received unknown return code",
                     "findbinblob");
            break;
      }
   }
   free(cdata);
}

/************************************************************/
segbinblobdigits(mis, par_x, par_y, prisp_w, prisp_h, bdata, w, h)
MIS **mis;
int **par_x, **par_y, **prisp_w, **prisp_h;
unsigned char *bdata;
int w, h;
{
   segbinblob(mis, par_x, par_y, prisp_w, prisp_h, bdata, w, h);
   paste_digit_blobs(*mis, *par_x, *par_y, *prisp_w, *prisp_h);
}

/************************************************************/
paste_digit_blobs(mis, par_x, par_y, prisp_w, prisp_h)
MIS *mis;
int *par_x, *par_y, *prisp_w, *prisp_h;
{
   int cur_i, next_i;
   unsigned char *entry;
   int entsize;
   int lx, rx, ty, by, nx, ny, nw, nh;
   int frx, fry, tox, toy;

   cur_i = 0;
   next_i = 1;
   entsize = SizeFromDepth(mis->entw, mis->enth, mis->misd);
   malloc_uchar(&entry, entsize, "paste_digit_blobs : entry");
   while(next_i < mis->ent_num){
      if(((par_y[next_i] + prisp_h[next_i] - par_y[cur_i]) <=
          (prisp_h[cur_i] >> 1)) &&
         ((par_x[next_i] - par_x[cur_i] - prisp_w[cur_i]) <=
          (prisp_w[cur_i] >> 1))){
         lx = min(par_x[cur_i], par_x[next_i]);
         rx = max(par_x[cur_i] + prisp_w[cur_i],
                  par_x[next_i] + prisp_w[next_i]);
         ty = min(par_y[cur_i], par_y[next_i]);
         by = max(par_y[cur_i] + prisp_h[cur_i],
                  par_y[next_i] + prisp_h[next_i]);
         nw = rx - lx;
         nh = by - ty;
         if((nw <= mis->entw) && (nh <= mis->enth)){
            nx = (mis->entw - nw)>>1;
            ny = (mis->enth - nh)>>1;
            memset(entry, NULL, entsize);
            /* copy current blob */
            frx = (mis->entw - prisp_w[cur_i])>>1;
            fry = (cur_i * mis->entw) + ((mis->enth - prisp_h[cur_i])>>1);
            tox = nx + par_x[cur_i] - lx;
            toy = ny + par_y[cur_i] - ty;
            binary_subimage_or(mis->data, mis->misw, mis->mish,
                                 entry, mis->entw, mis->enth,
                                 frx, fry, prisp_w[cur_i], prisp_h[cur_i],
                                 tox, toy);
            /* copy next blob */
            frx = (mis->entw - prisp_w[next_i])>>1;
            fry = (next_i * mis->entw) + ((mis->enth - prisp_h[next_i])>>1);
            tox = nx + par_x[next_i] - lx;
            toy = ny + par_y[next_i] - ty;
            binary_subimage_or(mis->data, mis->misw, mis->mish,
                                 entry, mis->entw, mis->enth,
                                 frx, fry, prisp_w[next_i], prisp_h[next_i],
                                 tox, toy);
            /* write new entry into current position */
            par_x[cur_i] = lx;
            remove_from_list(next_i, par_x, mis->ent_num);
            par_y[cur_i] = ty;
            remove_from_list(next_i, par_y, mis->ent_num);
            prisp_w[cur_i] = nw;
            remove_from_list(next_i, prisp_w, mis->ent_num);
            prisp_h[cur_i] = nh;
            remove_from_list(next_i, prisp_h, mis->ent_num);
            changemis(mis, cur_i, entry);
            deletemis(mis, next_i);
         }
         else{
            cur_i++;
            next_i++;
         }
      }
      else{
         cur_i++;
         next_i++;
      }
   }
   free(entry);
}
