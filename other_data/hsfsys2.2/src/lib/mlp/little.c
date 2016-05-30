/* Some little utility routines:

# proc: display_message - writes new value of a stationary message
# proc: exists          - finds out whether a file exists
# proc: fatal           - fatal(str) does fatalerr(str, NULL, NULL)
# proc: free_notnull    - frees pointer but only if it is not NULL
# proc: intcomp         - integer comparison function
# proc: lastcomp        - finds last component of a pathname
# proc: round           - rounds a float to an int
# proc: s2hms           - converts seconds to hours:minutes:seconds string
# proc: tilde_filename  - changes ~/string to home-dir/string
# proc: ups_secs        - user plus system seconds used by process
*/

/* [Perhaps some of these include files are not needed any more,
since this little.c has had many routines removed:] */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <mlp/lims.h>
#include <mlp/defs.h>
#include <mlp/little.h>

/*******************************************************************/

/* Writes a message string to stdout, after writing enough backspaces
to erase the previous message.  The intended use is for progress
messages; the messages will appear as a display that remains at a
fixed position.  Message should NOT end with a newline.  If the
calling program wants to do later writing that is to appear below the
messages display, it should printf a newline first, because otherwise
the new string will appear appended to the right end of the last
message. */

void
display_message(message)
char message[];
{
  int i;
  static int prev_message_len = 0;

  for(i = 0; i < prev_message_len; i++)
    printf("\b");
  fputs(message, stdout);
  fflush(stdout);
  prev_message_len = strlen(message);
}

/*******************************************************************/

/* Returns 1 if given file exists, 0 otherwise.  (Presumes that the
reason for a "stat" failure is nonexistence of the file.  Actually, it
is possible for stat to fail for an existing file because this process
lacks execute permission for one or more of the directories in the
path of the file.) */

int
exists(filename)
char *filename;
{
  struct stat strstat;

  return (stat(filename, &strstat) != -1);
}

/*******************************************************************/

/* Saves some typing for a commonly-used situation. */

void
fatal(str)
char *str;
{
  fatalerr(str, NULL, NULL);
}

/*******************************************************************/

/* Frees a pointer, but only if it is not (char *)NULL. */

void
free_notnull(p)
char *p;
{
  if(p != (char *)NULL)
    free(p);
}

/********************************************************************/

/* Integer comparison function. */

int
intcomp(f, s)
int *f, *s;
{
  if(*f < *s)
    return -1;
  if(*f > *s)
    return 1;
  return 0;
}

/*******************************************************************/

/* Finds last component of a pathname. */

char *
lastcomp(path)
char *path;
{
  static char value[200];
  char *p;

  p = strrchr(path, '/');
  if(p)
    strcpy(value, p + 1);
  else
    strcpy(value, path);
  return value;
}

/*******************************************************************/

/* Rounds a float to an int. */

int
round(x)
float x;
{
  return (int)(x >= 0. ? x + .5 : x - .5);
}

/*******************************************************************/

/* Converts a float number of seconds to a string of the form
<hours>:<minutes>:<seconds>, with <seconds> rounded to the nearest
tenth.  CAUTION: Returned function value is the address of a static
buffer, whose contents are overwritten each time the function is
called. */

char *
s2hms(s)
float s;
{
  static char buf[100];
  int h, m;

  s = (int)(10. * s + .5) / 10.;
  h = (int)(s / 3600.);
  s -= 3600 * h;
  m = (int)(s / 60.);
  s -= 60 * m;
  sprintf(buf, "%d:%02d:%04.1f", h, m, s);
  return buf;
}

/******************************************************************/

/* If input string begins with ~/, this expands the ~ to the user's
home dir; otherwise, it returns the input unchanged.  CAUTION: Returns
the address of a static buffer; so, if you call it multiple times with
the intention of storing the several resulting strings, you must copy
the strings into other buffers, not just store the returned addresses
(which will all be the same). */

char *
tilde_filename(str)
char str[];
{
  char *p;
  static char first = TRUE, homedir[200], outstr[200];

  if(first) {
    first = FALSE;
    if(!(p = getenv("HOME")))
      syserr("tilde_filename", "getenv", "HOME");
    strcpy(homedir, p);
  }
  if(str[0] == '~' && str[1] == '/')
    sprintf(outstr, "%s/%s", homedir, str + 2);
  else
    strcpy(outstr, str);
  return outstr;
}

/******************************************************************/

/* Returns the sum of the user time and system time used so far by the
process in which it is called, in seconds.  (Does not count time used
by child processes, if any.)

NOTE.  It is possible that this routine is not totally portable: there
may be some systems on which CLK_TCK is not defined in limits.h and
yet the unit in which times() reports its results is not the
"standard" 1/60 second. */

float
ups_secs()
{
  struct tms yow;

#ifdef CLK_TCK /* (may be defined in limits.h) */
/* Assume that on this system, the values set by times() are in units
of 1/CLK_TCK second: */
#define UPS_SECS_FAC (1./(float)CLK_TCK)
#else
/* Assume that the values set by times() are in the standard (?) units
of 1/60 second: */
#define UPS_SECS_FAC (1./60.)
#endif

  times(&yow);
  return (yow.tms_utime + yow.tms_stime) * UPS_SECS_FAC;
}

/********************************************************************/
