/*
# proc: calc_skew - takes a 1 pixel per byte image and returns an angle
# proc:             of skew in degrees.
# proc: sumlineexps - calculates a sum exponentials of slanted row sums. 
# proc:                  used to determine angle of skew in the image.
*/

#include <stdio.h>
#include <values.h>
#include <math.h>
#include <defs.h>
#include <deskew.h>

static unsigned char *inp;
static int iw, ih;

/***************************************************************************/
double calc_skew(pinp, piw, pih, linefunc)
unsigned char *pinp; int piw, pih; double (*linefunc)();
{
double theta, resonance, ax, bx, cx, tol, brent();

   inp = pinp; iw = piw; ih = pih;
   ax  = -HIGH_ANGLE/SCALE_ANGLE;
   bx  = 0.0;
   cx  =  HIGH_ANGLE/SCALE_ANGLE;
   tol = INCR_ANGLE / SCALE_ANGLE;
   resonance = brent(ax, bx, cx, linefunc, tol, &theta);

   linefunc(MAXFLOAT); /* free the internal space */
   return(theta);
}

/***************************************************************************/
double sumlineexps(theta)
double theta;
{
unsigned char *base;
static int nbres = 0, *xlist, *ylist;
int ystart, yfinal, ylef, yrit;
int j, n, y, num, *address, bx, by, bw, bh;
double rsp, yimax, tantheta;
static double pcave;

   if (!nbres)                          /* first time through but only  */
   {                                    /*   !!--for this image--!!     */
      find_corners8(&bx, &by, &bw, &bh, inp, iw, ih);
      pcave = (double)pixelcnt8(inp, iw*ih) / (double)bh;
   }

   if (theta > MAXFLOAT / 1000.0)       /* a huge theta value indicates */
   {                                    /* the last call for this image */
      nbres = 0;                        /* time to prepare for next one */
      free(xlist); free(ylist);
      return MAXFLOAT;
   }

   tantheta = tan(PIBY180 * (double)theta);      /* rad  */
   yimax  = (double)iw * tantheta;
   ystart = (int)max(0, -yimax);
   yfinal = (int)min(ih-1, ih-1-yimax);

   ylef = ystart;
   yrit = ystart + (int)yimax;
   bres_line_alloc(0,ylef, iw-1,yrit, &xlist, &ylist, &num, &nbres);

   /* overwrite ylist with addresses in the raster, calling it address  */
   for ( j = 0, address = ylist ; j < num ; j++ )
      address[j] = ylist[j]*iw + xlist[j];

   for ( rsp = 0.0, base = inp, y = ystart ; y <= yfinal ; y++ )
   {
      for ( j = n = 0 ; j < num ; j++ )
         if (*(base + address[j]))
            n++;

      rsp += exp((double)n / pcave);
      base += iw;
   }
   return(-rsp);        /* minimize function rather than maximizing it   */
}
