/*
# proc: black_pixel_count - returns the total number of pixels in a binary
# proc:                     image with value equal to one.
*/

#include <bitcount.h>

int black_pixel_count(bimage, width, height)
unsigned char *bimage;
int width, height;
{
   int b, i;
   static int ow = -1, oh = -1, n;

   if((ow != width) || (oh != height)){
      n = width * height;
      if ( n % 8 )
         fatalerr("black_pixel_count", "number of pixels", "not divisible by 8");
      ow = width;
      oh = height;
   }

   for ( b = 0, i = n >> 3 ; i ; i-- )
       b += Bit_Count[*bimage++];

   return(b);
}
