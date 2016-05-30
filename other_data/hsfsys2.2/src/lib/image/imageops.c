/*
# proc: cut_image - returns the subimage from a binary bitmap that is
# proc:             byte aligned in origin and width.
# proc: cut_exact_image - returns the exact subimage requested from a binary
# proc:                   bitmap.
# proc: imagedup - takes an image of specified depth and duplicates its
# proc:            contents in a new image.
*/

/************************************************************/
/*         File Name: ImageOps.c                            */
/*         Package:   NIST Raster Utilities                 */
/*                                                          */
/*         Contents:  Cut_Image()                           */
/************************************************************/
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <defs.h>
#include <imageops.h>

/************************************************************/
/*         Routine:   Cut_image()                           */
/*         Author:    Michael D. Garris                     */
/*         Date:      5/02/89                               */
/************************************************************/
/************************************************************/
/* Cut_image() takes a raw raster stream of given pixel     */
/* width and height, and clips a sub-raster image of "byte- */
/* aligned" origin and dimensions.                          */
/************************************************************/
unsigned char *cut_image(data,sw,sh,x,y,w,h)
unsigned char *data;
int sw,sh,*x,y,*w,h;
{
   unsigned char *cdata,*cptr;
   int l,source_width,dest_width,horiz_offset;

   /* word align truncating to the left */
   *x = (int)(*x/WD_SIZE) * WD_SIZE;
   /* word align padding to the right */
   *w = (int)((double)ceil((double)(*w/WD_SIZE))*WD_SIZE) ;
   /* byte width of source raster buffer */
   source_width = (int)(sw/BYTE_SIZE);
   /* byte width of sub-image */
   dest_width = (int)(*w/BYTE_SIZE);
   /* allocate sub-image buffer */
   malloc_uchar(&cdata, dest_width * h, "cut_image : cdata");
   cptr = cdata;
   /* horizontal byte offset into source data */
   horiz_offset = (int)(*x/BYTE_SIZE);
   /* foreach line to be clipped ... */
   for(l = y; l < (y+h); l++){
      /* foreach byte to be clipped ... */
      memcpy(cptr,(data + (l * source_width) + horiz_offset),dest_width);
      cptr += dest_width;
   }
   /* return the sub-image */
   return(cdata);
}

/************************************************************/
unsigned char *cut_exact_image(data, dw, dh, cx, cy, cw, ch)
unsigned char *data;
int dw, dh, cx, cy, *cw, ch;
{
   unsigned char *cdata;
   int aw, size;

   aw = ((int)ceil((double)((*cw)/8.0)))<<3;
   size = SizeFromDepth(aw, ch, 1);
   calloc_uchar(&cdata, size, "cut_exact_image : cdata");
   binary_subimage_copy(data, dw, dh, cdata, aw, ch, cx, cy, *cw, ch, 0, 0);
   *cw = aw;
   return(cdata);
}

/************************************************************/
unsigned char *imagedup(image, width, height, depth)
unsigned char *image;
int width, height, depth;
{
  unsigned char *dup = NULL, *mallocate_image();

  dup = mallocate_image(width, height, depth);
  memcpy(dup, image, SizeFromDepth(width, height, depth));
  return(dup);
}
