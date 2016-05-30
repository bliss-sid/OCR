/*
# proc: lsq_ind_line_params - given a list of indirectly addressed datapoints,
# proc:                       computes the slope and y-intercept using Least
# proc:                       Squares.
# proc: lsq_line_params - given a list of datapoints, computes the slope
# proc:                   and y-intercept using Least Squares.
# proc: determinant2 - computes the determinant given accumulated x and x*x
# proc:                and the number of datapoints.
# proc: scaleparam - computes the scale parameter for Least Squares with 2
# proc:              degrees of freedom.
# proc: deltaparam - computes the translation parameter for Least Squares
# proc:              with 2 degrees of freedom.
*/

#include <stdio.h>
#include <math.h>
#include <stats.h>
#define TOO_SMALL      0.0001

/***************************************************************************/
void lsq_ind_line_params(m, b, list, len, mxs, mys, n)
float *m, *b;
int *list, len;
int *mxs, *mys, n;
{
   int i, *xs, *ys;

   malloc_int(&xs, len, "lsq_ind_line_params : xs");
   malloc_int(&ys, len, "lsq_ind_line_params : ys");

   for(i = 0; i < len; i++){
      xs[i] = mxs[list[i]];
      ys[i] = mys[list[i]];
   }
   lsq_line_params(m, b, xs, ys, len);
   free(xs);
   free(ys);
}

/***********************************************************************/
/* Using a least squares fit, find the slope and y intercept for the   */
/* points past.                                                        */
/***********************************************************************/
/* m  ==> slope estimation.                                            */
/* b  ==> estimated y intercept                                        */
/* xs ==> list of x coordinates                                        */
/* ys ==> list of y coordinates                                        */
/* num==> number of points                                             */
/***********************************************************************/
void lsq_line_params(m, b, xs, ys, num)
float *m, *b;
int *xs, *ys, num;
{
   float fn, sy, sxy, sx, sxx;
   float scaleparam(), deltaparam(), determinant2();
   float det;

/***********************************************************************/
/* Given the linear equation (y = mx + b) with 2 unknowns: m & b       */
/*                                                                     */
/* A = | fn  sx   |                                                    */
/*     | sx  sxx  |                                                    */
/*                                                                     */
/* B = | sy   |                                                        */
/*     | sxy  |                                                        */
/*                                                                     */
/* P = | b |                                                           */
/*     | m |                                                           */
/*                                                                     */
/* Variable nomenclature:                                              */
/*    fn ==> number of points as a float                               */
/*     s ==> summation across all points                               */
/*     x ==> x-coordinate                                              */
/*     y ==> y-coordinate                                              */
/*    xx ==> x-coordinate of point squared                             */
/*    xy ==> product of x and y-coordinates                            */
/*                                                                     */
/***********************************************************************/
   /* A equations */
   fn = (float)num;
   sx = (float)sumints(xs, num);
   sxx = (float)sumprodints(xs, xs, num);

   /* B equations */
   sy = (float)sumints(ys, num);
   sxy = (float)sumprodints(xs, ys, num);

                          /* <--- A ---> */
   det = (float)determinant2(fn, sx, sxx);

                       /* <--- A --->  <- B -> */
   *m = (float)scaleparam(fn, sx, sxx, sy, sxy, det);
   *b = (float)deltaparam(fn, sx, sxx, sy, sxy, det);
}

/***********************************************************************/
float determinant2(fn, sr, srr)
float fn, sr, srr;
{
   float det;

   det = (fn*srr) - (sr*sr);
   if(fabs(det) < TOO_SMALL){
      if(det <= 0.0)
         det = -TOO_SMALL;
      else
         det = TOO_SMALL;
   }
   return(det);
}

/***********************************************************************/
float scaleparam(fn, sr, srr, sh, shr, det)
float fn, sr, srr, sh, shr, det;
{
   float numer, m;

   numer = (fn*shr) - (sh*sr);
   m = numer/det;
   return(m);
}

/***********************************************************************/
float deltaparam(fn, sr, srr, sh, shr, det)
float fn, sr, srr, sh, shr, det;
{
   float numer, d;

   numer = (sh*srr) - (shr*sr);
   d = numer/det;
   return(d);
}
