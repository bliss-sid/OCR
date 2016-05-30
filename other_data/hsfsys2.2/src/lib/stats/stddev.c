/*
# proc: standard_deviation - calculates std dev from the accumulated x and
# proc:                      x*x along with the number of datapoints.
# proc: variance - calculates the variance from the accumulated x and x*x
# proc:            along with the number of datapoints.
# proc: zscore - calculates the zscore for a datapoint given the mean and
# proc:          std dev.
# proc: compute_stddev - computes the standard deviation from a list of floating
# proc:                  point numbers.
# proc: stddev_from_ints - computes the standard deviation from a list of
# proc:                    integers.
*/

#include <stdio.h>
#include <math.h>

/*****************************************************/
/* Routine: Standard_deviation                       */
/* Author:  Darrin L. Dimmick                        */
/* Date:    4/25/90                                  */
/*****************************************************/
/*****************************************************/
/* Standard_deviation() accepts the sum of the       */
/* values, the sum of the squares of the values, and */
/* the number of values contained in the sum and     */
/* returns the standard deviation of the data.       */
/*****************************************************/

double standard_deviation(sum_x,sum_x2,count) /* return st. deviation */
double sum_x;          /* sum of the x values */
double sum_x2;         /* sum of the squares of the x values */
int    count;          /* number of items sumed */
{
  double var = 0;
  double variance(); /* return variance of data */
  var = variance(sum_x,sum_x2,count);
  if (var >= 0)
     return (sqrt(var));
  else
     return(var);
}

/*****************************************************/
/* Routine: Variance                                 */
/* Author:  Darrin L. Dimmick                        */
/* Date:    4/25/90                                  */
/*****************************************************/
/*****************************************************/
/* Variance() accepts the sum of the values, the sum */
/* of the squares of the values, and the number of   */
/* values contained in the sum and returns the       */
/* variance of the data.                             */
/*****************************************************/

double variance(sum_x,sum_x2,count)  /* return variance */
double sum_x;   /* sum of the x values */
double sum_x2;  /* sum of the squares of the values x */
int    count;   /* number of items that were sumed */
{
  double SSx(); /* return the sum of squares for x */
  double ssx;   /* holds value from SSx() */
  double variance; 
  if (count < 2)
     {  
       fprintf(stderr,"Invalid count passed to variance %d\n",count);
       return(-1);
     }
  ssx = SSx(sum_x,sum_x2,count);
  variance = ssx/(count-1);
  return(variance);
}

/*****************************************************/
/* Routine: zscore                                   */
/* Author:  Darrin L. Dimmick                        */
/* Date:    1/23/92                                  */
/*****************************************************/
/*****************************************************/
/* Zscore() accepts the datum, the mean of the       */
/* values, and the standard deviation values and     */
/* returns the zscore of the dataum.                             */
/*****************************************************/

double zscore(datum, mean, stddev)
double datum, mean, stddev;
{
  return((datum - mean)/stddev);
}

/***************************************************************************/
/* compute_stddev - computes the standard deviation from a given list of   */
/* floating point numbers.                                                 */
/***************************************************************************/
float compute_stddev(flist, num)
float *flist;
int num;
{
   int i;
   float stddev;
   float sum_x, sum_x2;
   double standard_deviation();

   sum_x = 0.0;
   sum_x2 = 0.0;
   for(i = 0; i < num; i++){
      sum_x += flist[i];
      sum_x2 += (flist[i]*flist[i]);
   }
   stddev = (float)standard_deviation(sum_x, sum_x2, num);
   return(stddev);
}

/************************************************************/
float stddev_from_ints(l, n)
int *l, n;
{
   int i;
   float stddev;
   int sum_x, sum_x2;
   float fsum_x, fsum_x2;
   double standard_deviation();

   sum_x = 0.0;
   sum_x2 = 0.0;
   for(i = 0; i < n; i++){
      sum_x += l[i];
      sum_x2 += (l[i] * l[i]);
   }
   fsum_x = (float)sum_x;
   fsum_x2 = (float)sum_x2;
   stddev = (float)standard_deviation(fsum_x, fsum_x2, n);

   return(stddev);
}
