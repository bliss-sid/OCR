/*
# proc: got_nc_c - Gets the next non-comment character of a specfile, and
# proc:            also tells its line-number.
The comment delimiters are those of the C
language, i.e. slash asterisk and asterisk slash.

Input arg:
  fp: FILE pointer.

Output args:
  thechar: Next non-comment character found.
  linenum: Number (starting at 1) of the line that contained the
    character.

Return value:
  TRUE: Got a non-comment character.
  FALSE: Reached end-of-file without encountering another non-comment
    character.
*/

#include <stdio.h>
#include <mlp/defs.h>

#define OUT        0
#define ALMOST_IN  1
#define IN         2
#define ALMOST_OUT 3

char
got_nc_c(fp, thechar, linenum)
FILE *fp;
char *thechar;
int *linenum;
{
  static char c;
  static int state = OUT, barf = FALSE, ln, ln_aislash;
  char got_c();

  if(barf) {
    barf = FALSE;
    *thechar = c;
    *linenum = ln;
    return TRUE;
  }
  while(1) {
    if(!got_c(fp, &c, &ln)) {
      state = OUT;
      barf = FALSE;      
      return FALSE;
    }
    if(state == OUT) {
      if(c == '/') {
	state = ALMOST_IN;
	ln_aislash = ln;
      }
      else {
	*thechar = c;
	*linenum = ln;
	return TRUE;
      }
    }
    else if(state == ALMOST_IN) {
      if(c == '*')
	state = IN;
      else {
	*thechar = '/';
	*linenum = ln_aislash;
	if(c != '/') {
	  state = OUT;
	  barf = TRUE;
	}
	return TRUE;
      }
    }
    else if(state == IN) {
      if(c == '*')
	state = ALMOST_OUT;
    }
    else { /* state == ALMOST_OUT */
      if(c == '/')
	state = OUT;
      else if(c != '*')
	state = IN;
    }
  }
}
