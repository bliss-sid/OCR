/* Routines for using a (hidden) error buffer, which holds a sequence
of error messages (or they could be any strings).

Contains:
# proc: eb_cat - cats a (hidden) error buffer, which holds a sequence
# proc:          of error messages
# proc: eb_get - gets a (hidden) error buffer, which holds a sequence
# proc:          of error messages
# proc: eb_clr - clears a (hidden) error buffer, which holds a sequence
# proc:          of error messages
*/

#include <stdio.h>
#include <stdlib.h>
#include <mlp/defs.h>

/* How many extra bytes to allocate -- beyond the amount that is going
to be just barely sufficient -- at the initial malloc and at each
realloc: */
#define NBYTES_EXTRA 500

static char *errbuf;
static int nbytes_used;

/********************************************************************/

/* Catenates a string onto the error buffer, after checking whether
buffer is going to be large enough and if not, enlarging it. */

void
eb_cat(str)
char str[];
{
  static char first = TRUE;
  static int nbytes_allocated;

  if(first) {
    first = FALSE;
    nbytes_used = strlen(str) + 1;
    nbytes_allocated = nbytes_used + NBYTES_EXTRA;
    if((errbuf = malloc(nbytes_allocated)) == (char *)NULL)
      syserr("eb_cat (eb.c)", "malloc", "errbuf");
    strcpy(errbuf, str);
  }
  else {
    nbytes_used += strlen(str);
    if(nbytes_used > nbytes_allocated) {
      nbytes_allocated = nbytes_used + NBYTES_EXTRA;
      if((errbuf = realloc(errbuf, nbytes_allocated)) == (char *)NULL)
	syserr("eb_cat (eb.c)", "realloc", "errbuf");
    }
    strcat(errbuf, str);
  }
}

/********************************************************************/

/* Returns the error buffer. */

char *
eb_get()
{
  return errbuf;
}

/********************************************************************/

/* Clears the error buffer, in effect, so that it can be filled with a
new sequence of strings.  (Does not free the buffer.) */

void
eb_clr()
{
  *errbuf = (char)NULL;
  nbytes_used = 1;
}
  
/********************************************************************/
