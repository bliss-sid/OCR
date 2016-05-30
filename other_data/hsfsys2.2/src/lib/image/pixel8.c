/*
# proc: find_pix_north - from a given point in an 8-bit pixel image, finds the
# proc:                  the first pixel upward that has the specified value.
# proc: find_pix_south - from a given point in an 8-bit pixel image, finds the
# proc:                  the first pixel downward that has the specified value.
# proc: find_pix_east - from a given point in an 8-bit pixel image, finds the
# proc:                 the first pixel rightward that has the specified value.
# proc: find_pix_west - from a given point in an 8-bit pixel image, finds the
# proc:                 the first pixel leftward that has the specified value.
# proc: find_pix_fwd_on_line - searches forward along a specified segment of a line
# proc:                 trajectory for the first pixel that has the specified value.
# proc: find_pix_bwd_on_line - searches backward along a specified segment of a line
# proc:                 trajectory for the first pixel that has the specified value.
# proc: n_pixels_on_line - counts the number of pixels with specified value along
# proc:                 a given line trajectory.
# proc: sub_column_eq - checks if the designated portion of a column in an 8-bit
# proc:                 pixel image equals the specified pixel value.
# proc: sub_row_eq - checks if the designated portion of a row in an 8-bit
# proc:              pixel image equals the specified pixel value.
# proc: next_pix_in_region_ptr - resumes search for a pixel in a region of a binary
# proc:                      char image specified by a top and bottom trajectory using
# proc:                      pointers.
# proc: next_pix_in_region - resumes search for a pixel in a region of a binary char
# proc:                      image specified by a top and bottom trajectory spanning the
# proc:                      entire width of the image.
*/

#include <stdio.h>
#include <defs.h>

/***************************************************************************/
/* find_pix_north - locates the next pixel of specified value upward,      */
/* given a starting point in the image from which to begin the search. If  */
/* pixel is not found, then the routine returns NOT_FOUND.                 */
/***************************************************************************/
find_pix_north(pix, sx, sy, cdata, w, h)
int pix, sx, sy, w, h;
unsigned char *cdata;
{
   unsigned char *cptr;
   int y;

   if(!(is_in_range(pix, 0, 255)))
      fatalerr("find_pix_north", "pixel value must be [0..255]", NULL);
   if((!is_in_range(sx, 0, w-1)) ||
      (!is_in_range(sy, 0, h-1))){
      fatalerr("find_pix_north", "coordinates exceed image dimensions", NULL);
   }

   y = sy;
   cptr = cdata + (sy*w) + sx;
   while((y >= 0) && (*cptr != pix)){
      y--;
      cptr -= w;
   }

   if(y < 0)
      return(NOT_FOUND);
   else
      return(y);
}

/***************************************************************************/
/* find_pix_south - locates the next pixel of specified value downward,    */
/* given a starting point in the image from which to begin the search. If  */
/* pixel is not found, then the routine returns NOT_FOUND.                 */
/***************************************************************************/
find_pix_south(pix, sx, sy, cdata, w, h)
int pix, sx, sy, w, h;
unsigned char *cdata;
{
   unsigned char *cptr;
   int y;

   if(!(is_in_range(pix, 0, 255)))
      fatalerr("find_pix_south", "pixel value must be [0..255]", NULL);
   if((!is_in_range(sx, 0, w-1)) ||
      (!is_in_range(sy, 0, h-1))){
      fatalerr("find_pix_south", "coordinates exceed image dimensions", NULL);
   }

   y = sy;
   cptr = cdata + (sy*w) + sx;
   while((y < h) && (*cptr != pix)){
      y++;
      cptr += w;
   }
   if(y >= h)
      return(NOT_FOUND);
   else
      return(y);
}

/***************************************************************************/
/* find_pix_east - locates the next pixel of specified value to the right, */
/* given a starting point in the image from which to begin the search. If  */
/* pixel is not found, then the routine returns NOT_FOUND.                 */
/***************************************************************************/
find_pix_east(pix, sx, sy, cdata, w, h)
int pix, sx, sy, w, h;
unsigned char *cdata;
{
   unsigned char *cptr;
   int x;

   if(!(is_in_range(pix, 0, 255)))
      fatalerr("find_pix_east", "pixel value must be [0..255]", NULL);
   if((!is_in_range(sx, 0, w-1)) ||
      (!is_in_range(sy, 0, h-1)))
      fatalerr("find_pix_east", "coordinates exceed image dimensions", NULL);
   x = sx;
   cptr = cdata + (sy*w) + sx;
   while((x < w) && (*cptr != pix)){
      x++;
      cptr++;
   }
   if(x >= w)
      return(NOT_FOUND);
   else
      return(x);
}

/***************************************************************************/
/* find_pix_west - locates the next pixel of specified value to the left,  */
/* given a starting point in the image from which to begin the search. If  */
/* pixel is not found, then the routine returns NOT_FOUND.                 */
/***************************************************************************/
find_pix_west(pix, sx, sy, cdata, w, h)
int pix, sx, sy, w, h;
unsigned char *cdata;
{
   unsigned char *cptr;
   int x;

   if(!(is_in_range(pix, 0, 255)))
      fatalerr("find_pix_west", "pixel value must be [0..255]", NULL);
   if((!is_in_range(sx, 0, w-1)) ||
      (!is_in_range(sy, 0, h-1)))
      fatalerr("find_pix_west", "coordinates exceed image dimensions", NULL);

   x = sx;
   cptr = cdata + (sy*w) + sx;
   while((x >= 0) && (*cptr != pix)){
      x--;
      cptr--;
   }
   if(x < 0)
      return(NOT_FOUND);
   else
      return(x);
}

/***************************************************************************/
/* find_pix_fwd_on_line - locates the next pixel of specified value        */
/* searching forward along a line trajectory given starting and ending     */
/* points on the line. If no pixel is found, then the routine returns      */
/* NOT_FOUND.                                                              */
/***************************************************************************/
find_pix_fwd_on_line(pix, fi, ti, lx, ly, num, cdata, w, h)
int pix, fi, ti, *lx, *ly, w, h;
unsigned char *cdata;
{
   int i, max_i;

   if(!(is_in_range(pix, 0, 255)))
      fatalerr("find_pix_fwd_on_line", "pixel value must be [0..255]", NULL);
   max_i = num - 1;
   if(!is_in_range(fi, 0, max_i))
      fatalerr("find_pix_fwd_on_line", "from-index exceeds list length", NULL);
   if(!is_in_range(ti, 0, max_i))
      fatalerr("find_pix_fwd_on_line", "to-index exceeds list length", NULL);
   i = fi;
   while(i <= ti){
      if (!valid_point(lx[i], ly[i], w, h))
         fatalerr("find_pix_fwd_on_line", "coordinates exceed image dimensions", NULL);
      if(*(cdata+(ly[i]*w)+lx[i]) == pix)
         return(i);
      i++;
   }
   return(NOT_FOUND);
}

/***************************************************************************/
/* find_pix_bwd_on_line - locates the next pixel of specified value        */
/* searching backward along a line trajectory given starting and ending    */
/* points on the line. If no pixel is found, then the routine returns      */
/* NOT_FOUND.                                                              */
/***************************************************************************/
find_pix_bwd_on_line(pix, fi, ti, lx, ly, num, cdata, w, h)
int pix, fi, ti, *lx, *ly, w, h;
unsigned char *cdata;
{
   int i, max_i;

   if(!(is_in_range(pix, 0, 255)))
      fatalerr("find_pix_bwd_on_line", "pixel value must be [0..255]", NULL);
   max_i = num - 1;
   if(!is_in_range(fi, 0, max_i))
      fatalerr("find_pix_bwd_on_line", "from-index exceeds list length", NULL);
   if(!is_in_range(ti, 0, max_i))
      fatalerr("find_pix_bwd_on_line", "to-index exceeds list length", NULL);
   i = fi;
   while(i >= ti){
      if (!valid_point(lx[i], ly[i], w, h))
         fatalerr("find_pix_bwd_on_line", "coordinates exceed image dimensions", NULL);
      if(*(cdata+(ly[i]*w)+lx[i]) == pix)
         return(i);
      i--;
   }
   return(NOT_FOUND);
}

/***************************************************************************/
/* n_pixels_on_line - counts the number of pixels with specified value     */
/* along a given line trajectory and determines the location of the left-  */
/* most and right-most pixels with the specified value on the line.        */
/***************************************************************************/
n_pixels_on_line(npix, lx, ly, rx, ry, pix, x1, y1, x2, y2, cdata, w, h)
int *npix, *lx, *ly, *rx, *ry;
int pix, x1, y1, x2, y2;
unsigned char *cdata;
int w, h;
{
   int i, li, ri, max_i;
   int *xlist, *ylist, num, alloc;

   if(!is_in_range(pix, 0, 255))
      fatalerr("n_pixels_on_line", "pixel value must be [0..255]", NULL);
   if(!valid_point(x1, y1, w, h))
      fatalerr("n_pixels_on_line","Point x1, y1","Invalid"); 
   if(!valid_point(x2, y2, w, h))
      fatalerr("n_pixels_on_line","Point x2, y2","Invalid");
   /* interpolate between line's endpoints */
   alloc = 0;
   bres_line_alloc(x1, y1, x2, y2, &xlist, &ylist, &num, &alloc);

   /* find the left-most pixel on the line */
   max_i = num - 1;
   if((li = find_pix_fwd_on_line(pix, 0, max_i, xlist, ylist, num, 
                                  cdata, w, h)) == NOT_FOUND){
      *lx = NOT_FOUND;
      *ly = NOT_FOUND;
      *rx = NOT_FOUND;
      *ry = NOT_FOUND;
      *npix = 0;
      free(xlist);
      free(ylist);
      return;
   }
   else {
      /* store the location of the left-most pixel */
      *lx = xlist[li];
      *ly = ylist[li];
   }

   /* find the right-most pixel on the line */
   if((ri = find_pix_bwd_on_line(pix, max_i, li, xlist, ylist, num, 
                                  cdata, w, h)) == NOT_FOUND){
      /* should never reach here, because fwd search would already have failed */
      fatalerr("n_pixels_on_line", "search backward for a specified pixel failed",
               "should never reach this point");
   }
   else{
      /* store the location of the right-most pixel */
      *rx = xlist[ri];
      *ry = ylist[ri];
   }

   /* count the pixels with specified value between the left-most and right-most pixels */
   *npix = 0;
   for(i = li; i <= ri; i++){
      if(*(cdata + (ylist[i]*w) + xlist[i]) == pix)
         (*npix)++;
   }
   free(xlist);
   free(ylist);
}

/***************************************************************************/
/* sub_column_eq - checks if designated portion of column in an 8-bit      */
/* per pixel image equals the specified pixel value.                       */
/***************************************************************************/
sub_column_eq(pix, x, y1, y2, cdata, w, h)
int pix, x, y1, y2;
unsigned char *cdata;
int w, h;
{
   int y, max_y;
   unsigned char *cptr;

   if(!is_in_range(pix, 0, 255))
      fatalerr("sub_column_eq", "pixel values not in range [0..255]", NULL);
   max_y = h - 1;
   if((!is_in_range(x, 0, w-1)) ||
      (!is_in_range(y1, 0, max_y)) ||
      (!is_in_range(y2, 0, max_y))){
      fatalerr("sub_column_eq", "coordinates exceed image dimensions", NULL);
   }

   cptr = cdata + (y1*w) + x;
   for(y = y1; y <= y2; y++){
      if(*cptr != pix)
         return(FALSE);
      cptr += w;
   }
   return(TRUE);
}

/***************************************************************************/
/* sub_row_eq - checks if designated portion of row in an 8-bit per pixel  */
/* image equals the specified pixel value.                                 */
/***************************************************************************/
sub_row_eq(pix, x1, x2, y, cdata, w, h)
int x1, x2, y;
unsigned char *cdata;
int w, h;
{
   int x, max_x;
   unsigned char *cptr;

   if(!is_in_range(pix, 0, 255))
      fatalerr("sub_row_eq", "pixel values not in range [0..255]", NULL);
   max_x = w - 1;
   if((!is_in_range(x1, 0, max_x)) ||
      (!is_in_range(x2, 0, max_x)) ||
      (!is_in_range(y, 0, h-1)))
      fatalerr("sub_row_eq", "coordinates exceed image dimensions", NULL);

   cptr = cdata + (y*w) + x1;
   for(x = x1; x <= x2; x++){
      if(*cptr != pix)
         return(FALSE);
      cptr ++;
   }
   return(TRUE);
}

/***********************************************************************************/
next_pix_in_region_ptr(pix, px, py, sx, sy, ex, ey, ptys, pbys, cdata, w, h)
int pix, *px, *py, sx, sy, ex, ey;
int **ptys, **pbys;
unsigned char *cdata;
int w, h;
{
   unsigned char *cptr;
   int cx, cy;
   int *tys, *bys;

   if(!is_in_range(pix, 0, 255))
      fatalerr("next_pix_in_region_ptr", "pixel value not in range [0..255]", NULL);
   if(!is_in_range(sx, 0, w-1) || !is_in_range(sy, 0, h-1))
      fatalerr("next_pix_in_region_ptr", "start location exceeds image dimensions", NULL);

   cx = sx;
   cy = sy;
   tys = *ptys;
   bys = *pbys;
   if(cy > *bys){
      cx++;
      tys++;
      bys++;
      if(cx > ex){
         *ptys = tys;
         *pbys = bys;
         return(NOT_FOUND);
      }
      cy = *tys;
   }
   cptr = cdata + (cy*w) + cx;
   while(cx <= ex){
      if(*cptr == pix){
         *px = cx;
         *py = cy;
         *ptys = tys;
         *pbys = bys;
         return(FOUND);
      }
      cy++;
      if(cy > *bys){
         cx++;
         tys++;
         bys++;
         if(cx > ex)
            break;
         cy = *tys;
      }
      cptr = cdata + (cy*w) + cx;
   }
   *ptys = tys;
   *pbys = bys;
   return(NOT_FOUND);
}

/***********************************************************************************/
next_pix_in_region(pix, px, py, sx, sy, tys, bys, cdata, w, h)
int pix, *px, *py, sx, sy;
int *tys, *bys;
unsigned char *cdata;
int w, h;
{
   unsigned char *cptr;
   int cx, cy;

   if(!is_in_range(pix, 0, 255))
      fatalerr("next_pix_in_region", "pixel value not in range [0..255]", NULL);
   if(!is_in_range(sx, 0, w-1) || !is_in_range(sy, 0, h-1))
      fatalerr("next_pix_in_region", "start location exceeds image dimensions", NULL);

   cx = sx;
   cy = sy;
   if(cy > bys[cx]){
      cx++;
      if(cx >= w)
         return(NOT_FOUND);
      cy = tys[cx];
   }
   cptr = cdata + (cy*w) + cx;
   while(cx < w){
      if(*cptr == pix){
         *px = cx;
         *py = cy;
         return(FOUND);
      }
      cy++;
      if(cy > bys[cx]){
         cx++;
         if(cx >= w)
            break;
         cy = tys[cx];
      }
      cptr = cdata + (cy*w) + cx;
   }
   return(NOT_FOUND);
}
