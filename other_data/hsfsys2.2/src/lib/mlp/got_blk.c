/*
# proc: got_blk - Scans enough of the specfile to get the next run-block (if
# proc:           there is one), filling the parms struture with the scanned and
# proc:           converted values and setting its setting-indicators.

Input arg:
  fp: FILE pointer of specfile.

Output args:
  parms: A PARMS structure.  The "val" members of its (parm-specific)
    members are set according to info scanned from the specfile; if
    yow is TRUE, the "set" members are also set, showing which parms
    have been set in the specfile and which have been left unset.
  gb_any_error: Comes back TRUE (FALSE) if any error situations
    were (were not) detected.
  runblock_start_linenum: The number of the first line of the block,
    defined as the first line used by an unignorable phrase of the
    block.

Return value:
  TRUE: Another run-block was scanned in.
  FALSE: The specfile is exhausted, i.e. no more run-blocks are left.
*/

#include <stdio.h>
#include <mlp/defs.h>
#include <mlp/get_phr.h>
#include <mlp/parms.h>

int
got_blk(fp, parms, gb_any_error, runblock_start_linenum)
FILE *fp;
PARMS *parms;
char *gb_any_error;
int *runblock_start_linenum;
{
  char namestr[200], valstr[200], illegal_phrase[500], errstr[500],
    errstr2[500], some_content = FALSE, ret;
  int linenum;
  char st_nv_ok();

  *gb_any_error = FALSE;
  memset(parms, 0, sizeof(PARMS));
  while(1) {
    ret = get_phr(fp, namestr, valstr, illegal_phrase, &linenum);
    if(ret == WORD_PAIR) {
      if(!some_content)
	*runblock_start_linenum = linenum;
      some_content = TRUE;
      if(!st_nv_ok(namestr, valstr, linenum, parms, errstr)) {
	eb_cat(errstr);
	*gb_any_error = TRUE;
      }
    }
    else if(ret == NEWRUN) {
      if(some_content)
	return TRUE;
      else
	continue;
    }
    else if(ret == ILLEGAL_PHRASE) {
      if(!some_content)
	*runblock_start_linenum = linenum;
      some_content = TRUE;
      sprintf(errstr2, "ERROR, line %d: illegal phrase %s", linenum,
        illegal_phrase);
      strm_fmt(errstr2, errstr);
      eb_cat(errstr);
      *gb_any_error = TRUE;
    }
    else /* ret == FINISHED */
      return some_content;
  }
}
