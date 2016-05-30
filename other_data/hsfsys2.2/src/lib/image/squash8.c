/*
# proc: squashx8_8 - takes a charimage and squashes it by a factor of 8 in both
# proc:            dimensions without throwing away rows and columns (~4 X slower).
*/

#include <stdio.h>
#include <math.h>
#include <defs.h>

/*************************************************************/
squashx8_8(idata, iw, ih, odata, ow, oh)
unsigned char *idata, **odata;
int iw, ih, *ow, *oh;
{
   unsigned char *cptr, *sptr, *optr;
   unsigned char *allocate_image();
   int i, r, c, h8, offset, rem, found;

   /* compute squashed dimensions */
   *ow = iw>>3;
   *oh = (int)ceil(ih/8.0);

   /***********************/
   /* squash horizontally */
   /***********************/

   /* allocate image for squash in 1-dimension */
   (*odata)  = allocate_image(*ow, ih, 8);

   cptr = idata;
   optr = (*odata);

   /* for each row in image ... */
   for(r = 0; r < ih; r++){
      /* for each output pixel in current row */
      for(c = 0; c < (*ow); c++){
         /* set pixel ptr to start of next byte */
         found = FALSE;
         /* for 8 pixels ... */
         for(i = 8; i > 0; i--){
            if(*cptr){
               found = TRUE;
               break;
            }
            /* next pixel */
            cptr++;
         }
         if(found)
            *optr = 1;
         else
            *optr = 0;
         optr++;
         cptr += i;
      }
   }

   /***********************/
   /* squash vertically   */
   /***********************/

   sptr = (*odata);
   optr = (*odata);

   h8 = ih >> 3;
   offset = (*ow)*7;

   for(r = 0; r < h8; r++){
      for(c = 0; c < (*ow); c++){
         cptr = sptr;
         found = FALSE;
         for(i = 0; i < 8; i++){
            if(*cptr){
               found = TRUE;
               break;
            }
            cptr += (*ow);
         }
         if(found)
            *optr = 1;
         else
            *optr = 0;
         optr++;
         sptr++;
      }
      sptr += offset;
   }
 
   if((rem = ih % 8) == 0)
      return;

   for(c = 0; c < (*ow); c++){
      cptr = sptr;
      for(i = 0; i < rem; i++){
         found = FALSE;
         if(*cptr == 1){
            found = TRUE;
            break;
         }
         if(found)
            *optr = 1;
         else
            *optr = 0;
         cptr += (*ow);
      }
      optr++;
      sptr++;
   }
}
