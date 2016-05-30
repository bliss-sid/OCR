/*
# proc: ldv - computes the Levenstein Distance between two strings and returns
# proc:       their corresponding alignment.
*/

#ifndef lint
  static char rcsid[] = "$Header$";
#endif


/* LINTLIBRARY */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/param.h>


#include <align.h>
#include <pointers.h>
#include <defs.h>



#define MIN3(x,y,z)		(( x < y) ? min( x, z ) : min( y, z ))

#define EQUAL( x, y )		( case_sensitive ? \
				     ( ref[x-1] == hyp[y-1] ) : \
				     ( toupper( ref[x-1] ) == toupper( hyp[y-1] )))


#define DISTANCE(i,j)		(    distance[ (i) * ( hyplen + 1 ) + (j) ] )
#define BP(i,j)			(          bp[ (i) * ( hyplen + 1 ) + (j) ] )
#define PATH(i,j)		(        path[ (i) * ( hyplen + 1 ) + (j) ] )
#define SUB_PENALTY(i,j)	( sub_penalty[ (i) * ( hyplen + 1 ) + (j) ] )



	/*
	 *  Returns distance between reference and hypothesis strings,
	 *	or -1 on failure.
	 *  On success, sets *vlen to the length of the alignment, and
	 *	sets v[] to contain *vlen integers representing the
	 *	positions in the alignment.
	 *  The array v[] must be allocated by the caller. The length
	 *	needed for it depends on the alignment itself, but it's
	 *	maximum length is the sum of the lengths of the strings.
	 *  The character strings need to be unsigned so that we can
	 *	index into arrays using their elements' values.
	 */


int ldv ( ref, hyp, ap, fp, vlen, v )
u_char *ref;
u_char *hyp;
struct align_params_t *ap;
FILE *fp;			/* if non-NULL, print debugging code to here */

int *vlen;			/* OUTPUT */
int v[];			/* OUTPUT */
{
register int i, j;
int *distance;
char *bp;
char *path;
int reflen, hyplen, arr_size;
int eq, d1, d2, d3, dmin;
int c;


int case_sensitive = ap->case_sensitive;
int *ins_penalty = ap->insv;
int *del_penalty = ap->delv;
int *sub_penalty = ap->subv;

reflen = strlen( (char *)ref );				/* >= 0 */
hyplen = strlen( (char *)hyp );				/* >= 0 */
arr_size = ( reflen + 1 ) * ( hyplen + 1 );	/* >= 1 */


distance = (int *) malloc( arr_size * sizeof(int) );
if ( distance == INULL )
	return -1;

bp = malloc( arr_size * sizeof(int) );
if ( bp == CNULL ) {
	free((char *) distance );
	return -1;
}

path = malloc( arr_size );
if ( path == CNULL ) {
	free((char *) distance );
	free( bp );
	return -1;
}


DISTANCE(0,0) = 0;
BP(0,0) = CHAR_OK;
PATH(0,0) = '*';

for ( j=1; j <= hyplen; j++ ) {
	DISTANCE(0,j) = DISTANCE(0,j-1) + ins_penalty[ hyp[j] ];
	BP(0,j) = CHAR_INS;
	PATH(0,j) = ' ';
}

for ( j=1; j <= reflen; j++ ) {
	DISTANCE(j,0) = DISTANCE(j-1,0) + del_penalty[ ref[j] ];
	BP(j,0) = CHAR_DEL;
	PATH(j,0) = ' ';
}

for ( i=1; i <= reflen; i++ )
  for ( j=1; j <= hyplen; j++ ) {

	PATH(i,j) = ' ';

	eq = EQUAL( i, j );

	d1 = DISTANCE(i-1,j-1);
	if ( ! eq )
		d1 += SUB_PENALTY( ref[i], hyp[j] );

	d2 = DISTANCE(i,j-1) + ins_penalty[ hyp[j] ];
	d3 = DISTANCE(i-1,j) + del_penalty[ ref[i] ];

	dmin = MIN3( d1, d2, d3 );
	DISTANCE(i,j) = dmin;

	if ( d1 == dmin )
		BP(i,j) = eq ? CHAR_OK : CHAR_SUB;
	else
		BP(i,j) = ( d2 == dmin ) ? CHAR_INS : CHAR_DEL;
  }


i = reflen;
j = hyplen;
*vlen = 0;
while ( i >= 0 && j >= 0 ) {

	PATH(i,j) = '*';
	v[ *vlen ] = BP(i,j);
	++*vlen;

	switch ( BP(i,j) ) {

		case CHAR_OK:
			i--; j--;
			break;
		case CHAR_SUB:
			i--; j--;
			break;
		case CHAR_INS:
			j--;
			break;
		case CHAR_DEL:
			i--;
			break;

		default:
#ifdef DEBUG
			(void) fprintf( stderr, "ldv: bad bp value %d\n",
					BP(i,j) );
#endif
			free((char *) distance );
			free( bp );
			free( path );
			return -1;

	}
}


if ( fp != FPNULL ) {
	for ( i=0; i < *vlen; i++ )
		(void) fprintf( fp, "%c ", getalignchar( v[i] ));
	(void) fprintf( fp, "\n" );
}

--*vlen;
reverse_ivec( *vlen, v );

if ( fp != FPNULL ) {
	for ( i=0; i <= *vlen; i++ )
		(void) fprintf( fp, "%c ", getalignchar( v[i] ));
	fprintf( fp, "\n" );
}
 

if ( fp != FPNULL ) {

	(void) fprintf( fp, "\n      " );
	for ( j=0; j <= hyplen; j++ )
		(void) fprintf( fp, "   %c %3d %c", ' ', j, ( j == 0 ) ? ' ' : hyp[j-1] );
	(void) fprintf( fp, "\n\n" );

	for ( i=0; i <= reflen; i++ ) {

		(void) fprintf( fp, "%3d %c: ", i, ( i == 0 ) ? '0' : ref[i-1] );

		for ( j=0; j <= hyplen; j++) {

			c = getalignchar( BP(i,j ));
			(void) fprintf( fp, "   %3d %c %c",
				DISTANCE(i,j), c, PATH(i,j) );

		}

		(void) fprintf( fp, "\n" );
	}

}

dmin = DISTANCE( reflen, hyplen );

free( (char *) distance );
free( bp );
free( path );

return dmin;

} /*  ldv()  */



