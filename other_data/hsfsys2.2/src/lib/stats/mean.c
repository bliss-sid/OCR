/*
# proc: compute_mean - takes a list of integers and computes the mean.
# proc:
*/

float compute_mean(ls, end)
int *ls, end;
{
   int i;
   float sum = 0.0;

   for(i = 0; i < end; i++)
      sum += (float)ls[i];
   sum = sum/(float)end;
   return(sum);
}
