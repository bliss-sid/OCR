/*
# proc: nn_maxpf - locates the maximum value in a list of floats
# proc:
*/

int nn_maxpf(x, n)
float *x; int n;
{
float m; int i, p;

   for ( i = n, m = x[--i], p = i ; i-- ; )
      if ( x[i] > m )
         m = x[i], p = i;
   return p;
}
