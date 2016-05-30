/*
# proc: char2bin_exact - converts an binary character image to a binary
# proc:                  bitmap byte-aligning the resulting image width.
*/

#include <stdio.h>
#include <math.h>

/************************************************************************/
char2bin_exact(bindata, bw, bh, chardata, w, h)
unsigned char **bindata, *chardata;
int *bw, *bh;
int w, h;
{
   unsigned char *tptr, cmask, *fptr;
   int memlen, groups_8;
   int i, j, k, bytes, rem;

   bytes = (int) ceil(w/8.0);
   *bw = bytes << 3;
   *bh = h;
   memlen = SizeFromDepth(*bw, *bh, 1);
   malloc_uchar(bindata, memlen, "char2bin_exact : bindata");
   groups_8 = w >> 3;
   rem = w % 8;
   fptr = chardata;
   tptr = (*bindata);
   for(i = 0; i < h; i++){
      for(j = 0; j < groups_8; j++){
         cmask = 0x00;
         for(k = 0; k < 7; k++){
            cmask |= *fptr++;
            cmask <<= 1;
         }
         cmask |= *fptr++;
         *tptr++ = cmask;
      }
      if(rem != 0){
         cmask = 0x00;
         for(k = 0; k < rem; k++){
            cmask |= *fptr++;
            cmask <<= 1;
         }
         for(k = 0; k < 7-rem; k++)
            cmask <<= 1;
         *tptr++ = cmask;
      }
   }
}
