/*
# proc: find_corners - determines the box bounding the black pixels within
# proc:                a binary bitmap based on histograms and thresholds.
# proc: find_corners1 - finds the top left and width height of the bounding
# proc:                 box of all pixels of a 1 bit per pixel 0/1 binary image
# proc: find_corners8 - finds the top left and width height of the bounding
# proc:                 box of all pixels of a 1 byte per pixel 0/1 char image 
*/

#include <stddef.h>
#include <values.h>
#include <histgram.h>
#include <defs.h>

/***********************************************************************/
/* find corners of region. returns top left x and y and object w and h */
/***********************************************************************/
find_corners(tlx,tly,objw,objh,src_image,width,height,thresh)
unsigned char *src_image;
int *tlx, *tly;
int width, height;
int *objw, *objh;
int thresh;
{ 
   /* when finding the true bounding box of all pixels this is very much */
   /* faster than using histograms thresholded at 0. Patrick 8-16-95     */
   if (thresh == 0)
   {
      find_corners1(tlx,tly,objw,objh,src_image,width,height);
      if((*objw == 0) || (*objh == 0))
         return(NOT_FOUND);
      else
         return(FOUND);
   }
   else
   {
      int *xbins = NULL,num_x_bins = 0;
      int *ybins = NULL,num_y_bins = 0;
      int top = 0,    topval = 0;
      int bottom = 0, bottomval = 0;
      int right = 0,  rightval = 0;
      int left = 0,   leftval = 0;

      compute_hist(src_image,width,height,X_HIST,&xbins,&num_x_bins);
      if(find_first_thresh_forward(xbins,0,num_x_bins,thresh,&top,&topval) &&
         find_first_thresh_backward(xbins,0,num_x_bins,thresh,
                                    &bottom,&bottomval)){
         compute_hist(src_image,width,height,Y_HIST,&ybins,&num_y_bins);
         if(find_first_thresh_forward(ybins,0,num_y_bins,thresh,&left,&leftval) &&
            find_first_thresh_backward(ybins,0,num_y_bins,thresh,
                                       &right,&rightval)){
            right = min(right+1,num_y_bins);
            bottom = min(bottom+1,num_x_bins);
            (*objw) = right - left;
            (*objh) = bottom - top;
            (*tlx) = left;
            (*tly) = top;
            free(xbins);
            free(ybins);
            return(FOUND);
         }
         else{
            (*objw) = 0;
            (*objh) = 0;
            (*tlx) = 0;
            (*tly) = 0;
            free(xbins);
            free(ybins);
            return(NOT_FOUND);
         }
      }
      else{
         (*objw) = 0;
         (*objh) = 0;
         (*tlx) = 0;
         (*tly) = 0;
         free(xbins);
         return(NOT_FOUND);
      }
   }
}

/* a look up table containing the index of the rightmost bits of the	*/
/* binary representation of an unsigned char. used for finding right	*/
/* edge of black in images. 			                        */
/*		unsigned char x;					*/
/*		int i, k;						*/
/*		for ( i = 0 ; i < 256 ; i++ )				*/
/*		{							*/
/*		   for ( x = (unsigned char)i, k = 0 ; x ; k++ )	*/
/*		   x = x >> 1;						*/
/*		   printf("%d,%c", k, (i+1) % 8 ? ' ' : '\n');		*/
/*		}							*/

static unsigned char rightlut[2<<BITSPERBYTE] = {
0, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
5, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
6, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
5, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
7, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
5, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
6, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
5, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
8, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
5, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
6, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
5, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
7, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
5, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
6, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1,
5, 1, 2, 1, 3, 1, 2, 1,
4, 1, 2, 1, 3, 1, 2, 1};

/* a alook up table containing the index of the leftmost bits of the	*/
/* binary representation of an unsigned char. used for finding left	*/
/* edge of black in images. 						*/
/*		for ( i = 0 ; i < 256 ; i++ )				*/
/*		{							*/
/*		   for ( x = (unsigned char)i, k = 1 ; !(x%2) ; k++ )	*/
/*		   x = x >> 1;						*/
/*		   printf("%d,%c", k, (i+1) % 8 ? ' ' : '\n');		*/
/*		}							*/
static unsigned char leftlut[2<<BITSPERBYTE] = {
0, 1, 2, 2, 3, 3, 3, 3,
4, 4, 4, 4, 4, 4, 4, 4,
5, 5, 5, 5, 5, 5, 5, 5,
5, 5, 5, 5, 5, 5, 5, 5,
6, 6, 6, 6, 6, 6, 6, 6,
6, 6, 6, 6, 6, 6, 6, 6,
6, 6, 6, 6, 6, 6, 6, 6,
6, 6, 6, 6, 6, 6, 6, 6,
7, 7, 7, 7, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 7,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8};

/**************************************************************/
find_corners1(ox,oy,ow,oh, image,iw,ih)
int *ox, *oy, *ow, *oh, iw, ih; unsigned char *image;
{
unsigned char *ptr, *qtr, k, x;
int i, j, bot, iw1;

   if (iw % BITSPERBYTE)
      fatalerr("find_corners1", "width is not multiple of", "BITSPERBYTE");

   *oy = *oh = *ow = *ox = bot = 0;
   iw1 = iw / BITSPERBYTE;

   /* traverse rows down through the image and look for the first	*/
   /* row with any true pixel in it, return this as top *oy		*/
   ptr = image;
   for ( i = 0 ; i < ih ; i++ )
      for ( j = 0 ; j < iw1 ; j++ )
         if (*ptr++)
            *oy = i, i = ih, j = iw1;	/* jump out of both loops	*/


   /* wind back from end of image along rows looking for bottom pixel	*/
   ptr = image + iw1 * ih - 1;		/* point to end of image	*/
   for ( i = ih ; i > *oy ; i-- )
      for ( j = 0 ; j < iw1 ; j++ )
         if (*ptr--)
            bot = i, i = 0, j = iw1;	/* jump out of both loops	*/
   *oh = bot - *oy;


   if (*oh == 0)			/* height is zero so the 	*/
      return;				/* image must be totally empty	*/



   qtr = image + *oy * iw1; x = 0;
   for ( i = 0 ; i < iw1 && x == 0 ; i++ )
   {
      ptr = qtr + i;
      for ( j = 0 ; j < *oh ; j++, ptr += iw1 )
      {
         if (*ptr)
         {
            k = leftlut[*ptr];
            x = max(x, k);

            if (x == BITSPERBYTE)	/* leftmost msb is set		*/
               j = *oh;		/* nothing can be further left so stop	*/
         }
      }
   }
   *ox = i*BITSPERBYTE - (int)x;


   qtr = image + bot * iw1 - 1;
   x = BITSPERBYTE + 1;
   for ( i = iw1 ; i && x == BITSPERBYTE + 1 ; i-- )
   {
      ptr = qtr - iw1 + i;
      for ( j = 0 ; j < *oh ; j++, ptr -= iw1 )
      {
         if (*ptr)
         {
            k = rightlut[*ptr];
            x = min(x, k);

            if (x == 1)		/* that is rightmost lsb is set		*/
               j = *oh;		/* nothing can be further right so stop	*/
         }
      }
   }
   *ow = (i+1)*BITSPERBYTE - (int)x - *ox + 1;
}

/**************************************************************/
/* find top left coords and sizes of bounding box of dark pixels	*/
/* in the image								*/

find_corners8(ox,oy,ow,oh, image,iw,ih)
int *ox, *oy, *ow, *oh, iw, ih; unsigned char *image;
{
unsigned char *ptr, *qtr;
int i, j, bot, rit;

   *ox = *oy = *ow = *oh = bot = rit = 0;

   /* traverse rows down through the image and look for the first	*/
   /* row with any true pixel in it, return this as top *oy		*/
   ptr = image;
   for ( i = 0 ; i < ih ; i++ )
      for ( j = 0 ; j < iw ; j++ )
         if (*ptr++)
            *oy = i, i = ih, j = iw;	/* jump out of both loops	*/


   /* wind back from end of image along rows looking for bottom pixel	*/
   ptr = image + iw * ih - 1;		/* point to end of image	*/
   for ( i = ih ; i > *oy ; i-- )
      for ( j = 0 ; j < iw ; j++ )
         if (*ptr--)
            bot = i, i = 0, j = iw;	/* jump out of both loops	*/
   *oh = bot - *oy;


   if (*oh == 0)			/* no height so image is empty,	*/
      return;				/* we've set outputs, so leave	*/



   /* jump down columns looking for leftmost pixel			*/
   qtr = image + *oy * iw;
   for ( i = 0 ; i < iw ; i++ )
   {
      ptr = qtr + i;
      for ( j = 0 ; j < *oh ; j++, ptr += iw )
         if (*ptr)
            *ox = i, i = iw, j = ih;	/* jump out of both loops	*/
   }


   qtr = image + bot * iw - 1;		/* point to end of last row	*/
   for ( i = iw ; i > *ox ; i-- )
   {
      ptr = qtr + i - iw;
      for ( j = 0 ; j < *oh ; j++, ptr -= iw )
         if (*ptr)
            rit = i, i = 0, j = ih;	/* jump out of both loops	*/
   }
   *ow = rit - *ox;
}
