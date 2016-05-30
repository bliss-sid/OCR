/*
# proc: strippath - takes a full pathname and returns a pointer to the
# proc:             information to the right of the rightmost directory level.
*/

/************************************************************/
/*         Routine:   Strippath()                           */
/*         Author:    Michael D. Garris                     */
/*         Date:      4/26/89                               */
/************************************************************/
/************************************************************/
/* Takes a filename string and returns just the root name.  */
/************************************************************/
void strippath(imagefile,fptr)
char *imagefile,**fptr;
{
   char *ptr;

   ptr = (char *)(imagefile + strlen(imagefile));
   while((ptr != imagefile) && (*ptr != '/'))
      ptr--;
   if(*ptr == '/')
      ptr++;
   *fptr = ptr;
}

