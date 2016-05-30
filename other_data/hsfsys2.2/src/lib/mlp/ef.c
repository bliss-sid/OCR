/* "Error functions" each of which, given an output activations
vector, and a target vector or an actual class, and possibly a parm
(alpha, for type_1), computes the resulting error contribution and its
gradient w.r.t. the activations vector.  For use by e_and_g, which
computes the error and its gradient w.r.t. the weights.

Contains:
# proc: ef_mse_t - computes "mse" error function, using a target vector.
# proc: ef_mse_c - computes "mse" error function, using a target class.
# proc: ef_t1_c - computes "type_1" error function, using a class.
# proc: ef_ps_c - computes "pos_sum" error function, using a class.


[Maybe some or all of these should use blas routines.]
*/

#include <math.h>

/*******************************************************************/

/* For "mse" error function, using a target vector.

Input args:
  nouts: Number of output nodes.
  acsvec: Output activations vector.
  targvec: Target activations vector.

Output args:
  e: Error contribution of this pattern.
  g: Gradient of the error contribution of this pattern w.r.t. the
     output activations.  A vector of nouts elts, to be allocated by
     caller.
*/

void
ef_mse_t(nouts, acsvec, targvec, e, g)
int nouts;
float *acsvec, *targvec, *e, *g;
{
  float a, e_yow, *ac_e;

  ac_e = acsvec + nouts;
  e_yow = 0.;
  while(acsvec < ac_e) {
    *g++ = 2. * (a = *acsvec++ - *targvec++);
    e_yow += a * a;
  }
  *e = e_yow;
}

/*******************************************************************/

/* For "mse" error function, using a class.

Input args:
  nouts: Number of output nodes.
  acsvec: Output activations vector.
  actual_class: The actual class of this pattern (in range 0 through
    nouts - 1).

Output args:
  e: Error contribution of this pattern.
  g: Gradient of the error contribution of this pattern w.r.t. the
     output activations.  A vector of nouts elts, to be allocated by
     caller.
*/

void
ef_mse_c(nouts, acsvec, actual_class, e, g)
int nouts;
short actual_class;
float *acsvec, *e, *g;
{
  float *actual_p, *ac_e, e_yow, a;

  actual_p = acsvec + actual_class;
  ac_e = acsvec + nouts;
  e_yow = 0.;
  while(acsvec < ac_e) {
    *g++ = 2. * (a = (acsvec == actual_p ? *acsvec++ - 1. :
      *acsvec++));
    e_yow += a * a;
  }
  *e = e_yow;
}

/*******************************************************************/

/* For "type_1" error function (using a class, the only possibility
for type_1).

Input args:
  nouts: Number of output nodes.
  acsvec: Output activations vector.
  actual_class: The actual class of this pattern (in range 0 through
    nouts - 1).
  alpha: Factor used (with minus sign) before taking the exp.

Output args:
  e: Error contribution of this pattern.
  g: Gradient of the error contribution of this pattern w.r.t. the
     output activations.  A vector of nouts elts, to be allocated by
     caller.
*/

void
ef_t1_c(nouts, acsvec, actual_class, alpha, e, g)
int nouts;
short actual_class;
float *acsvec, alpha, *e, *g;
{
  float *actual_p, actual_ac, *ac_e, beta, *g_p, ep;

  actual_ac = *(actual_p = acsvec + actual_class);
  ac_e = acsvec + nouts;
  beta = 0.;
  for(g_p = g; acsvec < ac_e; acsvec++, g_p++)
    if(acsvec != actual_p) {
      beta += (ep = exp((double)(-alpha * (actual_ac - *acsvec))));
      *g_p = alpha * ep;
    }
  *e = 1. - 1. / (1. + beta);
  *(g + actual_class) = -alpha * beta;
}

/*******************************************************************/

/* For "pos_sum" error function (using a class, the only possibility
for pos_sum).

Input args:
  nouts: Number of output nodes.
  acsvec: Output activations vector.
  actual_class: The actual class of this pattern (in range 0 through
    nouts - 1).

Output args:
  e: Error contribution of this pattern.
  g: Gradient of the error contribution of this pattern w.r.t. the
     output activations.  A vector of nouts elts, to be allocated by
     caller.
*/

void
ef_ps_c(nouts, acsvec, actual_class, e, g)
int nouts;
short actual_class;
float *acsvec, *e, *g;
{
  float *actual_p, *ac_e, e_yow, a;

  actual_p = acsvec + actual_class;
  ac_e = acsvec + nouts;
  e_yow = 0.;
  while(acsvec < ac_e) {
    if(acsvec == actual_p) {
      a = 1. - *acsvec++;
      *g++ = -20. * a - 1.;
    }
    else {
      a = *acsvec++;
      *g++ = 20. * a + 1.;
    }
    e_yow += (10. * a + 1.) * a;
  }
}

/*******************************************************************/
