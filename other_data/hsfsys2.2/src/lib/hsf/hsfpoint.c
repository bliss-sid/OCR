/*
# proc: hsfpoints - locates six registration points within an image of
# proc:             an hsf form.
*/

#include <stdio.h>
#include <hsform.h>
#include <defs.h>

#define MLX1_MIN 0
#define MLY1_MIN 234
#define ML1_W   461
#define ML1_H   488

#define MLX_MIN 0
#define MLY_MIN 510
#define ML_W   411
#define ML_H   490

#define TLX_MIN 471
#define TLY_MIN 105
#define TL_W   534
#define TL_H   393

#define TRX_MIN 1950
#define TRY_MIN 272
#define TR_W   500
#define TR_H   460

#define BLX_MIN 0
#define BLY_MIN 2839
#define BL_W   466
#define BL_H   461

#define BRX_MIN 1994
#define BRY_MIN 2850
#define BR_W   550
#define BR_H   450


hsfpoints(srcdata, srcw, srch, xhyp, yhyp)
unsigned char *srcdata;
int srcw, srch;
int *xhyp, *yhyp;
{
   int histx,histy,histw,histh;
   unsigned char *histdata, *cut_image();
   int *xbins,xblen,*ybins,yblen;

   int mlx_i, mlx_max, mly_i, mly_max, tmly_i, tmly_max;
   int mlx, mly;

   int tlx_i,tlx_max,tly_i,tly_max;
   int tlx, tly;

   int trx_i,trx_max,try_i,try_max;
   int trx, try;

   int blx_i,blx_max,bly_i,bly_max;
   int blx, bly;

   int blx2_i,blx2_max,bly2_i,bly2_max;
   int blx2, bly2;

   int brx_i,brx_max,bry_i,bry_max;
   int brx, bry;


/************** BOTTOM LEFT CORNER ***************************/
   /* cut out the form's bottom left corner for the histdata */
   histx = BLX_MIN;
   histy = BLY_MIN;
   histw = BL_W;
   histh = BL_H;
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   /* find the last box's bottom line on the left */
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_backward(xbins, 0, xblen, 25, &bly_i, &bly_max);
   bly = histy + bly_i;
   free(histdata);
   free(xbins);

   histy = max(bly - 75, 0);
   histh = min(150, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_forward(ybins, 0, yblen, 10, &blx_i, &blx_max);
   find_first_min_backward(ybins, blx_i, 0, &blx_i, &blx_max);
   blx = histx + blx_i;
   free(histdata);
   free(ybins);

   histx = blx;
   histw = min(30, srcw - histx);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_backward(xbins, 0, xblen, 2, &bly_i, &bly_max);
   find_first_min_forward(xbins, bly_i, xblen, &bly_i, &bly_max);
   bly = histy + bly_i;
   free(histdata);
   free(xbins);

   histy = max(bly - 15,0);
   histh = min(30, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_forward(ybins, 0, yblen, 0, &blx_i, &blx_max);
   blx = histx + blx_i;
   free(histdata);
   free(ybins);

/****************** TOP LEFT OF CONST BOX ************************/
   /* cut out the form's top left corner of const box for the histdata */
   histx = 0;
   histy = bly - (841 + 75);
   histw = BLX_MIN + BL_W;
   histh = 150;
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_forward(ybins, 0, yblen, 25, &blx2_i, &blx2_max);
   find_first_min_backward(ybins, blx2_i, 0, &blx2_i, &blx2_max);
   blx2 = histx + blx2_i;
   free(histdata);
   free(ybins);

   histx = blx2;
   histw = min(30, srcw - histx);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_min_backward(xbins, xblen-1, 0, &bly2_i, &bly2_max);
   bly2 = histy + bly2_i;
   free(histdata);
   free(xbins);

   histy = max(bly2 - 15, 0);
   histh = min(30, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_forward(ybins, 0, yblen, 0, &blx2_i, &blx2_max);
   blx2 = histx + blx2_i;
   free(histdata);
   free(ybins);

/****************** MIDDLE LEFT BOX ************************/
   /* cut out the form's top-left black box for the histdata */
   histx = MLX1_MIN;
   histy = MLY1_MIN;
   histw = ML1_W;
   histh = ML1_H;
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_forward(xbins, 0, xblen, 25, &mly_i, &mly_max);
   mly = histy + mly_i;
   free(histdata);
   free(xbins);

   histy = mly;
   histh = min(435, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_forward(ybins, 0, yblen, 25, &mlx_i, &mlx_max);
   find_first_min_backward(ybins, mlx_i, 0, &mlx_i, &mlx_max);
   mlx = histx + mlx_i;
   free(histdata);
   free(ybins);

   histx = mlx;
   histw = min(75, srcw - histx);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_forward(xbins, 0, xblen, 25, &mly_i, &mly_max);
   find_first_min_forward(xbins, mly_i, xblen, &mly_i, &mly_max);
   find_first_thresh_forward(xbins, mly_i, xblen, 10, &mly_i, &mly_max);
   find_first_min_forward(xbins, mly_i, xblen, &tmly_i, &tmly_max);
   find_first_thresh_forward(xbins, tmly_i, xblen, 25, &mly_i, &mly_max);
   find_first_min_backward(xbins, mly_i, tmly_i, &mly_i, &mly_max);
   mly = histy + mly_i;
   free(histdata);
   free(xbins);

   histy = mly;
   histh = min(435 - mly_i, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_forward(ybins, 0, yblen, 25, &mlx_i, &mlx_max);
   find_first_min_backward(ybins, mlx_i, 0, &mlx_i, &mlx_max);
   mlx = histx + mlx_i;
   free(histdata);
   free(ybins);

   histx = mlx;
   histw = min(30, srcw - histx);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_forward(xbins, 0, xblen, 0, &mly_i, &mly_max);
   mly = histy + mly_i;
   free(histdata);
   free(xbins);

   histy = mly;
   histh = min(30, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_forward(ybins, 0, yblen, 0, &mlx_i, &mlx_max);
   mlx = histx + mlx_i;
   free(histdata);
   free(ybins);

/****************** TOP LEFT TITLE ************************/
   /* cut out the form's top-left title for the histdata */
   histx = TLX_MIN;
   histy = TLY_MIN;
   histw = TL_W;
   histh = TL_H;
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_forward(xbins, 0, xblen, 25, &tly_i, &tly_max);
   tly = histy + tly_i;
   free(histdata);
   free(xbins);

   histy = max(tly - 75, 0);
   histh = min(tly + 75 - histy, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_forward(ybins, 0, yblen, 10, &tlx_i, &tlx_max);
   find_first_min_backward(ybins, tlx_i, 0, &tlx_i, &tlx_max);
   tlx = histx + tlx_i;
   free(histdata);
   free(ybins);

   histx = tlx;
   histw = min(30, srcw - histx);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_forward(xbins, 0, xblen, 2, &tly_i, &tly_max);
   find_first_min_backward(xbins, tly_i, 0, &tly_i, &tly_max);
   tly = histy + tly_i;
   free(histdata);
   free(xbins);

   histy = tly;
   histh = min(30, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_forward(ybins, 0, yblen, 0, &tlx_i, &tlx_max);
   tlx = histx + tlx_i;
   free(histdata);
   free(ybins);

/****************** TOP RIGHT CORNER ***********************/
   /* cut out the form's top-right corner for the histdata */
   histx = TRX_MIN;
   histy = TRY_MIN;
   histw = TR_W;
   histh = TR_H;
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   /* find the top-right box's top line on the right */
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_forward(xbins, 0, xblen, 25, &try_i, &try_max);
   try = histy + try_i;
   free(histdata);
   free(xbins);

   histy = max(try - 75, 0);
   histh = min(try + 100 - histy, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_backward(ybins, 0, yblen, 10, &trx_i, &trx_max);
   find_first_min_forward(ybins, trx_i, yblen, &trx_i, &trx_max);
   trx = histx + trx_i;
   free(histdata);
   free(ybins);

   histx = max(trx - 15,0);
   histw = min(30, srcw - histx);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_forward(xbins, 0, xblen, 2, &try_i, &try_max);
   find_first_min_backward(xbins, try_i, 0, &try_i, &try_max);
   try = histy + try_i;
   free(histdata);
   free(xbins);

   histy = try;
   histh = min(15, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_backward(ybins, 0, yblen, 0, &trx_i, &trx_max);
   trx = histx + trx_i;
   free(histdata);
   free(ybins);

/************* BOTTOM RIGHT CORNER ****************************/
   /* cut out the form's bottom right corner for the histdata */
   histx = BRX_MIN;
   histy = BRY_MIN;
   histw = BR_W;
   histh = BR_H;
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   /* find the last box's bottom line on the right */
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_backward(xbins, 0, xblen, 25, &bry_i, &bry_max);
   bry = histy + bry_i;
   free(histdata);
   free(xbins);

   histy = max(bry - 75, 0);
   histh = min(150, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_backward(ybins, 0, yblen, 10, &brx_i, &brx_max);
   find_first_min_forward(ybins, brx_i, yblen, &brx_i, &brx_max);
   brx = histx + brx_i;
   free(histdata);
   free(ybins);

   histx = max(brx - 15, 0);
   histw = min(30, srcw - histx);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_x_hist(histdata,histw,histh,&xbins,&xblen);
   find_first_thresh_backward(xbins, 0, xblen, 2, &bry_i, &bry_max);
   find_first_min_forward(xbins, bry_i, xblen, &bry_i, &bry_max);
   bry = histy + bry_i;
   free(histdata);
   free(xbins);

   histy = max(bry - 15, 0);
   histh = min(30, srch - histy);
   histdata=(unsigned char *)cut_image(srcdata,srcw,srch,
            &histx,histy,&histw,histh);
   compute_y_hist(histdata,histw,histh,&ybins,&yblen);
   find_first_thresh_backward(ybins, 0, yblen, 0, &brx_i, &brx_max);
   brx = histx + brx_i;
   free(histdata);
   free(ybins);

/******************* STORE RESULTS ******************************/
   xhyp[0] = mlx;
   yhyp[0] = mly;
   xhyp[1] = tlx;
   yhyp[1] = tly;
   xhyp[2] = trx;
   yhyp[2] = try;
   xhyp[3] = blx2;
   yhyp[3] = bly2;
   xhyp[4] = blx;
   yhyp[4] = bly;
   xhyp[5] = brx;
   yhyp[5] = bry;
}
