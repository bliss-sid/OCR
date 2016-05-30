/* Optimizes the network weights, either by using only an SCG
algorithm, or by doing a first part of the optimizing using SCG and
then doing the rest using a (faster) LBFGS algorithm.

Input args:
  scg_only: If TRUE, this routine just makes a call of the SCG
    optimizer (scg()) to do the entire optimization job (using the
    large value 101. as the pct arg to scg, so that it will not
    stop early).  (TRUE should be used if Boltzmann pruning is to be
    done during optimization (apparently), since LBFGS and pruning
    cannot be successfully used together.)
            If FALSE, this routine calls scg() using scg_earlystop_pct
    (next arg; see comment) as the pct arg; when scg() returns it
    calls lbfgs_dr() (driver for the LBFGS optimizer lbfgs()) to do
    the rest of the optimization.  (FALSE should be used if Boltzmann
    pruning is not to be done during optimization (apparently), since
    without pruning, both SCG and LBFGS can be used and, SCG then
    LBFGS gets the optimization done faster than if only SCG is used.
    (Apparently using only LBFGS, i.e. without SCG first, does not
    work well, even without pruning.))
  scg_earlystop_pct: If scg_only (preceding arg) is FALSE, then this
    routine uses scg_earlystop_pct as the pct arg in the call of scg()
    that it makes to do the first part of the optimization.  The
    meaning of the pct arg to scg(), is that as soon as the smallest
    percentage-correctly-classified for any single class reaches pct
    scg() returns.  So, this should be a value between 0. and 100.,
    and the smaller it is, the sooner scg will stop.  Apparently
    60. is a reasonable value for scg_earlystop_pct.  If scg_only is
    TRUE, then scg_earlystop_pct is ignored.
  do_confuse: If TRUE, will compute, and write to stderr and to the
    short outfile, the confusion matrices for the network, at the
    end of the scg training run, and if scg_only is FALSE also at the
    end of the lbfgs_dr (lbfgs driver) training run.
  do_long_outfile: If TRUE, will produce long_outfile at the end of
    the final training run here, which will be the scg run if scg_only
    is TRUE and the lbfgs_dr run otherwise.
  long_outfile: (Used only if do_long_outfile is TRUE.)  Filename of
    the long outfile to be produced.
  show_acs_times_1000: (Used only if do_long_outfile is TRUE.)
    Passed to e_and_g() via scg() or lbfgs_dr(); see comment in
    e_and_g.c.
  do_cvr: If TRUE, will compute, and write to stderr and to the short
    outfile, a correct-vs.-rejected table, at the end of the final
    training run here, which will be the scg run if scg_only is TRUE
    and the lbfgs_dr run otherwise.
  niter_max: Maximum number of training iterations allowed; but if
    <= 0, means this is a test run [just compute error value and
    confusion matrix resulting from initial weights?].
  ninps, nhids, nouts: Numbers of input, hidden, and output nodes.
  npats: Number of patterns.
  featvecs: Feature vectors of the patterns, an npats by ninps
    "matrix".
  use_targvecs: If TRUE, parm targvecs below is used; if FALSE, parm
    classes below is used.  Note that if errfunc != MSE, use_targvecs
    must be FALSE.
  targvecs: Target vectors, an npats by nouts matrix; used if
    use_targvecs is TRUE.  These must be used if the mlp is to be a
    function FITTER (not CLASSIFIER).  (If use_targvecs is FALSE, just
    set this to (float *)NULL.)
  classes: Classes of the patterns, an array of npats shorts; used if
    use_targvecs is FALSE.  (If use_targvecs is TRUE, just set this
    to (short *)NULL.)
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
  boltzmann: Decides whether to use Boltzmann pruning, and if
    so, what kind of threshold to use (see boltz.c).  Must be one of:
      NO_PRUNE: Do not prune.
      ABS_PRUNE: Prune using threshold exp(-|wt|/temperature),
        where wt is a weight being considered for pruning.
      SQUARE_PRUNE: Prune using threshold exp(-wt^t/temperature).
  temperature: For Boltzmann pruning.  (Not used if boltzmann
    is NO_PRUNE; in that case, set 0. as its value.)
  nfreq:
  egoal: Passed to scg(), and if scg_only is FALSE also passed to
    lbfgs_dr() later.  For both optimizers, this is an "error small"
    stopping condition: stop if *rmserr becomes < egoal.
  gwgoal: Passed to scg(), and if scg_only is FALSE also passed to
    lbfgs_dr() later.  For both optimizers, this is a "gradient small"
    stopping condition: stop if size(g) / size(w) becomes < gwgoal.
  oklvl: Threshold for rejection.
  long_classnames: Long names of the classes.
  short_classnames: Short names of the classes.
  purpose: CLASSIFIER or FITTER.
  lbfgs_gtol: Needed only if scg_only is FALSE, i.e. only if lbfgs
    optimization is going to be used by calling driver lbfgs_dr.
    See comment for lbfgs_gtol in lbfgs_dr, or comment for gtol in
    lbfgs.
  lbfgs_mem: Needed only if scg_only is FALSE, i.e. only if lbfgs
    optimization is going to be used by calling driver lbfgs_dr.
    See comment for lbfgs_mem in lbfgs_dr, or comment for m in lbfgs.

Input/output arg:
  w: The network weights, in this order:
    1st-layer weights (nhids by ninps "matrix", row-major);
    1st-layer biases (nhids elts);
    2nd-layer weights (nouts by nhids "matrix", row-major);
    2nd-layer biases (nouts elts).

Output args:
  rmserr: Error value.
  gw: Size(gradient) / size(weights).
  iter: How many iterations were used.
  ncalls: How many times e_and_g(), which computes the error and its
    gradient w.r.t. the weights, was called.  (Also, if scg_only is
    FALSE then this routine, before calling the LBFGS optimizer to do
    the second part of the optimization, writes ncalls to stderr and
    to the short outfile, showing how many calls SCG used.)
  ierr: Error code. [Show what the values mean; also, better yet,
    define names in an optwts.h.  Look out for same ierr value
    meaning two different things depending on whether it came from
    scg or from lbfgs_dr, in case that sometimes happens.]
*/

#include <stdio.h>
#include <mlp/defs.h>

void
optwts(scg_only, scg_earlystop_pct, do_confuse, do_long_outfile,
  long_outfile, show_acs_times_1000, do_cvr, niter_max, ninps, nhids,
  nouts, npats, featvecs, use_targvecs, targvecs, classes,
  acfunc_and_deriv_hids, acfunc_and_deriv_outs, errfunc, alpha,
  patwts, regfac, boltzmann, temperature, nfreq, egoal, gwgoal,
  oklvl, long_classnames, short_classnames, purpose, lbfgs_gtol,
  lbfgs_mem, w, rmserr, gw, iter, ncalls, ierr)
char scg_only, do_confuse, do_long_outfile, long_outfile[],
  show_acs_times_1000, do_cvr, use_targvecs, errfunc,
  boltzmann, **long_classnames, **short_classnames, purpose;
int niter_max, ninps, nhids, nouts, npats, nfreq, lbfgs_mem, *iter,
  *ncalls, *ierr;
float scg_earlystop_pct, *featvecs, *targvecs, alpha, *patwts, regfac,
  temperature, egoal, gwgoal, oklvl, lbfgs_gtol, *w, *rmserr, *gw;
short *classes;
void (*acfunc_and_deriv_hids)(), (*acfunc_and_deriv_outs)();
{
  char str[50], *scg_long_outfile, *lbfgs_dr_long_outfile,
    scg_do_long_outfile, scg_show_acs_times_1000, scg_do_cvr,
    lbfgs_dr_do_long_outfile, lbfgs_dr_show_acs_times_1000,
    lbfgs_dr_do_cvr;
  float *g;

  *ncalls = 0;
  if((g = (float *)malloc((nhids * (ninps + 1) + nouts * (nhids + 1))
    * sizeof(float))) == (float *)NULL)
    syserr("optwts", "malloc", "g");

  /* Run SCG optimizer, to do all (scg_only TRUE) or just a first part
  (scg_only FALSE) of the optimizing. */

  if(scg_only) {
    scg_do_long_outfile = do_long_outfile;
    scg_long_outfile = long_outfile;
    scg_show_acs_times_1000 = show_acs_times_1000;
    scg_do_cvr = do_cvr;
  }
  else {
    scg_do_long_outfile = FALSE;
    scg_long_outfile = (char *)NULL;
    scg_show_acs_times_1000 = FALSE;
    scg_do_cvr = FALSE;
    lbfgs_dr_do_long_outfile = do_long_outfile;
    lbfgs_dr_long_outfile = long_outfile;
    lbfgs_dr_show_acs_times_1000 = show_acs_times_1000;
    lbfgs_dr_do_cvr = do_cvr;
  }

  scg(do_confuse, scg_do_long_outfile, scg_long_outfile,
    scg_show_acs_times_1000, scg_do_cvr, niter_max, ninps, nhids,
    nouts, npats, featvecs, use_targvecs, targvecs, classes,
    acfunc_and_deriv_hids, acfunc_and_deriv_outs, errfunc, alpha,
    patwts, regfac, (scg_only ? 101. : scg_earlystop_pct),
    boltzmann, temperature, nfreq, egoal, gwgoal, oklvl,
    purpose, long_classnames, short_classnames, w, g, rmserr, gw,
    iter, ncalls, ierr);

  if(scg_only) {
    free((char *)g);
    return;
  }

  /* SCG did a first part of the optimizing; finish with LBFGS. */

  sprintf(str, " calls so far %d\n", *ncalls);
  fsaso(str);

  lbfgs_dr(do_confuse, lbfgs_dr_do_long_outfile, lbfgs_dr_long_outfile,
    lbfgs_dr_show_acs_times_1000, lbfgs_dr_do_cvr, niter_max, ninps,
    nhids, nouts, npats, featvecs, use_targvecs, targvecs, classes,
    acfunc_and_deriv_hids, acfunc_and_deriv_outs, errfunc, alpha,
    patwts, regfac, 101., nfreq, egoal, gwgoal, oklvl, purpose,
    long_classnames, short_classnames, lbfgs_gtol, lbfgs_mem, w, g,
    rmserr, gw, iter, ncalls, ierr);

  free((char *)g);
}
