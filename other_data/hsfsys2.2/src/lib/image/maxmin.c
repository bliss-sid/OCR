/*
# proc: find_first_max_forward - returns the location and value of the first
# proc:                          maximum searching forward through a
# proc:                          specified section of a list of integers.
# proc: find_first_min_forward - returns the location and value of the first
# proc:                          minimum searching forward through a
# proc:                          specified section of a list of integers.
# proc: find_first_min_backward - returns the location and value of the first
# proc:                           minimum searching backward through a
# proc:                           specified section of a list of integers.
*/

/************************************************************/
/*         File Name: Maxmin.c                              */
/*         Package:   Maxima/Minima Utilities               */
/*                                                          */
/*         Contents:  Find_First_Max_Forward()              */
/*                    Find_First_Min_Forward()              */
/*                    Find_First_Min_Backward()             */
/************************************************************/
#include <stdio.h>
#include <defs.h>

/************************************************************/
/*         Routine:   Find_First_Max_Forward()              */
/*         Author:    Michael D. Garris                     */
/*         Date:      8/20/89                               */
/************************************************************/
/* Find_first_max_forward() takes an array of integers and  */
/* finds the location of the first maximum value starting   */
/* at index 'start' and searching forward up to 'datlen'.   */
/************************************************************/
void find_first_max_forward(dat,start,datlen,index,maxval)
int *dat,datlen,*maxval,*index,start;
{
   int i;
   *maxval=0;
   *index=start;
   for(i=start;i<datlen;i++){
      if(dat[i]>*maxval){
         *maxval=dat[i];
         *index=i;
      }
   }
}

/************************************************************/
/*         Routine:   Find_First_Min_Forward()              */
/*         Author:    Michael D. Garris                     */
/*         Date:      8/20/89                               */
/************************************************************/
/* Find_first_min_forward() takes an array of integers and  */
/* finds the location of the first minimum starting         */
/* at index 'start' and searching forward up to 'datlen'.   */
/************************************************************/
void find_first_min_forward(dat,start,datlen,index,minval)
int *dat,datlen,*minval,*index,start;
{
   int i;
   *minval=dat[start];
   *index=start;
   for(i=start;i<datlen;i++){  
      if(dat[i] < *minval){
         *minval=dat[i];
         *index=i;
      }
   }
}

/************************************************************/
/*         Routine:   Find_First_Min_Backward()             */
/*         Author:    Michael D. Garris                     */
/*         Date:      8/20/89                               */
/************************************************************/
/* Find_first_min_backward() takes an array of integers and */
/* finds the location of the first minimum starting at index*/
/* 'start' and searching backward to 'end'.                 */
/************************************************************/
void find_first_min_backward(dat,start,end,index,minval)
int *dat,end,*minval,*index,start;  
{
   int i;

   *minval=dat[start];  
   *index=start;
   for(i=start;(i>=end)&&(i>=0);i--){
      if(dat[i] < *minval){
         *minval=dat[i];
         *index=i;   
      }
   }
}
