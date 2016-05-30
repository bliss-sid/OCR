/*
# proc: f_fit_param3_image2 - returns a transformed binary image by applying
# proc:                       precomputed Least Squares parameters and quickly
# proc:                       pushing pixels and conditionally filling speckle
# proc:                       noise in the resulting image (fastest).
*/

#include <stdio.h>
#include <memory.h>
#include <defs.h>

/*************************************************************/
f_fit_param3_image2(rdata, idata, iw, ih, dx, mxx, mxy, dy, myy, myx,
                    fill_flag)
unsigned char *idata, **rdata;
int iw, ih, fill_flag;
float dx, mxx, mxy, dy, myy, myx;
{
   unsigned char *cdata, *cptr, *fdata, *fptr, *odata;
   unsigned char *allocate_image();
   float fx, fy;
   int row, col, nx, ny, len;

   cdata = allocate_image(iw, ih, 8);
   len = iw * ih;
   bits2bytes(idata, cdata, len);
   fdata = allocate_image(iw, ih, 8);

   cptr = cdata;
   for(row = 0; row < ih; row++){
      for(col = 0; col < iw; col++){
         if(*cptr != 0){
             applyparam3_to_point(&fx, &fy, col, row,
                                  dx, mxx, mxy, dy, myy, myx);
             nx = sround(fx);
             ny = sround(fy);
             if(((nx >= 0) && (nx < iw)) &&
                ((ny >= 0) && (ny < ih))){
                fptr = fdata + (ny * iw) + nx;
                *fptr = *cptr;
             }
          }
          cptr++;
      }
   }

   if(fill_flag){
      dilate_charimage(fdata, cdata, iw, ih);
      erode_charimage(cdata, fdata, iw, ih);
   }
   free(cdata);

   odata = allocate_image(iw, ih, 1);
   bytes2bits(fdata, odata, len);
   free(fdata);
   *rdata = odata;
}
