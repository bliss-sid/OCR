/*
# proc: transpose_rect_matrix_s - transpose a rectangular matrix, float slower
# proc:
*/

#include <stddef.h>

transpose_rect_matrix_s(inp, ix, iy)
float  *inp;
int    ix, iy;
{
int  x, y;
float *out, *ipt, *opt;

/*      converts  ABCD    to    AEI in place with copy		*/
/*                EFGH          BFJ but not with pointer assign	*/
/*                IJKL          CGK				*/
/*                              DHL				*/
   if ((out = (float *)malloc(ix * iy * sizeof(float))) == NULL)
     fatalerr("transpose_rect_matrix_s", "space for duplicate", "malloc");

   /* do the transpose into some local memory block */
   for (x = 0, opt = out ; x < ix ; x++)
      for (y = 0, ipt = inp + x ; y < iy ; y++)
         *opt++ = *ipt, ipt += ix;

   /* and copy the result back in to its original place */
   memcpy(inp, out, ix*iy*sizeof(float));
   free(out);
}
