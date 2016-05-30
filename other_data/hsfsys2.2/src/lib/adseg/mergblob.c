/*
# proc: find_merge_blobs - determines if a blob should be merged with another nbring
# proc:                    blob based on horizontal overlap.
*/

#include <math.h>
#include <blobls.h>
#include <segchars.h>
#include <defs.h>

/************************************************************/
find_merge_blob(laps_i, laps_d, nlaps, alaps, blobls, ci, large_thresh, eh)
int **laps_i, **laps_d, *nlaps, *alaps;
BLOBLS *blobls;
int ci, large_thresh, eh;
{
   int ni;
   int li, ri;
   int max_i, max_d;
   int very_large;

   /* find all other blobs that overlap with current blob */
   blob_hori_overlaps(laps_i, laps_d, nlaps, alaps, blobls, ci);

   ni = NOT_FOUND;

   /* if no blob overlaps ... */
   if(*nlaps == 0)
      return(ni);

   /* if one blob overlaps ... */
   if(*nlaps == 1){
      ni = (*laps_i)[0];
   }
   /* if two blob overlaps ... */
   else if(*nlaps == 2){
      /* determin which nbr is left and right */
      if(blobls->blobs[(*laps_i)[0]]->x1 < blobls->blobs[(*laps_i)[1]]->x1){
         li = 0;
         ri = 1;
      }
      else{
         li = 1;
         ri = 0;
      }
      /* choose side with largest overlap, biasing the left */
      if(((*laps_d)[li] < (*laps_d)[ri]) &&
         (((*laps_d)[ri]/(double)(*laps_d)[li]) > OVER_FCTR))
         ni = (*laps_i)[ri];
      else
         ni = (*laps_i)[li];
   }
   /* if more than two blob ... */
   else if(*nlaps > 2){
      /* choose blob with largest overlap */
      find_first_max_forward((*laps_d), 0, *nlaps, &max_i, &max_d);
      ni = (*laps_i)[max_i];
   }

   very_large = sround(BIG_CHAR_H_FCTR * eh);
   /* if larger blob, handle more carefully */
   /* first check if nbring blobs are very tall and nearly the same height */
   if(((blobls->blobs[ci]->h >= very_large) && (blobls->blobs[ni]->h >= very_large) &&
       (min(blobls->blobs[ci]->h, blobls->blobs[ni]->h) /
          (float)max(blobls->blobs[ci]->h, blobls->blobs[ni]->h)) > SAME_PRCT) ||
      /* else check if current blob is tall and sufficiently close to nbr */
      (((blobls->blobs[ci]->h >= large_thresh) || (blobls->blobs[ni]->h >= large_thresh)) &&
       (fabs((blobls->blobs[ci]->cx - blobls->blobs[ni]->cx)/(double)eh) >= LARGE_FCTR)))
      ni = NOT_FOUND;

   return(ni);
}
