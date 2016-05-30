/*
# proc: segment_chars8 - takes a binary charimage of a field and segments it into
# proc:                  characters by carefully compositing and spliting blobs.
# proc: blob2chars8 - takes a list of blobs and the run lengths from a field and
# proc:               constructs single character images.
# proc: compose_chars8 - takes a list of blobs and attempts to reconstruct characters that
# proc:               are fragmented, place dots back on i's and j's, and identify other
# proc:                  small blobs as noise.
# proc: split_chars8 - takes a list of blobs and attempts to detect and split multiple
# proc:                touching characters.
# proc: blobs2alphas8 - processes a list of blobs into a list of alphabetic
# proc:                 characters.
# proc: compose_alphas8 - merge blobs of characters pieces into composite single
# proc:                   characters.
# proc: split_alphas8 - takes a list of blobs and attempts to detect and split multiple
# proc:                touching alphabetic characters.
*/

#include <blobls.h>
#include <segchars.h>
#include <multsort.h>
#include <defs.h>

/************************************************************/
segment_chars8(blobls, noisels, cdata, w, h)
BLOBLS **blobls, **noisels;
unsigned char *cdata;
int w, h;
{
   int *rlen, rn, ra;

   /* get list of connected components from field image */
   segblobs8rlen(blobls, BLOB_ALLS, cdata, w, h, &rlen, &rn, &ra);

   /* construct single characters from components */
   blobs2chars8(blobls, noisels, rlen, rn);
   free(rlen);
}

/************************************************************/
blobs2chars8(blobls, noisels, rlen, rn)
BLOBLS **blobls, **noisels;
int *rlen, rn;
{
   int i, esw, charh;
   int h_offset, max_i;
   float dlm;

   /* estimate character stroke width */
   esw = int_list_median(rlen, rn);

   /* allocate space to hold very small noise blobs */
   build_blobls(noisels, min(BLOB_CHUNKS, (*blobls)->num), BLOB_ALLS,
                (*blobls)->bpp, (*blobls)->b_g);

   /* take dots and stroke fragments and compose characters */
   compose_chars8(esw, blobls, (*noisels));

   /* get character height estimate from processed blobs */
   blob_offsetof(h_offset, h);
   find_first_max_blob_memb((*blobls), h_offset, &max_i, &charh);

   /* slope of touching char detector */
   dlm = -HVB_PT/HVW_PT;

   /* detect and split touching characters */
   split_chars8(esw, charh, dlm, HVW_PT, 0.0, blobls, (*noisels));

   /* get new character height estimate from processed blobs */
   find_first_max_blob_memb((*blobls), h_offset, &max_i, &charh);

   /* put the tops back on fives for example */
   paste_top_strokes_blobls(*blobls, esw, charh);

   /* get new character height estimate from processed blobs */
   find_first_max_blob_memb((*blobls), h_offset, &max_i, &charh);

   /* remove small punctuation-sized blobs */
   i = 0;
   while(i < (*blobls)->num){
      if(is_smallpunct_blob((*blobls)->blobs[i], esw, charh)){
         append_blobls((*noisels), (*blobls)->blobs[i]);
         remove_blobls(*blobls, i);
      }
      else
         i++;
   }
}

/************************************************************/
compose_chars8(esw, blobls, noisels)
int esw;
BLOBLS **blobls, *noisels;
{
   int ci, ni, i;
   int max_i, eh;
   int *laps_i, *laps_d, nlaps, alaps;
   int large_thresh;
   BLOBLS *dots, *sblobls;
   BLOB *cptr;
   int h_offset, cx_offset;

   /* if no original blobs, then done */
   if((*blobls)->num == 0)
      return;

   /* initial size of allocation */
   alaps =  min(BLOB_CHUNKS, (*blobls)->num);

   /* allocate space to hold larger dot-sized blobs */
   build_blobls(&dots, alaps, BLOB_ALLS, (*blobls)->bpp, (*blobls)->b_g);

   /* allocate space to hold overlapping blob indices */
   malloc_int(&laps_i, alaps, "compose_chars8 : malloc : laps_i");
   malloc_int(&laps_d, alaps, "compose_chars8 : malloc : laps_d");

   /* sort blob list on decreasing height, tallest blob to shortest blob */
   blob_offsetof(h_offset, h);
   sort_blobls_on_memb(&sblobls, DEC, (*blobls), h_offset);
   free((*blobls)->blobs);
   free((*blobls));
   (*blobls) = sblobls;

   /* INITIALIZE THRESHOLDS */
   /* estimate character height from blob hieghts */
   find_first_max_blob_memb((*blobls), h_offset, &max_i, &eh);
   /* threshold for height of larger character stroke pieces */
   large_thresh = sround(CHAR_H_FCTR * eh);

   /* start at end of list (for efficiency of removing items) */
   /* with shortest blobs */
   ci = (*blobls)->num - 1;
   while(ci >= 0){
      cptr = (*blobls)->blobs[ci];
      /* is the blob the size of potential noise? */
      if(is_noise_blob(cptr, esw)){
         /* store in a separate list for processing later */
         append_blobls(noisels, cptr);
         /* remove very small blob from list */
         remove_blobls((*blobls), ci);
         ci--;
      }
      /* dot blob or puncutation ? */
      else if(is_dot_blob(cptr, esw)){
         /* store in a separate list for processing later */
         append_blobls(dots, cptr);
         /* remove dot-sized blob from list */
         remove_blobls((*blobls), ci);
         ci--;
      }
      /* substantial character stroke (not noise or dots) */
      else {
         /* check to see if blob should be merged to another */
         if((ci > 0) &&
	    ((ni = find_merge_blob(&laps_i, &laps_d, &nlaps, &alaps,
                                     (*blobls), ci, large_thresh, eh)) != NOT_FOUND)){
            merge_within_blobls((*blobls), ci, ni);
            /* with new merge, start testing from end of list */
            /* ??? may be inefficient to retest for noise and dots though ??? */
            /* noise and dot test are very cheap however */
            ci = (*blobls)->num - 1;
         }
         else
            ci--;
      }
   }

   /* sort remaining blob list left to right */
   blob_offsetof(cx_offset, cx);
   if((*blobls)->num != 0){
      sort_blobls_on_memb(&sblobls, INC, (*blobls), cx_offset);
      free((*blobls)->blobs);
      free((*blobls));
      (*blobls) = sblobls;
   }

   /* process all the very small blobs collected */
   process_noise_blobs(noisels, (*blobls));

   /* process all the dot-sized blobs collected */
   process_dot_blobs(esw, eh, dots, (*blobls));

   /* free the overlap buffers */
   free(laps_i);
   free(laps_d);

   /* append remaining dot blobs to noise list */
   for(i = 0; i < dots->num; i++)
      append_blobls(noisels, dots->blobs[i]);
   free(dots->blobs);
   free(dots);
}

/************************************************************/
split_chars8(esw, charh, dlm, dlx, dly, blobls, noisels)
int esw, charh;
float dlm, dlx, dly;
BLOBLS **blobls, *noisels;
{
   int i;
   BLOBLS *oblobls;
   BLOB *dup_blob();

   /* if no original blobs, then done */
   if((*blobls)->num == 0)
      return;

   build_blobls(&oblobls, (*blobls)->num, (*blobls)->flags,
                (*blobls)->bpp, (*blobls)->b_g);

   for(i = 0; i < (*blobls)->num; i++){
      if(detect_multi_char_blob((*blobls)->blobs[i], esw, charh, dlm, dlx, dly)){
         /* split the blob up into individual chars */
         split_touching_chars((*blobls)->blobs[i], esw, charh, dlm, dlx, dly,
                              &oblobls, noisels);
      }
      else{
         /* append the single char blob to output list */
         append_blobls(oblobls, dup_blob((*blobls)->blobs[i], (*blobls)->bpp));
      }
   }
   free_blobls(*blobls);
   *blobls = oblobls;
}

/************************************************************/
blobs2alphas8(blobls, noisels, esw)
BLOBLS **blobls, **noisels;
int esw;
{
   int charh, h_offset;
   float dlm;

   /* allocate space to hold very small noise blobs */
   build_blobls(noisels, min(BLOB_CHUNKS, (*blobls)->num), BLOB_ALLS,
                (*blobls)->bpp, (*blobls)->b_g);

   /* estimate character height for each phrase */
   blob_offsetof(h_offset, h);
   charh = prct_blobls_memb((*blobls), h_offset, HEIGHT_PRCT);

   /* take dots and stroke fragments and compose characters */
   compose_alphas8(esw, charh, blobls, (*noisels));

   /* slope of touching char detector */
   dlm = -ALPHA_HVB_PT/ALPHA_HVW_PT;

   /* detect and split touching characters */
   split_alphas8(esw, charh, dlm, ALPHA_HVW_PT, 0.0, blobls, (*noisels));

   /* put the tops back on chars */
   paste_alpha_tops(*blobls, esw, charh);
}

/************************************************************/
compose_alphas8(esw, charh, blobls, noisels)
int esw, charh;
BLOBLS **blobls, *noisels;
{
   int ci, ni, i;
   int *laps_i, *laps_d, nlaps, alaps;
   int large_thresh;
   BLOBLS *dots, *sblobls;
   BLOB *cptr;
   int h_offset, cx_offset;
   int hthresh, onum;

   /* if no original blobs, then done */
   if((*blobls)->num == 0)
      return;

   /* initial size of allocation */
   alaps =  min(BLOB_CHUNKS, (*blobls)->num);

   /* allocate space to hold larger dot-sized blobs */
   build_blobls(&dots, alaps, BLOB_ALLS, (*blobls)->bpp, (*blobls)->b_g);

   /* allocate space to hold overlapping blob indices */
   malloc_int(&laps_i, alaps, "compose_alphas8 : malloc : laps_i");
   malloc_int(&laps_d, alaps, "compose_alphas8 : malloc : laps_d");

   /* sort blob list on decreasing height, tallest blob to shortest blob */
   blob_offsetof(h_offset, h);
   sort_blobls_on_memb(&sblobls, DEC, (*blobls), h_offset);
   free((*blobls)->blobs);
   free((*blobls));
   (*blobls) = sblobls;

   /* threshold for height of larger character stroke pieces */
   large_thresh = sround(CHAR_H_FCTR * charh);

   /* start at end of list (for efficiency of removing items) */
   /* with shortest blobs */
   ci = (*blobls)->num - 1;
   while(ci >= 0){
      cptr = (*blobls)->blobs[ci];
      /* is the blob the size of potential noise? */
      if(is_noise_blob(cptr, esw)){
         /* store in a separate list for processing later */
         append_blobls(noisels, cptr);
         /* remove very small blob from list */
         remove_blobls((*blobls), ci);
         ci--;
      }
      /* dot blob or puncutation ? */
      else if(is_dot_blob(cptr, esw)){
         /* store in a separate list for processing later */
         append_blobls(dots, cptr);
         /* remove dot-sized blob from list */
         remove_blobls((*blobls), ci);
         ci--;
      }
      /* substantial character stroke (not noise or dots) */
      else {
         /* check to see if blob should be merged to another */
         if((ci > 0) &&
	    ((ni = find_merge_blob(&laps_i, &laps_d, &nlaps, &alaps,
                        (*blobls), ci, large_thresh, charh)) != NOT_FOUND)){
            merge_within_blobls((*blobls), ci, ni);
            /* with new merge, start testing from end of list */
            /* ??? may be inefficient to retest for noise and dots though ??? */
            /* noise and dot test are very cheap however */
            ci = (*blobls)->num - 1;
         }
         else
            ci--;
      }
   }

   /* sort remaining blob list left to right */
   blob_offsetof(cx_offset, cx);
   if((*blobls)->num != 0){
      sort_blobls_on_memb(&sblobls, INC, (*blobls), cx_offset);
      free((*blobls)->blobs);
      free((*blobls));
      (*blobls) = sblobls;
   }

   /* process all the very small blobs collected */
   process_noise_blobs(noisels, (*blobls));

   /* process all the dot-sized blobs collected */
   process_dot_blobs(esw, charh, dots, (*blobls));

   /* free the overlap buffers */
   free(laps_i);
   free(laps_d);

   /* append remaining dot blobs back to main blob list or to noise list */
   hthresh = esw<<1;
   onum = (*blobls)->num;
   for(i = 0; i < dots->num; i++){
      if(dots->blobs[i]->h < hthresh)
         append_blobls(noisels, dots->blobs[i]);
      else
         append_blobls((*blobls), dots->blobs[i]);
   }
   free(dots->blobs);
   free(dots);

   /* sort remaining blob list left to right */
   if(((*blobls)->num != 0) && (onum != (*blobls)->num)){
      sort_blobls_on_memb(&sblobls, INC, (*blobls), cx_offset);
      free((*blobls)->blobs);
      free((*blobls));
      (*blobls) = sblobls;
   }
}

/************************************************************/
split_alphas8(esw, charh, dlm, dlx, dly, blobls, noisels)
int esw, charh;
float dlm, dlx, dly;
BLOBLS **blobls, *noisels;
{
   int i;
   BLOBLS *oblobls;
   BLOB *dup_blob();

   /* if no original blobs, then done */
   if((*blobls)->num == 0)
      return;

   build_blobls(&oblobls, (*blobls)->num, (*blobls)->flags,
                (*blobls)->bpp, (*blobls)->b_g);

   for(i = 0; i < (*blobls)->num; i++){
      if(detect_multi_alpha_blob((*blobls)->blobs[i], esw, charh, dlm, dlx, dly)){
         /* split the blob up into individual chars */
         split_touching_alphas((*blobls)->blobs[i], esw, charh, dlm, dlx, dly,
                              &oblobls, noisels);
      }
      else{
         /* append the single char blob to output list */
         append_blobls(oblobls, dup_blob((*blobls)->blobs[i], (*blobls)->bpp));
      }
   }
   free_blobls(*blobls);
   *blobls = oblobls;
}
