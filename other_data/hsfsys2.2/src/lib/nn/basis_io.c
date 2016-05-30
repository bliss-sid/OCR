/*
# proc: read_basis - get basis vectors, typically eigenvectors, from file
# proc:              whose format is also used for covariance matrices.
# proc: write_basis - write basis vectors to a file.
# proc:
*/


#include <stdio.h>
#include <string.h>
#include <sex.h>

/* xlen is the number of eigenvectors nEvect */
/* ylen is the length of the eigenvectors    */
/* ydim is the y dimension of the dap space  */
/*      that the eigenvectors are held in.   */

/* if ydim is given as zero no padding is performed */
/* positive ydim pads with zero out to the dap dimension */


read_basis(Evt_name, ylen, xlen, ydim, mean, stdv, base)
char	*Evt_name;
float   **base, **mean, **stdv;
int	*ylen, *xlen, ydim;
{
int	tdim, n, i, j;
FILE	*fp;
float	*ptr;
unsigned int *ptr2;

   if ((fp = fopen(Evt_name, "rb")) == NULL)
      fatalerr("read_basis", Evt_name, "fopen");

   if ((n = fread(xlen, sizeof(int  ),     1, fp)) != 1)
      fatalerr("read_basis", "num evects", "fread");
   *xlen = swap_word(*xlen);
   if ((n = fread(ylen, sizeof(int  ),     1, fp)) != 1)
      fatalerr("read_basis", "len evects", "fread");
   *ylen = swap_word(*ylen);


   malloc_flt(mean, *ylen, "read_basis : mean");
   malloc_flt(stdv, *ylen, "read_basis : stdv");

   if ((n = fread((*mean), sizeof(float), *ylen, fp)) != (*ylen))
      fatalerr("read_basis", "mean",       "fread");
   for(i = 0, ptr2 = (unsigned int *)(*mean); i < n; i++, ptr2++)
      *ptr2 = swap_word(*ptr2);


   if ((n = fread((*stdv), sizeof(float), *ylen, fp)) != (*ylen))
      fatalerr("read_basis", "stdv",       "fread");
   for(i = 0, ptr2 = (unsigned int *)(*stdv); i < n; i++, ptr2++)
      *ptr2 = swap_word(*ptr2);


   tdim = (ydim == 0) ? (*ylen)  :  ( ydim);
   calloc_flt(base, tdim*(*xlen), "read_basis : base");

   ptr = (*base);
   for (i = 0 ; i < *xlen; i++)
   {
      if ((n = fread(ptr, sizeof(float), (*ylen), fp)) != (*ylen))
         fatalerr("read_basis", "basis", "fread");
      for(j = 0, ptr2 = (unsigned int *)ptr; j < n; j++, ptr2++)
         *ptr2 = swap_word(*ptr2);
      ptr += tdim;
   }
   fclose(fp);
}

write_basis(Evt_name, ylen, xlen, ydim, mean, stdv, base)
char	*Evt_name;
float   *mean, *stdv, *base;
int	 ylen, xlen, ydim;
{
FILE	*fp;
int	 n, i, j;
float   *ptr;
unsigned int uint, *ptr2;

  if ((fp = fopen(Evt_name, "wb")) == NULL)
    syserr("write_basis", Evt_name, "fopen");

  uint = xlen;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int  ),    1, fp)) != 1)
    syserr("write_basis", "num evects", "fwrite");

  uint = ylen;
  uint = swap_word(uint);
  if ((n = fwrite(&uint, sizeof(int  ),    1, fp)) != 1)
    syserr("write_basis", "len evects", "fwrite");

  for(i = 0, ptr2 = (unsigned int *)mean; i < ylen; i++, ptr2++)
     *ptr2 = swap_word(*ptr2);
  if ((n = fwrite(mean,  sizeof(float), ylen, fp)) != ylen)
    syserr("write_basis", "mean", "fwrite");

  for(i = 0, ptr2 = (unsigned int *)stdv; i < ylen; i++, ptr2++)
     *ptr2 = swap_word(*ptr2);
  if ((n = fwrite(stdv,  sizeof(float), ylen, fp)) != ylen)
    syserr("write_basis", "stdv", "fwrite");

  ptr = base;
  for (i = 0 ; i < xlen ; i++)
  {
     for(j = 0, ptr2 = (unsigned int *)ptr; j < ylen; j++, ptr2++)
        *ptr2 = swap_word(*ptr2);
     if ((n = fwrite(ptr, sizeof(float), ylen, fp)) != ylen)
        syserr("write_basis", "base", "fwrite");
     ptr += ydim;
  }
  fclose(fp);
}
