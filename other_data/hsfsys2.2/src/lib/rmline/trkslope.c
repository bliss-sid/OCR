/*
# proc: slope_left_above - determines the slope of the left side of a character
# proc:                    piece touching above the line.
# proc: slope_left_below - determines the slope of the left side of a character
# proc:                    piece touching below the line.
# proc: slope_right_above - determines the slope of the right side of a character
# proc:                     piece touching above the line.
# proc: slope_right_below - determines the slope of the right side of a character
# proc:                     piece touching below the line.
*/

#include <stdio.h>
#include <values.h>
#include <defs.h>
#include <rmline.h>
#include <mytrace.h>

/***************************************************************************/
/* slope_left_above - determines the slope of the left side of a character */
/* piece touching above the line.                                          */
/***************************************************************************/
float slope_left_above(sx, sy, slimit, slen, cdata, w, h)
int sx, sy, slimit, slen;
unsigned char *cdata;
int w, h;
{
   unsigned char *cptr;
   int i, x, y, cx, ox;
   int dxs, n, diff;
   float m;

   my_trace("entered slope_left_above\n");

   x = sx;
   y = sy;
   cptr = cdata + (y * w) + x;
   /* if starting on a white pixel, look right for black */
   if(*cptr == WHITE_PIX){
      if((cx = find_pix_east(BLACK_PIX, x, y, cdata, w, h)) == NOT_FOUND){
         /* maybe a problem, but call it vertical for now */
         my_trace("slope not found, assuming vertical\n");
         my_trace("finished slope_left_above\n");
         return(MAXFLOAT);
      }
      else{
         x = cx;
         cptr = cdata + (y * w) + x;
      }
   }
   dxs = 0;
   n = 0;
   /* trace the character edge */
   for(i = 0; i < slen; i++){
      ox = x;
      /* move up a scanline */
      y--;
      if(y < 0)
         /* if traced off the image, then stop */
         break;
      cptr -= w;
      /* if white pixel, look right for black */
      if(*cptr == WHITE_PIX){
         if((cx = find_pix_east(BLACK_PIX, x, y, cdata, w, h)) == NOT_FOUND)
            /* if traced off the image, then stop */
            break;
      }
      else{
         /* if black pixel, look left for white */
         if((cx = find_pix_west(WHITE_PIX, x, y, cdata, w, h)) == NOT_FOUND)
            /* if traced off the image, then stop */
            break;
         else
            /* if white pixel found, then jump back one to black */
            cx++;
      }
      /* if still on the image ... */
      x = cx;
      cptr = cdata + (y * w) + x;

      /* compute delta in x */
      diff = (ox - x);
      /* if jump is too big, then stop */
      if(abs(diff) > slimit)
         break;
      else{
         dxs += (ox - x);
         n++;
      }
   }
   if(dxs == 0)
      m = MAXFLOAT;
   else
      m = n / (float)dxs;
   my_trace("finished slope_left_above\n");
   return(m);
}

/***************************************************************************/
/* slope_left_below - determines the slope of the left side of a char      */
/* piece touching below the line.                                          */
/***************************************************************************/
float slope_left_below(sx, sy, slimit, slen, cdata, w, h)
int sx, sy, slimit, slen;
unsigned char *cdata;
int w, h;
{
   unsigned char *cptr;
   int i, x, y, cx, ox;
   int dxs, n, diff;
   float m;

   my_trace("entered slope_left_below\n");

   x = sx;
   y = sy;
   cptr = cdata + (y * w) + x;
   /* if starting on a white pixel, look right for black */
   if(*cptr == WHITE_PIX){
      if((cx = find_pix_east(BLACK_PIX, x, y, cdata, w, h)) == NOT_FOUND){
         /* maybe a problem, but call it vertical for now */
         my_trace("slope not found, assuming vertical\n");
         my_trace("finished slope_left_below\n");
         return(MAXFLOAT);
      }
      else{
         x = cx;
         cptr = cdata + (y * w) + x;
      }
   }
   dxs = 0;
   n = 0;
   /* trace character edge */
   for(i = 0; i < slen; i++){
      ox = x;
      /* move down a scanline */
      y++;
      if(y >= h)
         /* if traced off the image, then stop */
         break;
      cptr += w;
      /* if white pixel, look right for black */
      if(*cptr == WHITE_PIX){
         if((cx = find_pix_east(BLACK_PIX, x, y, cdata, w, h)) == NOT_FOUND)
            /* if traced off the image, then stop */
            break;
      }
      else{
         /* if black pixel, look left for white */
         if((cx = find_pix_west(WHITE_PIX, x, y, cdata, w, h)) == NOT_FOUND)
            /* if traced off the image, then stop */
            break;
         else
            /* if white pixel found, then jump back one to black */
            cx++;
      }
      /* if still on the image ... */
      x = cx;
      cptr = cdata + (y * w) + x;

      /* compute delta in x */
      diff = (ox - x);
      /* if jump is too big, then stop */
      if(abs(diff) > slimit)
         break;
      else{
         dxs += (ox - x);
         n++;
      }
   }
   if(dxs == 0)
      m = MAXFLOAT;
   else
      m = (-1.0*n) / (float)dxs;
   my_trace("finished slope_left_below\n");
   return(m);
}

/***************************************************************************/
/* slope_right_above - determines the slope of the right side of a char    */
/* piece touching above the line.                                          */
/***************************************************************************/
float slope_right_above(sx, sy, slimit, slen, cdata, w, h)
int sx, sy, slimit, slen;
unsigned char *cdata;
int w, h;
{
   unsigned char *cptr;
   int i, x, y, cx, ox;
   int dxs, n, diff;
   float m;

   my_trace("entered slope_right_above\n");

   x = sx;
   y = sy;
   cptr = cdata + (y * w) + x;
   /* if starting on a white pixel, look left for black */
   if(*cptr == WHITE_PIX){
      if((cx = find_pix_west(BLACK_PIX, x, y, cdata, w, h)) == NOT_FOUND){
         /* maybe a problem, but call it vertical for now */
         my_trace("slope not found, assuming vertical\n");
         my_trace("finished slope_right_above\n");
         return(MAXFLOAT);
      }
      else{
         x = cx;
         cptr = cdata + (y * w) + x;
      }
   }
   dxs = 0;
   n = 0;
   /* trace character edge */
   for(i = 0; i < slen; i++){
      ox = x;
      /* move up a scanline */
      y--;
      if(y < 0)
         /* if traced off the image, then stop */
         break;
      cptr -= w;
      /* if white pixel, look left for black */
      if(*cptr == WHITE_PIX){
         if((cx = find_pix_west(BLACK_PIX, x, y, cdata, w, h)) == NOT_FOUND)
            /* if traced off the image, then stop */
            break;
      }
      else{
         /* if black pixel, look right for white */
         if((cx = find_pix_east(WHITE_PIX, x, y, cdata, w, h)) == NOT_FOUND)
            /* if traced off the image, then stop */
            break;
         else
            /* if white pixel found, then jump back one to black */
            cx--;
      }
      /* if still on the image ... */
      x = cx;
      cptr = cdata + (y * w) + x;

      /* compute delta in x */
      diff = (ox - x);
      /* if jump is too big, then stop */
      if(abs(diff) > slimit)
         break;
      else{
         dxs += (ox - x);
         n++;
      }
   }
   if(dxs == 0)
      m = MAXFLOAT;
   else
      m = n / (float)dxs;
   my_trace("finished slope_right_above\n");
   return(m);
}

/***************************************************************************/
/* slope_right_below - determines the slope of the right side of a char    */
/* piece touching below the line.                                          */
/***************************************************************************/
float slope_right_below(sx, sy, slimit, slen, cdata, w, h)
int sx, sy, slimit, slen;
unsigned char *cdata;
int w, h;
{
   unsigned char *cptr;
   int i, x, y, cx, ox;
   int dxs, n, diff;
   float m;

   my_trace("entered slope_right_below\n");

   x = sx;
   y = sy;
   cptr = cdata + (y * w) + x;
   /* if starting on a white pixel, look left for black */
   if(*cptr == WHITE_PIX){
      if((cx = find_pix_west(BLACK_PIX, x, y, cdata, w, h)) == NOT_FOUND){
         /* maybe a problem, but call it vertical for now */
         my_trace("slope not found, assuming vertical\n");
         my_trace("finished slope_right_below\n");
         return(MAXFLOAT);
      }
      else{
         x = cx;
         cptr = cdata + (y * w) + x;
      }
   }
   dxs = 0;
   n = 0;
   /* trace character edge */
   for(i = 0; i < slen; i++){
      ox = x;
      /* move down a scanline */
      y++;
      if(y >= h)
         /* if traced off the image, then stop */
         break;
      cptr += w;
      /* if white pixel, look left for black */
      if(*cptr == WHITE_PIX){
         if((cx = find_pix_west(BLACK_PIX, x, y, cdata, w, h)) == NOT_FOUND)
            /* if traced off the image, then stop */
            break;
      }
      else{
         /* if black pixel, look right for white */
         if((cx = find_pix_east(WHITE_PIX, x, y, cdata, w, h)) == NOT_FOUND)
            /* if traced off the image, then stop */
            break;
         else
            /* if white pixel found, then jump back one to black */
            cx--;
      }
      /* if still on the image ... */
      x = cx;
      cptr = cdata + (y * w) + x;

      /* compute delta in x */
      diff = (ox - x);
      if(abs(diff) > slimit)
         break;
      /* if jump is too big, then stop */
      else{
         dxs += (ox - x);
         n++;
      }
   }
   if(dxs == 0)
      m = MAXFLOAT;
   else
      m = (-1.0*n) / (float)dxs;
   my_trace("finished slope_right_below\n");
   return(m);
}
