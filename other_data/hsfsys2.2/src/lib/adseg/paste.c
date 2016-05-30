/*
# proc: paste_top_strokes_blobls - loops through a blob list and puts top
# proc:                    horizontal strokes back on characters such as 5s.
# proc: paste_alpha_tops - merges the tops of alphabetic characters with their
# proc:                    corresponing bodies.
# proc:
*/

#include <blobls.h>

/************************************************************/
paste_top_strokes_blobls(blobls, esw, charh)
BLOBLS *blobls;
int esw, charh;
{
   int cur_i, next_i;

   cur_i = 0;
   next_i = 1;

   while(next_i < blobls->num){
      if(is_top_of_5(blobls->blobs[cur_i], blobls->blobs[next_i],
                     esw, charh)){
         merge_within_blobls(blobls, next_i, cur_i);
      }
      cur_i++;
      next_i++;
   }
}

/************************************************************/
paste_alpha_tops(blobls, esw, charh)
BLOBLS *blobls;
int esw, charh;
{
   int cur_i, next_i;

   cur_i = 0;
   next_i = 1;

   while(next_i < blobls->num){
      if(is_top_of_alpha(blobls->blobs[cur_i], blobls->blobs[next_i],
                     esw, charh)){
         merge_within_blobls(blobls, next_i, cur_i);
      }
      cur_i++;
      next_i++;
   }
}
