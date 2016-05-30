/*
# proc: write_bin_patterns - write features vectors and their classes (no tree)
# proc:                     to a file for fast binary io.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mfs.h>
#include <sex.h>

#define JUSTPATSFILE    0

/***********************************************************************/
write_bin_patterns(fn, feats, class, class_set, INPS, nPats,nInps,nOuts)
char	*fn;
char	**class_set;
MFS 	*class;
float   *feats;
int	INPS;			/* dimension of matrix >= length nInps	*/
int     nPats, nInps, nOuts;
{
float *featsptr, target;
int   i, j, n, nChars;
int   targ_typ = 0, idum = 0, tree = JUSTPATSFILE;
int   wsize, wisize, wosize;
FILE  *fp;
unsigned int uint, *ptr2;

  if ((fp = fopen(fn, "wb")) == NULL)
     syserr("write_bin_patterns", fn, "fopen");

  wsize = 24;
  uint = wsize;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
     syserr("write_bin_patterns", fn, "wsize1a fwrite");

  uint = nPats;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
     syserr("write_bin_patterns", fn, "nPats fwrite");

  uint = nInps;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
     syserr("write_bin_patterns", fn, "nInps fwrite");

  uint = nOuts;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
     syserr("write_bin_patterns", fn, "nOuts fwrite");

  uint = targ_typ;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
     syserr("write_bin_patterns", fn, "targ_typ fwrite");

  uint = tree;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
     syserr("write_bin_patterns", fn, "dummy1 fwrite");

  uint = idum;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
     syserr("write_bin_patterns", fn, "dummy2 fwrite");

  uint = wsize;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
     syserr("write_bin_patterns", fn, "wsize1b fwrite");

  wsize = 32*nOuts;
  uint = wsize;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
     syserr("write_bin_patterns", fn, "wsize2a fwrite");

  for (i = 0; i < nOuts; i++)
  {
     nChars = strlen(class_set[i]);
     if ((n = fwrite(class_set[i], sizeof(char), nChars, fp)) != nChars)
        syserr("write_bin_patterns", fn, "string fwrite");
     for(j = 0; j < (32 - nChars); j++)
        if ((n = fwrite("\0", sizeof(char), 1, fp)) != 1)
           syserr("write_bin_patterns", fn, "null fwrite");
  }


  uint = wsize;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
     syserr("write_bin_patterns", fn, "wsize2b fwrite");

  featsptr = feats;

  if (class->num < nPats)
    fatalerr("write_bin_patterns", fn, "less class ID's than patterns");

  wisize = 4*nInps;
  wosize = 4*nOuts;
  for(i = 0; i < nPats ; i++)
  {
     uint = wisize;
     uint = swap_word(uint);
     if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
        syserr("write_bin_patterns", fn, "wsize3a fwrite");

     for(j = 0, ptr2 = (unsigned int *)featsptr; j < nInps; j++, ptr2++)
        *ptr2 = swap_word(*ptr2);
     if ((n = fwrite(featsptr, sizeof(float), nInps, fp)) != nInps)
        syserr("write_bin_patterns", fn, "feature vector fwrite");

     featsptr += INPS;        	/* skip DAP space beyond length	*/
				/* up to dimension		*/

     uint = wisize;
     uint = swap_word(uint);
     if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
        syserr("write_bin_patterns", fn, "wsize3b fwrite");

     uint = wosize;
     uint = swap_word(uint);
     if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
        syserr("write_bin_patterns", fn, "wsize4a fwrite");

     for(j = 0; j < nOuts; j++)		/* make a target vector */
     {
        if(strcmp(class_set[j], class->values[i]) == 0)
           target = 1.0;
        else
           target = 0.0;

        ptr2 = (unsigned int *)&target;
        *ptr2 = swap_word(*ptr2);
        if ((n = fwrite(&target, sizeof(float), 1, fp)) != 1)
	  syserr("write_bin_patterns", fn, "target fwrite");
     }

     uint = wosize;
     uint = swap_word(uint);
     if ((n = fwrite(&uint, sizeof(int), 1, fp)) != 1)
        syserr("write_bin_patterns", fn, "wsize4b fwrite");
  }
  fclose(fp);
}
