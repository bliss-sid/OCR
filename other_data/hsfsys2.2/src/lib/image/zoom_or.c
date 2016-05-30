/*
# proc: zoomN - shrinks image by logical OR of square tiles, handles 1/8 bit
# proc:         per pixel input -and- outputs, padding of outputs, prealloc
# proc:         or internal alloc of output data.
# proc: zoomx4 - shrinks an unsigned char image down by logical OR of tiles
# proc:          of size 4x4. wrapper call to zoomN
# proc: zoomx3 - shrinks an unsigned char image down by logical OR of tiles
# proc:          of size 3x3. wrapper call to zoomN
# proc: zoomx2 - shrinks an unsigned char image down by logical OR of tiles
# proc:          of size 2x2. wrapper call to zoomN
# proc: zoomx8 - shrinks an unsigned char image down by logical OR of tiles
# proc:          of size 8x8. wrapper call to zoomN
# proc: zoomxN - shrinks an unsigned char image down by logical OR of tiles
# proc:          of size NxN. wrapper call to zoomN
# proc: zoomaux{2,3,4,8,N} - auxialliary zooming routines called by zoomN.
# proc                 know what you are doing before calling these!
*/

#include <stddef.h>
#include <zoom_or.h>

#define BITSPERBYTE 8

zoomx2(inp, iw, ih, out, ow, oh)
unsigned char *inp; int iw, ih;
unsigned char **out; int *ow, *oh;
{
   zoomN(2, inp, iw, ih, INPUT_IS_8, NULL, out, ow, oh,
          NO_MK_1_BIT, MK_8_BIT, NO_ZM_ALLOC, ZM_ALLOC);
}
zoomx3(inp, iw, ih, out, ow, oh)
unsigned char *inp; int iw, ih;
unsigned char **out; int *ow, *oh;
{
   zoomN(3, inp, iw, ih, INPUT_IS_8, NULL, out, ow, oh,
          NO_MK_1_BIT, MK_8_BIT, NO_ZM_ALLOC, ZM_ALLOC);
}
zoomx4(inp, iw, ih, out, ow, oh)
unsigned char *inp; int iw, ih;
unsigned char **out; int *ow, *oh;
{
   zoomN(4, inp, iw, ih, INPUT_IS_8, NULL, out, ow, oh,
          NO_MK_1_BIT, MK_8_BIT, NO_ZM_ALLOC, ZM_ALLOC);
}
zoomx8(inp, iw, ih, out, ow, oh)
unsigned char *inp; int iw, ih;
unsigned char **out; int *ow, *oh;
{
   zoomN(8, inp, iw, ih, INPUT_IS_8, NULL, out, ow, oh,
          NO_MK_1_BIT, MK_8_BIT, NO_ZM_ALLOC, ZM_ALLOC);
}
zoomxN(f, inp, iw, ih, out, ow, oh)
unsigned char *inp; int f, iw, ih;
unsigned char **out; int *ow, *oh;
{
   zoomN(f, inp, iw, ih, INPUT_IS_8, NULL, out, ow, oh,
          NO_MK_1_BIT, MK_8_BIT, NO_ZM_ALLOC, ZM_ALLOC);
}


/* crunch by OR ing of NxN tiles. If any pixel in a tile is set the	*/
/* output image pixel is set.						*/
/* inputs:								*/
/*    n                 crunching factor, allowed 1, 2, 3, 4, 8		*/
/*   i18		Either  a 1 or 8 bit per pixel image		*/
/*   w, h	        with width w and height h.			*/
/*   in_1_or_8          if the input is 8 bits per pixel then set	*/
/*			in_1_or_8 to INPUT_IS_8	else INPUT_IS_1		*/
/*   mk_1_bit		if a 1 bit per pixel output is desired then	*/
/*                      set mk_1_bit to MK_1_BIT else set NO_MK_1_BIT	*/
/*   mk_8_bit		if a 8 bit per pixel output is desired then	*/
/*                      set mk_8_bit to MK_8_BIT else set NO_MK_8_BIT	*/
/*   alloc_1_bit	if a 1 bit output is desired and already 	*/
/*                  alloced then set this NO_ZM_ALLOC else set ZM_ALLOC	*/
/*   alloc_8_bit	if a 8 bit output is desired and already	*/
/*                  alloced then set this NO_ZM_ALLOC else set ZM_ALLOC	*/
/* outputs:								*/
/*   o1		The address of the pointer to the 1 bit per pixel	*/
/*   		output image.						*/
/*   o8         The address of the pointer to the 8 bit per pixel	*/
/*		output image.						*/
/*   ow		The address of the output width				*/
/*   oh		The address of the output height			*/
/* return codes:							*/
/*    1		nothing was done, neither 1 nor 8 bit images were	*/
/*		asked for.						*/
/*    2		1 bit output image was requested, the space was		*/
/*              apparently prealloced but the output block was NULL	*/
/*    3		8 bit output image was requested, the space was		*/
/*              apparently prealloced but the output block was NULL	*/
/*    4         input is apparently not 1 bit (INPUT_IS_1) nor 8 bit	*/
/*		(INPUT_IS_8)						*/
/*    5         mk_8_bit is neither MK_8_BIT nor NO_MK_8_BIT		*/
/*    6         alloc_1_bit is neither ZM_ALLOC nor NO_ZM_ALLOC		*/
/*    7         alloc_8_bit is neither ZM_ALLOC nor NO_ZM_ALLOC		*/
/*    8         attempt to crunch by a nonpositive integer.		*/
/*  notes:								*/
/*    if you crunch by a factor of 1 then output is a copy of input	*/
/*    unless 1 bit per pixel output is required whence padding is done	*/
/*    if necessary.							*/
/*									*/
/*    if you will eventually need 1 bit per pixel outputs then this	*/
/*    routine handle padding. It will optionally make both 1 and 8 bit	*/
/*    outputs, correctly padded. If you opt not to make 1 bit output	*/
/*    you will have to do the padding yourself later			*/
/*									*/
/*    all the internal operations are done on 8 bit per pixel ops	*/
/*    so the 8 bit output is always available, so you can get it for	*/
/*    free. only use NO_MK_8_BIT if you are going to free the 8 bit	*/
/*    output yourself anyway.						*/

int zoomN(n, i18, iw, ih, in_1_or_8, o1, o8, ow, oh,
       mk_1_bit, mk_8_bit, alloc_1_bit, alloc_8_bit)
unsigned char *i18, **o1, **o8;
int n, in_1_or_8, iw, ih, *ow, *oh;
int mk_1_bit, mk_8_bit, alloc_1_bit, alloc_8_bit;
{
unsigned char *inp8, *inp1, *out8, *out1, *tmp;
unsigned char *mallocate_image();
int outw, oldw, outh;

   if (mk_1_bit == NO_MK_1_BIT && mk_8_bit == NO_MK_8_BIT)
      return 1;

   switch (in_1_or_8)
   {
      case INPUT_IS_1 : inp1 = i18;
                        inp8 = mallocate_image(iw, ih, 8);
                        bits2bytes(inp1, inp8, iw*ih);
                        break;
      case INPUT_IS_8 : inp1 = NULL;
                        inp8 = i18;
                        break;
      default:          return 4;
   }

   outw = iw / n;
   outh = ih / n;

   switch (mk_8_bit)
   {
      case    MK_8_BIT : switch (alloc_8_bit)
                         {
                            case ZM_ALLOC :
                               out8 = mallocate_image(outw, outh, 8);
                               break;
                            case NO_ZM_ALLOC :
                               if ((out8 = *o8) == NULL)
                                  return 3;
                               break;
                            default :
                               return 7;
                         }
			 break;
      case NO_MK_8_BIT : out8 = mallocate_image(outw, outh, 8);
                         break;
      default          : return 5;
   }

   if ( n < 1 )
     return 8;

   switch (n)
   {
      case 1: memcpy(out8, inp8, iw*ih);
              outw = iw;
              outh = ih;
              break;
      case 2: zoomaux2(inp8, iw, ih, out8, &outw, &outh);
              break;
      case 3: zoomaux3(inp8, iw, ih, out8, &outw, &outh);
              break;
      case 4: zoomaux4(inp8, iw, ih, out8, &outw, &outh);
              break;
      case 8: zoomaux8(inp8, iw, ih, out8, &outw, &outh);
              break;
      default:zoomauxN(n, inp8, iw, ih, out8, &outw, &outh);
              break;
   }

   if (in_1_or_8 == INPUT_IS_1)
      free(inp8);

   if (mk_1_bit == MK_1_BIT)
   {
      if (outw % BITSPERBYTE)
      {
         for (oldw = outw ; outw % BITSPERBYTE ; outw++ );
         tmp = out8;
         image_pad(&out8, oldw, outh, outw, outh, 0); 
         free(tmp);
      }

      switch (alloc_1_bit)
      {
         case    ZM_ALLOC : out1 = mallocate_image(outw, outh, 1);
                            break;
         case NO_ZM_ALLOC : out1 = *o1;
                            if (out1 == NULL)
                               return 2;
                            break;
         default          : return 6;
      }

      bytes2bits(out8, out1, outw*outh);

      *o1 = out1;
      *ow = outw;
      *oh = outh;
   }

   if (mk_8_bit == MK_8_BIT)
   {
      *o8 = out8;
      *ow = outw;
      *oh = outh;
   }
   else
   if (mk_8_bit == NO_MK_8_BIT)
      free(out8);

   return 0;
}


/* if any of the pixels in 4x4 pixel tile of the input 1 byte per pix */
/* image is set true then set the output pixel true, this effects a   */
/* nice looking image downsampling */
zoomaux4(inp, iw, ih, out, ow, oh)
unsigned char *inp; int iw, ih;
unsigned char *out; int *ow, *oh;
{
unsigned char sum, *p1, *p2, *p3, *p4;
int i, j, w, h, n;

   n = iw + iw + iw;    /* loop adds 1*iw so we need to add 3*iw */
   *ow = w = (iw >> 2);
   *oh = h = (ih >> 2);
   n += iw - (w << 2);

   p1 = inp ; p2 = p1 + iw ; p3 = p2 + iw ; p4 = p3 + iw;
   for ( i = 0 ; i < h ; i++ )
   {
      for ( j = 0 ; j < w ; j++ )
      {
         /* any pixel is good enough for "true" output, which raises    */
         /* the possibility of not looking at all sixteen raster locs   */
         /* maybe breaking early from the 4 lines if sum already non    */
         /* zero. this, it turns out is slower since the pointers have  */
         /* to be adjusted if you break out early                       */
         sum  = *p1++  +  *p2++  +  *p3++  +  *p4++;
         sum += *p1++  +  *p2++  +  *p3++  +  *p4++;
         sum += *p1++  +  *p2++  +  *p3++  +  *p4++;
         sum += *p1++  +  *p2++  +  *p3++  +  *p4++;
         *out++ = (unsigned char)(sum > 0);   /* any true pixels */
      }
      p1 += n; p2 += n ; p3 += n ; p4 += n;
   }
}


zoomaux3(inp, iw, ih, out, ow, oh)
unsigned char *inp; int iw, ih;
unsigned char *out; int *ow, *oh;
{
unsigned char sum, *p1, *p2, *p3;
int i, j, w, h, n;

   n = iw + iw;
   *ow = w = iw / 3;
   *oh = h = ih / 3;
   n += iw - (w * 3);

   p1 = inp ; p2 = p1 + iw ; p3 = p2 + iw;
   for ( i = 0 ; i < h ; i++ )
   {
      for ( j = 0 ; j < w ; j++ )
      {
         sum  = *p1++  +  *p2++  +  *p3++;
         sum += *p1++  +  *p2++  +  *p3++;
         sum += *p1++  +  *p2++  +  *p3++;
         *out++ = (unsigned char)(sum > 0);   /* any true pixels */
      }
      p1 += n; p2 += n ; p3 += n;
   }
}


zoomaux2(inp, iw, ih, out, ow, oh)
unsigned char *inp; int iw, ih;
unsigned char *out; int *ow, *oh;
{
unsigned char sum, *p1, *p2;
int i, j, w, h, n;

   n = iw;    /* loop adds 1*iw so we need to add 1*iw */
   *ow = w = (iw >> 1);
   *oh = h = (ih >> 1);
   n += iw - (w << 1);

   p1 = inp; p2 = p1 + iw;
   for ( i = 0 ; i < h ; i++ )
   {
      for ( j = 0 ; j < w ; j++ )
      {
         sum  = *p1++ + *p2++;
         sum += *p1++ + *p2++;
         *out++ = (unsigned char)(sum > 0);
      }
      p1 += n; p2 += n;
   }
}


zoomaux8(inp, iw, ih, out, ow, oh)
unsigned char *inp; int iw, ih;
unsigned char *out; int *ow, *oh;
{
unsigned char sum, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8;
int i, j, w, h, n;

   n = (iw << 3) - iw;    /* loop adds 1*iw so we need to add 7*iw */
   *ow = w = (iw >> 3);
   *oh = h = (ih >> 3);
   n += iw - (w << 3);

   p1 = inp     ; p2 = p1 + iw ; p3 = p2 + iw ; p4 = p3 + iw;
   p5 = p4 + iw ; p6 = p5 + iw ; p7 = p6 + iw ; p8 = p7 + iw; 
   for ( i = 0 ; i < h ; i++ )
   {
      for ( j = 0 ; j < w ; j++ )
      {
         sum  = *p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++;
         sum += *p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++; 
         sum += *p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++; 
         sum += *p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++; 
         sum += *p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++; 
         sum += *p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++; 
         sum += *p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++; 
         sum += *p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++; 
         *out++ = (unsigned char)(sum > 0);   /* any true pixels */
      }
      p1 += n ; p2 += n ; p3 += n ; p4 += n;
      p5 += n ; p6 += n ; p7 += n ; p8 += n;
   }
}


zoomauxN(f, inp, iw, ih, out, ow, oh)
unsigned char *inp; int f, iw, ih;
unsigned char *out; int *ow, *oh;
{
unsigned char sum, **p;
int i, j, k, l, w, h, n;

   if ((p = (unsigned char **)malloc(f * sizeof(unsigned char *))) == NULL)
      syserr("zoomXN", "malloc", "space for pointer array");

   n = (f - 1)*iw;
   *ow = w = iw / f;
   *oh = h = ih / f;
   n += iw - (w * f);

   for ( k = 0 ; k < f ; k++ )
      p[k] = inp + k*iw;

   for ( i = 0 ; i < h ; i++ )
   {
      for ( j = 0 ; j < w ; j++ )
      {
         for ( k = 0, sum = 0 ; k < f ; k++ )
            for ( l = 0 ; l < f ; l++ )
               sum += *p[l]++;
         *out++ = (unsigned char)(sum > 0);
      }

      for ( k = 0 ; k < f ; k++ )
         p[k] += n;
   }
   free(p);
}
