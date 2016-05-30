/*
# proc: got_c - Gets the next character of a file, also telling the number
# proc:         of the line it was on.

Input arg:
  fp: FILE pointer.

Output args:
  thechar: The next character.
  linenum: Line number (starting at 1) of the character.

Return value:
  TRUE: Got a character.
  FALSE: Reached end-of-file.  The character returned in the preceding
    call was the last one in the file.
*/

#include <stdio.h>
#include <string.h>
#include <mlp/defs.h>

char
got_c(fp, thechar, linenum)
FILE *fp;
char *thechar;
int *linenum;
{
  static char line[500], *p;
  static int num_buffered = 0, ln = 0;

  if(num_buffered == 0) {
    if(!fgets(line, 500, fp)) {
      num_buffered = ln = 0;
      return FALSE;
    }
    ln++;
    p = line;
    num_buffered = strlen(line);
  }
  *thechar = *p++;
  num_buffered--;
  *linenum = ln;
  return TRUE;
}
