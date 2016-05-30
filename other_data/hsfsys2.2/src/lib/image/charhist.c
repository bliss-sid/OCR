/*
# proc: chr_x_hist - computes a horizontal spatial histogram from a
# proc:              1 pixel per byte binary image.
# proc: chr_y_hist - computes a vertical spatial histogram from a
# proc:              1 pixel per byte binary image.
*/

#include <stdio.h>

/*************************************************************************/
chr_x_hist(data,width,height,bins,len)
unsigned char *data;
int width,height,**bins,*len;
{
   unsigned char *pptr;
   int bnum, i, j;

   calloc_int(bins, height, "chr_x_hist : binx");
   pptr = data;
   bnum = 0;
   for(i = 0; i < height; i++){
      for(j = 0; j < width; j++){
         (*bins)[bnum] += *pptr;
         pptr++;
      }
      bnum++;
   }
   *len = bnum;
}

/*************************************************************************/
chr_y_hist(data,width,height,bins,len)
unsigned char *data;
int width,height,**bins,*len;
{
   unsigned char *pptr, *sptr;
   int bnum, i, j;

   calloc_int(bins, width, "chr_y_hist : binx");
   sptr = data;
   bnum = 0;
   for(i = 0; i < width; i++){
      pptr = sptr;
      for(j = 0; j < height; j++){
         (*bins)[bnum] += *pptr;
         pptr+=width;
      }
      sptr++;
      bnum++;
   }
   *len = bnum;
}
