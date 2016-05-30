/*
# proc: writeihdr - writes the contents of an IHead structure to an open
# proc:             file pointer.
*/

#include <stdio.h>
#include <ihead.h>

/************************************************************/
/*         Routine:   Writeihdr()                           */
/*         Author:    Michael D. Garris                     */
/*         Date:      2/08/90                               */
/************************************************************/
/************************************************************/
/* Writeihdr() writes the fixed length field and the header */
/* passed to the given file pointer.                        */
/************************************************************/
void writeihdr(fp,ihead)
FILE *fp;
IHEAD *ihead;
{
   int i;
   char lenstr[SHORT_CHARS];

   for (i = 0; i < SHORT_CHARS; i++)
      lenstr[i] = '\0';
   /* creates a string from of header size */
   sprintf(lenstr,"%d",sizeof(IHEAD));
   /* writes the length string in headerto the file */
   fwrite(lenstr,sizeof(char),SHORT_CHARS,fp);
   /* writes the given header to the file */
   fwrite(ihead,sizeof(IHEAD),1,fp);
}
