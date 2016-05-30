/* For setting, clearing, and getting the values of flags that record
the existence of (at least one) warning or error.

Contains:

# proc: is_w_set - sets the warning flag.
# proc: is_w_clr - clears the warning flag.
# proc: is_w_get - gets the values of the warning flag.
# proc: is_e_set - sets the error flag.
# proc: is_e_clr - clears the error flag.
# proc: is_e_get - gets the value of the error flag.
*/

#include <mlp/defs.h>

static char is_w = FALSE, is_e = FALSE;

/********************************************************************/

void
is_w_set()
{
  is_w = TRUE;
}

/********************************************************************/

void
is_w_clr()
{
  is_w = FALSE;
}

/********************************************************************/

char
is_w_get()
{
  return is_w;
}

/********************************************************************/

void
is_e_set()
{
  is_e = TRUE;
}

/********************************************************************/

void
is_e_clr()
{
  is_e = FALSE;
}

/********************************************************************/
  
char
is_e_get()
{
  return is_e;
}

/********************************************************************/
