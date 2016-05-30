/*
# proc: is_noise_blob - determines if blob is small enough to be noise based on the
# proc:                 estimated stoke width.
# proc: process_noise_blobs - small blobs are merges back with larger blobs that are
# proc:                 sufficiently close.
*/

#include <blobls.h>
#include <segchars.h>
#include <defs.h>

/************************************************************/
is_noise_blob(blob, esw)
BLOB *blob;
int esw;
{
   static int noise_a_thresh;
   static int oesw = -1;

   if(esw != oesw){
      noise_a_thresh = max(1, sround(esw * esw * SMALL_A_FCTR));
      oesw = esw;
   }

   if(blob->a < noise_a_thresh)
      return(TRUE);
    else
      return(FALSE);
}

/************************************************************/
process_noise_blobs(noise, blobls)
BLOBLS *noise, *blobls;
{
   int ni, bi;
   BLOB *nptr;

   /* if no original blobs, then done */
   if(blobls->num == 0)
      return;

   ni = noise->num - 1;
   while(ni >= 0){
      nptr = noise->blobs[ni];
      if((bi = find_first_blob_inside(nptr, blobls)) != NOT_FOUND){
         /* merge noise with blob it is inside */
         merge_among_blobls(noise, ni, blobls, bi);
      }
      ni--;
   }
}
