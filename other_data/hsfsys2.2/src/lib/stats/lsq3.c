/*
# proc: applyparam3_to_point - takes an x and y coordinate pair and applies
# proc:                        the specified Least Squares parameters with
# proc:                        three degrees of freedom.
# proc: chkfindparam3 - finds the best fit between two corresponding sets of
# proc:                 points while checking for errors in the location of
# proc:                 the hypothesized points.
# proc: findparam3 - finds the best fit between two corresponding sets of
# proc:              points using Least Squares with 3 degrees of freedom.
# proc: determinant3 - computes the determinant for Least Squares with 3
# proc:                degrees of freedom.
# proc: scale1param3 - computes the first scale parameter for Least Squares
# proc:                with 3 degrees of freedom.
# proc: scale1param3 - computes the second scale parameter for Least Squares
# proc:                with 3 degrees of freedom.
# proc: deltaparam3 - computes the translation parameter for Least Squares
# proc:                with 3 degrees of freedom.
*/

#include <stdio.h>
#include <math.h>
#include <defs.h>

#define TOO_SMALL 0.0001

/***********************************************************************/
applyparam3_to_point(fx, fy, col, row, dx, mxx, mxy, dy, myy, myx)
float *fx, *fy;
int col, row;
float dx, mxx, mxy, dy, myy, myx;
{
   *fx = (mxx * col) + (mxy * row) + dx;
   *fy = (myy * row) + (myx * col) + dy;
}

/***********************************************************************/
chknfindparam3(npts, dx, mxx, mxy, dy, myy, myx,
               xref, yref, nref, xhyp, yhyp, nhyp, rm_limit, err_limit)
int *npts;
float *dx, *mxx, *mxy, *dy, *myy, *myx;
int *xref, *yref, nref;
int *xhyp, *yhyp, nhyp;
int rm_limit, err_limit;
{
   int *err;
   int i, max_i, max_v;
   float tx, ty, dix, diy, d2;

   if(nref != nhyp)
      fatalerr("chknfindparam3",
               "reference and hypothesis lists must be of equal length", NULL);
   if((err = (int *)calloc(nref, sizeof(int))) == NULL)
      syserr("chknfindparam3", "calloc", "err");
   *npts = nref;
   while(*npts > rm_limit){
      findparam3(dx, mxx, mxy, dy, myy, myx, xref, yref, *npts,
                 xhyp, yhyp, *npts);
      for(i = 0; i < *npts; i++){
         applyparam3_to_point(&tx, &ty, xhyp[i], yhyp[i],
                              *dx, *mxx, *mxy, *dy, *myy, *myx);
         dix = tx - xref[i];
         diy = ty - yref[i];
         d2 = (dix * dix) + (diy * diy);
         err[i] = sround(sqrt((double)d2));
      }
      find_first_max_forward(err, 0, *npts, &max_i, &max_v);
      if(max_v > err_limit){
         remove_from_list(max_i, xhyp, *npts);
         remove_from_list(max_i, yhyp, *npts);
         remove_from_list(max_i, xref, *npts);
         remove_from_list(max_i, yref, *npts);
         (*npts)--;
      }
      else
         break;
   }
   if(*npts <= rm_limit)
      fatalerr("chknfindparam3", "form registration failed",
               "too few registration points located");
   free(err);
}

/***********************************************************************/
/* Using a least squares fit, find the pair of x parameters and the    */
/* pair of y parameters estimating the amount of distortions           */
/* existing between the hypothesized points and the reference points   */
/***********************************************************************/
/*  dx ==> estimated translation in x.                                 */
/* mxx ==> x equation's estimated scale in x.                          */
/* mxy ==> x equation's estimated scale in y.                          */
/*  dy ==> estimated translation in y.                                 */
/* myy ==> y equation's estimated scale in y.                          */
/* myx ==> y equation's estimated scale in x.                          */
/***********************************************************************/
findparam3(dx, mxx, mxy, dy, myy, myx, hypx, hypy, hypnum, refx, refy,
                refnum)
float *dx, *mxx, *mxy, *dy, *myy, *myx;
int *hypx, *hypy, hypnum;
int *refx, *refy, refnum;
{
   float fn, sxh, sxr, syh, syr;
   float sxhxr, syhyr;
   float sxrxr, syryr;
   float sxhyr, syhxr;
   float sxryr;
   float detx, dety;
   float determinant3(), scale1param3(), scale2param3(), deltaparam3();

   if(hypnum != refnum)
      fatalerr("findparam3",
               "number of hyp points not equal to number of ref points",
               NULL);

/***********************************************************************/
/* Given the linear equation for x with 3 unknowns:                    */
/*                                                                     */
/* A = | fn  sxr   syr   |                                             */
/*     | sxr sxrxr sxryr |                                             */
/*     | syr sxryr syryr |                                             */
/*                                                                     */
/* B = | sxh   |                                                       */
/*     | sxhxr |                                                       */
/*     | sxhyr |                                                       */
/*                                                                     */
/* P = | dx  |                                                         */
/*     | mxx |                                                         */
/*     | mxy |                                                         */
/*                                                                     */
/* Given the linear equation for y with 3 unknowns:                    */
/*                                                                     */
/* A = | fn  syr   sxr   |                                             */
/*     | syr syryr sxryr |                                             */
/*     | sxr sxryr sxrxr |                                             */
/*                                                                     */
/* B = | syh   |                                                       */
/*     | syhyr |                                                       */
/*     | syhxr |                                                       */
/*                                                                     */
/* P = | dy  |                                                         */
/*     | myy |                                                         */
/*     | myx |                                                         */
/*                                                                     */
/* Variable nomenclature:                                              */
/*    fn ==> number of points as a float                               */
/*     s ==> summation across all points                               */
/*     x ==> x-coordinate                                              */
/*     y ==> y-coordinate                                              */
/*     r ==> reference point                                           */
/*     h ==> hypothesis point                                          */
/*  xrxr ==> x-coordinate of reference point squared                   */
/*                                                                     */
/***********************************************************************/
   /* x & y equations' A */
   fn = (float)hypnum;
   sxr = (float)sumints(refx, refnum);
   syr = (float)sumints(refy, refnum);
   sxrxr = (float)sumprodints(refx, refx, hypnum);
   syryr = (float)sumprodints(refy, refy, hypnum);
   sxryr = (float)sumprodints(refx, refy, hypnum);

   /* x equation's B */
   sxh = (float)sumints(hypx, hypnum);
   sxhxr = (float)sumprodints(hypx, refx, hypnum);
   sxhyr = (float)sumprodints(hypx, refy, hypnum);

   /* y equation's B */
   syh = (float)sumints(hypy, hypnum);
   syhyr = (float)sumprodints(hypy, refy, hypnum);
   syhxr = (float)sumprodints(hypy, refx, hypnum);

                           /* <--------------- A -------------> */
   detx = (float)determinant3(fn, sxr, syr, sxrxr, syryr, sxryr);
   dety = (float)determinant3(fn, syr, sxr, syryr, sxrxr, sxryr);

                          /* <--------------- A -------------> */
   *dx =  (float)deltaparam3(fn, sxr, syr, sxrxr, syryr, sxryr,
                          /* <------ B ------> */
                             sxh, sxhxr, sxhyr, detx);
   *mxx = (float)scale1param3(fn, sxr, syr, sxrxr, syryr, sxryr,
                             sxh, sxhxr, sxhyr, detx);
   *mxy = (float)scale2param3(fn, sxr, syr, sxrxr, syryr, sxryr,
                             sxh, sxhxr, sxhyr, detx);

   *dy =  (float)deltaparam3(fn, syr, sxr, syryr, sxrxr, sxryr,
                             syh, syhyr, syhxr, dety);
   *myy = (float)scale1param3(fn, syr, sxr, syryr, sxrxr, sxryr,
                             syh, syhyr, syhxr, dety);
   *myx = (float)scale2param3(fn, syr, sxr, syryr, sxrxr, sxryr,
                             syh, syhyr, syhxr, dety);
}

/***********************************************************************/
float determinant3(fn, s1r, s2r, s1r1r, s2r2r, s1r2r)
float fn, s1r, s2r, s1r1r, s2r2r, s1r2r;
{
   float det;

   det = (fn*s1r1r*s2r2r) + (s1r*s1r2r*s2r) +  (s2r*s1r*s1r2r) -
         (s2r*s1r1r*s2r)  - (s1r2r*s1r2r*fn) - (s2r2r*s1r*s1r);
   if(fabs(det) < TOO_SMALL)
      fatalerr("determinant3",
               "determinant too small to be significant", NULL);
   return(det);
}

/***********************************************************************/
float scale1param3(fn, s1r, s2r, s1r1r, s2r2r, s1r2r, s1h, s1h1r, s1h2r, det)
float fn, s1r, s2r, s1r1r, s2r2r, s1r2r, s1h, s1h1r, s1h2r, det;
{
   float numer, m;

   numer = (s1h*((s1r2r*s2r) - (s1r*s2r2r))) +
           (s1h1r*((fn*s2r2r) - (s2r*s2r))) +
           (s1h2r*((s2r*s1r) - (fn*s1r2r)));
   m = numer/det;
   return(m);
}

/***********************************************************************/
float scale2param3(fn, s1r, s2r, s1r1r, s2r2r, s1r2r, s1h, s1h1r, s1h2r, det)
float fn, s1r, s2r, s1r1r, s2r2r, s1r2r, s1h, s1h1r, s1h2r, det;
{
   float numer, m;

   numer = (s1h*((s1r*s1r2r) - (s1r1r*s2r))) +
           (s1h1r*((s1r*s2r) - (fn*s1r2r))) +
           (s1h2r*((fn*s1r1r) - (s1r*s1r)));
   m = numer/det;
   return(m);
}

/***********************************************************************/
float deltaparam3(fn, s1r, s2r, s1r1r, s2r2r, s1r2r, s1h, s1h1r, s1h2r, det)
float fn, s1r, s2r, s1r1r, s2r2r, s1r2r, s1h, s1h1r, s1h2r, det;
{
   float numer, d;

   numer = (s1h*((s1r1r*s2r2r) - (s1r2r*s1r2r))) +
           (s1h1r*((s2r*s1r2r) - (s1r*s2r2r))) +
           (s1h2r*((s1r*s1r2r) - (s2r*s1r1r)));
   d = numer/det;
   return(d);
}
