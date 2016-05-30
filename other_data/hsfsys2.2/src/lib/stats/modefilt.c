/*
# proc: modefilteri - modal filter (frequency-based) for an integer signal
# proc:
# proc: modesubfilteri - modal filter (frequency-based) for a specified
# proc:                  segment of an integer signal
*/

#include <stdio.h>

/***************************************************************************/
int *modefilteri(x, n, r)
int *x; int n, r;
{
int *s;
int i, h, m;
int max_i, max_v, *hist, nbins;
int *lptr, *rptr;

   if (r % 2 != 1)
      fatalerr("modefilteri", "window size", "not odd");

   /* size of window should not exceed length of list */
   if (r > n){
      fprintf(stderr, "Warning: modefilteri\n");
      fprintf(stderr, "         specified window exceeds data length,\n");
      fprintf(stderr, "         truncating window size from %d to %d,\n", r, n);
      fprintf(stderr, "         process proceeding.\n");
      r = n;
   }

   malloc_int(&s, n, "modefilteri : s");

   /* initialize histogram in first window */
   find_first_max_forward(x, 0, n, &max_i, &max_v);
   nbins = max_v + 1;
   int_histogram(&hist, nbins, x, r);
   find_first_max_forward(hist, 0, nbins, &max_i, &max_v);

   /* if length of list equals the size of a single window ... */
   if (r == n){
      for ( i = 0 ; i < n ; i++ )
         s[i] = max_i;
      free(hist);
      return s;
   }

   h = (r-1) >> 1;	/* the number of elements either side	*/

   for ( i = 0 ; i < h ; i++ )
      s[i] = max_i;

   lptr = x;
   rptr = x+r-1;
   for (m = n-h ; i < m ; i++ )
   {
      (hist[*lptr])--;
      (hist[*rptr])++;
      if(hist[*rptr] > hist[max_i])
         max_i = *rptr;
      s[i] = max_i;
      lptr++;
      rptr++;
   }

   for (         ; i < n ; i++ )
      s[i] = max_i;

   free(hist);
   return s;
}

/***************************************************************************/
int *modesubfilteri(si, ei, x, n, r)
int si, ei;
int *x, n, r;
{
   int in;
   int *s;
   int i, h, m;
   int max_i, max_v, *hist, nbins;
   int *lptr, *sptr, *rptr;


   in = ei - si + 1;

   if(in > n)
      fatalerr("modesubfilteri", "sublist exceeds list length", NULL);

   if (r % 2 != 1)
      fatalerr("modesubfilteri", "window size", "not odd");

   /* size of window should not exceed length of list */
   if (r > in){
      fprintf(stderr, "Warning: modesubfilteri\n");
      fprintf(stderr, "         specified window exceeds data length,\n");
      fprintf(stderr, "         truncating window size from %d to %d,\n", r, in);
      fprintf(stderr, "         process proceeding.\n");
      r = in;
   }

   calloc_int(&s, n, "modesubfilteri: s");

   /* initialize histogram in first window */
   lptr = x+si;
   sptr = s+si;
   rptr = x+si+r-1;
   find_first_max_forward(lptr, 0, in, &max_i, &max_v);
   nbins = max_v + 1;
   int_histogram(&hist, nbins, lptr, r);
   find_first_max_forward(hist, 0, nbins, &max_i, &max_v);

   /* if length of list equals the size of a single window ... */
   if (r == in){
      for ( i = 0 ; i < in ; i++ ){
         (*sptr) = max_i;
         sptr++;
      }
      free(hist);
      return s;
   }

   h = (r-1) >> 1;	/* the number of elements either side	*/

   for ( i = 0 ; i < h ; i++ ){
      (*sptr) = max_i;
      sptr++;
   }

   for (m = in-h ; i < m ; i++ )
   {
      (hist[*lptr])--;
      (hist[*rptr])++;
      if(hist[*rptr] > hist[max_i])
         max_i = *rptr;
      (*sptr) = max_i;
      lptr++;
      rptr++;
      sptr++;
   }

   for (         ; i < in ; i++ ){
      (*sptr) = max_i;
      sptr++;
   }

   free(hist);
   return s;
}
