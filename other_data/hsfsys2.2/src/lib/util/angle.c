/*
# proc: theta_npi2pi - converts an angle in radians to its equivalent angle
# proc:                in the rnage [-PI/2 .. PI/2].
*/

#include <math.h>

/***************************************************************************/
/* theta_npi2pi - takes an angle in radians and converts it into its       */
/* equivalent angle within the range [-PI/2 .. PI/2] (ie. quadrants        */
/* 4 and 1). This helps to control the sign on certain trig functions.     */
/***************************************************************************/
float theta_npi2pi(theta)
float theta;
{
   float ntheta;
   int n;

   /* how many multple periods exist */
   n = (int)(theta / (2.0 * M_PI));
   /* subtract them off */
   ntheta = theta - (n * (2.0 * M_PI));
   /* if angle still too big, reflect it back */
   if(ntheta > M_PI_2)
      ntheta -= M_PI;
   /* if angle still too small, reflect it forward */
   else if(ntheta <= -M_PI_2)
      ntheta += M_PI;
   /* return the equivalent angle */
   return(ntheta);
}
