/*
# proc: reverse_ivec - reverses a vector of integers in place.
# proc:
*/

#ifndef lint
  static char rcsid[] = "$Header$";
#endif

/* LINTLIBRARY */



reverse_ivec( c, v )
int c, v[];
{
int iter, itmp, i1, i2;

iter = c / 2;
if ( iter < 1 )
	return;

i1 = 0;
i2 = c - 1;

while ( iter-- > 0 ) {

	itmp = v[ i1 ];
	v[ i1 ] = v[ i2 ];
	v[ i2 ] = itmp;

	i1++;
	i2--;

} /* while */

} /*  reverse_ivec()  */
