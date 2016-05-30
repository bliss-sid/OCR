/*
# proc: segment_paragraph_blobls - processes a list of blobs extracted from a
# proc:             binary char image of a free-format handprinted paragraph
# proc:             into a list of character images.
*/

#include <blobls.h>
#include <segchars.h>
#include <defs.h>

/*******************************************************************/
segment_paragraph_blobls(blobls, pi_lists, pi_lens, nphrases, noise, nlens,
                         rlen, rn, w, h)
BLOBLS **blobls, **noise;
int ***pi_lists, **pi_lens, *nphrases, **nlens;
int *rlen, rn;
int w, h;
{
   int i, j, k;
   unsigned char *mcdata;
   BLOBLS *para, *line, *lnoise;
   int esw, charh, h_offset;

   /* estimate stroke width */
   esw = int_list_median(rlen, rn);

   /* estimate character height */
   blob_offsetof(h_offset, h);
   charh = prct_blobls_memb((*blobls), h_offset, HEIGHT_PRCT);

   /* build a phrase region map */
   if(!build_phrase_map(&mcdata, w, h, nphrases, (*blobls), esw, charh))
      return(FALSE);

   /* construct phrases from map */
   phrases_from_map(pi_lists, pi_lens, *nphrases, (*blobls), esw, charh,
                    mcdata, w, h);
   free(mcdata);

   build_blobls(&para, (*blobls)->num, BLOB_ALLS,
                (*blobls)->bpp, (*blobls)->b_g);
   build_blobls(noise, min(BLOB_CHUNKS,(*blobls)->num),
                BLOB_ALLS, (*blobls)->bpp, (*blobls)->b_g);
   malloc_int(nlens, *nphrases, "segment_paragraph_blobls : nlens");

   /* compose and split blobs in each phrase list into characters */
   k = 0;
   for(i = 0; i < *nphrases; i++){
      pi_list_to_blobls(&line, (*blobls), (*pi_lists)[i], (*pi_lens)[i]);

      blobs2alphas8(&line, &lnoise, esw);
      
      /* add line blobs to para */
      for(j = 0; j < line->num; j++, k++){
         append_blobls(para, line->blobs[j]);
         /* reindex pi_list */
         ((*pi_lists)[i])[j] = k;
      }
      /* store new line length */
      (*pi_lens)[i] = line->num;

      /* add line noise blobs to noise */
      for(j = 0; j < lnoise->num; j++)
         append_blobls(*noise, lnoise->blobs[j]);
      (*nlens)[i] = lnoise->num;
      free(line->blobs);
      free(line);
      free(lnoise->blobs);
      free(lnoise);
   }

   free((*blobls)->blobs);
   free(*blobls);
   *blobls = para;
   return(TRUE);
}
