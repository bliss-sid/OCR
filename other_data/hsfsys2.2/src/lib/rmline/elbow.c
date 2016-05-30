/*
# proc: der2_change_right_up - locates an elbow that bends upward left to right
# proc:             by analyzing 2nd derivative information from a list of integers.
# proc: der2_change_right_down - locates an elbow that bends downward left to right
# proc:             by analyzing 2nd derivative information from a list of integers.
# proc: der2_change_left_up - locates an elbow that bends upward right to left
# proc:             by analyzing 2nd derivative information from a list of integers.
# proc: der2_change_left_down - locates an elbow that bends downward right to left
# proc:             by analyzing 2nd derivative information from a list of integers.
*/

#include <defs.h>

/***************************************************************************/
/* der2_change_right_up - locates a point of inflection within a list of   */
/* integers where the summed change in the 2nd derivative exceeds a        */
/* specified threshold. This routine searches the list left to right,      */
/* looking for an elbow that bends upward.                                 */
/***************************************************************************/
der2_change_right_up(si, slen, thr, list, llen)
int si, slen, thr;
int *list, llen;
{
   int i, t, sum;

   /* if start index plus search length exceeds list length ... */
   if((si + slen) > llen)
      /* cut down the search length */
      slen = llen - si;

   /* if search length not long enough to support 2nd derivatives ... */
   if(slen < 3)
      /* return failure */
      return(NOT_FOUND);

   sum = 0;
   for(i = 1, t = si+1; i < slen-1; i++, t++){
      /* compute and accumulate 2nd derivative */
      sum += (-list[t-1] + (2*list[t]) - list[t+1]);
      /* if accumulated change in 2nd derivatives exceeds the threshold ... */
      if(sum > thr)
         /* return the location of the point of inflection */
         return(t);
   }
   /* otherwise return failure */
   return(NOT_FOUND);
}

/***************************************************************************/
/* der2_change_right_down - locates a point of inflection within a list of */
/* integers where the summed change in the 2nd derivative exceeds a        */
/* specified threshold. This routine searches the list left to right,      */
/* looking for an elbow that bends downward.                               */
/***************************************************************************/
der2_change_right_down(si, slen, thr, list, llen)
int si, slen, thr;
int *list, llen;
{
   int i, t, sum;

   /* if start index plus search length exceeds list length ... */
   if((si + slen) > llen)
      /* cut down the search length */
      slen = llen - si;

   /* if search length not long enough to support 2nd derivatives ... */
   if(slen < 3)
      /* return failure */
      return(NOT_FOUND);

   sum = 0;
   for(i = 1, t = si+1; i < slen-1; i++, t++){
      /* compute and accumulate 2nd derivative */
      sum += (list[t-1] - (2*list[t]) + list[t+1]);
      /* if accumulated change in 2nd derivatives exceeds the threshold ... */
      if(sum > thr)
         /* return the location of the point of inflection */
         return(t);
   }
   /* otherwise return failure */
   return(NOT_FOUND);
}

/***************************************************************************/
/* der2_change_left_up - locates a point of inflection within a list of    */
/* integers where the summed change in the 2nd derivative exceeds a        */
/* specified threshold. This routine searches the list right to left,      */
/* looking for an elbow that bends upward.                                 */
/***************************************************************************/
der2_change_left_up(si, slen, thr, list, llen)
int si, slen, thr;
int *list, llen;
{
   int i, t, sum;

   /* if start index minus search length exceeds front of list ... */
   if((si - slen) < 0)
      slen = si;

   /* if search length not long enough to support 2nd derivatives ... */
   if(slen < 3)
      /* return failure */
      return(NOT_FOUND);

   sum = 0;
   for(i = slen - 2, t = si-1; i >= 1; i--, t--){
      /* compute and accumulate 2nd derivative */
      sum += (-list[t-1] + (2*list[t]) - list[t+1]);
      /* if accumulated change in 2nd derivatives exceeds the threshold ... */
      if(sum > thr)
         /* return the location of the point of inflection */
         return(t);
   }
   /* otherwise return failure */
   return(NOT_FOUND);
}

/***************************************************************************/
/* der2_change_left_down - locates a point of inflection within a list of  */
/* integers where the summed change in the 2nd derivative exceeds a        */
/* specified threshold. This routine searches the list right to left,      */
/* looking for an elbow that bends downward.                               */
/***************************************************************************/
der2_change_left_down(si, slen, thr, list, llen)
int si, slen, thr;
int *list, llen;
{
   int i, t, sum;

   /* if start index minus search length exceeds front of list ... */
   if((si - slen) < 0)
      slen = si;

   /* if search length not long enough to support 2nd derivatives ... */
   if(slen < 3)
      /* return failure */
      return(NOT_FOUND);

   sum = 0;
   for(i = slen - 2, t = si-1; i >= 1; i--, t--){
      /* compute and accumulate 2nd derivative */
      sum += (list[t-1] - (2*list[t]) + list[t+1]);
      /* if accumulated change in 2nd derivatives exceeds the threshold ... */
      if(sum > thr)
         /* return the location of the point of inflection */
         return(t);
   }
   /* otherwise return failure */
   return(NOT_FOUND);
}
