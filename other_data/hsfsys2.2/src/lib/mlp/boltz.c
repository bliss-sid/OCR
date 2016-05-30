/* [Unlike previous C-language version, this version goes through the
weights in an order equivalent to the order the Fortran version went
through them in.] */

/*
# proc: boltz - for each weight in turn, makes a threshold based on the weight,
# proc:         generates a uniform pseudorandom number, then prunes (zeros) the
# proc:         weight if random number <= threshold.
# proc: (boltz_work - called by boltz.)
The type of threshold used is controlled by boltzmann (see
comment below); in any case, (absolutely) small weights are more
likely to be pruned.  Writes a status message containing various
pruning statistics, to stderr and also (if desired) to a specified
FILE pointer.

[Replaces old tboltz and boltz.]

[Find out whether xmean is really never set in original fortran
version, and if so, decide whether to get rid of theta (which uses
xmean) or to define xmean somehow.]

Input parms:
  ninps, nhids, nouts: Numbers of input, hidden, and output nodes.
  boltzmann: Decides what kind of threshold to use.  Must be
    one of these two values (defined in parms.h):
      ABS_PRUNE: use threshold exp(-|wt|/temperature),
        where wt is a weight being considered for pruning.
      SQUARE_PRUNE: use threshold exp(-wt^t/temperature).
    (NO_PRUNE is a defined boltzmann value, but is not a legal
    value to use when calling this routine.)
  temperature: The higher this is, the more intense will be the
    pruning.  (If zero, routine returns without doing anything.)

Input/output parm:
  w: The network weights, in this (new) order:
    1st-layer weights (nhids by ninps "matrix", row-major);
    1st-layer biases (nhids elts);
    2nd-layer weights (nouts by nhids "matrix", row-major);
    2nd-layer biases (nouts elts).
*/

#include <stdio.h>
#include <math.h>
#include <mlp/parms.h>

void
boltz(ninps, nhids, nouts, boltzmann, temperature, w)
int ninps, nhids, nouts;
char boltzmann;
float temperature, *w;
{
#define RLOG2 1.442695 /* 1/log(2) */
  char str[200];
  int n_pru_1, n_pru_2, n_orig_1, n_orig_2, n_pru, n_unpru, h, i, j;
  float wl2sum, sum, sum2, wmax, wmin, range, c, entropy, wmean,
    theta, r, xmean, sumlogabs, *w1, *b1, *w2, *b2;
  void boltz_work();

  if(!(boltzmann == ABS_PRUNE || boltzmann ==
    SQUARE_PRUNE)) {
    sprintf(str, "boltzmann must be either ABS_PRUNE (%d) \
or SQUARE_PRUNE (%d); it is %d .", (int)ABS_PRUNE, (int)SQUARE_PRUNE,
      (int)boltzmann);
    fatalerr("boltz", str, NULL);
  }
  if(temperature < 0.) {
    sprintf(str, "temperature must be >= 0.; it is %e .",
      temperature);
    fatalerr("boltz", str, NULL);
  }
  if(temperature == 0.)
    return;

  /* For each weight in turn, decide whether to prune it because
  small.  Go through the weights in the order corresponding to the
  order used in the Fortran version. */
  n_pru_1 = n_pru_2 = 0;
  sumlogabs = sum = sum2 = wmax = 0.;
  wmin = 100000.;
  n_orig_1 = nhids * (ninps + 1);
  n_orig_2 = nouts * (nhids + 1);
  b2 = (w2 = (b1 = (w1 = w) + nhids * ninps) + nhids) + nouts * nhids;
  for(h = 0; h < nhids; h++) {
    for(i = 0; i < ninps; i++)
      boltz_work(w1 + h * ninps + i, boltzmann, temperature, 1,
        &n_pru_1, &n_pru_2, &sum, &sum2, &sumlogabs, &wmax, &wmin);
    boltz_work(b1 + h, boltzmann, temperature, 1,
        &n_pru_1, &n_pru_2, &sum, &sum2, &sumlogabs, &wmax, &wmin);
  }
  for(j = 0; j < nouts; j++) {
    for(h = 0; h < nhids; h++)
      boltz_work(w2 + j * nhids + h, boltzmann, temperature, 2,
        &n_pru_1, &n_pru_2, &sum, &sum2, &sumlogabs, &wmax, &wmin);
    boltz_work(b2 + j, boltzmann, temperature, 2,
        &n_pru_1, &n_pru_2, &sum, &sum2, &sumlogabs, &wmax, &wmin);
  }

  /* Finish computing some statistics. */
  n_unpru = n_orig_1 + n_orig_2 - (n_pru = n_pru_1 + n_pru_2);
  wmean = sum / n_unpru;
  range = RLOG2 * (log((double)wmax) - log((double)wmin)) + 1.;
  c = n_unpru * range;
  wl2sum = RLOG2 * sumlogabs + n_unpru *
    (1. - RLOG2 * log((double)wmin));
  entropy = c - wl2sum;
  r = wl2sum / c;

  /* xmean, used here, is never set.  For now, fakily set xmean to
  zero, just so it will be well-defined. */
  xmean = 0.; /* FAKE */
  theta = (sum2 - 2. * sum * xmean + n_unpru * xmean * xmean) /
    n_unpru;

  sprintf(str, " pruned %5d %5d %5d   C %12.5e  H %12.5e  R %6.2f\
  M %6.2f  T %7.4f\n", n_pru_1, n_pru_2, n_pru, c, entropy, 100. * r,
    wmean, theta);
  fsaso(str);
}

/********************************************************************/

void
boltz_work(awt_p, boltzmann, temperature, layer, n_pru_1, n_pru_2,
  sum, sum2, sumlogabs, wmax, wmin)
float *awt_p, temperature, *sum, *sum2, *sumlogabs, *wmax, *wmin;
char boltzmann;
int layer, *n_pru_1, *n_pru_2;
{
  float awt, abw;
  float uni(); /* uniform pseudorandom number generator */

  awt = *awt_p;
  if(uni(0) <= exp(-(double)(
    ((boltzmann == ABS_PRUNE) ? fabs((double)awt) : awt * awt)
    / temperature))) { /* Do prune this weight. */
    *awt_p = 0.;
    if(layer == 1)
      (*n_pru_1)++;
    else
      (*n_pru_2)++;
  }
  else {               /* Don't prune this weight. */
    *sum += awt;
    *sum2 += awt * awt;
    *sumlogabs += (float)log((double)(abw = fabs((double)awt)));
    if(abw > *wmax)
      *wmax = abw;
    if(abw < *wmin)
      *wmin = abw;
  }
}    

/********************************************************************/
