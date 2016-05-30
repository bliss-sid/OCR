/*
# proc: uni - this routine generates quasi uniform random numbers on [0,1)
# proc:       and can be used on any computer with at least 16 bit integers.

***date written   810915
***revision date  810915
***category no.  g4a23
***keywords  random numbers, uniform random numbers
***author    blue, james, scientific computing division, nbs
             kahaner, david, scientific computing division, nbs
             marsaglia, george, computer science dept., wash state univ

***purpose  this routine generates quasi uniform random numbers on [0,1)
            and can be used on any computer with at least 16 bit integers,
            e.g., with a largest integer at least 32767.
***description

       this routine generates quasi uniform random numbers on the interval
       [0,1).  it can be used with any computer with at least 16 bit
       integers, e.g., with a largest integer at least equal to 32767.


   use
       first time....
                   z = uni(jd)
                     here jd is any  n o n - z e r o  integer.
                     this causes initialization of the program
                     and the first random number to be returned as z.
       subsequent times...
                   z = uni(0)
                     causes the next random number to be returned as z.

   machine dependencies...
      MDIG = a lower bound on the number of binary digits available
              for representing integers.

   remarks...
     a. this program can be used in two ways:
        (1) to obtain repeatable results on different computers,
            set MDIG to the smallest of its values on each, or,
        (2) to allow the longest sequence of random numbers to be
            generated without cycling (repeating) set MDIG to the
            largest possible value.
     b. the sequence of numbers generated depends on the initial
          input jd as well as the value of MDIG.
          if MDIG=16 one should find that
            the first evaluation
              z=uni(305) gives z=.027832881...
            the second evaluation
              z=uni(0) gives   z=.56102176...
            the third evaluation
              z=uni(0) gives   z=.41456343...
            the thousandth evaluation
              z=uni(0) gives   z=.19797357...

***references  (none)
***routines called  (none)
***end prologue  uni

  Converted from Fortran to C by gtc.
*/

#include <mlp/macros.h>

/* Uncomment either line, to choose between 16 and 32 bits: */
/* #define MDIG_16 */
#define MDIG_32

/* Do NOT change these lines: */
#ifdef MDIG_16
#define MDIG 16
#define M1 ((int)0x7fff)
#else
#define MDIG 32
#define M1 ((int)0x7fffffff)
#endif
#define M2 ((int)(1<<(MDIG/2)))

float
uni(jd)
int jd;
{
  int jseed, k0, k1, j0, j1, k;
  static int i, j, m[17];

  if(jd) {
    jseed = min(abs(jd), M1);
    if(!(jseed & 1))
      jseed--;
    k0 = 9069 % M2;
    k1 = 9069 / M2;
    j0 = jseed % M2;
    j1 = jseed / M2;
    for(i = 0; i < 17; i++) {
      jseed =j0 * k0;
      j1 = (jseed / M2 + j0 * k1 + j1 * k0) % (M2 / 2);
      j0 = jseed % M2;
      m[i] = j0 + M2 * j1;
    }
    i = 4;
    j = 16;
  }
  k = m[i] - m[j];
  if(k < 0)
    k += M1;
  m[j] = k;
  if(i)
    i--;
  else
    i = 16;
  if(j)
    j--;
  else
    j = 16;
  return (float)k / M1;
}
