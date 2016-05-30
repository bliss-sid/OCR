/*
# proc: rcall_- wrapper for call to ratqr_().
# proc:
*/

/* rcall.f -- translated by f2c (version of 15 October 1990  19:58:17).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include <f2c.h>

/* Subroutine */ int rcall_(n, eps1, d, e, e2, m, w, ind, bd, idef, ierr)
integer *n;
real *eps1, *d, *e, *e2;
integer *m;
real *w;
integer *ind;
real *bd;
integer *idef, *ierr;
{
    static logical type;
    extern /* Subroutine */ int ratqr_();




    /* Parameter adjustments */
    --bd;
    --ind;
    --w;
    --e2;
    --e;
    --d;

    /* Function Body */
    type = FALSE_;
    ratqr_(n, eps1, &d[1], &e[1], &e2[1], m, &w[1], &ind[1], &bd[1], &type, 
	    idef, ierr);
    return 0;
} /* rcall_ */

