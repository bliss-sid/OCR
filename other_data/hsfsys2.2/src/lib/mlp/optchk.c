/* [This version uses file-scope static variables, set by calling
helper routines, so as to keep many values that would normally be args
of optchk out of its arg list.]

Contains:
# proc: optchk_store_unchanging_vals - Copies its args to file-scope static
# proc:              variables which optchk reads later.
# proc: optchk_store_e1_e2 - Stores main part of error and the mean squared
# proc:              weight.
# proc: optchk - Checks progress of neural net optimization and perhaps prints.
*/

#include <stdio.h>
#include <math.h>
#include <mlp/defs.h>
#include <mlp/macros.h>
#include <mlp/parms.h>

/* These are set by optchk_store_unchanging_vals when it is called
(once, near start of training run), and later used repeatedly by
optchk. */
static int npats, nouts, nfreq, nokdel;
static char **long_classnames, **short_classnames, boltzmann;
static float temperature, errdel, oklvl;

/* These are repeatedly
  1. set by optchk_store_e1_e2 and then
  2. used by optchk. */
static float e1, e2;

/*******************************************************************/

/* optchk_store_unchanging_vals: Copies its args to file-scope static
variables which optchk reads later.  Since these values do not change
during a training run, it is sufficient to call this routine just once
in a run, before the first optchk call.

Input args:
  npats_in:
  long_classnames_in:
  short_classnames_in:
  nouts_in:
  boltzmann_in:
  temperature_in:
  nfreq_in:
  errdel_in:
  nokdel_in:
  oklvl_in:
  
Side effect: Copies its input args to file-scope static variables
  which are used later by optchk.
*/

void
optchk_store_unchanging_vals(npats_in, long_classnames_in,
  short_classnames_in, nouts_in, boltzmann_in, temperature_in,
  nfreq_in, errdel_in, nokdel_in, oklvl_in)
int npats_in, nouts_in, nfreq_in, nokdel_in;
char **long_classnames_in, **short_classnames_in, boltzmann_in;
float temperature_in, errdel_in, oklvl_in;
{
  npats = npats_in;
  long_classnames = long_classnames_in;
  short_classnames = short_classnames_in;
  nouts = nouts_in;
  boltzmann = boltzmann_in;
  temperature = temperature_in;
  nfreq = nfreq_in;
  errdel = errdel_in;
  nokdel = nokdel_in;
  oklvl = oklvl_in;
}

/*******************************************************************/

/*
Input args:
  e1_in: Main part of error, basically.
  e2_in: Mean squared weight.

Side effect: Copies its input args to file-scope static variables
  which are used later by optchk.
*/

void
optchk_store_e1_e2(e1_in, e2_in)
float e1_in, e2_in;
{
  e1 = e1_in;
  e2 = e2_in;
}

/*******************************************************************/

/* optchk: Checks progress of neural net optimization; perhaps prints.
Returns ierr of 0 if okay convergence so far.  After kmin iterations,
starts checking convergence.  Stops with error return in ierr if
convergence has been slow for NNOT times in a row.  If either of the
following two situations occurs, that is defined to be slow
convergence:
  3. Error hasn't gone down by at least a factor of errdel over the
       most recent block of nfreq iterations.
  4. For each of the most recent NNOT blocks of nfreq iterations,
       the weighted "number right", and the weighted "number right"
       minus weighted "number wrong", have both failed to increase by
       at least nokdel over the block.

Input args:
  do_confuse: Sent to accum_print if iter is 0.  If TRUE, accum_print
    finds and prints the confusion matrix and the matrix of the
    average output level.
  iter: How many iterations the optimization has used so far.
  w: Network weights.
  err: Error.

Output args:
  ierr: A code number whose values mean the following:
    0: Convergence is ok so far.
    3: Convergence is slow by criterion 3 above.
    4: Convergence is slow by criterion 4 above.
  wtd_rpct_min: Minimum, across classes, of weighted
    "right-percentage by class".

NOTICE: This routine is affected by the file-scope static variables
defined at the top of this file, some of which are set by
optchk_store_unchanging_vals and some of which are set by
optchk_store_e1_e2.
*/

#define NF 2       /* Don't quit until NF * nfreq iters or */
#define NITER 40   /* until NITER iters, whichever is larger */
#define NBOLTZ 100 /* until NBOLTZ iters, if doing Boltzmann. */
#define NNOT 3     /* Quit if not improving NNOT times in row. */

void
optchk(do_confuse, iter, w, err, ierr, wtd_rpct_min)
char do_confuse;
int iter, *ierr;
float w[], err, *wtd_rpct_min;
{
  static int wtd_nr, wtd_nw, wtd_nr_prv, wtd_nw_prv, ncount, notimp,
    kmin;
  float rmserr;
  static float rmsold;

  *ierr = 0;
  if(iter == 0) {
    accum_print(do_confuse, CLASSIFIER, npats, 0, err, e1, e2, ' ', w,
      long_classnames, short_classnames, &wtd_nr, &wtd_nw,
      wtd_rpct_min);
    ncount = 0; /* iterations since last check */
    notimp = 0; /* number of bad checks in a row */
    kmin = max(NF * nfreq, NITER);
    if(boltzmann != NO_PRUNE && temperature > 0.)
      kmin = max(kmin, NBOLTZ);
    rmsold = sqrt((double)(2. * err));
  }
  else {
    if(iter == kmin) {
      wtd_nr_prv = wtd_nr;
      wtd_nw_prv = wtd_nw;
    }
    ncount++;
    if(ncount >= nfreq) {
      ncount = 0;
      accum_print(FALSE, CLASSIFIER, npats, iter, err, e1, e2, ' ',
        w, long_classnames, short_classnames, &wtd_nr, &wtd_nw,
        wtd_rpct_min);
      rmserr = sqrt((double)(2. * err));
      if(iter >= kmin && rmserr > errdel * rmsold) {
	*ierr = 3;
	return;
      }
      rmsold = rmserr;
      if(iter >= kmin && (wtd_nr - wtd_nr_prv < nokdel) &&
        (wtd_nr - wtd_nw) - (wtd_nr_prv - wtd_nw_prv) < nokdel)
	notimp++;
      else
	notimp = 0;
      if(notimp >= NNOT) {
	*ierr = 4;
	return;
      }
      wtd_nr_prv = max(wtd_nr, wtd_nr_prv);
      wtd_nw_prv = min(wtd_nw, wtd_nw_prv);
    }
    else
      cwrite(iter);
  }
}

/*******************************************************************/
