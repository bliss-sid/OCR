/*
# proc: norm_2nd_gen_blobls8 - conducts second generation spatial normalization on
# proc:           the characters in the specified 1 pixel per byte mis structure.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>
#include <blobls.h>

#define NORM_W 32
#define NORM_H 32
#define RED_W  20
#define RED_H  32

/*************************************************************************/
norm_2nd_gen_blobls8(outmis, scal_x, scal_y, blobls)
MIS **outmis;
float **scal_x, **scal_y;
BLOBLS *blobls;
{
   int i, oesize;
   unsigned char *optr, *zdata, *mallocate_image();
   unsigned char *mcdata;
   float xfactor, yfactor;
   int zw, zh, ox, oy;
   int npix;
   int tlx, tly, objw, objh;
   BLOB *blob;

   (*outmis) = allocmis(NULL, 8, 0, NORM_W, NORM_H, blobls->num);
   malloc_flt(scal_x, blobls->num, "norm_2nd_gen_blobls8 : scal_x");
   malloc_flt(scal_y, blobls->num, "norm_2nd_gen_blobls8 : scal_y");

   oesize = NORM_W * NORM_H;
   mcdata = mallocate_image(NORM_W, NORM_H, 8);

   optr = (*outmis)->data;
   for(i = 0; i < blobls->num; i++){
      blob = blobls->blobs[i];
      /* find right edge of character */
      find_corners8(&tlx, &tly, &objw, &objh, blob->data, blob->w, blob->h);

      /* scale the character image */
      xfactor = RED_W/(float)objw;
      (*scal_x)[i] = xfactor;
      yfactor = RED_H/(float)objh;
      (*scal_y)[i] = yfactor;
      zoom8(&zdata, &zw, &zh, blob->data, blob->w, blob->h, xfactor, yfactor);

      /* center the scaled character image */
      ox = (NORM_W - zw)>>1;
      oy = (NORM_H - zh)>>1;
      subimage_copy(zdata, zw, zh, optr, NORM_W, NORM_H,
                    0, 0, zw, zh, ox, oy);
      free(zdata);

      /* apply morphing to the scaled character image */
      npix = pixelcnt(optr, NORM_W, NORM_H);
      if(npix > 412){
         if(npix > 560){
            erode_charimage(optr, mcdata, NORM_W, NORM_H);
            erode_charimage(mcdata, optr, NORM_W, NORM_H);
         }
         else{
            erode_charimage(optr, mcdata, NORM_W, NORM_H);
            subimage_copy(mcdata, NORM_W, NORM_H, optr, NORM_W, NORM_H,
                          0, 0, NORM_W, NORM_H, 0, 0);
         }
      }
      else{
         if(npix < 256){
            if(npix < 108){
               dilate_charimage(optr, mcdata, NORM_W, NORM_H);
               dilate_charimage(mcdata, optr, NORM_W, NORM_H);
            }
            else {
               dilate_charimage(optr, mcdata, NORM_W, NORM_H);
               subimage_copy(mcdata, NORM_W, NORM_H, optr, NORM_W, NORM_H,
                             0, 0, NORM_W, NORM_H, 0, 0);
            }
         }
      }
      optr += oesize;
      (*outmis)->ent_num++;
   }
   free(mcdata);
}
