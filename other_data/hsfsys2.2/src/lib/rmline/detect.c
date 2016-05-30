/*
# proc: hough_lines_hori - computes the hough transform(s) for detecting lines
# proc:                    with angles in the range [-range .. +range].
# proc: hough_lines - computes the hough transform for detecting lines within the
# proc:               specified range of "normal" angles.
# proc: gen_slope_point - computes a line's slope and normal vector intersection
# proc:                   point given a location within a hough transform matrix.
*/

#include <stdio.h>
#include <math.h>
#include <values.h>
#include <defs.h>
#include <mytrace.h>


/***************************************************************************/
/* hough_lines_hori - conducts two separate hough line transforms, one for */
/* lines with angles in the range [-range .. 0], and the other for lines   */
/* with angles in the range [0 .. range].                                  */
/***************************************************************************/
hough_lines_hori(cdata, w, h, delta, range, pmin, pmax, pacc, pax, pay,
                 nmin, nmax, nacc, nax, nay)
unsigned char *cdata;
int w, h;
float delta, range;
float *pmin, *pmax, *nmin, *nmax;
int **pacc, **nacc;
int *pax, *pay, *nax, *nay;
{
   /* two separate hough transforms are required because the lines with angles */
   /* in the range [-range .. +range] are divided into two separate groups     */
   /* ( [(90 - range) .. 90] and [-90 .. (-90 + range)] ) when represented by  */
   /* their normal vectors, which are used by the hough transform to avoid     */
   /* discontinuities of lines with perfectly vertical slope.                  */

   /* the normal vectors for lines with angle [-range .. 0] */
   /* are on the range [(90 - range) .. 90] */
   *pmax = M_PI_2;
   *pmin = *pmax - range;

   /* conduct hough transform for lines [-range .. 0] */
   hough_lines(cdata, w, h, *pmin, *pmax, delta, pacc, pax, pay);

   /* the normal vectors for lines with angle [0 .. range] */
   /* are on the range [-90 .. (-90 + range)] */
   *nmin = -M_PI_2;
   *nmax = *nmin + range;

   /* conduct hough transform for lines [0 .. range] */
   hough_lines(cdata, w, h, *nmin, *nmax, delta, nacc, nax, nay);
}

/***************************************************************************/
/* hough_lines - conducts a hough line transformation on the given image   */
/* across a specified range of "normal" angles. The routine returns a      */
/* matrix whose locations of maximal value represent the dominant lines in */
/* the image. The locations in the matrix are (rho, theta) pairs that      */
/* represent unique lines in the image according to each line's normal     */
/* vector representation.                                                  */
/***************************************************************************/
hough_lines(data, w, h, tmin, tmax, td, acc, ax, ay)
unsigned char *data;
int w, h;
float tmin, tmax, td;
int **acc, *ax, *ay;
{
   int x, y, *aptr, hax;
   unsigned char *dptr;
   float theta;
   int rho;

   /* calculate allocation dimensions */

   /* the hough line transform is conducted using the normal vector   */
   /* representation of the line. this avoids discontinuities in      */
   /* perfectly vertical slopes. each possible line in the image is   */
   /* represented in the hough matrix by 1.) the angle of the normal  */
   /* vector extending from the origin to the point on the line       */
   /* where the intersection forms a right angle and 2.) the radial   */
   /* distance from the origin to the intersection point on the line. */
   /* the resulting transform matrix will be dimensioned with y       */
   /* coordinates representing the angle of the normal vector and x   */
   /* coordinates representing the radial distance to the "normal"    */
   /* point on the line.                                              */

   /* y dimension is based on the specified range of angles to be searched */
   /* over divided by the angle increment. */
   *ay = (int)ceil((tmax - tmin)/td);

   /* x dimension is based on the diagonal distance across the image */
   /* time 2 because the radial distance can be measured both positive */
   /* and negative. */
   hax = (int)ceil(sqrt((float)(w*w + h*h)));
   *ax = 2 * hax;

   /* allocate the 2d hough transform matrix */
   calloc_int(acc, (*ax)*(*ay), "hough_lines : acc");

   /* for every point in the image ... */
   dptr = data;
   for(y = 0; y < h; y++){
      for(x = 0; x < w; x++){
         /* if the current pixel is black ... */
         if(*dptr++){
            /* for all the possible angles of lines intersecting this pixel ... */
            for(theta = tmin, aptr = (*acc); theta <= tmax; theta += td, aptr+=*ax){

               /* compute the radial distance to the point on the line */
               /* where its "normal" vector intersects the line */
               rho = sround(x * cos(theta)) + (y * sin(theta));

               /* increment the location corresponding to the current  */
               /* (rho, theta) pair. each (rho, theta) pair represents */
               /* a unique line in the image. once every pixel in the  */
               /* image is processed, the locations in the matrix with */
               /* the highest count will represent the most dominant   */
               /* lines in the image.                                  */
               (*(aptr+(rho+hax)))++;
            }
         }
      }
   }
}

/***************************************************************************/
/* gen_slope_point - determines the line's slope and the normal vector's   */
/* intersection point represented by the specified location within a hough */
/* transform matrix. The routine requires a number of the hough transform  */
/* set-up parameters to make the calculations.                             */
/***************************************************************************/
gen_slope_point(slope, nx, ny, mx, my, tmin, tmax, td, xoffset)
float *slope, tmin, tmax, td;
int *nx, *ny, xoffset;
int mx, my;
{
   float mt, nt, theta_npi2pi();
   int mr;

   /* the x dimension is divided into positive and negative rho's */
   /* so an offset is needed to translate the x address into its  */
   /* real rho value. */
   mr = mx - xoffset;

   /* the y dimension represents a range of angles [tmin .. tmax] */
   /* sampled according to the increment td, so to compute the    */
   /* real angle represented by my, multiply it by the sample     */
   /* increment and then translate it by tmin. this give the      */
   /* angle of the "normal" vector to the line.                   */
   nt = (my * td) + tmin;

   /* convert the normal vector angle to the actual angle of the  */
   /* line by reflecting the normal angle forward 90 degrees and  */
   /* then make sure the resulting angle is in quadrants 1 or 4.  */
   mt = nt + M_PI_2;
   mt = theta_npi2pi(mt);

   /* calculate the line's slope from its angle */
   *slope = tan(mt);

   /* calculate the coordinates of the point where the line's */
   /* normal vector intersects the line. */
   *nx = sround(mr * cos(nt));
   *ny = sround(mr * sin(nt));

   my_trace4("line's: theta = %f, slope = %f, point = (%d, %d)\n",
             mt * (180.0/M_PI), *slope, *nx, *ny);
}
