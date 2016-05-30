/*
# proc: is_in_range - determines if an integer is within the specified range.
# proc:
# proc: f_is_in_range - determines if a float is within the specified range.
# proc:
# proc: is_within_tolerance - verifies that two floats are within a specified
# proc:                       distance from each other.
*/

/************************************************************/
/*         File Name: RANGE.C                               */
/*         Package:   Range and Tolerance Checkers          */
/*                                                          */
/*         Contents:  is_in_range()                         */
/*                    f_is_in_range()                       */
/*                    is_within_tolerance()                 */
/************************************************************/
#include <math.h>
#include <defs.h>

/************************************************************/
/*         Routine:   is_in_range()                         */
/*         Author:    Michael D. Garris                     */
/*                                                          */
/* Is_in_range() verifies that the integer 'i' is in the    */
/* range of values between min and max inclusive.  This     */
/* routine returns True if 'i' is in the range and False    */
/* otherwise.                                               */
/************************************************************/
/* i      - value to be tested.                             */
/* minval - smallest value of range.                        */
/* maxval - largest value of range.                         */
/************************************************************/
int is_in_range(i,minval,maxval)
int i,minval,maxval;
{
  if (i>=minval && i<=maxval)
     return(True);
  else
     return(False);
}

/************************************************************/
/*         Routine:   f_is_in_range()                       */
/*         Author:    Michael D. Garris                     */
/*         Date:      03/05/91                              */
/*                                                          */
/* F_is_in_range() verifies that the float 'f' is in the    */
/* range of values between min and max inclusive.  This     */
/* routine returns True if 'f' is in the range and False    */
/* otherwise.                                               */
/************************************************************/
/* f      - value to be tested.                             */
/* minval - smallest value of range.                        */
/* maxval - largest value of range.                         */
/************************************************************/
int f_is_in_range(f, minval, maxval)
float f, minval, maxval;
{
   if((f >= minval) && (f <= maxval))
      return(TRUE);
   else
      return(FALSE);
}

/************************************************************/
/*         Routine:   is_within_tolerance()                 */
/*         Author:    Michael D. Garris                     */
/*         Date:      03/05/91                              */
/*                                                          */
/* Is_within_tolerance() verifies that two floating point   */
/* numbers are withing a specified distance of each other.  */
/************************************************************/
/* f   - first floating point number.                       */
/* p   - second floating point number.                      */
/* tol - acceptable distance (tolerance) within which the   */
/*       the two floating point values are to be considered */
/*       cooincident.                                       */
/************************************************************/
int is_within_tolerance(f, p, tol)
float f, p, tol;
{
   float diff;

   diff = fabs(f - p);
   if(diff <= tol)
      return(TRUE);
   else
      return(FALSE);
}
