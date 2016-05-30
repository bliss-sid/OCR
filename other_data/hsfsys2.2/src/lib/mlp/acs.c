/* Some activation functions suitable for use by e_and_g.  Each must
have the following form: return value is void, and function takes
three args, the first arg being the input value (float) and the second
and third being the output function value and output derivative value
(float pointers).

The functions implemented here so far, all have the same derivative at
zero, namely 1/4.  This is a normalization that makes for better
comparison tests between nets using the various functions.

Contains:
# proc: ac_sinusoid - MLP sinusoid activation function (returns derivative).
# proc: ac_v_sinusoid - MLP sinusoid activation function (activation only).
# proc: ac_sigmoid - MLP sigmoid (logistic) activation function (returns
# proc:              derivative).
# proc: ac_v_sigmoid - MLP sigmoid (logistic) activation function (activation
# proc:              only).
# proc: ac_linear - MLP linear activaiton function (returns derivative).
# proc: ac_v_linear - MLP linear activaiton function (activation only).
*/

#include <math.h>

/*******************************************************************/

/* Sinusoid activation function and its derivative.  The scaling by .5
before taking the sine, and the adding of 1 and scaling by .5 after
taking the sine, cause this function to have the following desirable
properties: range is [0, 1] (almost same as range of sigmoid, which is
(0,1)); value at 0 is 1/2 (like sigmoid); and derivative at 0 is 1/4
(like sigmoid). */

void
ac_sinusoid(x, val, deriv)
float x, *val, *deriv;
{
  double s, c, a;

#ifndef HAVE_SINCOS
  *val = .5 * (1. + (float)sin(a = .5 * x));
  *deriv = .25 * (float)cos(a);
#else
  /* If sincos exists, using it is presumably going to be faster than
  calling sin and cos. */
  sincos(.5 * x, &s, &c);
  *val = .5 * (1. + (float)s);
  *deriv = .25 * (float)c;
#endif
}

/*******************************************************************/

/* Sinusoid activation function, value only.

Input/output arg:
  p: The address for input of the value, and for output of the
    result of applying the activation function to this value.
*/

void
ac_v_sinusoid(p)
float *p;
{
  *p = .5 * (1. + (float)sin((double)(.5 * *p)));
}

/*******************************************************************/

/* Sigmoid activation function (also called the logistic function) and
its derivative.  (The idea with SMIN is that it is a large-magnitude
negative number, such that exp(-SMIN), a large positive number, just
barely avoids overflow.) */

#define SMIN -1.e6 /* ok for now */

void
ac_sigmoid(x, val, deriv)
float x, *val, *deriv;
{
  float v;

  *val = v = (x >= SMIN ? 1. / (1. + (float)exp(-x)) : 0.);
  *deriv = v * (1. - v);
}

/*******************************************************************/

/* Sigmoid (also called logistic) activation function, value only.

Input/output arg:
  p: The address for input of the value, and for output of the
    result of applying the activation function to this value.
*/


void
ac_v_sigmoid(p)
float *p;
{
  *p = (*p >= SMIN ? 1. / (1. + (float)exp(-*p)) : 0.);
}
/*******************************************************************/

/* A linear activation function and its derivative. */

void
ac_linear(x, val, deriv)
float x, *val, *deriv;
{
  *val = .25 * (float)x;
  *deriv = .25;
}

/*******************************************************************/
/* Linear activation function, value only.

Input/output arg:
  p: The address for input of the value, and for output of the
    result of applying the activation function to this value.
*/

void
ac_v_linear(p)
float *p;
{
  *p = .25 * (float)*p;
}

