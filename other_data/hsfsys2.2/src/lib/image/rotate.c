/*
proc: rotate_push8_int - rotates a charimage about a point using an efficient
proc:	    pixel pushing algorithm and integer arithmetic - fast but "noisy"
proc: rotate_push8_float - rotates a charimage about a point using an efficient
proc:	    pixel pushing algorithm and float arithmetic - fast
proc: rotate_push8 - rotates a charimage about a point using an efficient
proc:		     pixel pushing algorithm
proc: rot_push_params - calculates the transformation parameters ahead of
proc:			the pushing rotation
proc: rotntrans_push_params - calculates the push transformation parameters for
proc:                         both rotation and translation.
proc: push_param3_chrimage - applies the supplied push transformation to a
proc:			     charimage
# proc: grayscale_rotate_90 - rotates a grayscale image exactly 90 degrees
# proc:                       clockwise by copying pixel rows to pixel columns.
# proc: grayscale_rotate_n90 - rotates a grayscale image exactly -90 degrees
# proc:                 counter-clockwise by copying pixel rows to pixel columns.
*/

#include <stdio.h>
#include <math.h>
#include <lsq3.h>
#include <defs.h>

/*************************************************************************/
rotate_push8_int(cdata, rdata, w, h, cenx, ceny, theta)
unsigned char *cdata, *rdata;
int w, h, cenx, ceny; float theta;
{
unsigned char *src, **add;
int x, xprime, y, yprime;
float s, c, yc, xc, py, px;
int ssyy, ccyy, *c_x, *s_x;

   theta /= -DEG2RAD;
   s = (float)sin((double)theta);
   c = (float)cos((double)theta);
   yc = (float)ceny;
   xc = (float)cenx;
   px = yc*s - xc*c + xc;
   py = yc - yc*c - xc*s;
 
   malloc_int(&c_x, w, "rotate_push8_int : c_x");
   malloc_int(&s_x, w, "rotate_push8_int : s_x");
   malloc_dbl_uchar_l1(&add, h, "rotate_push8_int : add");

   for ( y = 1, add[0] = rdata ; y < h ; y++ )
      add[y] = add[y-1] + w;

   for ( x = 0 ; x < w ; x++ )
      c_x[x] = sround(c*x),
      s_x[x] = sround(s*x);

   for ( y = 0, src = cdata ; y < h ; y++ )
   {
      ssyy = sround(px - s * y);
      ccyy = sround(py + c * y);
      for ( x = 0 ; x < w ; x++ )
         if (*src++)
         {
            xprime = c_x[x] + ssyy; /* integer arithmetic ! */
            yprime = s_x[x] + ccyy;
            if (xprime>=0 && xprime < w && yprime>=0 && yprime < h)
               *(add[yprime] + xprime) = 1;
         }
   }
   free(c_x); free(s_x); free(add);
}

/*************************************************************************/
rotate_push8_float(cdata, rdata, w, h, cenx, ceny, theta)
unsigned char *cdata, *rdata;
int w, h, cenx, ceny; float theta;
{
unsigned char *src, **add;
int x, xprime, y, yprime;
float s, c, yc, xc, py, px;
float ssyy, ccyy, *c_x, *s_x;

   theta /= -DEG2RAD;
   s = (float)sin((double)theta);
   c = (float)cos((double)theta);
   yc = (float)ceny;
   xc = (float)cenx;
   px = yc*s - xc*c + xc;
   py = yc - yc*c - xc*s;
 
   malloc_flt(&c_x, w, "rotate_push8_float : c_x");
   malloc_flt(&s_x, w, "rotate_push8_float : s_x");
   malloc_dbl_uchar_l1(&add, h, "rotate_push8_float : add");

   for ( y = 1, add[0] = rdata ; y < h ; y++ )
      add[y] = add[y-1] + w;

   for ( x = 0 ; x < w ; x++ )
      c_x[x] = c*x + 0.5,
      s_x[x] = s*x + 0.5;

   for ( y = 0, src = cdata ; y < h ; y++ )
   {
      ssyy = px - s * y;
      ccyy = py + c * y;
      for ( x = 0 ; x < w ; x++ )
         if (*src++)
         {
            xprime = (int)(c_x[x] + ssyy);
            yprime = (int)(s_x[x] + ccyy);
            if (xprime>=0 && xprime < w && yprime>=0 && yprime < h)
               *(add[yprime] + xprime) = 1;
         }
   }
   free(c_x); free(s_x); free(add);
}

/*************************************************************************/
rotate_push8(cdata, rdata, iw, ih, cenx, ceny, theta)
unsigned char *cdata, *rdata;
int iw, ih, cenx, ceny; float theta;
{
float dx, mxx, mxy, dy, myy, myx;

   rot_push_params(theta, cenx, ceny, &dx, &mxx, &mxy, &dy, &myy, &myx);
   push_param3_chrimage(rdata, cdata, iw, ih, dx, mxx, mxy, dy, myy, myx);
}

/*************************************************************************/
rot_push_params(theta, cx, cy, dx, mxx, mxy, dy, myy, myx)
int cx, cy;
float theta, *dx, *mxx, *mxy, *dy, *myy, *myx;
{
   float rad, c, c1, s;

   rad = -theta / DEG2RAD;

   c = (float)cos((double)rad);
   s = (float)sin((double)rad);
   c1 = 1.0 - c;

   *dx = (cx * c1) + (cy * s);
   *mxx = c;
   *mxy = -s;

   *dy = (cy * c1) - (cx * s);
   *myy = c;
   *myx = s;
}

/*****************************************************************************/
rotntrans_push_params(theta, cx, cy, tranx, trany, dx, mxx, mxy, dy, myy, myx)
int tranx, trany;
int cx, cy;
float theta, *dx, *mxx, *mxy, *dy, *myy, *myx;
{
   float rad, c, c1, s;

   rad = -theta / DEG2RAD;

   c = (float)cos((double)rad);
   s = (float)sin((double)rad);
   c1 = 1.0 - c;

   *dx = (cx * c1) + (cy * s) - tranx;
   *mxx = c;
   *mxy = -s;

   *dy = (cy * c1) - (cx * s) - trany;
   *myy = c;
   *myx = s;
}

/*************************************************************************/
push_param3_chrimage(rdata, cdata, iw, ih, dx, mxx, mxy, dy, myy, myx)
unsigned char *cdata, *rdata;
int iw, ih;
float dx, mxx, mxy, dy, myy, myx;
{
unsigned char *cptr, **add;
float fx, fy;
int row, col, nx, ny;

   malloc_dbl_uchar_l1(&add, ih, "push_param3_chrimage : add");

   for ( row = 1, add[0] = rdata ; row < ih ; row++ )
      add[row] = add[row-1] + iw;

   cptr = cdata;
   for(row = 0; row < ih; row++){
      for(col = 0; col < iw; col++){
         if(*cptr++){
             Mapplyparam3_to_point(&fx, &fy, col, row,
                                  dx, mxx, mxy, dy, myy, myx);
             nx = (int)(fx+0.5);
             ny = (int)(fy+0.5);
             if(((nx >= 0) && (nx < iw)) &&
                ((ny >= 0) && (ny < ih))){
                *(add[ny] + nx) = 1;
             }
          }
      }
   }
   free(add);
}

/***************************************************************/
grayscale_rotate_90(idata, odata, w, h)
unsigned char *idata, *odata;
int w, h;
{
   int x, y;
   unsigned char *iptr, *siptr, *optr, *soptr;

   siptr = idata;
   soptr = odata + h - 1;
   for(y = 0; y < h; y++){
      iptr = siptr;
      optr = soptr;
      for(x = 0; x < w; x++){
         *optr = *iptr;
         optr += h;
         iptr++;
      }
      siptr += w;
      soptr--;
   }
}

/***************************************************************/
grayscale_rotate_n90(idata, odata, w, h)
unsigned char *idata, *odata;
int w, h;
{
   int x, y;
   unsigned char *iptr, *siptr, *optr, *soptr;

   siptr = idata;
   soptr = odata + ((w-1) * h);
   for(y = 0; y < h; y++){
      iptr = siptr;
      optr = soptr;
      for(x = 0; x < w; x++){
         *optr = *iptr;
         optr -= h;
         iptr++;
      }
      siptr += w;
      soptr++;
   }
}
