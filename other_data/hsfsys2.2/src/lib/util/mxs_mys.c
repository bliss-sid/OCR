/*
# proc: compute_mxs_mys - computes the geometric centers of boxes given
# proc:                   a list of box x, y, w, and h.
*/

compute_mxs_mys(xs, ys, ws, hs, mxs, mys, end)
int *xs, *ys, *ws, *hs;
int *mxs, *mys;
int end;
{
   int i;

   for(i = 0; i < end; i++){
      mxs[i] = xs[i] + (ws[i]>>1);
      mys[i] = ys[i] + (hs[i]>>1);
   }
}
