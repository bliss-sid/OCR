/*
# proc: scanspec - Makes a scan through the whole specfile.  Counts the "run
# proc:            blocks" and reports the count.  For each run block, checks
# proc:            the parm settings, and reports to stderr both illegal
# proc:            situations and situations which are worthy of warnings.
Some errors or
warnable situations may go unreported on first scan, if there are
multiple kinds of errors (e.g. a parm is set to an illegal value, and
is also set again later), but repeated fixing of the specfile and
re-running should turn up all errors and warnable situations, so that
eventually the specfile can be gotten into a correct state.

Input arg:
  specfile: The specification file, containing the name-value pairs
    for one or more runs, with each run-block separated from the next
    one by "newrun" or "NEWRUN", and with C-style comments allowed.

Output args:
  n_runblocks: How many run-blocks the specfile contains.
  any_warnings: TRUE (FALSE) if specfile contains (does not contain)
    any warnable situation (e.g. setting of a superfluous parm) in
    any run-block.
  any_errors: TRUE (FALSE) if specfile contains (does not contain)
    any error situation (e.g. omission of a mandotory parm) in any
    run-block.
*/

#include <stdio.h>
#include <string.h>
#include <mlp/defs.h>
#include <mlp/parms.h>

void
scanspec(specfile, n_runblocks, any_warnings, any_errors)
char specfile[], *any_warnings, *any_errors;
int *n_runblocks;
{
  FILE *fp;
  char *barf, heading_str[100], gb_any_errors, cb_any_warnings,
    cb_any_errors;
  int i, runblock_start_linenum;
  static PARMS parms;
  char *eb_get();

  if((fp = fopen(specfile, "rb")) == (FILE *)NULL)
    syserr("scanspec", "fopen for reading failed", specfile);
  for(*n_runblocks = 0, *any_warnings = *any_errors = FALSE; ;
    (*n_runblocks)++) {

    /* Read next run-block of parameter settings from specfile into
    a structure, doing some checking. */
    if(!got_blk(fp, &parms, &gb_any_errors, &runblock_start_linenum)) {
      fclose(fp);
      return;
    }
    if(gb_any_errors)
      *any_errors = TRUE;

    /* Do additional checking of the parm settings. */
    ch_bprms(&parms, &cb_any_warnings, &cb_any_errors);
    if(cb_any_warnings)
      *any_warnings = TRUE;
    if(cb_any_errors)
      *any_errors = TRUE;

    /* If any errors or warnings occurred in this run-block, write
    their error messages under a heading. */
    if(gb_any_errors || cb_any_warnings || cb_any_errors) {
      if(parms.train_or_test.ssl.set)
	barf = ((parms.train_or_test.val == TRAIN) ? "Training run" :
          "Testing run");
      else
	barf = "Run";
      sprintf(heading_str, "%s starting at line %d:", barf,
        runblock_start_linenum);
      fprintf(stderr, "\n  %s\n  ", heading_str);
      for(i = strlen(heading_str); i; i--)
	fprintf(stderr, "-");
      fprintf(stderr, "\n");
      fputs(eb_get(), stderr);
      eb_clr();
    }
  }
}
