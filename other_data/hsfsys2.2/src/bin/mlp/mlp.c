/*
# pgrm: mlp - Main routine for running the Multi-Layer Perceptron training
# pgrm:       and testing program.

This file also contains:
  parse_cmd_line
  get_parms_vals
  get_a_parms_val
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mlp/blas.h>
#include <mlp/lims.h>
#include <mlp/defs.h>
#include <mlp/little.h>
#include <mlp/parms.h>
#include <mlp/acsmaps.h>

/*******************************************************************/

/* Main routine for mlp comand. */

main(argc, argv) int argc; char **argv;
{
  FILE *fp_specfile;
  char str[200], specfile[100], *long_outfile, *short_outfile,
    *patterns_infile, *wts_infile, *wts_outfile, *class_wts_infile,
    *pattern_wts_infile, *lcn_scn_infile, errfunc, purpose,
    boltzmann, train_or_test, acfunc_hids, acfunc_outs,
    priors, *hms, **long_classnames, **short_classnames,
    patsfile_ascii_or_binary, do_confuse, show_acs_times_1000, do_cvr,
    check_only, any_warnings, any_errors, gb_any_error,
    scg_only, use_targvecs, doity_long_outfile;
  short *classes; /* (if to be stored) of the patterns */
  int i, n_runblocks, irun, nruns, runblock_start_linenum, npats,
    ninps, nhids, nouts, seed, niter_max, nfreq, nokdel, iter, ierr,
    ncalls, mpats, lbfgs_mem, numwts, nright_unused, nwrong_unused,
    junkint0, junkint1;
  static int i1 = 1;
  float regfac, alpha, temperature, egoal, gwgoal, errdel, oklvl,
    *featvecs, /* feature vectors of the patterns */
    *targvecs, /* target vectors (if to be stored) of the patterns */
    *patwts,
    *w,        /* weights of the MLP */
    *wsav,     /* saved original weights of the MLP */
    rmserr,    /* latest error value */
    afac, secs, gw, dif, scg_earlystop_pct, trgoff, lbfgs_gtol,
    pctmin_unused, err, e1, e2, junkfloat;
  PARMS parms;
  void get_parms_vals(), parse_cmd_line();

  parse_cmd_line(argc, argv, &check_only, specfile);

  if(!specfile[0])
    strcpy(specfile, "spec");

  fprintf(stderr, "Checking specfile ...\n");
  scanspec(specfile, &n_runblocks, &any_warnings, &any_errors);
  /* "FITTER" ability currently not supported */
  use_targvecs = FALSE;
  fprintf(stderr, "\n... %s; %s\n\n",
    (any_warnings ? "note above warning(s)" : "no warnings"),
    (any_errors ? "note above error(s)" : "no errors"));

  if(check_only)
    exit((any_warnings || any_errors) ? 1 : 0);

  if(any_errors) {
    fprintf(stderr, "Will exit now because of above error(s) in \
specfile\n");
    exit(1);
  }

  if(any_warnings)
    fprintf(stderr, "Will run despite above warning(s) about \
specfile\n\n");

  /* Does initialization for making correct-vs.-rejected tables. */
  cvr_init();

  fprintf(stderr, "Will do %d run%s\n\n", n_runblocks,
    n_runblocks > 1 ? "s" : "");

  fp_specfile = fopen(specfile, "rb");
  for(irun = 1; irun <= n_runblocks; irun++) {

    fprintf(stderr, "\n Run %d of %d:\n\n", irun, n_runblocks);

    short_classnames = (char **)NULL;
    classes = (short *)NULL;
    targvecs = (float *)NULL;
    wsav = (float *)NULL;

    /* Read parm values from current block of specfile into parms
    structure. */
    got_blk(fp_specfile, &parms, &gb_any_error,
      &runblock_start_linenum);

    /* Set the network weights, either randomly or by reading a file.
    (These routines also allocate the weights buffer, w). */
    if((parms.train_or_test.val == TRAIN && parms.wts_infile.ssl.set)
      || parms.train_or_test.val == TEST)
      /* Reads network weights, and also various "network architecture"
      specifications, from file indicated in parms structure; returns
      weights, and loads arch. specs into parms structure. */
      readwts(&parms, &w);
    else /* TRAIN, and weights infile not set: get random initial
         weights. */
      /* randwts(parms.ninps.val, parms.nhids.val, parms.nouts.val,
      parms.seed.val, &w); */
      randwts_oldorder(parms.ninps.val, parms.nhids.val,
        parms.nouts.val, parms.seed.val, &w);

    /* TEMPORARY: */
    if(parms.purpose.val == FITTER)
      fatalerr("purpose FITTER (i.e. function-approximator) is not \
implemented yet", NULL, NULL);

    if(parms.train_or_test.val == TRAIN) {
      numwts = parms.nhids.val * (parms.ninps.val + 1) +
        parms.nouts.val * (parms.nhids.val + 1);
      if((wsav = (float *)malloc(numwts * sizeof(float))) ==
        (float *)NULL)
	syserr("main (mlp.c)", "malloc", "wsav");
      memcpy((char *)wsav, (char *)w, numwts * sizeof(float));
    }

    /* Copies the parm values from the parms structure into separate
    variables. */
    get_parms_vals(&parms, &long_outfile, &short_outfile,
      &patterns_infile, &wts_infile, &wts_outfile, &class_wts_infile,
      &pattern_wts_infile, &lcn_scn_infile, &npats, &ninps, &nhids,
      &nouts, &seed, &niter_max, &nfreq, &nokdel, &lbfgs_mem, &regfac,
      &alpha, &temperature, &egoal, &gwgoal, &errdel, &oklvl, &errfunc,
      &purpose, &boltzmann, &train_or_test, &acfunc_hids,
      &acfunc_outs, &priors, &patsfile_ascii_or_binary,
      &do_confuse, &show_acs_times_1000, &do_cvr, &trgoff,
      &scg_earlystop_pct, &lbfgs_gtol);

    /* Reads the patterns file.  Stores the feature vectors.  If
    CLASSIFIER, stores classes, even if patterns file contains target
    vectors; otherwise (FITTER), stores target vectors. */
    getpat(patterns_infile, patsfile_ascii_or_binary, npats, ninps,
      nouts, purpose, trgoff, &long_classnames, &featvecs, &targvecs,
      &classes, &mpats);

    if(priors == CLASS || priors == BOTH || do_confuse == TRUE)
      /* Gets the short class-names. */
      lng2shrt(nouts, long_classnames, lcn_scn_infile,
        &short_classnames);

    /* Sets the pattern-weights, in two versions */
    set_fpw(priors, class_wts_infile, nouts, short_classnames,
      pattern_wts_infile, npats, classes, &patwts);

    /* After the following call, any call of the form fsaso(str),
    by any routine, will write str both to stderr and to the
    short outfile. */
    fsaso_init(short_outfile);

    /* Reports the parm values. */
    rprt_prs(&parms, mpats);

    accum_init(nouts, do_confuse, oklvl);

    /* Do (and time) the current training or testing run. */

    secs = ups_secs();

    doity_long_outfile = parms.long_outfile.ssl.set;

    if(train_or_test == TRAIN) {

      /* Train (optimize) the network weights, using either SCG only,
      or using SCG for a while and then LBFGS. */
      optchk_store_unchanging_vals(npats, long_classnames,
        short_classnames, nouts, boltzmann, temperature, nfreq,
        errdel, nokdel, oklvl);
      scg_only = (boltzmann != NO_PRUNE);
      optwts(scg_only, scg_earlystop_pct, do_confuse,
        doity_long_outfile, long_outfile, show_acs_times_1000, do_cvr,
        niter_max, ninps, nhids, nouts, npats, featvecs,
        (purpose != CLASSIFIER), targvecs, classes,
        acsmaps_code_to_fn(acfunc_hids),
        acsmaps_code_to_fn(acfunc_outs), errfunc, alpha, patwts,
        regfac, boltzmann, temperature, nfreq, egoal, gwgoal,
        oklvl, long_classnames, short_classnames, purpose, lbfgs_gtol,
        lbfgs_mem, w, &rmserr, &gw, &iter, &ncalls, &ierr);
      endopt(iter, ncalls, ierr, rmserr, gw);
      for(i = 0; i < numwts; i++)
        wsav[i] -= w[i];
      dif = snrm2_(&numwts, wsav, &i1) / sqrt((double)numwts);
      sprintf(str, " Rms change in weights %6.3f\n", dif);
      fsaso(str);
    }

    else { /* train_or_test == TEST */

      fprintf(stderr, " Sending patterns through mlp...\n");
      e_and_g(FALSE, TRUE, do_confuse, doity_long_outfile,
        long_outfile, show_acs_times_1000, do_cvr, ninps, nhids,
        nouts, w, npats, featvecs, use_targvecs, targvecs, classes,
        acsmaps_code_to_fn(acfunc_hids),
        acsmaps_code_to_fn(acfunc_outs), errfunc, alpha, patwts,
        regfac, oklvl, &err, (float *)NULL, &e1, &e2);
      fprintf(stderr, " ...done\n");

      accum_print(do_confuse, purpose, npats, 0, err, e1, e2, ' ', w,
        long_classnames, short_classnames, &junkint0, &junkint1,
        &junkfloat);

      if(do_cvr)
	cvr_print(TEST, npats);

    }

    secs = ups_secs() - secs;
    hms = s2hms(secs);
    sprintf(str, "\n User+system time used:  %.1f (s)\
  %s (h:m:s)\n", secs, hms);
    fsaso(str);

    accum_free();

    if(train_or_test == TRAIN) {
      /* Write final network weights. */
      putwts(wts_outfile, w, purpose, ninps, nhids, nouts, acfunc_hids,
        acfunc_outs);
      sprintf(str, " Wrote weights as file %s\n", wts_outfile);
      fsaso(str);
    }

    for(i = 0; i < nouts; i++) {
      free((char *)(long_classnames[i]));
      if(short_classnames != (char **)NULL)
	free((char *)(short_classnames[i]));
    }
    free((char *)long_classnames);
    free_notnull((char *)short_classnames);
    free((char *)featvecs);
    free_notnull((char *)classes);
    free_notnull((char *)targvecs);
    free((char *)patwts);
    free((char *)w);
    free_notnull((char *)wsav);

  } /* (loop over the runs) */

  fclose(fp_specfile);
  fprintf(stderr, "\nDid %d run%s\n\n", n_runblocks,
    n_runblocks > 1 ? "s" : "");
  exit(0);
}

/*******************************************************************/

/* parse_cmd_line: Parses, using getopt, the command line.

The usage is:

  mlp [-c] [<specfile>]

where:
  -c means: do checking of specfile only, i.e. do not run mlp even if
     specfile has no errors.
  <specfile> is the filename of the spec (specification) file; if
     this arg is abset, specfile is set to file "spec" in current dir.

If the optional specfile name is present, it must be the last arg on
the command line; and if that filename begins with a -, the arg must
be preceded by the special arg --.

Usage examples:

  mlp
    (Runs, using specfile "spec".)

  mlp ../foo
    (Runs, using specfile "../foo".)

  mlp -c
    (Checks specfile only; specfile is "spec".)

  mlp -c ../foo
    (Checks specfile only; specfile is "../foo".)


Input args:   
  argc, argv: The arg-count and arg-vector args of the "main"
    function.

Output args:   
  check_only: TRUE (FALSE) if command line did (did not) set the "-c"
    option, which means that mlp should only check the specfile.  
  specfile: Spec (specification) file name; this buffer must be
    provided already allocated (to at least a reasonable size) by
    caller.
*/

void
parse_cmd_line(argc, argv, check_only, specfile)
int argc;
char **argv, *check_only, specfile[];
{
  char str[300];
  static char usage_str[] = "Usage: mlp [-c] [<specfile>]";
  int c;
  extern char *optarg;
  extern int optind, opterr;

  opterr = 0; /* turn off getopt's writing of error messages */

  *check_only = FALSE;

  while((c = getopt(argc, argv, "c")) != -1)
    switch(c) {

    case 'c':
      *check_only = TRUE;
      break;

    case '?':
      fatalerr(usage_str, NULL, NULL);
      break;
    }

  /* After any options and their args, there should be either nothing,
  or a specfile name. */
  if(optind == argc)
    /* Nothing, i.e. no specfile name; set name to empty string. */
    strcpy(specfile, "");

  else if(optind == argc - 1)
    /* One arg: specfile name */
    strcpy(specfile, argv[optind]);

  else
    /* Two or more args: not allowed */
    fatalerr(usage_str, NULL, NULL);

}

/*******************************************************************/

/* get_parms_vals: Gets just the "val" members of the parm-specific
members of the PARMS structure.  Putting all this mess down here
allows the main routine to be cleaner (few member references).

(Obvious) WARNING: If the PARMS structure typedef (in parms.h) is
changed, then this routine has to be changed accordingly. */

void
get_parms_vals(parms, long_outfile, short_outfile, patterns_infile,
  wts_infile, wts_outfile, class_wts_infile, pattern_wts_infile,
  lcn_scn_infile, npats, ninps, nhids, nouts, seed, niter_max, nfreq,
  nokdel, lbfgs_mem, regfac, alpha, temperature, egoal, gwgoal,
  errdel, oklvl, errfunc, purpose, boltzmann, train_or_test,
  acfunc_hids, acfunc_outs, priors, patsfile_ascii_or_binary,
  do_confuse, show_acs_times_1000, do_cvr, trgoff, scg_earlystop_pct,
  lbfgs_gtol)
PARMS *parms;
char **long_outfile, **short_outfile, **patterns_infile, **wts_infile,
  **wts_outfile, **class_wts_infile, **pattern_wts_infile,
  **lcn_scn_infile, *errfunc, *purpose, *boltzmann,
  *train_or_test, *acfunc_hids, *acfunc_outs, *priors,
  *patsfile_ascii_or_binary, *do_confuse, *show_acs_times_1000,
  *do_cvr;
int *npats, *ninps, *nhids, *nouts, *seed, *niter_max, *nfreq, *nokdel,
  *lbfgs_mem;
float *regfac, *alpha, *temperature, *egoal, *gwgoal, *errdel, *oklvl,
  *trgoff, *scg_earlystop_pct, *lbfgs_gtol;
{
  void get_a_parms_val();

  get_a_parms_val(PARMTYPE_FILENAME, (char *)(&(parms->long_outfile)),
    (char *)long_outfile);
  get_a_parms_val(PARMTYPE_FILENAME, (char *)(&(parms->short_outfile)),
    (char *)short_outfile);
  get_a_parms_val(PARMTYPE_FILENAME,
    (char *)(&(parms->patterns_infile)), (char *)patterns_infile);
  get_a_parms_val(PARMTYPE_FILENAME, (char *)(&(parms->wts_infile)),
    (char *)wts_infile);
  get_a_parms_val(PARMTYPE_FILENAME, (char *)(&(parms->wts_outfile)),
    (char *)wts_outfile);
  get_a_parms_val(PARMTYPE_FILENAME,
    (char *)(&(parms->class_wts_infile)),
    (char *)class_wts_infile);
  get_a_parms_val(PARMTYPE_FILENAME,
    (char *)(&(parms->pattern_wts_infile)),
    (char *)pattern_wts_infile);
  get_a_parms_val(PARMTYPE_FILENAME,
    (char *)(&(parms->lcn_scn_infile)), (char *)lcn_scn_infile);
  get_a_parms_val(PARMTYPE_INT, (char *)(&(parms->npats)),
    (char *)npats);
  get_a_parms_val(PARMTYPE_INT, (char *)(&(parms->ninps)),
    (char *)ninps);
  get_a_parms_val(PARMTYPE_INT, (char *)(&(parms->nhids)),
    (char *)nhids);
  get_a_parms_val(PARMTYPE_INT, (char *)(&(parms->nouts)),
    (char *)nouts);
  get_a_parms_val(PARMTYPE_INT, (char *)(&(parms->seed)),
    (char *)seed);
  get_a_parms_val(PARMTYPE_INT, (char *)(&(parms->niter_max)),
    (char *)niter_max);
  get_a_parms_val(PARMTYPE_INT, (char *)(&(parms->nfreq)),
    (char *)nfreq);
  get_a_parms_val(PARMTYPE_INT, (char *)(&(parms->nokdel)),
    (char *)nokdel);
  get_a_parms_val(PARMTYPE_INT, (char *)(&(parms->lbfgs_mem)),
    (char *)lbfgs_mem);
  get_a_parms_val(PARMTYPE_FLOAT, (char *)(&(parms->regfac)),
    (char *)regfac);
  get_a_parms_val(PARMTYPE_FLOAT, (char *)(&(parms->alpha)),
    (char *)alpha);
  get_a_parms_val(PARMTYPE_FLOAT, (char *)(&(parms->temperature)),
    (char *)temperature);
  get_a_parms_val(PARMTYPE_FLOAT, (char *)(&(parms->egoal)),
    (char *)egoal);
  get_a_parms_val(PARMTYPE_FLOAT, (char *)(&(parms->gwgoal)),
    (char *)gwgoal);
  get_a_parms_val(PARMTYPE_FLOAT, (char *)(&(parms->errdel)),
    (char *)errdel);
  get_a_parms_val(PARMTYPE_FLOAT, (char *)(&(parms->oklvl)),
    (char *)oklvl);
  get_a_parms_val(PARMTYPE_SWITCH, (char *)(&(parms->errfunc)),
    (char *)errfunc);
  get_a_parms_val(PARMTYPE_SWITCH, (char *)(&(parms->purpose)),
    (char *)purpose);
  get_a_parms_val(PARMTYPE_SWITCH,
    (char *)(&(parms->boltzmann)),
    (char *)boltzmann);
  get_a_parms_val(PARMTYPE_SWITCH, (char *)(&(parms->train_or_test)),
    (char *)train_or_test);
  get_a_parms_val(PARMTYPE_SWITCH, (char *)(&(parms->acfunc_hids)),
    (char *)acfunc_hids);
  get_a_parms_val(PARMTYPE_SWITCH, (char *)(&(parms->acfunc_outs)),
    (char *)acfunc_outs);
  get_a_parms_val(PARMTYPE_SWITCH, (char *)(&(parms->priors)),
    (char *)priors);
  get_a_parms_val(PARMTYPE_SWITCH,
    (char *)(&(parms->patsfile_ascii_or_binary)),
    (char *)patsfile_ascii_or_binary);
  get_a_parms_val(PARMTYPE_SWITCH, (char *)(&(parms->do_confuse)),
    (char *)do_confuse);
  get_a_parms_val(PARMTYPE_SWITCH,
    (char *)(&(parms->show_acs_times_1000)),
    (char *)show_acs_times_1000);
  get_a_parms_val(PARMTYPE_SWITCH, (char *)(&(parms->do_cvr)),
    (char *)do_cvr);
  get_a_parms_val(PARMTYPE_FLOAT, (char *)(&(parms->trgoff)),
    (char *)trgoff);
  get_a_parms_val(PARMTYPE_FLOAT,
    (char *)(&(parms->scg_earlystop_pct)), (char *)scg_earlystop_pct);
  get_a_parms_val(PARMTYPE_FLOAT, (char *)(&(parms->lbfgs_gtol)),
    (char *)lbfgs_gtol);
}

/*******************************************************************/

#define MYNULL 0

/* get_a_parms_val: For use by get_parms_vals.  Copies a parm out; but
if the parm is not set, the output gets loaded with MYNULL. */

void
get_a_parms_val(parmtype, member, val)
char parmtype, *member, *val;
{
  char str[100];

  switch(parmtype) {

  case PARMTYPE_FILENAME:
    *((char **)val) = ((((PARM_FILENAME *)member)->ssl).set ?
      ((PARM_FILENAME *)member)->val : (char *)MYNULL);
    break;

  case PARMTYPE_INT:
    *((int *)val) = ((((PARM_INT *)member)->ssl).set ?
      ((PARM_INT *)member)->val : (int)MYNULL);
    break;

  case PARMTYPE_FLOAT:
    *((float *)val) = ((((PARM_FLOAT *)member)->ssl).set ?
      ((PARM_FLOAT *)member)->val : (float)MYNULL);
    break;

  case PARMTYPE_SWITCH:
    *val = ((((PARM_SWITCH *)member)->ssl).set ?
      ((PARM_SWITCH *)member)->val : (char)MYNULL);
    break;

  default:
    sprintf(str, "parmtype must be PARMTYPE_FILENAME (%d), \
PARMTYPE_INT (%d), PARMTYPE_FLOAT (%d), or PARMTYPE_SWITCH \
(%d); it is %d", (int)PARMTYPE_FILENAME, (int)PARMTYPE_INT,
      (int)PARMTYPE_FLOAT, (int)PARMTYPE_SWITCH, (int)parmtype);
    fatalerr("get_a_parms_val (mlp.c)", str, MYNULL);
    break;

  }
}

/*******************************************************************/
