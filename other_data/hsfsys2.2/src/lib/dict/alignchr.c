/*
# proc: getalignchar - converts an alignment character to an output ASCII
# proc:                character for diagnostic printing.
*/

#ifndef lint
  static char rcsid[] = "$Header$";
#endif

/* LINTLIBRARY */

#include <stdio.h>
#include "align.h"



int getalignchar( status )
int status;
{
int c;

switch ( status ) {

	case CHAR_OK:
		c = '-';
		break;
	case CHAR_SUB:
		c = 'S';
		break;
	case CHAR_INS:
		c = 'I';
		break;
	case CHAR_DEL:
		c = 'D';
		break;

	default:
#ifdef DEBUG
		(void) fprintf( stderr, "getalignchar: bad status %d\n",
				status );
#endif
		c = '?';

} /* switch */

return c;
} /*  getalignchar()  */
