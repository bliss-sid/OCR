/*
# proc: strm_fmt - Used to format the first, "stream" part of a warning or
# proc:            error message.

Input arg:
  str_in: Input string.  Should consist of words separated by single
    spaces.

Output arg:
  str_out: Output string.  Allocated by caller.  (Allocate large
    enough for the insertion of spaces that implement the left
    margins.)
*/

#include <stdio.h>
#include <mlp/defs.h>
#include <mlp/fmt_msgs.h>

void
strm_fmt(str_in, str_out)
char str_in[], str_out[];
{
  char *ip, *ip_lastchar, *ip_lastcol, *ip_yow, *op,
    *op_e, firstline;
  int firstcol;

  ip_lastchar = str_in + strlen(str_in) - 1;
  for(firstline = TRUE, ip = str_in, op = str_out; ;
    firstline = FALSE) {

    firstcol = (firstline ? MESSAGE_FIRSTCOL_FIRSTLINE:
      MESSAGE_FIRSTCOL_LATERLINES);
    for(op_e = op + firstcol; op < op_e;)
      *op++ = ' ';

    ip_lastcol = ip + MESSAGE_LASTCOL - firstcol;
    if(ip_lastcol >= ip_lastchar) { /* last line of output */
      while(ip <= ip_lastchar)
	*op++ = *ip++;
      *op++ = '\n';
      *op = '\0';
      return;
    }

    /* Rest of input won't all fit on next line; find a suitable
    point, if one exists, at which to insert a line break. */
    if(*ip_lastcol == ' ' || *(ip_lastcol + 1) == ' ') {
      ip_yow = ((*ip_lastcol == ' ') ? ip_lastcol : ip_lastcol + 1);
      while(ip < ip_yow)
	*op++ = *ip++;
      *op++ = '\n';
      ip++;
    }
    else { /* Scan backward, stopping at first space. */
      for(ip_yow = ip_lastcol - 1; ip_yow > ip && *ip_yow != ' ';
        ip_yow--);
      if(ip_yow == ip) {
	/* There is no space at which to insert a line break, so as to
        make a sufficiently short output line.  So, just shear off
        the line. */
	while(ip <= ip_lastcol)
	  *op++ = *ip++;
	*op++ = '\n';
      }
      else { /* Found a suitable line-breaking point. */
	while(ip < ip_yow)
	  *op++ = *ip++;
	*op++ = '\n';
	ip++;
      }
    }
  }
}
