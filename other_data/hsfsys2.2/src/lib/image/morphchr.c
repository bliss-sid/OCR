/*
# proc: erode_charimage - set true pixel to zero if any of 4 neighbors is zero
# proc:                   in a character image.
# proc: dilate_charimage - set false pixel to one if any of 4 neighbors is one
# proc:                    in a character image.
# proc: get_south8 - return value of char image pixel 1 below of current pixel
# proc:              if defined else return (char)0
# proc: get_north8 - return value of char image pixel 1 above of current pixel
# proc:              if defined else return (char)0
# proc: get_east8  - return value of char image pixel 1 right of current pixel
# proc:              if defined else return (char)0
# proc: get_west8  - return value of char image pixel 1 left  of current pixel
# proc:              if defined else return (char)0
*/


#include <memory.h>

/******************************************************************/
/* erode a one bit per byte char image, inp. Result is out which  */
/* must be disjoint with inp. The data in out before the call is  */
/* irrelevant, and is zeroed and filled by this routine. iw and   */
/* ih are the width and height of the image in pixels. Both inp   */
/* and out point to iw*ih bytes                                   */
/******************************************************************/
 
erode_charimage(inp, out, iw, ih)
unsigned char *inp, *out; int iw, ih;
{
int row, col;
unsigned char *itr = inp, *otr = out;
 
   memcpy(out, inp, iw*ih);
 
   /* for true pixels. kill pixel if there is at least one false neighbor */
   for ( row = 0 ; row < ih ; row++ )
      for ( col = 0 ; col < iw ; col++ )
      {  
         if (*itr)      /* erode only operates on true pixels */
         {
            /* more efficient with C's left to right evaluation of     */
            /* conjuctions. E N S functions not executed if W is false */
            if (!(get_west8 (itr, col        ) &&
                  get_east8 (itr, col, iw    ) &&
                  get_north8(itr, row, iw    ) &&
                  get_south8(itr, row, iw, ih)))
               *otr = 0;
         }
         itr++ ; otr++;
      }  
}

/******************************************************************/
/* dilate a one bit per byte char image, inp. Result is out which  */
/* must be disjoint with inp. The data in out before the call is  */
/* irrelevant, and is zeroed and filled by this routine. iw and   */
/* ih are the width and height of the image in pixels. Both inp   */
/* and out point to iw*ih bytes                                   */
/******************************************************************/
 
dilate_charimage(inp, out, iw, ih)
unsigned char *inp, *out; int iw, ih;
{
int row, col;
unsigned char *itr = inp, *otr = out;
 
   memcpy(out, inp, iw*ih);
 
   /* for all pixels. set pixel if there is at least one true neighbor */
   for ( row = 0 ; row < ih ; row++ )
      for ( col = 0 ; col < iw ; col++ )
      {  
         if (!*itr)     /* pixel is already true, neighbors irrelevant */
         {
            /* more efficient with C's left to right evaluation of     */
            /* conjuctions. E N S functions not executed if W is false */
            if (get_west8 (itr, col        ) ||
                get_east8 (itr, col, iw    ) ||
                get_north8(itr, row, iw    ) ||
                get_south8(itr, row, iw, ih))
               *otr = 1;
         }
         itr++ ; otr++;
      }  
}

/************************************************************************/
/* routines for accessing individual neighbors of pixel at (row,col) 	*/
/* row and pixel are zero oriented. That is 0 <= row < ih and           */
/* 0 <= col < iw. ptr points to the (row,col) element of the ih by iw 	*/
/* sized char image which contains as many >bytes< as there are pixels  */
/* in the image. 							*/
/************************************************************************/

get_south8(ptr, row, iw, ih)
unsigned char *ptr; int row, iw, ih;
{
   if (row >= ih-1) /* catch case where image is undefined southwards   */
      return 0;     /* use plane geometry and return false.             */

      return *(ptr+iw);
}

get_north8(ptr, row, iw)
unsigned char *ptr; int row, iw;
{
   if (row < 1)     /* catch case where image is undefined northwards   */
      return 0;     /* use plane geometry and return false.             */

      return *(ptr-iw);
}

get_east8(ptr, col, iw)
unsigned char *ptr; int col, iw;
{
   if (col >= iw-1) /* catch case where image is undefined eastwards    */
      return 0;     /* use plane geometry and return false.             */

      return *(ptr+ 1);
}

get_west8(ptr, col)
unsigned char *ptr; int col;
{
   if (col < 1)     /* catch case where image is undefined westwards     */
      return 0;     /* use plane geometry and return false.              */

      return *(ptr- 1);
}
