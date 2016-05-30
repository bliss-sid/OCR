/*
# proc: readihdr - reads the contents of an open file pointer into an
# proc:            IHead structure.
*/

#include <stdio.h>
#include <ihead.h>

/************************************************************/
/*         Routine:   Readihdr()                            */
/*         Author:    Michael D. Garris                     */
/*         Date:      4/26/89                               */
/*                                                          */
/*         Modifications:                                   */
/*		1/11/91 Stan Janet                          */
/*			check return codes                  */
/*			declare malloc()                    */
/************************************************************/
/************************************************************/
/* Readihdr() allocates and reads header information into an*/
/* ihead structure and returns the initialized structure.   */
/************************************************************/
IHEAD *readihdr(fp)
register FILE *fp;
{
   extern char *malloc();
   IHEAD *head;
   char lenstr[SHORT_CHARS];
   int n, len;

   n = fread(lenstr,1,SHORT_CHARS,fp);
   if (n != SHORT_CHARS) {
	(void) fprintf(stderr,"readihdr: fread returned %d (expected %d)\n",
		n,SHORT_CHARS);
	exit(1);
   }

   if (sscanf(lenstr,"%d",&len) != 1)
	fatalerr("readihdr","cannot parse length field",(char *)NULL);

   if (len != IHDR_SIZE)
      fatalerr("readihdr","Record Sync Error: Header not found or old format.",
            NULL);

   head = (IHEAD *) malloc(sizeof(IHEAD));
   if (head == (IHEAD *) NULL)
      syserr("readihdr","malloc","head");

   n = fread(head,1,len,fp);
   if (n != len) {
	(void) fprintf(stderr,"readihdr: fread returned %d (expected %d)\n",
		n,len);
	exit(1);
   }

   return head;
}
