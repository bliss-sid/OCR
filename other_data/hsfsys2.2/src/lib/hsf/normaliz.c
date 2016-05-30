/*
# proc: norm_2nd_gen - conducts second generation spatial normalization on
# proc:                the characters in the specified mis structure given
# proc:                the primary space statistics on each character.
# proc: norm_2nd_gen2 - conducts second generation spatial normalization on
# proc:                 the characters in the specified mis structure without
# proc:                 the use of primary space statistics.
*/

#include <stdio.h>
#include <math.h>
#include <ihead.h>
#include <mis.h>

#define NORM_W 32
#define NORM_H 32
#define RED_W  20
#define RED_H  32

/*************************************************************************/
norm_2nd_gen(outmis, scal_x, scal_y, inmis, prisp_w, prisp_h)
MIS **outmis, *inmis;
float **scal_x, **scal_y;
int *prisp_w, *prisp_h;
{
   int i, acw, iesize, oesize;
   unsigned char *iptr, *optr, *cdata, *zdata, *allocate_image();
   unsigned char *ocdata, *mcdata, *rcptr;
   float xfactor, yfactor;
   int azw, zw, zh, ox, oy;
   int frx, fry;
   int len, npix;
   int entw, enth, entd;

   entw = inmis->entw;
   enth = inmis->enth;
   entd = inmis->misd;
   (*outmis) = allocmis(NULL, 1, 0, NORM_W, NORM_H, inmis->ent_num);
   malloc_flt(scal_x, inmis->ent_num, "norm_2nd_gen : scal_x");
   malloc_flt(scal_y, inmis->ent_num, "norm_2nd_gen : scal_y");
   iesize = SizeFromDepth(entw, enth, entd);
   oesize = SizeFromDepth(NORM_W, NORM_H, entd);
   ocdata = allocate_image(NORM_W, NORM_H, 8);
   mcdata = allocate_image(NORM_W, NORM_H, 8);

   iptr = inmis->data;
   optr = (*outmis)->data;
   for(i = 0; i < inmis->ent_num; i++){
      /* extract character */
      acw = (int)ceil(prisp_w[i]/8.0)<<3;
      cdata = allocate_image(acw, prisp_h[i], entd);
      frx = (entw - prisp_w[i])>>1;
      fry = (enth - prisp_h[i])>>1;
      binary_subimage_copy(iptr, entw, enth, cdata, acw, prisp_h[i],
                           frx, fry, prisp_w[i], prisp_h[i], 0, 0);
      iptr += iesize;
      /* scale the character image */
      xfactor = RED_W/(float)prisp_w[i];
      (*scal_x)[i] = xfactor;
      yfactor = RED_H/(float)prisp_h[i];
      (*scal_y)[i] = yfactor;
      zoom(&zdata, &azw, &zh, cdata, acw, prisp_h[i], xfactor, yfactor);
      free(cdata);
      /* center the scaled character image */
      zw = RED_W;
      ox = (NORM_W - zw)>>1;
      oy = (NORM_H - zh)>>1;
      binary_subimage_copy(zdata, azw, zh, optr, NORM_W, NORM_H,
                           0, 0, zw, zh, ox, oy);
      free(zdata);
      /* apply morphing to the scaled character image */
      len = NORM_W * NORM_H;
      bits2bytes(optr, ocdata, len);
      npix = pixelcnt(ocdata, NORM_W, NORM_H);
      rcptr = ocdata;
      if(npix > 412){
         if(npix > 560){
            erode_charimage(ocdata, mcdata, NORM_W, NORM_H);
            erode_charimage(mcdata, ocdata, NORM_W, NORM_H);
         }
         else{
            erode_charimage(ocdata, mcdata, NORM_W, NORM_H);
            rcptr = mcdata;
         }
      }
      else{
         if(npix < 256){
            if(npix < 108){
               dilate_charimage(ocdata, mcdata, NORM_W, NORM_H);
               dilate_charimage(mcdata, ocdata, NORM_W, NORM_H);
            }
            else {
               dilate_charimage(ocdata, mcdata, NORM_W, NORM_H);
               rcptr = mcdata;
            }
         }
      }
      bytes2bits(rcptr, optr, len);
      optr += oesize;
      (*outmis)->ent_num++;
   }
   free(ocdata);
   free(mcdata);
}

/*************************************************************************/
norm_2nd_gen2(outmis, scal_x, scal_y, inmis)
MIS **outmis, *inmis;
float **scal_x, **scal_y;
{
   int i, acw, iesize, oesize;
   unsigned char *iptr, *optr, *cdata, *zdata, *allocate_image();
   unsigned char *ocdata, *mcdata, *rcptr;
   float xfactor, yfactor;
   int azw, zw, zh, ox, oy;
   int len, npix;
   int tlx, tly, objw, objh;
   int entw, enth, entd;

   entw = inmis->entw;
   enth = inmis->enth;
   entd = inmis->misd;
   (*outmis) = allocmis(NULL, 1, 0, NORM_W, NORM_H, inmis->ent_num);
   malloc_flt(scal_x, inmis->ent_num, "norm_2nd_gen2 : scal_x");
   malloc_flt(scal_y, inmis->ent_num, "norm_2nd_gen2 : scal_y");
   iesize = SizeFromDepth(entw, enth, entd);
   oesize = SizeFromDepth(NORM_W, NORM_H, entd);
   ocdata = allocate_image(NORM_W, NORM_H, 8);
   mcdata = allocate_image(NORM_W, NORM_H, 8);

   iptr = inmis->data;
   optr = (*outmis)->data;
   for(i = 0; i < inmis->ent_num; i++){
      /* extract character */
      find_corners(&tlx, &tly, &objw, &objh, iptr,entw, enth, 0);
      acw = (int)ceil(objw/8.0)<<3;
      cdata = allocate_image(acw, objh, entd);
      binary_subimage_copy(iptr, entw, enth, cdata, acw, objh,
                           tlx, tly, objw, objh, 0, 0);
      iptr += iesize;
      /* scale the character image */
      xfactor = RED_W/(float)objw;
      (*scal_x)[i] = xfactor;
      yfactor = RED_H/(float)objh;
      (*scal_y)[i] = yfactor;
      zoom(&zdata, &azw, &zh, cdata, acw, objh, xfactor, yfactor);
      free(cdata);
      /* center the scaled character image */
      zw = RED_W;
      ox = (NORM_W - zw)>>1;
      oy = (NORM_H - zh)>>1;
      binary_subimage_copy(zdata, azw, zh, optr, NORM_W, NORM_H,
                           0, 0, zw, zh, ox, oy);
      free(zdata);
      /* apply morphing to the scaled character image */
      len = NORM_W * NORM_H;
      bits2bytes(optr, ocdata, len);
      npix = pixelcnt(ocdata, NORM_W, NORM_H);
      rcptr = ocdata;
      if(npix > 412){
         if(npix > 560){
            erode_charimage(ocdata, mcdata, NORM_W, NORM_H);
            erode_charimage(mcdata, ocdata, NORM_W, NORM_H);
         }
         else{
            erode_charimage(ocdata, mcdata, NORM_W, NORM_H);
            rcptr = mcdata;
         }
      }
      else{
         if(npix < 256){
            if(npix < 108){
               dilate_charimage(ocdata, mcdata, NORM_W, NORM_H);
               dilate_charimage(mcdata, ocdata, NORM_W, NORM_H);
            }
            else {
               dilate_charimage(ocdata, mcdata, NORM_W, NORM_H);
               rcptr = mcdata;
            }
         }
      }
      bytes2bits(rcptr, optr, len);
      optr += oesize;
      (*outmis)->ent_num++;
   }
   free(ocdata);
   free(mcdata);
}
