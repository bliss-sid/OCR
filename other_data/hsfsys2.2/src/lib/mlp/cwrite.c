/*
# proc: cwrite - Non-fortrany version of the original C program
(arg iter is now an int, not an int pointer).  Also, it now writes to stderr,
not stdout. */

#include <stdio.h>

/* cwrite_ 27-May-92 09:06
 *      write a line with an integer, a CR, but no NL
 */

void
cwrite(iter)
int iter;
{
  if(isatty(0)) {
    fprintf(stderr, "%4d\r", iter);
    fflush(stderr);
  }
}
