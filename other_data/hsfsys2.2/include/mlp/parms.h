/* Stuff having to do with reading parms from a specfile. */


#define PARMTYPE_FILENAME ((char)0)
#define PARMTYPE_INT      ((char)1)
#define PARMTYPE_FLOAT    ((char)2)
#define PARMTYPE_SWITCH   ((char)3)

#define PARM_FILENAME_VAL_DIM 100

typedef struct {
  char set_tried, set;
  int linenum;
} SSL;

typedef struct {
  char val[PARM_FILENAME_VAL_DIM];
  SSL ssl;
} PARM_FILENAME;

typedef struct {
  int val;
  SSL ssl;
} PARM_INT;

typedef struct {
  float val;
  SSL ssl;
} PARM_FLOAT;

typedef struct {
  char val;
  SSL ssl;
} PARM_SWITCH;

typedef struct {
  PARM_FILENAME long_outfile, short_outfile, patterns_infile,
    wts_infile, wts_outfile, class_wts_infile, pattern_wts_infile,
    lcn_scn_infile;
  PARM_INT npats, ninps, nhids, nouts, seed, niter_max, nfreq, nokdel,
    lbfgs_mem;
  PARM_FLOAT regfac, alpha, temperature, egoal, gwgoal, errdel, oklvl,
    trgoff, scg_earlystop_pct, lbfgs_gtol;
  PARM_SWITCH errfunc, purpose, boltzmann, train_or_test, acfunc_hids,
    acfunc_outs, priors, patsfile_ascii_or_binary, do_confuse,
    show_acs_times_1000, do_cvr;
} PARMS;


/* Symbolic names of values of "switch" parms.  The corresponding
value strings (expected in the spec file) are these names but in lower
case; the numerical values are also ok in the spec file.  For example,
to set errfunc to MSE, use either of the following in the spec file:
  errfunc mse
  errfunc 0
Note that the names and corresponding code-numbers here must match the
contents of the legal_names_codes_str parms in the calls of mtch_pnm()
by st_nv_ok(), but with the names in lower case in those calls. */

/* For errfunc: */
#define MSE     ((char)0)
#define TYPE_1  ((char)1)
#define POS_SUM ((char)2)

/* For purpose: */
#define CLASSIFIER ((char)0)
#define FITTER     ((char)1)

/* For boltzmann: */
#define NO_PRUNE     ((char)0)
#define ABS_PRUNE    ((char)2)
#define SQUARE_PRUNE ((char)3)

/* For train_or_test: */
#define TRAIN ((char)0)
#define TEST  ((char)1)

/* For acfunc_hids and acfunc_outs: */
#define SINUSOID    ((char)0)
#define SIGMOID     ((char)1)
#define LINEAR      ((char)2)
#define BAD_AC_CODE ((char)127)

/* For priors: */
#define ALLSAME ((char)0)
#define CLASS   ((char)1)
#define PATTERN ((char)2)
#define BOTH    ((char)3)

/* For patsfile_ascii_or_binary: */
#define ASCII  ((char)0)
#define BINARY ((char)1)

/* The allowed values for the following "logical" switch parms are
TRUE and FALSE (defined in defs.h), which should be represented in the
spec file as true and false: do_confuse, show_acts_times_1000,
do_cvr. */
