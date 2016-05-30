/*
# proc: zoom8 - scales an unsigned char image based on the x & y factors
# proc:         passed.
*/

#include <stdio.h>
#include <math.h>
#include <ihead.h>

/************************************************************************/
zoom8(zmdata, zw, zh, data, iw, ih, xfctr, yfctr)
unsigned char **zmdata, *data;
int *zw, *zh, iw, ih;
float xfctr, yfctr;
{
   unsigned char *chardata;
   unsigned char *xzmdata, *yzmdata;
   int xw, xh;

   if((xfctr == 1.0) && (yfctr == 1.0)){
      *zmdata = (unsigned char *)imagedup(data, iw, ih, 8);
      *zw = iw;
      *zh = ih;
   }
   else{
      /* if shrinking y, then do it first to mimimize length of col copies */
      if(yfctr <= 1.0){
         chardata = (unsigned char *)imagedup(data, iw, ih, 8);

         zoom_y(&yzmdata, &xw, &xh, chardata, iw, ih, yfctr);
         free(chardata);
         zoom_x(zmdata, zw, zh, yzmdata, xw, xh, xfctr);
         free(yzmdata);
      }
      /* if enlarging y, then do it last to mimimize length of col copies */
      if(yfctr > 1.0){
         chardata = (unsigned char *)imagedup(data, iw, ih, 8);

         zoom_x(&xzmdata, &xw, &xh, chardata, iw, ih, xfctr);
         free(chardata);
         zoom_y(zmdata, zw, zh, xzmdata, xw, xh, yfctr);
         free(xzmdata);
      }
   }
}
