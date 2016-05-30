/*
# proc: xkthi - return the kth largest element of an integer signal
# proc:         with only partial sorting.
# proc: xkthf - return the kth largest element of an integer signal
# proc:         with only partial sorting.
*/

/*
*  C translations of web contained in /usr/local/nn/src/lib/nn/xkth.web
*  Find the |k|${}^{th}$-largest of |N| elements without sorting.
*  Algorithm is procedure |find| from p. 84 of Wirth, 
*  {\em Algorithms + Data Structures = Programs}. In average case,
*  takes about $O(N)$ work; worst case takes $O(N^2)$ work.
*  
*  Re-arranges data in |x| array. Ends up with |x(k)| the |k|${}^{th}$-
*  largest, |x(i) <= x(k)| for |i < k|, and |x(j) >= x(k)| for |j > k|.
*/



int xkthi(k, x, npts)
int *x; int k, npts;
{
int left, right, i, j;
int value, temp;

   for ( left = 0, right = npts-1 ; left < right ; )
   {
      value = x[k], i = left, j = right;
      do 
      {
         for ( ; x[i] < value ; i++ );
         for ( ; value < x[j] ; j-- );
         if ( i <= j )
         {
            temp = x[i];
            x[i] = x[j];
            x[j] = temp;
            i++;
            j--;
         }
      } while (i <= j);
      if (j < k) left = i;
      if (k < i) right = j;
   }
   return x[k];
}



float xkthf(k, x, npts)
float *x; int k, npts;
{
int left, right, i, j;
float value, temp;

   for ( left = 0, right = npts-1 ; left < right ; )
   {
      value = x[k], i = left, j = right;
      do 
      {
         for ( ; x[i] < value ; i++ );
         for ( ; value < x[j] ; j-- );
         if ( i <= j )
         {
            temp = x[i];
            x[i] = x[j];
            x[j] = temp;
            i++;
            j--;
         }
      } while (i <= j);
      if (j < k) left = i;
      if (k < i) right = j;
   }
   return x[k];
}
