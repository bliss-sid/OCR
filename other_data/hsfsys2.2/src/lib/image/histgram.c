/*
# proc: compute_hist - computes a horizontal or vertical spatial histogram
# proc:                from a binary bitmap.
# proc: compute_x_hist - computes a horizontal spatial histogram from a
# proc:                  binary bitmap.
# proc: compute_y_hist - computes a vertical spatial histogram from a
# proc:                  binary bitmap.
# proc: hist_nruns_hori - given a list of horizontal runs, computes a histogram
# proc:                   by summing the top-n longest runs in each row.
# proc: hist_nruns_hori2 - given a list of horizontal runs, computes a histogram
# proc:                 by summing the top-n longest runs in each row and returns
# proc:                 the sorted order of the runs.
*/

/*************************************************************/
/*                                                           */
/*       File Name:    Histogram.c                           */
/*       Package:      Spatial Histogram Computations        */
/*                                                           */
/*       Contents:     Compute_Hist()                        */
/*                     Compute_X_Hist()                      */
/*                     Compute_Y_Hist()                      */
/*************************************************************/
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <masks.h>
#include <bits.h>
#include <defs.h>
#include <histgram.h>

/************************************************************/
/*         Routine:   Compute_Hist()                        */
/*         Author:    Michael D. Garris                     */
/*         Date:      7/21/89                               */
/************************************************************/
/*************************************************************/
/* Compute_hist() Serves as a switch for orthogonal hist-    */
/* ogram orientations.                                       */
/*************************************************************/
void compute_hist(data,width,height,orient,bins,len)
unsigned char *data;
int width,height,orient,**bins,*len;
{
    switch (orient){
       case Y_HIST:
            compute_y_hist(data,width,height,bins,len);
            break;
       case X_HIST:
            compute_x_hist(data,width,height,bins,len);
            break;
       default:
            fatalerr("comput_hist","Unknown histogram orientation",NULL);
    }
}

/************************************************************/
/*         Routine:   Compute_X_Hist()                      */
/*         Author:    Michael D. Garris                     */
/*         Date:      7/21/89                               */
/************************************************************/
/*************************************************************/
/* Compute_x_hist() Computes an X-oriented histogram on the  */
/* defined rectangular image region passed.                  */
/*************************************************************/
void compute_x_hist(data,width,height,bins,len)
unsigned char *data;
int width,height,**bins,*len;
{
int y, bytewidth;

   bytewidth = width >> 3;
   malloc_int(bins, height, "compute_x_hist : bins");

   for ( y = 0 ; y < height ; y++, data += bytewidth )
      (*bins)[y] = black_pixel_count(data, width, 1);
   
   *len = height;
}

/************************************************************/
/*         Routine:   Compute_Y_Hist()                      */
/*         Author:    Michael D. Garris                     */
/*         Date:      7/21/89                               */
/************************************************************/
/*************************************************************/
/* Compute_y_hist() Computes a Y-oriented histogram on the   */
/* defined rectangular image region passed.                  */
/*************************************************************/
void compute_y_hist(data,width,height,bins,len)
unsigned char *data;
int width,height,**bins,*len;
{
   int bytewidth,bytelines,y,x_byte,bit,bnum,memsize;
   unsigned char byte;

   bytewidth = width/8;
   memsize = width * sizeof(int);
   malloc_int(bins, memsize, "compute_y_hist : bins");
   memset((*bins),0,memsize);
   for(y = 0; y < height; y++){
      bnum = 0;
      bytelines = y * bytewidth;
      for(x_byte = 0; x_byte < bytewidth; x_byte++){
         byte = *(data + bytelines + x_byte);
         for(bit = 7; bit >= 0; bit--){
            ((*bins)[bnum])+= get_bit(byte,bit);
            bnum++;
         }
      }
   }
   *len = width;
}

/**********************************************************/
hist_nruns_hori(grab, rx, ry, rlen, nruns, w, h, bins, nbins)
int grab, *rx, *ry, *rlen, nruns, w, h;
int **bins, *nbins;
{
   int *rank, *sort_index_on_int_2id();
   int ri, cy, c;

   rank = sort_index_on_int_2id(ry, rlen, nruns);

   calloc_int(bins, h, "hist_nruns_hori : bins");
   *nbins = h;
   ri = 0;
   cy = 0;
   while(ri < nruns){
      c = 0;
      while((ri < nruns) && (ry[rank[ri]] == cy) && (c < grab)){
         (*bins)[cy] += rlen[rank[ri]];
         c++;
         ri++;
      }
      while((ri < nruns) && (ry[rank[ri]] == cy))
         ri++;
      cy++;
   }
   free(rank);
}

/**********************************************************/
hist_nruns_hori2(grab, rx, ry, rlen, nruns, w, h, bins, nbins, rank)
int grab, *rx, *ry, *rlen, nruns, w, h;
int **bins, *nbins, **rank;
{
   int *sort_index_on_int_2id();
   int ri, cy, c;

   (*rank) = sort_index_on_int_2id(ry, rlen, nruns);

   calloc_int(bins, h, "hist_nruns_hori2 : bins");
   *nbins = h;
   ri = 0;
   cy = 0;
   while(ri < nruns){
      c = 0;
      while((ri < nruns) && (ry[(*rank)[ri]] == cy) && (c < grab)){
         (*bins)[cy] += rlen[(*rank)[ri]];
         c++;
         ri++;
      }
      while((ri < nruns) && (ry[(*rank)[ri]] == cy))
         ri++;
      cy++;
   }
}
