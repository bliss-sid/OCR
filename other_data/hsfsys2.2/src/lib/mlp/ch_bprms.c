/* [Now this calls uni(12345) if the seed has not been set: that is
so that an mlp instance whose first run reads the initial network
weights (hence no seed needed for the weights), but which does
pruning (needs random numbers), will get uni seeded.] */

/*
# proc: ch_bprms - checks a run-block of parms that have already been read
# proc:            from a specfile.
Performs checking beyond that done by st_nv_ok.  For
example, detects the illegal situation wherein a parm is set to a
certain value but an auxiliary parm that would then also have to be
set, is not set.

Input arg:
  parms: A PARMS (defined in parms.h) structure that has been filled
    in according to the name-value pairs scanned from the run-block.

Output args:
  cb_any_warnings: TRUE (FALSE) if there was (was not) at least one
    warning.
  cb_any_errors: TRUE (FALSE) if there was (was not) at least one
    error.

Side effects:
  Uses eb_cat_w() and eb_cat_e() to catenate warning and error
    messages (if any) onto the hidden error buffer.
  Uses is_w_clr(), is_e_clr(), is_w_get(), and is_e_get() to clear and
    get the hidden warning and error existence flags, and calls
    routines (eb_cat_w() and eb_cat_e()) that use is_w_set() and
    is_e_set() to set those flags.
*/

#include <mlp/defs.h>
#include <mlp/parms.h>

void
ch_bprms(parms, cb_any_warnings, cb_any_errors)
PARMS *parms;
char *cb_any_warnings, *cb_any_errors;
{
  char str[1000], mmm_gotten;
  int mpats, minps, mouts;
  char is_w_get(), is_e_get(), got_mmm();
  float uni();

  is_w_clr();
  is_e_clr();

  /* Do those parameter checks that should be done regardless of
  whether this is a training or testing run.  Error if an
  always-needed parm has not been set; warning if a parm that will be
  ignored, has been set. */

  if(!parms->train_or_test.ssl.set_tried)
    neverset("train_or_test");
  if(!parms->short_outfile.ssl.set_tried)
    neverset("short_outfile");
  if(!parms->errfunc.ssl.set_tried)
    neverset("errfunc");
  if(!parms->regfac.ssl.set_tried)
    neverset("regfac");
  if(!parms->do_confuse.ssl.set_tried)
    neverset("do_confuse");
  if(!parms->do_cvr.ssl.set_tried)
    neverset("do_cvr");

  if(parms->errfunc.ssl.set) {
    if(parms->errfunc.val == TYPE_1 && !parms->alpha.ssl.set_tried) {
      sprintf(str, "errfunc is set to type_1 (line %d), but alpha \
is never set", parms->errfunc.ssl.linenum);
      eb_cat_e(str);
    }
    else if(parms->errfunc.val == MSE && parms->alpha.ssl.set_tried) {
      sprintf(str, "errfunc is set to mse (line %d), but alpha is \
set (line %d); it will not be used",
        parms->errfunc.ssl.linenum, parms->alpha.ssl.linenum);
      eb_cat_w(str);
    }
    else if(parms->errfunc.val == POS_SUM &&
      parms->alpha.ssl.set_tried) {
      sprintf(str, "errfunc is set to pos_sum (line %d), but alpha \
is set (line %d); it will not be used",
        parms->errfunc.ssl.linenum, parms->alpha.ssl.linenum);
      eb_cat_w(str);
    }
  }

  if(!parms->patterns_infile.ssl.set_tried)
    neverset("patterns_infile");
  if(!parms->npats.ssl.set_tried)
    neverset("npats");
  if(!parms->oklvl.ssl.set_tried)
    neverset("oklvl");
  if(!parms->priors.ssl.set_tried)
    neverset("priors");

  if(parms->priors.ssl.set) {
    if(parms->priors.val == ALLSAME) {
      if(parms->class_wts_infile.ssl.set_tried) {
	sprintf(str, "priors is set to allsame (line %d), \
but class_wts_infile is set (line %d); that file will not be \
read", parms->priors.ssl.linenum,
          parms->class_wts_infile.ssl.linenum);
	eb_cat_w(str);
      }
      if(parms->pattern_wts_infile.ssl.set_tried) {
	sprintf(str, "priors is set to allsame (line %d), \
but pattern_wts_infile is set (line %d); that file will not be \
read", parms->priors.ssl.linenum,
          parms->pattern_wts_infile.ssl.linenum);
	eb_cat_w(str);
      }
    }
    else if(parms->priors.val == CLASS) {
      if(!parms->class_wts_infile.ssl.set_tried) {
	sprintf(str, "priors is set to class (line %d), \
but class_wts_infile is never set", parms->priors.ssl.linenum);
	eb_cat_e(str);
      }
      if(parms->pattern_wts_infile.ssl.set_tried) {
	sprintf(str, "priors is set to class (line %d), \
but pattern_wts_infile is set (line %d); that file will not be \
read", parms->priors.ssl.linenum,
          parms->pattern_wts_infile.ssl.linenum);
	eb_cat_w(str);
      }
    }
    else if(parms->priors.val == PATTERN) {
      if(!parms->pattern_wts_infile.ssl.set_tried) {
	sprintf(str, "priors is set to pattern \
(line %d), but pattern_wts_infile is never set",
          parms->priors.ssl.linenum);
        eb_cat_e(str);
      }
      if(parms->class_wts_infile.ssl.set_tried) {
	sprintf(str, "priors is set to pattern \
(line %d), but class_wts_infile is set; that file will not be \
read", parms->priors.ssl.linenum);
	eb_cat_w(str);
      }
    }
    else { /* parms->priors.val == BOTH */
      if(!parms->pattern_wts_infile.ssl.set_tried) {
	sprintf(str, "priors is set to both \
(line %d), but class_wts_infile is never set",
          parms->priors.ssl.linenum);
        eb_cat_e(str);
      }
      if(!parms->pattern_wts_infile.ssl.set_tried) {
	sprintf(str, "priors is set to both \
(line %d), but pattern_wts_infile is never set",
          parms->priors.ssl.linenum);
        eb_cat_e(str);
      }
    }
  }

  if(!parms->patsfile_ascii_or_binary.ssl.set_tried)
    neverset("patsfile_ascii_or_binary");
  if(!parms->trgoff.ssl.set_tried)
    neverset("trgoff");

  if(parms->long_outfile.ssl.set &&
    !parms->show_acs_times_1000.ssl.set_tried) {
    sprintf(str, "long_outfile is set (line %d), but \
show_acs_times_1000 is never set", parms->long_outfile.ssl.linenum);
    eb_cat_e(str);
  }
  if(!parms->long_outfile.ssl.set_tried &&
    parms->show_acs_times_1000.ssl.set) {
    sprintf(str, "long_outfile is not set, but \
show_acs_times_1000 is set (line %d); value not used, since it \
would affect only long_outfile",
      parms->show_acs_times_1000.ssl.linenum);
    eb_cat_w(str);
  }

  if(parms->priors.ssl.set && parms->do_confuse.ssl.set)
    if(parms->priors.val == CLASS || parms->priors.val == BOTH ||
      parms->do_confuse.val == TRUE) {
      if(!parms->lcn_scn_infile.ssl.set_tried) {
        if(parms->priors.val == CLASS || parms->priors.val == BOTH)
          sprintf(str, "priors is set to %s (line %d), so short \
class-names are required; but lcn_scn_infile is never set",
            (parms->priors.val == CLASS ? "class" : "both"),
            parms->priors.ssl.linenum);
        else /* parms->do_confuse.val == TRUE */
          sprintf(str, "do_confuse is set to true (line %d), so \
short class-names are required; but lcn_scn_infile is never set",
            parms->do_confuse.ssl.linenum);
        eb_cat_e(str);
      }
    }
    else if(parms->lcn_scn_infile.ssl.set) {
      sprintf(str, "Not the case that ((priors is class) or (priors \
is both) or (do_confuse is true)), so short class-names are not \
required; but lcn_scn_infile is set (line %d).  That file will not \
be read.", parms->lcn_scn_infile.ssl.linenum);
      eb_cat_w(str);
    }

    if(parms->patterns_infile.ssl.set &&
      parms->patsfile_ascii_or_binary.ssl.set) {
      if(mmm_gotten = got_mmm(parms->patterns_infile.val,
        parms->patsfile_ascii_or_binary.val, &mpats, &minps, &mouts,
        str)) {
        if(parms->npats.ssl.set && parms->npats.val > mpats) {
          sprintf(str, "npats value, %d, set in line %d, is larger \
than mpats (total number of patterns) value, %d, of patterns file \
%s", parms->npats.val, parms->npats.ssl.linenum, mpats,
            parms->patterns_infile.val);
          eb_cat_e(str);
        }
      }
      else
        eb_cat_e(str);
    }
    else
      mmm_gotten = FALSE;

  /* (End of those parameter checks that should be done regardless of
  whether this is a training or testing run.) */

  if(!parms->train_or_test.ssl.set) {
    *cb_any_warnings = is_w_get();
    *cb_any_errors = is_e_get();

    if(!parms->seed.ssl.set)
      uni(12345);

    return;
  }

  if(parms->train_or_test.val == TRAIN) {
    /* Check that all parms needed only for training have been set.
    Check that any additional parms required as a result of the
    settings of these mandatory parms (e.g., temperature, if pruning
    is to be done), are also set.  Warn if irrelevant parms, or parms
    that will be ignored, have been set. */

    if(!parms->boltzmann.ssl.set_tried)
      neverset("boltzmann");
    if(parms->boltzmann.ssl.set) {
      if(parms->boltzmann.val == NO_PRUNE) {
        if(parms->temperature.ssl.set_tried) {
          sprintf(str, "boltzmann is set to no_prune \
(line %d), but temperature is set (line %d); it will not be used",
            parms->boltzmann.ssl.linenum,
            parms->temperature.ssl.linenum);
          eb_cat_w(str);
        }
        if(!parms->scg_earlystop_pct.ssl.set_tried) {
          sprintf(str, "boltzmann is set to no_prune \
(line %d), causing hybrid SCG/LBFGS training, but \
scg_earlystop_pct is never set", parms->boltzmann.ssl.linenum);
          eb_cat_e(str);
        }
        if(!parms->lbfgs_gtol.ssl.set_tried) {
          sprintf(str, "boltzmann is set to no_prune \
(line %d), causing hybrid SCG/LBFGS training, but \
lbfgs_gtol is never set", parms->boltzmann.ssl.linenum);
          eb_cat_e(str);
        }
        if(!parms->lbfgs_mem.ssl.set_tried) {
          sprintf(str, "boltzmann is set to no_prune \
(line %d), causing hybrid SCG/LBFGS training, but \
lbfgs_mem is never set", parms->boltzmann.ssl.linenum);
          eb_cat_e(str);
        }
      }
      else { /* parms->boltzmann.val != NO_PRUNE */
        if(!parms->temperature.ssl.set_tried) {
          sprintf(str, "boltzmann is set to %s (line %d), but \
temperature is never set",
            (parms->boltzmann.val == ABS_PRUNE ? "abs_prune" :
            "square_prune"), parms->boltzmann.ssl.linenum);
	  eb_cat_e(str);
        }
        if(parms->scg_earlystop_pct.ssl.set_tried) {
          sprintf(str, "boltzmann is set to %s (line %d), \
causing SCG training, but scg_earlystop_pct is set (line %d); it \
will not be used",
            (parms->boltzmann.val == ABS_PRUNE ? "abs_prune" :
            "square_prune"), parms->boltzmann.ssl.linenum,
            parms->scg_earlystop_pct.ssl.linenum);
          eb_cat_w(str);
        }
        if(parms->lbfgs_gtol.ssl.set_tried) {
          sprintf(str, "boltzmann is set to %s (line %d), \
causing SCG training, but lbfgs_gtol is set (line %d); it \
will not be used",
            (parms->boltzmann.val == ABS_PRUNE ? "abs_prune" :
            "square_prune"), parms->boltzmann.ssl.linenum,
            parms->lbfgs_gtol.ssl.linenum);
          eb_cat_w(str);
        }
        if(parms->lbfgs_mem.ssl.set_tried) {
          sprintf(str, "boltzmann is set to %s (line %d), \
causing SCG training, but lbfgs_mem is set (line %d); it \
will not be used",
            (parms->boltzmann.val == ABS_PRUNE ? "abs_prune" :
            "square_prune"), parms->boltzmann.ssl.linenum,
            parms->lbfgs_mem.ssl.linenum);
          eb_cat_w(str);
        }
      }
    }

    if(parms->wts_infile.ssl.set_tried) {
      if(parms->seed.ssl.set_tried) {
	sprintf(str, "wts_infile is set (line %d), but seed is set \
(line %d); weights will be read from the file, and seed will not be \
used", parms->wts_infile.ssl.linenum, parms->seed.ssl.linenum);
	eb_cat_w(str);
      }
      csopiwh(parms);
    }
    else {
      if(!parms->seed.ssl.set_tried)
	eb_cat_e("wts_infile is not set, meaning that random initial \
weights should be generated, but seed is never set");
      if(!parms->purpose.ssl.set_tried)
	eb_cat_e("wts_infile is not set, so purpose must be set in \
specfile, but it is not");
      if(!parms->ninps.ssl.set_tried)
	eb_cat_e("wts_infile is not set, so ninps must be set in \
specfile, but it is not");
      if(!parms->nhids.ssl.set_tried)
	eb_cat_e("wts_infile is not set, so nhids must be set in \
specfile, but it is not");
      if(!parms->nouts.ssl.set_tried)
	eb_cat_e("wts_infile is not set, so nouts must be set in \
specfile, but it is not");
      if(!parms->acfunc_hids.ssl.set_tried)
	eb_cat_e("wts_infile is not set, so acfunc_hids must be set \
in specfile, but it is not");
      if(!parms->acfunc_outs.ssl.set_tried)
	eb_cat_e("wts_infile is not set, so acfunc_outs must be set \
in specfile, but it is not");
    }

    if(!parms->wts_outfile.ssl.set_tried)
      neverset("wts_outfile");
    if(!parms->niter_max.ssl.set_tried)
      neverset("niter_max");
    if(!parms->egoal.ssl.set_tried)
      neverset("egoal");
    if(!parms->gwgoal.ssl.set_tried)
      neverset("gwgoal");
    if(!parms->nfreq.ssl.set_tried)
      neverset("nfreq");
    if(!parms->errdel.ssl.set_tried)
      neverset("errdel");
    if(!parms->nokdel.ssl.set_tried)
      neverset("nokdel");

    if(mmm_gotten && !parms->wts_infile.ssl.set_tried) {
      if(parms->ninps.ssl.set && parms->ninps.val > minps) {
        sprintf(str, "ninps value, %d, set in line %d, is larger \
than minps (number of elements in each feature vector) value, %d, \
of patterns file %s", parms->ninps.val, parms->ninps.ssl.linenum,
          minps, parms->patterns_infile.val);
        eb_cat_e(str);
      }
      if(parms->nouts.ssl.set && parms->nouts.val != mouts) {
        sprintf(str, "nouts value, %d, set in line %d, does not \
equal mouts (number of elements in each target vector, or number of \
classes) value, %d, of patterns file %s", parms->nouts.val,
          parms->nouts.ssl.linenum, mouts,
          parms->patterns_infile.val);
        eb_cat_e(str);
      }
    }

  } /* parms->train_or_test.val == TRAIN */

  else { /* parms->train_or_test.val == TEST */
    /* Warn if irrelevant parms, or parms that will be ignored, have
    been set.  Check that the parm needed only for testing, namely
    wts_infile, has been set. */

    csopiwh(parms);

    if(parms->boltzmann.ssl.set_tried) {
      sprintf(str, "boltzmann has been set (line %d); its \
value will not be used", parms->boltzmann.ssl.linenum);
      eb_cat_w(str);
    }

    if(parms->temperature.ssl.set_tried) {
      sprintf(str, "temperature has been set (line %d); its value \
will not be used", parms->temperature.ssl.linenum);
      eb_cat_w(str);
    }

    if(!parms->wts_infile.ssl.set_tried)
      neverset("wts_infile");

    if(parms->wts_outfile.ssl.set_tried) {
      sprintf(str, "wts_outfile has been set (line %d); there will \
be no writing to that file", parms->wts_outfile.ssl.linenum);
      eb_cat_w(str);
    }

    if(parms->seed.ssl.set_tried) {
      sprintf(str, "seed has been set (line %d); its value will \
not be used", parms->seed.ssl.linenum);
      eb_cat_w(str);
    }

    if(parms->niter_max.ssl.set_tried)
      tsp_w("niter_max", parms->niter_max.ssl.linenum);
    if(parms->egoal.ssl.set_tried)
      tsp_w("egoal", parms->egoal.ssl.linenum);
    if(parms->gwgoal.ssl.set_tried)
      tsp_w("gwgoal", parms->gwgoal.ssl.linenum);
    if(parms->nfreq.ssl.set_tried)
      tsp_w("nfreq", parms->nfreq.ssl.linenum);
    if(parms->errdel.ssl.set_tried)
      tsp_w("errdel", parms->errdel.ssl.linenum);
    if(parms->nokdel.ssl.set_tried)
      tsp_w("nokdel", parms->nokdel.ssl.linenum);

  } /* parms->train_or_test.val == TEST */

  *cb_any_warnings = is_w_get();
  *cb_any_errors = is_e_get();

  if(!parms->seed.ssl.set)
    uni(12345);
}
