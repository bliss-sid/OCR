/*
# proc: shear_mis8 - takes characters in a 1 pixel per byte mis structure and
# proc:             horizontally shears them in order to straighten them up.
# proc: shear_char8 - takes a binary charimage of a character and horizontally
# proc:              shears it in order to straighten the character up.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>
#include <defs.h>

/*************************************************************************/
shear_mis8(smis, sfctr, imis)
MIS **smis, *imis;
float **sfctr;
{
   unsigned char *iptr, *sptr;
   float shear_char8();
   int esize;
   int i;

   if(imis->misd != 8)
      fatalerr("shear_mis8", "image must be of depth 8", NULL);

   *smis = allocmis(NULL, imis->misd, 0, imis->entw, imis->enth, imis->ent_num);
   esize = SizeFromDepth(imis->entw, imis->enth, imis->misd);
   malloc_flt(sfctr, imis->ent_num, "shear_mis8 : sfctr");
   iptr = imis->data;
   sptr = (*smis)->data;
   for(i = 0; i < imis->ent_num; i++){
      (*sfctr)[i] = shear_char8(iptr, sptr, imis->entw, imis->enth);
      iptr += esize;
      sptr += esize;
   }
   (*smis)->ent_num = imis->ent_num;
}

/*************************************************************************/
float shear_char8(cdata, odata, w, h)
unsigned char *cdata, *odata;
int w, h;
{
   int toprow, botrow, topleft, botleft;
   int r, sval, ch;
   int mh;
   unsigned char *cptr, *sptr;
   float fctr;

   if((toprow = char_top(cdata, w, h)) == -1){
/*    fatalerr("shear_char8", "top of character not found", NULL); */
      subimage_copy(cdata, w, h, odata, w, h, 0, 0, w, h, 0, 0);
      return(0.0);
   }
   if((botrow = char_bottom(cdata, w, h)) == -1)
      fatalerr("shear_char8", "bottom of character not found", NULL);
   ch = botrow - toprow + 1;
   if(ch <= 0)
      fatalerr("shear_char8", "character not found", NULL);
   if(ch <= 5){
      subimage_copy(cdata, w, h, odata, w, h, 0, 0, w, h, 0, 0);
      return(0.0);
   }
   /* skip potentially noisy edges */
   toprow = min(toprow+2, h-1);
   botrow = max(botrow-2, 0);
   if(toprow == botrow)
      fatalerr("shear_char8",
               "top row of character should not equal bottom row of character",
               NULL);
   if((topleft = left_pix(cdata + (toprow * w), w)) == -1){
      subimage_copy(cdata, w, h, odata, w, h, 0, 0, w, h, 0, 0);
      return(0.0);
   }
   if((botleft = left_pix(cdata + (botrow * w), w)) == -1){
      subimage_copy(cdata, w, h, odata, w, h, 0, 0, w, h, 0, 0);
      return(0.0);
   }
   if(topleft == botleft){
      subimage_copy(cdata, w, h, odata, w, h, 0, 0, w, h, 0, 0);
      return(0.0);
   }
   /* compute shear factor so that topleft and botleft line up when done */
   fctr = (topleft - botleft)/(float)(botrow - toprow);

   mh = h>>1;
   cptr = cdata;
   sptr = odata;
   for(r = 0; r < h; r++){
      sval = sround((r - mh) * fctr);
      shift_row(cptr, sptr, w, sval);
      cptr += w;
      sptr += w;
   }
   return(fctr);
}
