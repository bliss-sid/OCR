/*
# proc: bits2bytes - promotes a binary image to a grayscale image.
# proc:        destination pointer must be double-aligned.
# proc:        not generalized by byte-order.
# proc:
*/

#include <stdio.h>
#include <values.h>
#include <sys/param.h>
#include <sys/types.h>


bits2bytes(p,q,pixels)
u_char *p, *q;
u_int pixels;
{
double **rptrs, *dst;
static u_char *ptrs[1 << BITSPERBYTE];
static int init = 0;

   if (p == (u_char *) NULL)
      fatalerr("bits2bytes", "source data pointer is", "NULL");
   if (q == (u_char *) NULL)
      fatalerr("bits2bytes", "destination data pointer is", "NULL");

   if (!init)
   {
      int i, j;
      u_char *r;
      static u_char table[(1 << BITSPERBYTE) * BITSPERBYTE];
      static u_char mask[BITSPERBYTE] =
         { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

      for ( r = table, i = 0 ; i < (1 << BITSPERBYTE) ; i++ )
         for ( j = 0, ptrs[i] = r ; j < BITSPERBYTE; j++ )
            *r++ = (i & mask[j]) ? 1 : 0;

      init = 1;
   }

   pixels = howmany(pixels,BITSPERBYTE);
   for ( dst = (double *)q, rptrs = (double **)ptrs ; pixels-- ; )
      *dst++ = *rptrs[*p++];
}
