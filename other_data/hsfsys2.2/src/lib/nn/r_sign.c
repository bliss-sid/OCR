/*
# proc: r_sign - fundamental routine required by f2c distribution.
# proc:
*/

#include <f2c.h>

double r_sign(a,b)
real *a, *b;
{
double x;
x = (*a >= 0 ? *a : - *a);
return( *b >= 0 ? x : -x);
}
