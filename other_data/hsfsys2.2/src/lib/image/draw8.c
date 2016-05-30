/*
# proc: drawline8 - draws a line between two points in an 8-bit
# proc:             grayscale image.
# proc: drawline8_xor - draws a line using logical XOR onto an 8-bit per pixel
# proc:                 binary image by interpolating between given endpoints.
# proc:                 really flips the image pixels equal to XOR with 1.
# proc: valid_point - determines if a point lies with the specified
# proc:               dimensions.
# proc: drawvertline8 - draws a vertically straight line in a binary char image.
# proc:
# proc: tstndrawvertline8 - draws a vertically straight line in a binary char
# proc:              image and prempts itself is destination pixel already set
# proc:              to non-zero.
*/

#include <stddef.h>
#include <defs.h>

/*************************************************************************/
drawline8(image, width, height, x0, y0, x1, y1, barpix)
unsigned char *image;
int width, height, barpix, x0, y0, x1, y1;
{
  int num = 0, nbres = 0, *xlist, *ylist;

  if (!valid_point(x0, y0, width, height))
     fatalerr("drawline","Point x0, y0","Invalid"); 
  if (!valid_point(x1, y1, width, height))
     fatalerr("drawline","Point x1, y1","Invalid"); 
  bres_line_alloc(x0, y0, x1, y1, &xlist, &ylist, &num, &nbres);
  while (num--)
     *(image + ylist[num]*width + xlist[num]) = barpix;
  free(xlist); free(ylist);
}
	
/*************************************************************************/
drawline8_xor(image, width, height, depth, x0, y0, x1, y1)
unsigned char *image;
int width, height, depth, x0, y0, x1, y1;
{
  int num = 0, nbres = 0, *xlist, *ylist;
  unsigned char *iptr;

  if (!valid_point(x0, y0, width, height))
     fatalerr("drawline","Point x0, y0","Invalid"); 
  if (!valid_point(x1, y1, width, height))
     fatalerr("drawline","Point x1, y1","Invalid"); 
  bres_line_alloc(x0, y0, x1, y1, &xlist, &ylist, &num, &nbres);
  while (num--)
     iptr = image + ylist[num]*width + xlist[num],  *iptr = !*iptr;
  free(xlist); free(ylist);
}

/*************************************************************************/
int valid_point(x, y, w, h)
int x, y, w, h;
{
  if (w < 1)  fatalerr("valid_point","w","Invalid");
  if (h < 1)  fatalerr("valid_point","h","Invalid");
  if (x < 0 ||  w <= x)  return(0);
  if (y < 0 ||  h <= y)  return(0);
  return(1);
}

/***************************************************************************/
drawvertline8(cdata, w, h, x1, y1, x2, y2, pix)
unsigned char *cdata;
int w, h, x1, y1, x2, y2, pix;
{
   int y;
   unsigned char *cptr;

   cptr = cdata + (y1 * w) + x1;
   for(y = y1; y <= y2; y++){
      *cptr = pix;
      cptr += w;
   }
}

/***************************************************************************/
tstndrawvertline8(cdata, w, h, x1, y1, x2, y2, pix)
unsigned char *cdata;
int w, h, x1, y1, x2, y2, pix;
{
   int y;
   unsigned char *cptr;

   cptr = cdata + (y1 * w) + x1;
   for(y = y1; y <= y2; y++){
      if(*cptr != 0)
        return(FALSE);
      *cptr = pix;
      cptr += w;
   }
   return(TRUE);
}
