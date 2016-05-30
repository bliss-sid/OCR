/*
# proc: build_phrase_map - generate an image map of the central mass of each line of
# proc:                    of handprint.
# proc: phrases_from_map - sort blobs into phrase lines using the map.
# proc:
# proc: norm_small_tall_lists - sort blobs into categories of normal, small, and tall.
# proc:
# proc: pi_lines_from_map - generate lists of blob indices sorting the blobs
# prod:                     into lines.
# proc: should_split_tall - test to determine if a tall character should be split
# proc:                     across multiple lines of handprint.
# proc: tst_phrase_overlap - tests the amount of character overlap with a line in the
# proc:                      phrase map.
# proc: split_across_lines - horizontally splits a character across multiple lines.
# proc:
# proc: find_closest_line_in_map - finds the closest line in the phrase map to a
# proc:                      specified blob.
# proc: hist_blob_colors - generates a histogram from the black pixels in a blob
# proc:                    that overlap 0 or more lines in the phrase map.
# proc: draw_phrase_map - actually construct the phrase map image.
# proc:
# proc: short_phrases_to_problems - removes and appends too short phrases to the
# proc:                   problem list.
*/

#include <stdio.h>
#include <blobls.h>
#include <ihead.h>
#include <mis.h>
#include <phrase.h>
#include <segchars.h>
#include <defs.h>

/***************************************************************************/
/*     Phase I - Build the phrase region map                               */
/***************************************************************************/
build_phrase_map(mcdata, w, h, nphrases, blobls, esw, charh)
unsigned char **mcdata;
int w, h, *nphrases;
BLOBLS *blobls;
int esw, charh;
{
   int **pi_lists, *pi_lens, sum;
   int *problems, nprob, aprob;
   int *nis, nnum, *sis, snum, *tis, tnum;
   int *mxs, *mys;
   int i,j;
   float hmean;

   /* extract "normal" sized blobs, pruning small and tall ones */
   norm_small_tall_lists(&nis, &nnum, &sis, &snum, &tis, &tnum, blobls, esw, charh);
   /* throw away too short and too tall blobs */
   free(sis);
   free(tis);

   /* if no normal sized blobs, then paragraph is empty */
   if(nnum == 0){
      free(nis);
      return(FALSE);
   }

   malloc_int(&mxs, nnum, "build_phrase_map : mxs");
   malloc_int(&mys, nnum, "build_phrase_map : mys");
   sum = 0;
   for(i = 0; i < nnum; i++){
      mxs[i] = blobls->blobs[nis[i]]->cx;
      mys[i] = blobls->blobs[nis[i]]->cy;
      sum += blobls->blobs[nis[i]]->h;
   }
   hmean = sum / (float)nnum;

   /* build phrase lists from "normal" size blobs */
   build_pi_lists_Rel2(&pi_lists, &pi_lens, nphrases, &problems, &nprob,
                   hmean, mxs, mys, nnum);
   free(mxs);
   free(mys);

   /* move points in too short phrases to problem list */
   aprob = nprob;
   short_phrases_to_problems(&problems, &nprob, &aprob,
                             pi_lists, pi_lens, nphrases);
   /* throw away problem blobs */
   free(problems);

   /* if no phrases remain, then paragraph is empty */
   if(*nphrases == 0){
      free(nis);
      free(pi_lists);
      free(pi_lens);
      return(FALSE);
   }

   /* remove 1 level of indirection */
   for(i = 0; i < *nphrases; i++){
      for(j = 0; j < pi_lens[i]; j++){
         (pi_lists[i])[j] = nis[(pi_lists[i])[j]];
      } 
   } 
   free(nis);

   /* draw phrase region map */
   draw_phrase_map(mcdata, w, h, blobls, pi_lists, pi_lens, *nphrases);

   for(i = 0; i < *nphrases; i++)
      free(pi_lists[i]);
   free(pi_lists);
   free(pi_lens);
   return(TRUE);
}

/***************************************************************************/
/*     Phase II - Construct phrases using the region map                   */
/***************************************************************************/
phrases_from_map(pi_lists, pi_lens, nphrases, blobls, esw, charh,
                    mcdata, w, h)
int ***pi_lists, **pi_lens, nphrases;
BLOBLS *blobls;
int esw, charh;
unsigned char *mcdata;
int w, h;
{

   int i, *pi_alens, alen;

   malloc_dbl_int_l1(pi_lists, nphrases, "phrases_from_map : pi_lists");
   malloc_int(&pi_alens, nphrases, "phrases_from_map : pi_alens");
   alen = PHRASE_LEN_CHUNKS;
   for(i = 0; i < nphrases; i++){
      malloc_int(&((*pi_lists)[i]), alen, "phrases_from_map : pi_lists[i]");
      pi_alens[i] = alen;
   }
   calloc_int(pi_lens, nphrases,  "phrases_from_map : pi_lens");

   /* reconstruct lines from phrase region map */
   pi_lines_from_map(*pi_lists, *pi_lens, pi_alens, nphrases, blobls, esw, charh,
                      mcdata, w, h);

   free(pi_alens);
}

/***************************************************************************/
norm_small_tall_lists(nis, nnum, sis, snum, tis, tnum, blobls, esw, charh)
BLOBLS *blobls;
int **nis, *nnum, **sis, *snum, **tis, *tnum;
int esw, charh;
{
   int i, too_tall, head;

   malloc_int(nis, blobls->num, "norm_small_tall_lists : nis");
   malloc_int(sis, blobls->num, "norm_small_tall_lists : sis");
   malloc_int(tis, blobls->num, "norm_small_tall_lists : tis");

   /* prune out too small blobs */
   *nnum = 0;
   *snum = 0;
   for(i = 0; i < blobls->num; i++){
      /* store non-dot sized blob height */
      if(!is_dot_blob(blobls->blobs[i], esw)){
         (*nis)[*nnum] = i;
         (*nnum)++;
      }
      /* otherwise skip too small blob */
      else
         (*sis)[(*snum)++] = i;
   }

   /* prune out too tall blobs */
   too_tall = sround(charh * TALL_FACTOR);
   head = 0;
   *tnum = 0;
   for(i = 0; i < *nnum; i++){
      if(blobls->blobs[(*nis)[i]]->h < too_tall){
         /* if not copying itself */
         if(head != i)
            (*nis)[head] = (*nis)[i];
         head++;
      }
      /* otherwise skip too tall blob */
      else
         (*tis)[(*tnum)++] = (*nis)[i];
   }
   *nnum = head;
}

/***************************************************************************/
pi_lines_from_map(pi_lists, pi_lens, pi_alens, nphrases, blobls, esw, charh,
                  mcdata, w, h)
int **pi_lists, *pi_lens, *pi_alens, nphrases;
BLOBLS *blobls;
unsigned char *mcdata;
int w, h, esw, charh;
{
   int i, map_i, max_v, pi, *mxs;
   int *color_hist, hnum, hsize;
   int *xphrases, nx, lastblob;

   hnum = nphrases + 1;
   malloc_int(&color_hist, hnum, "pi_lines_from_map : color_hist");
   hsize = hnum * sizeof(int);
   malloc_int(&xphrases, nphrases, "pi_lines_from_map : xphrases");

   lastblob = blobls->num;
   for(i = 0; i < lastblob; i++){
      memset(color_hist, '\0', hsize);
      hist_blob_colors(color_hist, hnum, blobls->blobs[i], mcdata, w, h);


      /* if blob is too tall */
      if((blobls->blobs[i]->h > charh) &&
         (should_split_tall(xphrases, &nx, blobls->blobs[i], color_hist, hnum,
                               mcdata, w, h))){
         split_across_lines(i, blobls, esw, pi_lists, pi_lens, pi_alens, nphrases,
                            mcdata, w, h, xphrases, nx);
      }
      /* if tall but not split, then treat as any other blob */
      else{
         /* find max phrase region overlap */
         find_first_max_forward(color_hist, 0, hnum, &map_i, &max_v);
         /* if no overlap with a phrase region ... */
         if(max_v == 0){
            /* then assign to closest */
            if((map_i = find_closest_line_in_map(blobls->blobs[i],
                             mcdata, w, h)) == NOT_FOUND)
               fatalerr("pi_lines_from_map", "no line found in map", NULL);
         }
         pi = map_i - 1;

         /* add blob to appropriate pi_list */
         add_pi_list(&(pi_lists[pi]), &(pi_lens[pi]), &(pi_alens[pi]), i);
      }
   }
   free(color_hist);
   free(xphrases);

   /* sort the blobs in each line on cx's */
   malloc_int(&mxs, blobls->num, "pi_lines_from_map : mxs");
   for(i = 0; i < blobls->num; i++)
      mxs[i] = blobls->blobs[i]->cx;
   for(i = 0; i < nphrases; i++){
      sort_pi_list_on_x(pi_lists[i], pi_lens[i], mxs, blobls->num);
   }
   free(mxs);
}

/***************************************************************************/
should_split_tall(xphrases, nx, blob, color_hist, hnum, mcdata, w, h)
BLOB *blob;
int *color_hist, hnum;
unsigned char *mcdata;
int w, h;
int *xphrases, *nx;
{
   int i, max1 = -1, max2 = -1;
   float pb;

   *nx = 0;
   for(i = 1; i < hnum; i++){
      if(color_hist[i] != 0){
         xphrases[*nx] = i-1;
         (*nx)++;
         if(color_hist[i] > max1){
            max2 = max1;
            max1 = color_hist[i];
         }
         else if(color_hist[i] > max2){
            max2 = color_hist[i];
         }
      }
   }
   if(*nx < 2)
      return(FALSE);

   pb = max2 / (float)blob->pixcnt;
   if(pb < MIN_2ND_OVER) 
      return(FALSE);

   if(tst_phrase_overlap(xphrases[(*nx-1)]+1, blob, mcdata, w, h, PRCT_OVERLAP))
      return(TRUE);
   else{
      /* remove last overlapping phrase */
      (*nx)--;
      if(*nx < 2)
         return(FALSE);
      else
         return(TRUE);
   }
}

/***************************************************************************/
tst_phrase_overlap(pi, blob, mcdata, w, h, thresh)
int pi;
BLOB *blob;
unsigned char *mcdata;
int w, h;
float thresh;
{
   int x, y, found, sy, ty, by, pwidth, bover;
   unsigned char *sbptr, *bptr, *smptr, *mptr;
   int min_x = -1, min_y = -1, max_x = -1, max_y = -1;
   float pover;

   /* find range of blob overlap with phrase region */
   /* start by finding top of range */
   bptr = blob->data;
   smptr = mcdata + (blob->y1 * w) + blob->x1;
   for(y = 0, found = 0; (y < blob->h) && (!found); y++){
      mptr = smptr;
      for(x = 0; x < blob->w; x++){
         if(*bptr && (*mptr == pi)){
            min_y = y;
            min_x = x;
            found = 1;
            break;
         }
         bptr++;
         mptr++;
      }
      smptr += w;
   }

   if(!found)
      return(FALSE);

   /* now find bottom of overlap range */
   sbptr = blob->data + ((blob->h-1) * blob->w);
   smptr = mcdata + (blob->y2 * w) + blob->x1;
   for(y = blob->h-1, found = 0; (y >= 0) && (!found); y--){
      mptr = smptr;
      bptr = sbptr;
      for(x = 0; x < blob->w; x++){
         if(*bptr && (*mptr == pi)){
            max_y = y;
            max_x = x;
            found = 1;
            break;
         }
         bptr++;
         mptr++;
      }
      smptr -= w;
      sbptr -= blob->w;
   }

   if(!found)
      fatalerr("tst_phrase_overlap",
               "map pixel not found when locating bottom of overlap", NULL);

   bover = max_y - min_y + 1;

   /* find phrase band width */
   sy = blob->y1 + max_y;
   smptr = mcdata + (sy * w) + blob->x1 + max_x;
   ty = sy;
   mptr = smptr;
   while((ty >= 0) && (*mptr == pi)){
      ty--;
      mptr-=w;
   }
   by = sy;
   mptr = smptr;
   while((by < h) && (*mptr == pi)){
      by++;
      mptr+=w;
   }
   pwidth = by - ty + 1;

   /* if percent overlap large enough */
   pover = bover / (float)pwidth;

   if(pover < thresh)
      return(FALSE);
   else{
      return(TRUE);
   }
}

/***************************************************************************/
split_across_lines(bi, blobls, esw, pi_lists, pi_lens, pi_alens, nphrases,
                         mcdata, w, h, xphrases, nx)
BLOBLS *blobls;
int bi, esw;
int **pi_lists, *pi_lens, *pi_alens, nphrases;
unsigned char *mcdata;
int w, h;
int *xphrases, nx;
{
   unsigned char *rcdata, *mallocate_image();
   BLOB *lblob, *rblob, *sblob;
   int ncuts, rw, rh;
   int pi, dx, cx;
   int *px, *py, pn, pa;
   int newx1, newy1, nbi;

   pi = 0;
   rw = blobls->blobs[bi]->h;
   rh = blobls->blobs[bi]->w;

   /* initialize x cut point */
   ncuts = nx - 1;
   dx = rw / nx;
   cx = dx;

   /* if cx is zero then nothing to cut */
   if(cx == 0){
      /* this should't happen, but just in case */
      /* assign blob to first phrase */
      add_pi_list(&(pi_lists[xphrases[pi]]), &(pi_lens[xphrases[pi]]),
                  &(pi_alens[xphrases[pi]]), bi);
      return;
   }

   /* rotate original blob image onto its side for cutting */
   /* creating a new blob */
   rcdata = mallocate_image(rw, rh, 8);
   grayscale_rotate_n90(blobls->blobs[bi]->data, rcdata,
                        blobls->blobs[bi]->w, blobls->blobs[bi]->h);
   build_blob(&rblob, blobls->flags,
              rcdata, blobls->blobs[bi]->x1, blobls->blobs[bi]->y1, rw, rh);

   /* initially set working blob to rotated blob */
   sblob = rblob;

   /* cut the blob along path starting at cx */
   find_cut_path(&px, &py, &pn, &pa, sblob->data, cx, esw, sblob->w, sblob->h);
   split_blob_along_path(&lblob, &rblob, px, py, pn, sblob);
   free(px);
   free(py);

   /* if left blob cut ... */
   if(lblob != (BLOB *)NULL){
      /* rotate cut piece back */
      rw = lblob->h;
      rh = lblob->w;
      rcdata = mallocate_image(rw, rh, 8);
      grayscale_rotate_90(lblob->data, rcdata, lblob->w, lblob->h);
      free_blob(lblob);

      /* replace current blob image with left cut piece */
      blobls->blobs[bi]->x1 = sblob->x1 + sblob->h - lblob->y1 - lblob->h;
      blobls->blobs[bi]->y1 = sblob->y1 + lblob->x1;
      blobls->blobs[bi]->w = rw;
      blobls->blobs[bi]->h = rh;
      free(blobls->blobs[bi]->data);
      blobls->blobs[bi]->data = rcdata;
      compute_blob_stats(blobls->blobs[bi], blobls->flags);

      /* add modified blob index to first phrase */
      add_pi_list(&(pi_lists[xphrases[pi]]), &(pi_lens[xphrases[pi]]),
                  &(pi_alens[xphrases[pi]]), bi);

   }
   /* if left not cut, then skip current phrase split */
   else{
      /* advance cx to next split */
      if(rblob != (BLOB *)NULL)
         cx += dx;
   }

   /* if right cut piece exists, then continue to make cuts */
   for(pi = 1; (pi < ncuts) && (rblob != (BLOB *)NULL); pi++){
      /* don't let cx run off remaining right image */
      cx = min(cx, rblob->w-1);
      /* if cx is zero then nothing to cut */
      if(cx <= 0){
         free_blob(sblob);
         /* assign right to current phrase */
         add_pi_list(&(pi_lists[xphrases[pi]]), &(pi_lens[xphrases[pi]]),
                     &(pi_alens[xphrases[pi]]), bi);
         break;
      }

      /* update right blob and set to working blob for next cut */
      newx1 = sblob->x1 + sblob->h - rblob->y1 - rblob->h;
      newy1 = sblob->y1 + rblob->x1;
      rblob->x1 = newx1;
      rblob->y1 = newy1;
      free_blob(sblob);
      sblob = rblob;

      /* cut working blob along path starting at cx */
      find_cut_path(&px, &py, &pn, &pa, sblob->data, cx, esw, sblob->w, sblob->h);
      split_blob_along_path(&lblob, &rblob, px, py, pn, sblob);
      free(px);
      free(py);

      if(lblob != (BLOB *)NULL){
         /* rotate cut piece back */
         rw = lblob->h;
         rh = lblob->w;
         rcdata = mallocate_image(rw, rh, 8);
         grayscale_rotate_90(lblob->data, rcdata, lblob->w, lblob->h);

         /* replace left blob image with rotated cut piece */
         newx1 = sblob->x1 + sblob->h - lblob->y1 - lblob->h;
         newy1 = sblob->y1 + lblob->x1;
         lblob->x1 = newx1;
         lblob->y1 = newy1;
         lblob->w = rw;
         lblob->h = rh;
         free(lblob->data);
         lblob->data = rcdata;
         compute_blob_stats(lblob, blobls->flags);

         /* add new blob to blob list */
         nbi = blobls->num;
         append_blobls(blobls, lblob);

         /* add new blob index to current phrase */
         add_pi_list(&(pi_lists[xphrases[pi]]), &(pi_lens[xphrases[pi]]),
                     &(pi_alens[xphrases[pi]]), nbi);
      }
      /* if left not cut, then skip current phrase split */
      /* and advance cx to next split */
      else{
         if(rblob != (BLOB *)NULL)
            cx += dx;
      }
   }

   /* if final cut piece exists ... */
   if(rblob != (BLOB *)NULL){
      pi = ncuts;

      /* rotate last cut piece back */
      rw = rblob->h;
      rh = rblob->w;
      rcdata = mallocate_image(rw, rh, 8);
      grayscale_rotate_90(rblob->data, rcdata, rblob->w, rblob->h);

      /* replace blob image with rotated cut piece */
      newx1 = sblob->x1 + sblob->h - rblob->y1 - rblob->h;
      newy1 = sblob->y1 + rblob->x1;
      rblob->x1 = newx1;
      rblob->y1 = newy1;
      rblob->w = rw;
      rblob->h = rh;
      free(rblob->data);
      rblob->data = rcdata;
      compute_blob_stats(rblob, blobls->flags);

      /* add new blob to blob list */
      nbi = blobls->num;
      append_blobls(blobls, rblob);

      /* add new blob index to current phrase */
      add_pi_list(&(pi_lists[xphrases[pi]]), &(pi_lens[xphrases[pi]]),
                     &(pi_alens[xphrases[pi]]), nbi);
   }
   /* free up final working blob */
   free_blob(sblob);
}

/***************************************************************************/
find_closest_line_in_map(blob, mcdata, w, h)
BLOB *blob;
unsigned char *mcdata;
int w, h;
{
   int y, up, down, ufound, dfound, umap, dmap;
   unsigned char *smptr, *mptr;

   smptr = mcdata + (blob->cy * w) + blob->cx;

   if(*smptr)
      return(*smptr);

   /* search upwards */
   ufound = FALSE;
   for(y = blob->cy-1, up = 1, mptr = smptr-w; y >= 0; y--, up++, mptr-=w){
      if(*mptr){
         ufound = TRUE;
         umap = *mptr;
         break;
      } 
   }
   /* search downwards */
   dfound = FALSE;
   for(y = blob->cy+1, down = 1, mptr = smptr+w; y < h; y++, down++, mptr+=w){
      if(*mptr){
         dfound = TRUE;
         dmap = *mptr;
         break;
      } 
   }
   if(!ufound && !dfound)
      return(NOT_FOUND);
   if(ufound && dfound){
      if(up < down)
         return(umap);
      else
         return(dmap);
   }
   else if (ufound)
      return(umap);
   else
      return(dmap);
}

/***************************************************************************/
hist_blob_colors(color_hist, hnum, blob, mcdata, w, h)
int *color_hist, hnum;
BLOB *blob;
unsigned char *mcdata;
int w, h;
{
   int x, y;
   unsigned char *bptr, *smptr, *mptr;

   bptr = blob->data;
   smptr = mcdata + (blob->y1 * w) + blob->x1;
   for(y = 0; y < blob->h; y++){
      mptr = smptr;
      for(x = 0; x < blob->w; x++){
         if(*bptr && (*mptr != 0))
            color_hist[*mptr]++;
         bptr++;
         mptr++;
      }
      smptr += w;
   }
}

/***************************************************************************/
draw_phrase_map(odata, w, h, blobls, pi_lists, pi_lens, nphrases)
unsigned char **odata;
int w, h;
BLOBLS *blobls;
int **pi_lists, *pi_lens, nphrases;
{
   int i, j, k, *tys, *bys, max_len, max_i;
   int lx, tly, rx, try, *txlist, *tylist, tlen, talen;
   int bly, bry, *bxlist, *bylist, blen, balen;
   int pix, last, x;
   unsigned char *allocate_image();

   if(nphrases >= 256)
      fatalerr("draw_phrase_map", "# of phrases exceeds 8-bit pixel value for map",
               NULL);

   *odata = allocate_image(w, h, 8);

   find_first_max_forward(pi_lens, 0, nphrases, &max_i, &max_len);
   malloc_int(&tys, max_len, "draw_phrase_map : tys");
   malloc_int(&bys, max_len, "draw_phrase_map : bys");

   talen = 0;
   balen = 0;
   for(i = 0; i < nphrases; i++){
      pix = i+1;
      for(j = 0; j < pi_lens[i]; j++){
         tys[j] = blobls->blobs[(pi_lists[i])[j]]->y1;
         bys[j] = blobls->blobs[(pi_lists[i])[j]]->y2;
      }
      three_smooth(tys, pi_lens[i]);
      three_smooth(bys, pi_lens[i]);

      tly = tys[0];
      bly = bys[0];
      lx = blobls->blobs[(pi_lists[i])[0]]->cx;

      drawvertline8(*odata, w, h, lx, tly, lx, bly, pix);
      for(j = 1; j < pi_lens[i]; j++){
         /* interpolate top line segment */
         try = tys[j];
         rx = blobls->blobs[(pi_lists[i])[j]]->cx;
         dx_line_alloc(lx, tly, rx, try, &txlist, &tylist, &tlen, &talen);

         /* interpolate bottom line segment */
         bry = bys[j];
         dx_line_alloc(lx, bly, rx, bry, &bxlist, &bylist, &blen, &balen);

         if(tlen != blen){
            fatalerr("draw_phrase_map", "length of interpolated segments not equal",
                     NULL);
         }
         for(k = 1; k < tlen; k++)
            drawvertline8(*odata, w, h, txlist[k], tylist[k],
                          bxlist[k], bylist[k], pix);
         tly = try;
         bly = bry;
         lx = rx;
      }

      tly = tys[0];
      bly = bys[0];
      lx = blobls->blobs[(pi_lists[i])[0]]->cx - 1;
      for(x = lx; x >= 0; x--){
         if(!tstndrawvertline8(*odata, w, h, x, tly, x, bly, pix))
            break;
      }
      last = pi_lens[i]-1;
      try = tys[last];
      bry = bys[last];
      rx = blobls->blobs[(pi_lists[i])[last]]->cx + 1;
      for(x = rx; x < w; x++){
         if(!tstndrawvertline8(*odata, w, h, x, try, x, bry, pix))
            break;
      }

   }

   free(tys);
   free(bys);

   if(talen != 0){
      free(txlist);
      free(tylist);
   }
   if(balen != 0){
      free(bxlist);
      free(bylist);
   }
}

/***************************************************************************/
short_phrases_to_problems(problems, nprob, aprob, pi_lists, pi_lens, nphrases)
int **problems, *nprob, *aprob;
int **pi_lists, *pi_lens, *nphrases;
{
   int i, j, aincr;

   aincr = max(TOO_SHORT, PHRASE_LEN_CHUNKS);
   i = 0;
   while(i < *nphrases){
      if(pi_lens[i] <= TOO_SHORT){
         if(((*nprob)+TOO_SHORT) >= (*aprob)){
            (*aprob) += aincr;
            realloc_int(problems, *aprob, "short_phrases_to_problems : problems");
         }
         for(j = 0; j < pi_lens[i]; j++){
            (*problems)[*nprob] = (pi_lists[i])[j];
            (*nprob)++;
         }
         remove_from_pi_lists(i, pi_lists, pi_lens, (*nphrases));
         (*nphrases)--;
      }
      else
         i++;
   }
}
