/*
# proc: find_first_thresh_forward - finds the position and value of the first
# proc:                             integer that exceeds a threshold searching
# proc:                             forward in a specified section of a list.
# proc: find_first_thresh_backward - finds the position and value of the first
# proc:                              integer that exceeds a threshold searching
# proc:                              backward in a specified section of a list.
*/

/************************************************************/
/*                    Find_First_Thresh_Forward()           */
/*                    Find_First_Thresh_Backward()          */
/************************************************************/
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/************************************************************/
/*         Routine:   Find_First_Thresh_Forward()           */
/*         Author:    Michael D. Garris                     */
/*         Date:      8/20/89                               */
/************************************************************/
/* Find_first_thersh_forward() takes an array of integers   */
/* and finds the first location where a value exceeds the   */
/* value of 'thresh'starting at index 'start' and searching */
/* forward up to 'datlen'.                                  */
/************************************************************/
int find_first_thresh_forward(dat,start,datlen,thresh,index,value)
int *dat,datlen,*value,*index,start,thresh;
{
   int i;

   *value = -1;
   *index = -1;
   for(i=start;i<datlen;i++){
      if(dat[i] > thresh){
         *index = i;
         *value = dat[i];
         return(TRUE);
      }
   }
   return(FALSE);
}
/************************************************************/
/*         Routine:   Find_First_Thresh_Backward()          */
/*         Author:    Darrin L. Dimmick                     */
/*         Date:      6/4/90                               */
/************************************************************/
/* Find_first_thersh_backward() takes an array of integers   */
/* and finds the first location where a value exceeds the   */
/* value of 'thresh'starting at index 'datlen' and searching */
/* backward up to 'start'.                                  */
/************************************************************/
int find_first_thresh_backward(dat,start,datlen,thresh,index,value)
int *dat,datlen,*value,*index,start,thresh;
{
   int i;

   *value = -1;
   *index = -1;
   for(i=datlen-1;i>=start;i--){	/* was i>start which is wrong */
      if(dat[i] > thresh){
         *index = i;
         *value = dat[i];
         return(TRUE);
      }
   }
   return(FALSE);
}
