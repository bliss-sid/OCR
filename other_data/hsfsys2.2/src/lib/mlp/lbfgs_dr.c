/* Contains:

# proc: lbfgs_dr - Driver routine for lbfgs optimizer.
  survey: Does optional surveying.  Activated by uncommenting the
    "#define SURVEY" line near the top of lbfgs.c.
*/

#include <math.h>
#include <stdio.h>
#include <mlp/blas.h>
#include <mlp/defs.h>
#include <mlp/macros.h>
#include <mlp/parms.h>
#include <mlp/lbfgs_dr.h>

/* These exist to get several values into survey(), which is called by
lbfgs() (which is called by this routine), but without messing up the
general optimization routine lbfgs() by just passing through it these
values, which pertain to a particular kind of neural net. */
static char errfunc_s;
static short *classes_s;
static int ninps_s, nhids_s, nouts_s, npats_s, use_targvecs_s;
static float *featvecs_s, *targvecs_s, alpha_s, *patwts_s, regfac_s,
  oklvl_s;
static void (*acfunc_and_deriv_hids_s)(),
  (*acfunc_and_deriv_outs_s)();

/*******************************************************************/

/* A driver routine for the lbfgs optimizer.

Input args:
  do_confuse: If TRUE, will compute, and write to stderr and to the
    short outfile, the confusion matrices for the network at the
    end of the lbfgs training run.
  do_long_outfile: If TRUE, will produce long_outfile at the end of
    the lbfgs training run.
  long_outfile: (Used only if do_long_outfile is TRUE.)  Filename of
    the long outfile to be produced.
  show_acs_times_1000: (Used only if do_long_outfile is TRUE.)
    Passed to e_and_g(); see comment in e_and_g.c.
  do_cvr: If TRUE, will compute, and write to stderr and to the short
    outfile, a correct-vs.-rejected table.
  niter_max: Maximum number of training iterations allowed.
  ninps, nhids, nouts: Numbers of input, hidden, and output nodes.
  npats: Number of patterns.
  featvecs: Feature vectors of the patterns, an npats by ninps
    "matrix".
  use_targvecs: If TRUE, parm targvecs below is used; if FALSE, parm
    classes below is used.  Note that if errfunc != MSE, use_targvecs
    must be FALSE.
  targvecs: Target vectors, an npats by nouts matrix; used if
    use_targvecs is TRUE.  (If not used, set to (float *)NULL.)
    These must be used if the mlp is to be a function fitter (not
    classifier).
  classes: Classes of the patterns, an array of npats unsigned chars;
    used if used_targvecs is FALSE.  (If not used, set to
    (short *)NULL.)  If the mlp is to be a classifier (not
    function fitter), it is better to use these classes rather than
    target vectors, to save memory.
  acfunc_and_deriv_hids: A function that computes the activation
    function to be used on the hidden nodes, and its derivative.
    This should be a void-returning function that takes three args:
    the input value (float), the output activation function value
    (float pointer), and the output activation function derivative
    value (float pointer).
  acfunc_and_deriv_outs: Like acfunc_and_deriv_hids, but for the
    output nodes.
  errfunc: Type of function used to compute the error contribution
    of a pattern from the activations vector and either the target
    vector or the actual class.  Must be one of the following
    (defined in parms.h):
    MSE: mean-squared error between activations and targets, or its
      equivalent using actual class instead of targets.
    TYPE_1: error function of type 1, using parm alpha (below).  (If
      this is used, use_targvecs must be FALSE.)
    POS_SUM: positive sum.  (If this is used, use_targvecs must be
      FALSE.)
  alpha: A parm that must be set if errfunc is TYPE_1.  (If errfunc is
    not TYPE_1, set value 0. for this parm.)
  patwts: Pattern-weights.
  regfac: Regularization factor.  The regularization term of the error
    is this factor times half the average of the squares of the
    weights.
  pct: A threshold for pctmin, such that if pctmin >= pct then
    the routine returns (under some conditions).
  nfreq:
  egoal: If *rmserr becomes < egoal, the routine returns.
  gwgoal: If size(g) / size(w) becomes < gwgoal, the routine returns.
  oklvl: Threshold for rejection.
  purpose, long_classnames, short_classnames: Passed to accum_print().
  lbfgs_gtol: Used as the gtol arg in the call of lbfgs(); see comment
    in lbfgs.c.
  lbfgs_mem: Used as the m arg in the call of lbfgs(); see comment
    in lbfgs.c.

Input/output args:
  w: The network weights, in this order:
    1st-layer weights (nhids by ninps "matrix", row-major);
    1st-layer biases (nhids elts);
    2nd-layer weights (nouts by nhids "matrix", row-major);
    2nd-layer biases (nouts elts).
  ncalls: A counter which this routine increments each time it calls
    e_and_g(), which computes the error and its gradient.

Scratch-buffer arg:
  g: For holding the gradient of the error w.r.t the network weights.
    Caller is to provide this buffer, allocated to (at least) as many
    floats as there are weights, i.e. nhids * (ninps + 1) + nouts *
    (nhids + 1) floats.  This buffer's contents upon entry to this
    routine do not matter, and its contents after return are not
    intended to be used.

Output args:
  rmserr: Error value.
  gw: Size(gradient) / size(weights).
  iter: How many iterations were used.
  ierr: Error code.
*/

void
lbfgs_dr(do_confuse, do_long_outfile, long_outfile,
  show_acs_times_1000, do_cvr, niter_max, ninps, nhids, nouts, npats,
  featvecs, use_targvecs, targvecs, classes, acfunc_and_deriv_hids,
  acfunc_and_deriv_outs, errfunc, alpha, patwts, regfac, pct, nfreq,
  egoal, gwgoal, oklvl, purpose, long_classnames, short_classnames,
  lbfgs_gtol, lbfgs_mem, w, g, rmserr, gw, iter, ncalls, ierr)
char do_confuse, do_long_outfile, long_outfile[], show_acs_times_1000,
  do_cvr, use_targvecs, errfunc, purpose, **long_classnames,
  **short_classnames;
int niter_max, ninps, nhids, nouts, npats, lbfgs_mem, *iter,
  *ncalls, *ierr;
short *classes;
float *featvecs, *targvecs, alpha, *patwts, regfac, pct, egoal,
  gwgoal, oklvl, lbfgs_gtol, *w, *g, *rmserr, *gw;
void (*acfunc_and_deriv_hids)(), (*acfunc_and_deriv_outs)();
{
  char str[100];
  int numwts, i, fcount, iprint[2], iflag, info, jiter, jiterp,
    junkint0, junkint1;
  static int i1 = 1;
  float err, pctmin, xtol, stp, gsiz, wsiz, e1, e2, *work, *diag,
    junkfloat;

  /* Load some values needed by survey() into variables whose scope is
  this file (which includes survey). */
  errfunc_s = errfunc;
  classes_s = classes;
  ninps_s = ninps;
  nhids_s = nhids;
  nouts_s = nouts;
  npats_s = npats;
  use_targvecs_s = use_targvecs;
  featvecs_s = featvecs;
  targvecs_s = targvecs;
  alpha_s = alpha;
  patwts_s = patwts;
  regfac_s = regfac;
  oklvl_s = oklvl;
  acfunc_and_deriv_hids_s = acfunc_and_deriv_hids;
  acfunc_and_deriv_outs_s = acfunc_and_deriv_outs;

  numwts = nhids * (ninps + 1) + nouts * (nhids + 1);

  sprintf(str, "\n LBFGS_DR: doing <= %d iterations; %d variables\n",
    niter_max, numwts);
  fsaso(str);

  /* Get initial error and gradient. */
  e_and_g(TRUE, TRUE, FALSE, FALSE, (char *)NULL, FALSE, FALSE, ninps,
    nhids, nouts, w, npats, featvecs, use_targvecs, targvecs, classes,
    acfunc_and_deriv_hids, acfunc_and_deriv_outs, errfunc, alpha,
    patwts, regfac, oklvl, &err, g, &e1, &e2);
  (*ncalls)++;

  optchk_store_e1_e2(e1, e2);
  optchk(FALSE, 0, w, err, ierr, &pctmin);

  *rmserr = sqrt((double)(err * 2.));
  iflag = 0;

  iprint[0] = -1; /* negative suppresses messages from lbfgs */
  iprint[1] = 0;
  xtol = 1.e-7; /* machine accuracy, more or less */

  if((work = (float *)malloc((numwts * (2 * lbfgs_mem + 1) +
    2 * lbfgs_mem) * sizeof(float))) == (float *)NULL)
    syserr("lbfgs_dr", "malloc", "work");
  if((diag = (float *)malloc(numwts * sizeof(float))) ==
    (float *)NULL)
    syserr("lbfgs_dr", "malloc", "diag");

  *iter = fcount = 0;
  for(i = 0; i < numwts; i++)
    diag[i] = 1.;

  jiter = 0;
  while(*iter < niter_max) {
    *ierr = 0;
    jiterp = jiter;
    optchk_store_e1_e2(e1, e2);
    lbfgs(numwts, lbfgs_mem, w, err, g, (int)FALSE, diag, iprint, xtol,
      work, &iflag, &info, stderr, stderr, lbfgs_gtol, STPMIN, STPMAX,
      iter, &jiter, ierr, &stp);
    if(iflag == 0) /* success */
      break; 
    if(iflag < 0) {
      fprintf(stderr, " lbfgs error %d\n", iflag);
      if(iflag == -1)
	fprintf(stderr, "   info %d\n", info);
      fcount++;
      if(fcount > 1) {
	*ierr = 10;
	fprintf(stderr, " two failures in a row; quit\n");
	break;
      }
      else {
	fprintf(stderr, " restart LBFGS_DR %d\n", *iter);
	iflag = 0;
	for(i = 0; i < numwts; i++)
	  diag[i] = 1.;
	jiter = 1;
	continue;
      }
    }
    if(*ierr != 0)
      break;
    if(jiterp != jiter) { /* completed another iteration */
      (*iter)++;
      fcount = 0;
      /* Terminate when error satisfactory */
      *rmserr = sqrt((double)(2. * err));
      if(*rmserr < egoal || pctmin > pct) {
	*ierr = 0;
	break;
      }
      /* Terminate when gradient is too small */
      gsiz = snrm2_(&numwts, g, &i1);
      wsiz = snrm2_(&numwts, w, &i1);
      if(gsiz < gwgoal * max(1., wsiz)) {
	*ierr = 2;
	break;
      }
    }
    if(iflag == 2)
      for(i = 0; i < numwts; i++)
	diag[i] = 1.;
    if(iflag == 1) {
      /* Get error and gradient. */
      e_and_g(TRUE, TRUE, FALSE, FALSE, (char *)NULL, FALSE, FALSE,
        ninps, nhids, nouts, w, npats, featvecs, use_targvecs,
        targvecs, classes, acfunc_and_deriv_hids,
        acfunc_and_deriv_outs, errfunc, alpha, patwts, regfac, oklvl,
        &err, g, &e1, &e2);
      (*ncalls)++;
    }

  } /* while(*iter < niter_max) */

  if(*iter == niter_max)
    *ierr = 1;
  *gw = gsiz / wsiz;

  /* Do another call of this with the (same) final weights, to
  accumulate at least the minimal counting information, which is
  always needed; and the switches also may activate optional
  computations: confusion matrices, long outfile, and
  correct-vs.-rejected table. */
  e_and_g(FALSE, TRUE, do_confuse, do_long_outfile, long_outfile,
    show_acs_times_1000, do_cvr, ninps, nhids, nouts, w, npats,
    featvecs, use_targvecs, targvecs, classes, acfunc_and_deriv_hids,
    acfunc_and_deriv_outs, errfunc, alpha, patwts, regfac, oklvl,
    &err, (float *)NULL, &e1, &e2);

  /* Finishes and writes the minimal counting info, and if desired
  also the confusion matrices. */
  accum_print(do_confuse, purpose, npats, *iter, err, e1, e2, 'F', w,
    long_classnames, short_classnames, &junkint0, &junkint1,
    &junkfloat);

  if(do_cvr)
    /* Finishes and writes correct-vs.-rejected table. */
    cvr_print(TRAIN, npats);

  free((char *)work);
  free((char *)diag);
}

/*******************************************************************/

/* This is called by lbfgs if the "#define SURVEY" line near the top
of lbfgs.c is uncommented.  It computes, and writes to stderr and to
the short outfile, the error at the points w + k*stp*p for
0 <= k <= 4, where w is a current weights vector and p is a
step-direction vector. */

void
survey(numwts, w, p, stp)
int numwts;
float *w, *p, stp;
{
  char str[50];
  int i, k;
  float err, delta, *wnew, e1_unused, e2_unused;

  fsaso(" surveying along a direction:\n");
  if((wnew = (float *)malloc(numwts * sizeof(float))) == (float *)NULL)
    syserr("survey (lbfgs_dr.c)", "malloc", "wnew");
  for(k = 0; k < 5; k++) {
    delta = k * stp;
    for(i = 0; i < numwts; i++)
      wnew[i] = w[i] + delta * p[i];
    e_and_g(FALSE, FALSE, FALSE, FALSE, (char *)NULL, NULL, NULL,
      ninps_s, nhids_s, nouts_s, wnew, npats_s, featvecs_s,
      use_targvecs_s, targvecs_s, classes_s, acfunc_and_deriv_hids_s,
      acfunc_and_deriv_outs_s, errfunc_s, alpha_s, patwts_s, regfac_s,
      oklvl_s, &err, (float *)NULL, &e1_unused, &e2_unused);
    sprintf(str, "   %e %e\n", delta, err);
    fsaso(str);
  }
  free((char *)wnew);
}

/*******************************************************************/
