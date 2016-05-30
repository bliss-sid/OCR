/* Trivial functions that map between different representations of
activation functions.  These functions must have code added when a new
activation function is implemented.  Contains:

# proc: acsmaps_code_to_fn - Code char (e.g. SINUSOID) to function (in C
# proc:                      sense) (e.g. ac_sinusoid).
# proc: acsmaps_code_to_str - Code char (e.g. SINUSOID) to string (e.g.
# proc:                       "sinusoid").
# proc: acsmaps_str_to_code - String to code char.
*/

#include <stdio.h>
#include <mlp/parms.h>

/*******************************************************************/

/* acsmaps_code_to_fn: Maps each activation function code char to the
function (in the C, not mathemetical, sense) that implements it.

Input arg:
  code: The code value of an activation function type.

Return value: The function (actually, pointer to void-returning
  function) that implements this activation function type, i.e. that
  computes the function and its derivative.
*/

void
(*acsmaps_code_to_fn(code))()
char code;
{
  char str[50];
  void ac_sinusoid(), ac_sigmoid(), ac_linear();

  switch(code) {

  case SINUSOID: return ac_sinusoid; break;
  case SIGMOID:  return ac_sigmoid;  break;
  case LINEAR:   return ac_linear;   break;

  default:
    sprintf(str, "unsupported code value %d", (int)code);
    fatalerr("acsmaps_code_to_fn (acsmaps.c)", str, NULL);
    break;
  }
}

/*******************************************************************/

/* acsmaps_code_to_str: Maps each activation function code char to the
corresponding string.

Input arg:
  code: The code value of an activation function type.

Return value: Corresonding string.  CAUTION: return value is the
  address of a static buffer.
*/

char *
acsmaps_code_to_str(code)
char code;
{
  static char str[50];

  switch(code) {

  case SINUSOID: strcpy(str, "sinusoid"); break;
  case SIGMOID:  strcpy(str, "sigmoid");  break;
  case LINEAR:   strcpy(str, "linear");   break;

  default:
    sprintf(str, "unsupported code value %d", (int)code);
    fatalerr("acsmaps_code_to_str (acsmaps.c)", str, NULL);
    break;
  }

  return str;
}

/*******************************************************************/

/* acsmaps_str_to_code: Maps each activation function string to the
corresponding code char.

Input arg:
  str: The string representing an activation function type.

Return value: Corresonding code char; BAD_AC_CODE if str is not
  one that this routine knows about.
*/

char
acsmaps_str_to_code(str)
char str[];
{
  if(!strcmp(str, "sinusoid"))
    return SINUSOID;
  else if(!strcmp(str, "sigmoid"))
    return SIGMOID;
  else if(!strcmp(str, "linear"))
    return LINEAR;

  else
    return BAD_AC_CODE;
}

/*******************************************************************/
