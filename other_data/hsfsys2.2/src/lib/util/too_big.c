/*
# proc: too_big - determines if the two floats passed, when summed together,
# proc:           exceed plus or minus a specified limit.
*/

#include <defs.h>

too_big(prevdelta, curdelta, succlimit)
float prevdelta, curdelta, succlimit;
{
   float sumdelta;

   sumdelta = prevdelta + curdelta;
   if((sumdelta > succlimit) || (sumdelta < -succlimit))
      return(TRUE);
   else
      return(FALSE);
}
