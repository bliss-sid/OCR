/* [This new version allows for the set of defined ierr values to have
holes in it.  Fill in a real message for ierr value 10, and add cases
for any other values that are not covered now.] */

/*
# proc: endopt - Does some output at the end of the optimization.

Input args:
  iter: How many iterations were used.
  ncalls: How many function calls were used.
  ierr: Error code.  [Returned by optwts, which returned the error code
    from whichever of scg() and lmbfgs() it called last?]
  err: Error.
  gw: size(gradient) / size(weights).
*/

#include <stdio.h>

void
endopt(iter, ncalls, ierr, err, gw)
int iter, ncalls, ierr;
float err, gw;
{
  char msg[80], str[200];

  switch(ierr) {
  case 0:
    strcpy(msg, "achieved error goal");
    break;
  case 1:
    strcpy(msg, "iteration limit");
    break;
  case 2:
    strcpy(msg, "gradient small");
    break;
  case 3:
    strcpy(msg, "slow convergence of error");
    break;
  case 4:
    strcpy(msg, "slow convergence of OK");
    break;
  case 10:
    strcpy(msg, "<fill in message for ierr value 10>");
    break;
  default:
    sprintf(msg, "unknown ierr value %d", ierr);
    break;
  }
  sprintf(str, " Iter %d; ierr %d : %s\n Used %d iterations; \
%d function calls; Err %.3f; |g|/|w| %.3e\n", iter, ierr, msg,
    iter + 1, ncalls, err, gw);
  fsaso(str);
}
