/*
# proc: current_time - returns a string containing the current date on the
# proc:                system.
*/

/************************************************************/
/*         Routines:   Current_Time()                       */
/*         Author:    Michael D. Garris                     */
/*         Date:      6/04/90                               */
/************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

/************************************************************/
/* Current_Time() gets the time of day from the system and  */
/* returns an ASCII date string without a newline char.     */
/************************************************************/
char *current_time()
{
   long tm;
   char *dptr;

   tm = time((long *)NULL);
   dptr = (char *)ctime(&tm);
   *(dptr + strlen(dptr) - 1) = '\0';
   return(dptr);
}
