/*
# proc: zoom - scales a binary bitmap up or down depending on the x & y factors
# proc:        passed.
# proc: zoom_x - scales a binary bitmap up or down based on the x factor
# proc:          passed.
# proc: zoom_y - scales a binary bitmap up or down based on the y factor
# proc:          passed.
# proc: enlarge_x - expand a binary bitmap's columns by the x factor passed.
# proc:
# proc: enlarge_y - expand a binary bitmap's rows by the y factor passed.
# proc:
# proc: shrink_x - down sample a binary bitmap's columns by the x factor
# proc:            passed.
# proc: shrink_y - down sample a binary bitmap's rows by the y factor passed.
# proc:
# proc: copy_cols - copies a single column of pixels to a range of column
# proc:             locations in a destination image.
# proc: copy_col - copies a column of pixels to a column location in a
# proc:            destination image.
# proc: copy_rows - copies a single row of pixels to a range of row locations
# proc:             in a destination image.
# proc: copy_row - copies a row of pixels to a row location in a destination
# proc:            image.
*/

#include <stdio.h>
#include <math.h>
#include <ihead.h>
#include <imageops.h>

/************************************************************************/
zoom(zmdata, zw, zh, bindata, iw, ih, xfctr, yfctr)
unsigned char **zmdata, *bindata;
int *zw, *zh, iw, ih;
float xfctr, yfctr;
{
   unsigned char *chardata, *mallocate_image();
   unsigned char *xzmdata, *yzmdata;
   int xw, xh, yw, yh;

   if((xfctr == 1.0) && (yfctr == 1.0)){
      *zmdata = imagedup(bindata, iw, ih, 1);
      *zw = iw;
      *zh = ih;
   }
   else{
      /* if shrinking y, then do it first to mimimize length of col copies */
      if(yfctr <= 1.0){
         chardata = mallocate_image(iw, ih, 8);
         bits2bytes(bindata, chardata, iw*ih);

         zoom_y(&yzmdata, &xw, &xh, chardata, iw, ih, yfctr);
         free(chardata);
         zoom_x(&xzmdata, &yw, &yh, yzmdata, xw, xh, xfctr);
         free(yzmdata);
         char2bin_exact(zmdata, zw, zh, xzmdata, yw, yh);
         free(xzmdata);
      }
      /* if enlarging y, then do it last to mimimize length of col copies */
      if(yfctr > 1.0){
         chardata = mallocate_image(iw, ih, 8);
         bits2bytes(bindata, chardata, iw*ih);

         zoom_x(&xzmdata, &xw, &xh, chardata, iw, ih, xfctr);
         free(chardata);
         zoom_y(&yzmdata, &yw, &yh, xzmdata, xw, xh, yfctr);
         free(xzmdata);
         char2bin_exact(zmdata, zw, zh, yzmdata, yw, yh);
         free(yzmdata);
      }
   }
}

/************************************************************************/
zoom_x(xzmdata, zw, zh, chardata, iw, ih, xfctr)
unsigned char **xzmdata, *chardata;
int *zw, *zh, iw, ih;
float xfctr;
{
   if(xfctr == 1.0){
      *xzmdata = (unsigned char *)imagedup(chardata, iw, ih, 8);
      *zw = iw;
      *zh = ih;
   }
   else if(xfctr > 1.0)
      enlarge_x(xzmdata, zw, zh, chardata, iw, ih, xfctr);
   else
      shrink_x(xzmdata, zw, zh, chardata, iw, ih, xfctr);
}

/************************************************************************/
zoom_y(yzmdata, zw, zh, chardata, iw, ih, yfctr)
unsigned char **yzmdata, *chardata;
int *zw, *zh, iw, ih;
float yfctr;
{
   if(yfctr == 1.0){
      *yzmdata = (unsigned char *)imagedup(chardata, iw, ih, 8);
      *zw = iw;
      *zh = ih;
   }
   else if(yfctr > 1.0)
      enlarge_y(yzmdata, zw, zh, chardata, iw, ih, yfctr);
   else
      shrink_y(yzmdata, zw, zh, chardata, iw, ih, yfctr);
}

/************************************************************************/
enlarge_x(xzmdata, zw, zh, chardata, iw, ih, xfctr)
unsigned char **xzmdata, *chardata;
int *zw, *zh, iw, ih;
float xfctr;
{
   int from, to1, to2;
   float f_current;
   unsigned char *allocate_image();

   *zw = (int)((xfctr * iw) + 0.5);
   *zh = ih;
   *xzmdata = allocate_image(*zw, *zh, 8);

   f_current = -1.0;
   from = 0;
   to1 = from;
   do{
      f_current += xfctr;
      to2 = (int)(f_current + 0.5);

      copy_cols((*xzmdata), *zw, *zh, to1, to2, chardata, iw, ih, from);

      from++;
      to1 = to2+1;
   } while(from < iw);
}

/************************************************************************/
enlarge_y(yzmdata, zw, zh, chardata, iw, ih, yfctr)
unsigned char **yzmdata, *chardata;
int *zw, *zh, iw, ih;
float yfctr;
{
   int from, to1, to2;
   float f_current;
   unsigned char *allocate_image();

   *zw = iw;
   *zh = (int)((yfctr * ih) + 0.5);
   *yzmdata = allocate_image(*zw, *zh, 8);

   f_current = -1.0;
   from = 0;
   to1 = from;
   do{
      f_current += yfctr;
      to2 = (int)(f_current + 0.5);

      copy_rows((*yzmdata), *zw, *zh, to1, to2, chardata, iw, ih, from);

      from++;
      to1 = to2+1;
   } while(from < ih);
}

/************************************************************************/
shrink_x(xzmdata, zw, zh, chardata, iw, ih, xfctr)
unsigned char **xzmdata, *chardata;
int *zw, *zh, iw, ih;
float xfctr;
{
   int from, to;
   float recip, f_current;
   unsigned char *mallocate_image();

   recip = 1.0 / xfctr;
   *zw = (int)((xfctr * iw) + 0.5);
   *zh = ih;
   *xzmdata = mallocate_image(*zw, *zh, 8);

   f_current = 0.0;
   from = 0;
   to = from;
   do{
      copy_col((*xzmdata), *zw, *zh, to, chardata, iw, ih, from);
      f_current += recip;
      from = (int)(f_current + 0.5);
      to++;
   } while(to < *zw);
}

/************************************************************************/
shrink_y(yzmdata, zw, zh, chardata, iw, ih, yfctr)
unsigned char **yzmdata, *chardata;
int *zw, *zh, iw, ih;
float yfctr;
{
   int from, to;
   float recip, f_current;
   unsigned char *mallocate_image();

   recip = 1.0 / yfctr;
   *zw = iw;
   *zh = (int)((yfctr * ih) + 0.5);
   *yzmdata = mallocate_image(*zw, *zh, 8);

   f_current = 0.0;
   from = 0;
   to = from;
   do{
      copy_row((*yzmdata), *zw, *zh, to, chardata, iw, ih, from);
      f_current += recip;
      from = (int)(f_current + 0.5);
      to++;
   } while(to < *zh);
}

/************************************************************************/
copy_cols(xzmdata, zw, zh, to1, to2, chardata, iw, ih, from)
unsigned char *xzmdata, *chardata;
int zw, zh, iw, ih;
int to1, to2, from;
{
   int i;

   for(i = to1; i <= to2; i++)
      copy_col(xzmdata, zw, zh, i, chardata, iw, ih, from);
}

/************************************************************************/
copy_col(xzmdata, zw, zh, to, chardata, iw, ih, from)
unsigned char *xzmdata, *chardata;
int zw, zh, iw, ih;
int to, from;
{
   unsigned char *fptr, *tptr;
   int i;

   if(zh != ih)
      fatalerr("copy_col", "length of columns not equal", NULL);
   fptr = chardata + from;
   tptr = xzmdata + to;
   for(i = 0; i < zh; i++){
      *tptr = *fptr;
      tptr += zw;
      fptr += iw;
   }
}

/************************************************************************/
copy_rows(yzmdata, zw, zh, to1, to2, chardata, iw, ih, from)
unsigned char *yzmdata, *chardata;
int zw, zh, iw, ih;
int to1, to2, from;
{
   int i;

   for(i = to1; i <= to2; i++)
      copy_row(yzmdata, zw, zh, i, chardata, iw, ih, from);
}

/************************************************************************/
copy_row(yzmdata, zw, zh, to, chardata, iw, ih, from)
unsigned char *yzmdata, *chardata;
int zw, zh, iw, ih;
int to, from;
{
   unsigned char *fptr, *tptr;

   if(zw != iw)
      fatalerr("copy_row", "length of rows not equal", NULL);
   fptr = chardata + (from * iw);
   tptr = yzmdata + (to * zw);
   memcpy(tptr, fptr, zw);
}
