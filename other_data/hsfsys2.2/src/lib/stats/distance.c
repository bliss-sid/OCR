/*
# proc: distance - computes the distance between two points.
# proc:
# proc: diff_sq_distance - computes the squared distance between two points.
# proc:
# proc: perp_dist - given a point in space and the slope and a point on a line,
# proc:             computes the perpendicular distance from the first point to
# proc:             the line.
*/

#include <math.h>

/************************************************************/
/*         Routine:   distance()                            */
/*                                                          */
/* Distance(), given two coordinate points, returns the     */
/* Euclidean distance between the two points.               */
/************************************************************/
/* x0, y0 - first coordinate point.                         */
/* x1, y1 - second coordinate point.                        */
/************************************************************/
float distance(x0, y0, x1, y1)
float x0, y0, x1, y1;
{
   float d, d0, d1;

   d0 = (x1 - x0);
   d1 = (y1 - y0);
   d = sqrt((double)((d0 * d0) + (d1 * d1)));
   return(d);
}

/*******************************************************************/
/* Returns sum of squares distance */
/* Darrin L. Dimmick 5-21-93 */

int diff_sq_distance(px, py, ex, ey)
int px, py, ex, ey;
{
  int xdiff, ydiff, dist;

  xdiff = px - ex;
  xdiff = xdiff * xdiff;
  ydiff = py - ey;
  ydiff = ydiff * ydiff;
  dist = xdiff + ydiff;
  return(dist);
}

/*******************************************************************/
/* (x2, y2) ==> point in space */
/* (x1, y1) ==> point on line  */
/* m ==> slope of line         */
float perp_dist(x2, y2, x1, y1, m)
float x2, y2, x1, y1, m;
{
   float dist;

/*
       d = (y2-y1) - m(x2-x1)
           ------------------
             sqrt(1 + m^2)

   Geometric Interpretation:

     numerator: determines the vertical distance from (x2,y2) to the point
                that intersects the line (forms a hypotenus).

        (y2-y1) ==> vertical distance between the two points
        m(x2-x1) ==> vertical adjustment to coincide with the line

     demoninator: factor of converting hypotenus units to units of
                  orthogonal distance

        sqrt(1 + m^2) ==> lenth of hypotenus when dx = 1

   Algebraic Solution:

      let (xn, yn) be the orthogonal point on the line

         d^2 = sqrt((xn - x2)^2 + (yn - y2)^2)

      equation of given line:
      
         y = mx - mx1 + y1

      equation of perpendicular line:

         y = -(x/m) + (x1/m) + y2

      The formula is derived by setting the the two linear equations equal to
      each other and solving for x, and then substituting the value of x
      into the first linear equation and solving for y. The new values for
      x and y are then subsituted for xn and yn in the distance equation
      and the terms are expanded and recombined, ultimately producing the
      formula.

*/

   dist = fabs(((y2-y1) - (m * (x2-x1)))/sqrt(1.0 + (m * m)));
   return(dist);
}
