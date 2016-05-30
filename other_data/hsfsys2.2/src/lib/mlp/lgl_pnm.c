/*
# proc: lgl_pnm - Finds out whether a word is one of the legal parameter-names.

Input arg:
  word: The word to be tested to find out if it is a legal parm-name.

Return value:
  TRUE (FALSE): The input word is (is not) a legal parm-name.
*/

#include <string.h>
#include <mlp/defs.h>

char
lgl_pnm(word)
char word[];
{
  /* (Not a very efficient way to check whther a string matches one
  of a set of stored strings, but probably fast enough for this
  application.) */
  if(!strcmp(word, "errfunc"))
    return TRUE;
  if(!strcmp(word, "purpose"))
    return TRUE;
  if(!strcmp(word, "boltzmann"))
    return TRUE;
  if(!strcmp(word, "train_or_test"))
    return TRUE;
  if(!strcmp(word, "acfunc_hids"))
    return TRUE;
  if(!strcmp(word, "acfunc_outs"))
    return TRUE;
  if(!strcmp(word, "priors"))
    return TRUE;
  if(!strcmp(word, "long_outfile"))
    return TRUE;
  if(!strcmp(word, "short_outfile"))
    return TRUE;
  if(!strcmp(word, "patterns_infile"))
    return TRUE;
  if(!strcmp(word, "wts_infile"))
    return TRUE;
  if(!strcmp(word, "wts_outfile"))
    return TRUE;
  if(!strcmp(word, "class_wts_infile"))
    return TRUE;
  if(!strcmp(word, "pattern_wts_infile"))
    return TRUE;
  if(!strcmp(word, "lcn_scn_infile"))
    return TRUE;
  if(!strcmp(word, "npats"))
    return TRUE;
  if(!strcmp(word, "ninps"))
    return TRUE;
  if(!strcmp(word, "nhids"))
    return TRUE;
  if(!strcmp(word, "nouts"))
    return TRUE;
  if(!strcmp(word, "seed"))
    return TRUE;
  if(!strcmp(word, "niter_max"))
    return TRUE;
  if(!strcmp(word, "nfreq"))
    return TRUE;
  if(!strcmp(word, "nokdel"))
    return TRUE;
  if(!strcmp(word, "regfac"))
    return TRUE;
  if(!strcmp(word, "alpha"))
    return TRUE;
  if(!strcmp(word, "temperature"))
    return TRUE;
  if(!strcmp(word, "egoal"))
    return TRUE;
  if(!strcmp(word, "gwgoal"))
    return TRUE;
  if(!strcmp(word, "errdel"))
    return TRUE;
  if(!strcmp(word, "oklvl"))
    return TRUE;
  if(!strcmp(word, "patsfile_ascii_or_binary"))
    return TRUE;
  if(!strcmp(word, "trgoff"))
    return TRUE;
  if(!strcmp(word, "lbfgs_mem"))
    return TRUE;
  if(!strcmp(word, "scg_earlystop_pct"))
    return TRUE;
  if(!strcmp(word, "lbfgs_gtol"))
    return TRUE;
  if(!strcmp(word, "do_confuse"))
    return TRUE;
  if(!strcmp(word, "show_acs_times_1000"))
    return TRUE;
  if(!strcmp(word, "do_cvr"))
    return TRUE;

  return FALSE;
}
