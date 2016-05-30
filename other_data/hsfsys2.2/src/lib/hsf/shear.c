/*
# proc: shear_mis - takes characters in the specified mis structure and
# proc:             horizontally shears them in order to straighten them up.
# proc: shear_char - takes a binary bitmap of a character and horizontally
# proc:              shears it in order to straighten the character up.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>
#include <defs.h>

/*************************************************************************/
shear_mis(smis, sfctr, imis)
MIS **smis, *imis;
float **sfctr;
{
   unsigned char *iptr, *sptr;
   float shear_char();
   int esize;
   int i;

   *smis = allocmis(NULL, 1, 0, imis->entw, imis->enth, imis->ent_num);
   esize = SizeFromDepth(imis->entw, imis->enth, imis->misd);
   malloc_flt(sfctr, imis->ent_num, "shear_mis : sfctr");
   iptr = imis->data;
   sptr = (*smis)->data;
   for(i = 0; i < imis->ent_num; i++){
      (*sfctr)[i] = shear_char(iptr, sptr, imis->entw, imis->enth);
      iptr += esize;
      sptr += esize;
   }
   (*smis)->ent_num = imis->ent_num;
}

/*************************************************************************/
float shear_char(idata, odata, w, h)
unsigned char *idata, *odata;
int w, h;
{
   int len, toprow, botrow, topleft, botleft;
   int r, sval, ch;
   int mh;
   unsigned char *cdata, *sdata, *cptr, *sptr, *allocate_image();
   float fctr;

   len = w * h;
   cdata = allocate_image(w, h, 8);
   bits2bytes(idata, cdata, len);
   if((toprow = char_top(cdata, w, h)) == -1)
      fatalerr("shear_char", "top of character not found", NULL);
   if((botrow = char_bottom(cdata, w, h)) == -1)
      fatalerr("shear_char", "bottom of character not found", NULL);
   ch = botrow - toprow + 1;
   if(ch <= 0)
      fatalerr("shear_char", "character not found", NULL);
   if(ch <= 5){
      binary_subimage_copy(idata, w, h, odata, w, h, 0, 0, w, h, 0, 0);
      free(cdata);
      return(0.0);
   }
   /* skip potentially noisy edges */
   toprow = min(toprow+2, h-1);
   botrow = max(botrow-2, 0);
   if(toprow == botrow)
      fatalerr("shear_char",
               "top row of character should not equal bottom row of character",
               NULL);
   if((topleft = left_pix(cdata + (toprow * w), w)) == -1){
      binary_subimage_copy(idata, w, h, odata, w, h, 0, 0, w, h, 0, 0);
      free(cdata);
      return(0.0);
   }
   if((botleft = left_pix(cdata + (botrow * w), w)) == -1){
      binary_subimage_copy(idata, w, h, odata, w, h, 0, 0, w, h, 0, 0);
      free(cdata);
      return(0.0);
   }
   if(topleft == botleft){
      binary_subimage_copy(idata, w, h, odata, w, h, 0, 0, w, h, 0, 0);
      free(cdata);
      return(0.0);
   }
   /* compute shear factor so that topleft and botleft line up when done */
   fctr = (topleft - botleft)/(float)(botrow - toprow);

   sdata = allocate_image(w, h, 8);
   mh = h>>1;
   cptr = cdata;
   sptr = sdata;
   for(r = 0; r < h; r++){
      sval = sround((r - mh) * fctr);
      shift_row(cptr, sptr, w, sval);
      cptr += w;
      sptr += w;
   }
   bytes2bits(sdata, odata, len);
   free(cdata);
   free(sdata);
   return(fctr);
}
