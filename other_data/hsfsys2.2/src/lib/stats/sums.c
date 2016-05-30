/*
# proc: sumints - computes the summation of the given integer list, sum(x).
# proc:
# proc: sumprodints - computes the summation of two integer lists pointwise
# proc:               multiplied, sum(x*y).
# proc: SSx - calculates SS(x) = (sum_x2 - ((sum_x * sum_x)/count))
# proc:
# proc: SSxy - calculates   SS(xy) = (sum_xy - ((sum_x*sum_y)/count))
# proc:
*/

/*****************************************************/
sumints(l, n)
int *l, n;
{
   int i, s = 0;

   for(i = 0; i < n; i++)
      s += l[i];
   return(s);
}

/*****************************************************/
sumprodints(l1, l2, n)
int *l1, *l2, n;
{
   int i, s = 0;

   for(i = 0; i < n; i++)
      s += (l1[i] * l2[i]);
   return(s);
}

/*****************************************************/
/* Routine: SSx                                      */
/* Author:  Darrin L. Dimmick                        */
/* Date:    4/25/90                                  */
/*****************************************************/
/*****************************************************/
/* SSx() accepts the sum of the values, sum_x, the   */
/* sum of the squares of the values, sum_x2 and the  */
/* number of values contained in the sums, count and */
/* returns the value of the sum of the squares       */
/* calculation, SS(x).                               */
/*****************************************************/

double SSx(sum_x,sum_x2,count)
double sum_x;  /* sum of x  values */
double sum_x2; /* sum of the squares of the x values */
int count;     /* number of values sumed */
{ /* SS(x) SS(y) */
  /* SS(x) = (sum_x2 - ((sum_x * sum_x)/count)) */
  double ssx;
  ssx = sum_x * sum_x;
  ssx = ssx/count;
  ssx = sum_x2 - ssx;
  return (ssx);
}

/*****************************************************/
/* Routine: SSxy                                      */
/* Author:  Darrin L. Dimmick                        */
/* Date:    4/25/90                                  */
/*****************************************************/
/*****************************************************/
/* SSxy() accepts the sum of the x values, sum_x,    */
/* the sum of the y values, sum_y, the sum of (x*y), */
/* sum_xy and the number of values contained in the  */
/* sums, count and returns the value of the sum of   */
/* the squares calculation,SS(xy)                    */
/*****************************************************/

double SSxy(sum_x,sum_y,sum_xy,count) /* return sum of squares x*y */
double sum_x;  /* sum of x  values */
double sum_y;  /* sum of y  values */
double sum_xy; /* sum of x*y  values */
int count;     /* number of values sumed */
{       /* SS(xy) */
  return (sum_xy - ((sum_x*sum_y)/count));
}
