/*------------------------------------------------------------------*/

/* When compiling for a machine type on which (apparently the
exception) an underscore should not be used after the called routine
name when C calls Fortran, one should define BLAS_NO_USCORES, say by
having -DBLAS_NO_USCORES as an arg to cc in makefile for that machine
type.  That will cause the underscores to be removed in the function
declarations below, in the calls (since all routines in the collection
that call any blas routines, include blas.h), and, if the provided
homeamde blas routines are linked instead of an installed blas
library, then those C functions will also have the underscores removed
from their names in their definitions, since those C files all include
blas.h. */

#ifdef BLAS_NO_USCORES

#define saxpy_ saxpy
#define scopy_ scopy
#define sdot_  sdot
#define sgemv_ sgemv
#define snrm2_ snrm2
#define sscal_ sscal

#endif

/*------------------------------------------------------------------*/

/* Declarations, in C form, of just those of the blas routines that
are used in the mlp collection (maybe some of these are not
actually used now). */

void saxpy_();
void scopy_();
double sdot_();
void sgemv_();
double snrm2_();
void sscal_();
