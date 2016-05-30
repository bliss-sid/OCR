/*
# proc: pixelcnt8 - counts the number of black pixels in a binary character
# proc:            stream, one byte per pixel, faster than pixelcnt.
# proc: subpixelcnt8 - counts the number of black pixels in the specified subimage of
# proc:                a binary character stream, one byte per pixel.
# proc: pixelcnt - counts the number of black pixels in a binary character
# proc:            image, one byte per pixel, same result as pixelcnt8.
# proc: shift_row - takes a row of character pixels and shifts them left or
# proc:             right by a given number of pixel into an output row.
# proc: left_pix - locates the first nonzero pixel left-to-right in a given
# proc:            row of character pixels and returns -1 if no pixel is found.
# proc: char_top - locates the first row of pixels top-to-bottom in a character
# proc:            image that contains a nonzero pixel and returns -1 if no
# proc:            row is found.
# proc: char_bottom - locates the first row of pixels bottom-to-top in a
# proc:               character image that contains a nonzero pixel and
# proc:               returns -1 if no row is found.
*/


#include <stddef.h>



/*******************************************************************/
int pixelcnt8(inp, n)
unsigned char *inp; int n;
{
int i, b;
unsigned char t, *ptr, *str;
 
   /* temporarily set last pixel to 1 so that we know which inner	*/
   /* loop must test for image completion: the other one does no test!	*/ 
   /* loops n-1 times then checks the last stop pixel			*/
   /* subtle but gives over 10 sd20 images 27->22 secs = 5secs gain	*/ 

   t = inp[n-1]; inp[n-1] = 1;
   for ( i = n-1, ptr = inp, b = 0 ; i ; )
   {
      for ( str = ptr ; !(*ptr)      ; ptr++ );         /* skip white	*/
      i -= (ptr-str);                                   /* num skiped	*/ 
      for ( str = ptr ;  (*ptr) && i ; ptr++, i-- );    /* count black	*/ 
      b += (ptr-str);
   }
   if (t) b++;   inp[n-1] = t;	/* consider the last one, and restore	*/

   return b;
}

/*******************************************************************/
subpixelcnt8(cdata, w, h, sx, sy, sw, sh)
unsigned char *cdata;
int w, h, sx, sy, sw, sh;
{
   int count, x, y;
   int xlim, ylim;
   unsigned char *sptr, *pptr;

   if((sx < 0) || (sx >= w))
      fatalerr("subpixelcnt8",
               "subimage x coordinate exceeds image dimensions", NULL);
   if((sy < 0) || (sy >= h))
      fatalerr("subpixelcnt8",
               "subimage y coordinate exceeds image dimensions", NULL);
   if((xlim = sx+sw) > w)
      fatalerr("subpixelcnt8",
               "subimage w exceeds image width", NULL);
   if((ylim = sy+sh) > h)
      fatalerr("subpixelcnt8",
               "subimage h exceeds image height", NULL);

   count = 0;
   sptr = cdata + (sy * w) + sx;
   for(y = sy; y < ylim; y++){
      pptr = sptr;
      for(x = sx; x < xlim; x++){
         if(*pptr)
            count++;
         pptr++;
      }
      sptr+=w;
   }
   return(count);
}

/*******************************************************************/
pixelcnt(cdata, w, h)
unsigned char *cdata; int w, h;
{
   static int ow = -1, oh = -1, olen;

   if((ow != w) || (oh != h)){
      olen = w*h;
      ow = w;
      oh = h;
   }

   return pixelcnt8(cdata, olen);
}


/*******************************************************************/
shift_row(irow, orow, w, sval)
unsigned char *irow, *orow;
int w, sval;
{
   if(sval == 0){
      memcpy(orow, irow, w);
      return;
   }
   if(abs(sval) >= w){
      memset(orow, 0, w);
      return;
   }
   if(sval > 0){
      memset(orow, 0, w);
      memcpy(orow + sval, irow, w - sval);
      return;
   }
   if(sval < 0){
      memset(orow, 0, w);
      memcpy(orow, irow - sval, w + sval);
      return;
   }
}

/*************************************************************************/
left_pix(row, w)
unsigned char *row;
int w;
{
   int i;
   unsigned char *rptr;

   rptr = row;
   for(i = 0; i < w; i++){
      if(*rptr != 0)
         return(i);
      else
         rptr++;
   }
   return(-1);
}

/*************************************************************************/
char_top(cdata, w, h)
unsigned char *cdata;
int w, h;
{
   int r, c;
   unsigned char *cptr;

   cptr = cdata;
   for(r = 0; r < h; r++){
      for(c = 0; c < w; c++){
         if(*cptr != 0)
            return(r);
         else
            cptr++;
      }
   }
   return(-1);
}

/*************************************************************************/
char_bottom(cdata, w, h)
unsigned char *cdata;
int w, h;
{
   int len, r, c;
   unsigned char *cptr;

   len = w * h;
   cptr = cdata + len - 1;
   for(r = h-1; r >= 0; r--){
      for(c = 0; c < w; c++){
         if(*cptr != 0)
            return(r);
         else
            cptr--;
      }
   }
   return(-1);
}
