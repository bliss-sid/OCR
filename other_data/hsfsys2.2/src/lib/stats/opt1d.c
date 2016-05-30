/*
# proc: brent - performs a 1D parabolic minimization of an arbitrary function,
# proc:         and returns the value at the minimum.
*/

#include <math.h>
#include <defs.h>

/* defines for brent */
#define ITMAX 100
#define CGOLD 0.3819660
#define ZEPS 1.0e-10

#define SIGN(x, y) (((y) < 0) ? -fabs((x)) : fabs((x)))

double brent(ax, bx, cx, f, tol, xmin)
double ax, bx, cx, tol, *xmin, (*f)();
{
double fu, fx, fv, fw;
double a, b, d, e, u, v, w, x;
double xm, tol1, tol2, r, p, q, etemp;
int iter;

   a = min(ax, cx);
   b = max(ax, cx);

   e = 0.0;
    v =  w =  x = bx;
   fv = fw = fx = (*f)(x);

   for ( iter = 0 ; iter < ITMAX ; iter++ )
   {
      xm = 0.5 * (a + b);
#ifndef RIGID_BRENT_ADHERENCE
      tol1 = tol;
#else
      tol1 = tol * fabs(x) + ZEPS;
#endif
      tol2 = 2.0 * tol1;

      if (fabs(x-xm) <= tol2 - 0.5*(b-a))
         break;		/* time to go, we are done */

      if (fabs(e) > tol1)
      {
         r = (x-w) * (fx-fv);
         q = (x-v) * (fx-fw);
         p = (x-v)*q - (x-w)*r;
         q = 2.0 * (q-r);
         if (q > 0.0)
            p = -p;
         q = fabs(q);
         etemp = e;
         e = d;

         if ((fabs(p) >= fabs(0.5*q*etemp)) ||
             (p <= q*(a-x)) || (p >= q*(b-x)))
                 goto ONE;
         
         d = p / q;
         u = x + d; 
         if ((u-a < tol2) || (b-u < tol2))
            d = SIGN(tol1, xm-x);
         goto TWO;
        
      }
ONE:  if (x >= xm)
         e = a - x;
      else
         e = b - x;

      d = CGOLD * e;

TWO:  if (fabs(d) >= tol1)
         u = x + d;
      else
         u = x + SIGN(tol1, d);

      fu = (*f)(u);

      if (fu <= fx)
      {
         if (u >= x)
            a = x;
         else
            b = x;

         v = w; fv = fw;
         w = x; fw = fx;
         x = u; fx = fu;
      }
      else
      {
         if (u < x)
            a = u;
         else
            b = u;

         if (fu <= fw || w == x)
         {
             v = w; fv = fw;
             w = u; fw = fu;
         }
         else
         if (fu <= fv || v == x || v == w)
         {
             v = u; fv = fu;
         }
      }
   }
   if (iter == ITMAX)
      fatalerr("brent", "exceeded max iterations", "no return");

   *xmin = x;
   return fx;
}
