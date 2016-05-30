/*
# proc: diag - finds some eigen{values,vectors} of a real symmetric
# proc:        matrix (calls a sequence of EISPACK routines)
*/

/*
Input args:
  matrix: real and symmetric.  Only the upper triangle (in a calling
    program written in C or other row-major language; lower triangle
    for Fortran or other column-major language) need be filled in.
    CAUTION: matrix's contents will be altered.
  order: of matrix.
  n_requested: how many largest eigenvalues, and corresponding
    eigenvectors, are requested; must be <= order.
  messages: 1 (0) if routine is (isn't) to write some status, and
    if applicable error, messages to stderr.

Output args:
  n_returned: if return value is 1, how many eigenvalues and
    eigenvectors are being returned; will be <= n_requested.
  eigenvals, eigenvecs: if return value is 1, these buffers will have
    been malloced and will contain *n_returned eigenvalues in
    decreasing order, and corresponding eigenvectors; otherwise,
    these won't have been malloced.

Return value: 1 (0) if routine did (didn't) return some eigenvalues
  and eigenvectors.
*/

#include <stdio.h>
#include <defs.h>

#define BISECT_LB 0.002

static int *ind;
static float *w, *d, *e, *e2, *bd, *e2s, *w1, *w2, *w3, *w4, *w5;

int
diag(matrix, order, n_requested, messages, n_returned, eigenvals,
  eigenvecs)
float *matrix, **eigenvals, **eigenvecs;
int order, n_requested, messages, *n_returned;
{

    la_eigen(n_requested, n_returned, eigenvecs, eigenvals, matrix,
             order);

    return(1);
}
