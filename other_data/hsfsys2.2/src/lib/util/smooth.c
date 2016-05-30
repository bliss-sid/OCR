/*
# proc: three_smooth - smooths the integers in an array using a three-point
# proc:                Gausing function.
*/

/************************************************************/
/*         File Name: Smooth.c                              */
/*         Package:   Smoothing Utilities                   */
/*                                                          */
/*         Contents:  Three_Smooth()                        */
/************************************************************/
#include <stdio.h>

/************************************************************/
/*         Routine:   Three_Smooth()                        */
/*         Author:                                          */
/*         Date:                                            */
/************************************************************/
/* Three_smooth() computes a three-point Gausing Smooth.    */
/************************************************************/
void three_smooth(array,length)
int *array;
int length;
{
   int i,size = length - 1;
   int orig_1_midpt_ago;
   int orig_this_midpt;

   /* first set of 3 ... */
   i = 1;
   orig_1_midpt_ago = array[i-1];
   orig_this_midpt = array[i];
   array[i] = (orig_1_midpt_ago +(2 * array[i]) + array[i+1])/4.0;
   /* set up values for next 5 pt average */
   orig_1_midpt_ago = orig_this_midpt;
   /* put first 3 pt average into locations 0 as well */
   array[i-1] = array[i];
   for(i = 2; i < size; i++){
      orig_this_midpt = array[i];
      array[i] = (orig_1_midpt_ago + (2 * array[i]) + array[i+1])/4.0;
      orig_1_midpt_ago = orig_this_midpt;
   }
   /* put last 3 pt average into last location */
   array[i] = array[i-1];
}
