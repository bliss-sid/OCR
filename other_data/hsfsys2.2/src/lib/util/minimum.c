/*
# proc: minv - dap style function to return minimum value of a vector of
# proc:        floats.
# proc: minp - returns the position of the first minimum value in a vector
# proc:        of floats.
# proc: minl - returns a vector of characters equal to 1 in those locations
# proc:        where the given vector of floats is minimum.
# proc: minpv - returns the first position and value of minimum in a vector
# proc:         of floats.
# proc: minv_feats - returns the column vector of minimums extracted along
# proc:              the rows of a 2-D column major array.
*/

#include <stdio.h>
#include <malloc.h>
#include <values.h>


/* dap style function to return minimum value of a vector of floats */
float minv(avec, n)
float *avec;  
int   n;  
{
float minval, *ptr;
int   j;
 
      ptr    = avec;
      minval = *ptr++;
      for (j = 1; j < n; j++)
      {  
         if (*ptr < minval)
            minval = *ptr;
         ptr++;
      }  
      return(minval);
}
 
 
/* returns integer position of minimum value in a vector of floats */
/* in event of min being at two or more positions the first is returned */
int   minp(avec, n)
float *avec;
int   n;
{
float minval, *ptr;
int   minpos,  j;
 
      ptr    = avec;
      minval = *ptr++;
      minpos = 0;
      for (j = 1; j < n; j++)
      {
         if (*ptr < minval)
         {
            minval = *ptr;
            minpos = j;
         }   
         ptr++;
      }
      return(minpos);
}

/* returns a char vector equal to 1 in those locations where avec is minimum */
char *minl(avec, n)
float *avec;
int   n;
{
float minval, *ptr;
int   j;
char  *minloc;
 
      if ((minloc = (char *)calloc(n, sizeof(char))) == NULL)
        syserr("minl", "space for char mask", "calloc");
 
      minval = minv(avec, n);
      ptr    = avec;
      for ( j = 0 ; j < n ; j++)
        if (*ptr++ == minval)
           minloc[j] = 1;
             
      return(minloc);
}


/* returns first position and value of minimum of a vector of floats */
minpv(avec, n, minv, minp)
float *avec, *minv;
int   n,     *minp;
{
float *ptr, minval;
int   j,    minpos;

      ptr    = avec;
      minval = *ptr++;
      minpos = 0;
      for (j = 1; j < n; j++)
      {
         if (*ptr < minval)
         {
            minval = *ptr;
            minpos = j;
         }   
         ptr++;
      }
      *minv = minval;
      *minp = minpos;
}


/* finds the nInps mimima of the rows over nPats columns of an array */
minv_feats(feats, nPats, nInps, maxv)
float *feats, **maxv;
int   nPats, nInps;
{
int j, k;
float *fptr;

    if (((*maxv) = (float *)malloc(nInps * sizeof(float))) == NULL)
      syserr("maxv_feats", "space for min values vector", "malloc");

    for (k = 0 ; k < nInps ; k++)
       (*maxv)[k] =  MAXFLOAT;

    fptr = feats;
    for (j = 0 ; j < nPats ; j++)
      for (k = 0 ; k < nInps ; k++)
      {  
         if (*fptr < (*maxv)[k])
           (*maxv)[k] = *fptr;
         fptr++;
      }  
}
