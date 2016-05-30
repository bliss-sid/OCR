/*
# proc: add_pi_list - adds blob index to specified list dynamically reallocating the
# proc:               index list if necessary.
# proc: pi_list_to_blobls - converts the indirection in a list of blob indices
# proc:                     into a list of blobs.
*/

#include <blobls.h>
#include <phrase.h>

/***************************************************************************/
add_pi_list(pi_list, pi_len, pi_alen, item)
int **pi_list, *pi_len, *pi_alen;
int item;
{
   if((*pi_len >= *pi_alen)){
      *pi_alen += PHRASE_LEN_CHUNKS;
      realloc_int(pi_list, *pi_alen, "add_pi_list : pi_list");
   }
   (*pi_list)[*pi_len] = item;
   (*pi_len)++;
}

/***************************************************************************/
pi_list_to_blobls(line, blobls, pi_list, pi_len)
BLOBLS **line, *blobls;
int *pi_list, pi_len;
{
   int i;

   build_blobls(line, pi_len, BLOB_ALLS, blobls->bpp, blobls->b_g);
   for(i = 0; i < pi_len; i++)
      append_blobls(*line, blobls->blobs[pi_list[i]]);
}
