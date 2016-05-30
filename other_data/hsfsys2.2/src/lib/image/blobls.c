/*
# proc: build_blobls - allocates and initializes a list of blobs.
# proc:
# proc: alloc_blobls - allocates a list of blobs.
# proc:
# proc: free_blobls - deallocates a list of blobs including image data if not NULL.
# proc:
# proc: realloc_blobls - reallocates a list of blobs that has overflowed.
# proc:
# proc: append_blobls - adds a blob to the end of a blob list.
# proc:
# proc: merge_with_blobls - merges two blobs contained in a blob list and frees
# proc:                 up the location of the source blob.
# proc: merge_among_blobls - merges two blobs, each in a separate blob list, and frees
# proc:                 up the location of the source blob.
# proc: merge_blobs - composites two blobs back together according to their cut
# proc:               corrdinates from their parent image.
# proc: remove_blobls - removes a blob from its blob list at a specified location by
# proc:                 sliding (collapsing) the contents of the list down in memory.
# proc: init_blobls - sets up global blob list attributes.
# proc:
# proc: build_blob - allocates and initializes a blob given image data and attributes.
# proc:
# proc: dup_blob - returns a duplicate copy of the blob passed.
# proc:
# proc: update_blobls_stats - recomputes blob attributes specified by the flag passed
# proc:                       across all the blobs in the list.
# proc: compute_blob_stats - computes blob attributes specified by the flag passed.
# proc:
# proc: alloc_blob - allocates a blob structure without any allocation for image data.
# proc:
# proc: free_blob - deallocates a blob structure and its image data if not NULL.
# proc:
# proc: char2bin_blobls - converts all the blobs in a list containing binary char image
# proc:                   data to one bit per pixel data.
# proc: find_first_max_blob_memb - used in coordination with the blob_offsetof() macro,
# proc:           this routine will find the maximum value for a specific blob attribute.
# proc: compute_avr_blob_memb - used in coordination with the blob_offsetof() macro,
# proc:           this routine computes the average value for a specific blob attribute.
# proc: prct_blobls_memb - selects the specified percentile from the list of blob
# proc:             attributes.
# proc: median_blob_memb - finds median from the list of blob attributes.
# proc:
# proc: sort_blobls_on_memb - used in coordination with the blob_offsetof() macro,
# proc:             this routine sorts a list of blobs on a specific blob attribute.
# proc: is_blob_above - determines if one blob is above another.
# proc:
# proc: find_blob_nbrs - given a list of blobs sorted left-to-right, returns blobs to the
# proc:                  left and right of a given blob.
# proc: find_blob_ind_blob_nbrs - given a list of blobs and a sorted rank of the list,
# proc:                  returns blobs to the left and right of a given blob.
# proc: find_first_blob_inside - determines if a blob is inside one of the blobs
# proc:                          contained in a list.
# proc: blob_hori_overlaps - given a blob and a list of blobs, finds all those blobs
# proc:                 that horizontally overlap the first blob and measures and stores
# proc:                 the distance of the overlaps.
*/

#include <stdio.h>
#include <blobls.h>
#include <multsort.h>
#include <defs.h>

/*******************************************************************/
build_blobls(blobls, blob_alloc, flags, bpp, b_g)
BLOBLS **blobls;
int blob_alloc, flags, bpp, b_g;
{
   alloc_blobls(blobls, blob_alloc);
   init_blobls((*blobls), flags, bpp, b_g);
}

/*******************************************************************/
alloc_blobls(blobls, blob_alloc)
BLOBLS **blobls;
int blob_alloc;
{
   if(((*blobls) = (BLOBLS *)malloc(sizeof(BLOBLS))) == NULL)
      syserr("alloc_blobls", "malloc", "blobls");

   if(blob_alloc == 0)
      (*blobls)->blobs = (BLOB **)NULL;
   else
      if(((*blobls)->blobs = (BLOB **)malloc(blob_alloc * sizeof(BLOB *))) == NULL)
         syserr("alloc_blobls", "malloc", "blobs");

   (*blobls)->num = 0;
   (*blobls)->alloc = blob_alloc;
}

/*******************************************************************/
free_blobls(blobls)
BLOBLS *blobls;
{
   int i;

   if(blobls->blobs != (BLOB **)NULL){
      for(i = 0; i < blobls->num; i++){
        free_blob(blobls->blobs[i]);
      }
      free(blobls->blobs);
   }
   free(blobls);
}

/*******************************************************************/
realloc_blobls(blobls)
BLOBLS *blobls;
{
   blobls->alloc += BLOB_CHUNKS;
   if((blobls->blobs = (BLOB **)realloc(blobls->blobs,
                               blobls->alloc * sizeof(BLOB *))) == NULL)
      syserr("realloc_blobls", "realloc", "blob list");
}

/*******************************************************************/
append_blobls(blobls, blob)
BLOBLS *blobls;
BLOB *blob;
{
   if(blobls->num >= blobls->alloc)
      /* realloc first */
      realloc_blobls(blobls);
   blobls->blobs[blobls->num] = blob;
   (blobls->num)++;
}

/************************************************************/
merge_within_blobls(blobls, src, dst)
BLOBLS *blobls;
int src, dst;
{
   BLOB *nblob, *sptr, *dptr;

   if((src < 0) || (src >= blobls->num))
      fatalerr("merge_within_blobs", "src blob index not within blob list", NULL);
   if((dst < 0) || (dst >= blobls->num))
      fatalerr("merge_within_blobls", "dst blob index not within blob list", NULL);

   sptr = blobls->blobs[src];
   dptr = blobls->blobs[dst];

   merge_blobs(&nblob, sptr, dptr, blobls->flags);
   free(dptr->data);
   free(dptr);
   blobls->blobs[dst] = nblob;
   free(sptr->data);
   free(sptr);
   remove_blobls(blobls, src);
}

/*******************************************************************/
merge_among_blobls(sblobls, src, dblobls, dst)
BLOBLS *sblobls, *dblobls;
int src, dst;
{
   BLOB *nblob, *sptr, *dptr;

   if((src < 0) || (src >= sblobls->num))
      fatalerr("merge_among_blobs", "src blob index not within blob list", NULL);
   if((dst < 0) || (dst >= dblobls->num))
      fatalerr("merge_among_blobls", "dst blob index not within blob list", NULL);

   sptr = sblobls->blobs[src];
   dptr = dblobls->blobs[dst];

   merge_blobs(&nblob, sptr, dptr, dblobls->flags);
   free(dptr->data);
   free(dptr);
   dblobls->blobs[dst] = nblob;
   /* remove dot blob from list */
   free(sptr->data);
   free(sptr);
   remove_blobls(sblobls, src);
}

/*******************************************************************/
merge_blobs(nblob, blob1, blob2, flags)
BLOB **nblob, *blob1, *blob2;
int flags;
{
   unsigned char *allocate_image();
   int lx, rx, ty, by, tflags;

   alloc_blob(nblob);

   lx = min(blob1->x1, blob2->x1);
   rx = max(blob1->x1 + blob1->w,
            blob2->x1 + blob2->w);
   ty = min(blob1->y1, blob2->y1);
   by = max(blob1->y1 + blob1->h,
            blob2->y1 + blob2->h);

   (*nblob)->x1 = lx;
   (*nblob)->y1 = ty;
   (*nblob)->w = rx - lx;
   (*nblob)->h = by - ty;

   (*nblob)->data = allocate_image((*nblob)->w, (*nblob)->h, 8);

   /* copy first blob */
   subimage_copy(blob1->data, blob1->w, blob1->h,
                 (*nblob)->data, (*nblob)->w, (*nblob)->h, 0, 0, blob1->w, blob1->h,
                 blob1->x1 - lx, blob1->y1 - ty);
   /* OR second blob */
   subimage_or(blob2->data, blob2->w, blob2->h,
               (*nblob)->data, (*nblob)->w, (*nblob)->h, 0, 0, blob2->w, blob2->h,
               blob2->x1 - lx, blob2->y1 - ty);

   /* compute new pixcnt without rescanning image */
   if(flags & BLOB_PIXCNTS){
      (*nblob)->pixcnt = blob1->pixcnt + blob2->pixcnt;
      /* don't recount pixels, already taken care of */
      tflags = flags & ~BLOB_PIXCNTS;
   }
   else
      tflags = flags;

   /* compute other blob statistics according to flags set */
   compute_blob_stats((*nblob), tflags);
}

/*******************************************************************/
remove_blobls(blobls, bi)
BLOBLS *blobls;
int bi;
{
   if((bi < 0) || (bi >= blobls->num))
      fatalerr("remove_blobls", "blob index out of range of current list", NULL);

   (blobls->num)--;
   /* shift memory if blob not at end of list */
   if(bi != blobls->num)
      memcpy(&(blobls->blobs[bi]), &(blobls->blobs[bi+1]),
             (blobls->num - bi) * sizeof(BLOB *));
}

/*******************************************************************/
init_blobls(blobls, flags, bpp, b_g)
BLOBLS *blobls;
int flags, bpp, b_g;
{
   blobls->flags = flags;
   blobls->bpp = bpp;
   blobls->b_g = b_g;
}

/*******************************************************************/
build_blob(blob, flags, bdata, bx, by, bw, bh)
BLOB **blob;
int flags;
unsigned char *bdata;
int bx, by, bw, bh;
{
   alloc_blob(blob);

   (*blob)->data = bdata;
   (*blob)->x1 = bx;
   (*blob)->y1 = by;
   (*blob)->w = bw;
   (*blob)->h = bh;

   compute_blob_stats((*blob), flags);
}

/*******************************************************************/
BLOB *dup_blob(blob, bpp)
BLOB *blob;
int bpp;
{
   BLOB *oblob;
   unsigned char *imagedup();

   alloc_blob(&oblob);

   memcpy(oblob, blob, sizeof(BLOB));
   /* allocate and copy separate image space */
   oblob->data = imagedup(blob->data, blob->w, blob->h, bpp);
   return(oblob);
}

/*******************************************************************/
update_blobls_stats(blobls, flags)
BLOBLS *blobls;
int flags;
{
   int i;
   int do_xy2s, do_cxys, do_as, do_pixcnt;

/*
   if(!(blobls->flags & BLOB_XY1S) || !(blobls->flags & BLOB_WHS))
      fatalerr("update_blobls_stats",
               "must have at least (x1, y1) and (w, h) already in blob structures",
               NULL);
*/

   /* set up tests for flags */
   do_xy2s = (flags & BLOB_XY2S)?1:0;
   do_cxys = (flags & BLOB_CXYS)?1:0;
   do_as = (flags & BLOB_AS)?1:0;
   do_pixcnt = (flags & BLOB_PIXCNTS)?1:0;

   /* update blob stats accordingly */
   for(i = 0; i < blobls->num; i++){
      if(do_xy2s){
         blobls->blobs[i]->x2 = blobls->blobs[i]->x1 + blobls->blobs[i]->w - 1;
         blobls->blobs[i]->y2 = blobls->blobs[i]->y1 + blobls->blobs[i]->h - 1;
      }
      if(do_cxys){
         blobls->blobs[i]->cx = blobls->blobs[i]->x1 + (blobls->blobs[i]->w >>1);
         blobls->blobs[i]->cy = blobls->blobs[i]->y1 + (blobls->blobs[i]->h >>1);
      }
      if(do_as){
         blobls->blobs[i]->a = blobls->blobs[i]->w * blobls->blobs[i]->h;
         if(do_pixcnt)
            blobls->blobs[i]->pixcnt=pixelcnt8(blobls->blobs[i]->data,blobls->blobs[i]->a);
      }
      else if(do_pixcnt)
         blobls->blobs[i]->pixcnt=pixelcnt8(blobls->blobs[i]->data,
                                            blobls->blobs[i]->w * blobls->blobs[i]->h);
   }

   blobls->flags |= flags;
}

/*******************************************************************/
compute_blob_stats(blob, flags)
BLOB *blob;
int flags;
{
   int do_pixcnt;

/*
   if(!(flags & BLOB_XY1S) || !(flags & BLOB_WHS))
      fatalerr("compute_blob_stats",
               "must have at least (x1, y1) and (w, h) already in blob structure",
               NULL);
*/

   if(flags & BLOB_XY2S){
      blob->x2 = blob->x1 + blob->w - 1;
      blob->y2 = blob->y1 + blob->h - 1;
   }
   if(flags & BLOB_CXYS){
      blob->cx = blob->x1 + (blob->w >>1);
      blob->cy = blob->y1 + (blob->h >>1);
   }
   do_pixcnt = (flags & BLOB_PIXCNTS)?1:0;
   if(flags & BLOB_AS){
      blob->a = blob->w * blob->h;
      if(do_pixcnt)
         blob->pixcnt = pixelcnt8(blob->data, blob->a);
   }
   else if(do_pixcnt)
      blob->pixcnt = pixelcnt8(blob->data, blob->w * blob->h);
}

/*******************************************************************/
alloc_blob(blob)
BLOB **blob;
{
   if(((*blob) = (BLOB *)malloc(sizeof(BLOB))) == NULL)
      syserr("alloc_blob", "malloc", "blob");
   (*blob)->data = (unsigned char *)NULL;
}

/*******************************************************************/
free_blob(blob)
BLOB *blob;
{
   if(blob->data != (unsigned char *)NULL)
      free(blob->data);
   free(blob);
}

/*******************************************************************/
char2bin_blobls(blobls)
BLOBLS *blobls;
{
   unsigned char *bdata;
   int i, bw, bh;

   if((blobls->bpp != 8) || (blobls->b_g != BLOB_BIN))
      fatalerr("char2bin_blobls",
               "input image data must be binary and one byte per pixel", NULL);

   /* convert blobs to binary (8 bits per byte) */
   for(i = 0; i < blobls->num; i++){
      char2bin_exact(&bdata, &bw, &bh,
                     blobls->blobs[i]->data, blobls->blobs[i]->w, blobls->blobs[i]->h);
      free(blobls->blobs[i]->data);
      blobls->blobs[i]->data = bdata;
      blobls->blobs[i]->w = bw;
      blobls->blobs[i]->h = bh;
   }
   blobls->bpp = 1;
}

/************************************************************/
find_first_max_blob_memb(blobls, blob_offset, max_i, max_v)
BLOBLS *blobls;
int blob_offset, *max_i, *max_v;
{
   int i, val;
   char *bptr;

   *max_i = -1;
   *max_v = -1;
   for(i = 0; i < blobls->num; i++){
      bptr = (char *)(blobls->blobs[i]);
      val = *((int *)(bptr+blob_offset));
      if(val > *max_v){
         *max_v = val;
         *max_i = i;
      }
   }
}

/************************************************************/
float compute_avr_blob_memb(blobls, blob_offset)
BLOBLS *blobls;
int blob_offset;
{
   int sum = 0;
   int i, val;
   char *bptr;
   float avr;

   for(i = 0; i < blobls->num; i++){
      bptr = (char *)(blobls->blobs[i]);
      val = *((int *)(bptr+blob_offset));
      sum += val;
   }
   avr = sum / (float)blobls->num;
   return(avr);
}

/************************************************************/
prct_blobls_memb(blobls, offset, prct)
BLOBLS *blobls;
int offset;
float prct;
{
   BLOBLS *sblobls;
   int topn, val;
   char *bptr;

   sort_blobls_on_memb(&sblobls, INC, blobls, offset);
   topn = max(0, sround(sblobls->num * prct)-1);
   bptr = (char *)(sblobls->blobs[topn]);
   val = *((int *)(bptr+offset));
   free(sblobls->blobs);
   free(sblobls);

   return(val);
}

/*************************************************************************/
median_blob_memb(blobls, blob_offset)
BLOBLS *blobls;
int blob_offset;
{
   int i, *list, med;
   char *bptr;

   malloc_int(&list, blobls->num, "find_median_blob_memb: member list");

   for(i = 0; i < blobls->num; i++){
      bptr = (char *)(blobls->blobs[i]);
      list[i] = *((int *)(bptr+blob_offset));
   }

   med = int_list_median(list, blobls->num);
   free(list);
   return(med);
}

/************************************************************/
sort_blobls_on_memb(sblobls, order, blobls, blob_offset)
BLOBLS **sblobls, *blobls;
int order, blob_offset;
{
   int i;
   char *bptr;

   if(blobls->num == 0)
      return;

   build_blobls(sblobls, blobls->num, blobls->flags, blobls->bpp, blobls->b_g);

   with_1_INDEX_alloc(istruct, blobls->num, INTTYPE)
      for(i = 0; i < blobls->num; i++){
         bptr = (char *)(blobls->blobs[i]);
         add_to_1_INDEX(istruct, i, *((int *)(bptr+blob_offset)));
      }
      multisort_1_INDEX(istruct, order);
      i = 0;
      foreach_in_INDEX(istruct)
         append_blobls((*sblobls), blobls->blobs[this_item_int]);
      endfor
   end_with_INDEX_alloc(istruct)
}
/************************************************************/
is_blob_above(ablob, bblob)
BLOB *ablob, *bblob;
{
   /* is first blob's bottom y above second blob's top y */
   if(ablob->y2 < bblob->y1)
      return(TRUE);
   else
      return(FALSE);
}

/************************************************************/
find_blob_nbrs(li, ri, nnbrs, blob, blobls)
int *li, *ri, *nnbrs;
BLOB *blob;
BLOBLS *blobls;
{
   /* Warning: blobls should already be sorted left to right */

   (*ri) = 0;
   while(((*ri) < blobls->num) && (blobls->blobs[(*ri)]->cx < blob->cx))
      (*ri)++;
   /* if only one nbr */
   if(((*ri) == 0) || ((*ri) == blobls->num)){
      *nnbrs = 1;
      if((*ri) == blobls->num)
         (*ri)--;
      (*li) = (*ri);
   }
   else{
      (*li) = (*ri) - 1;
      *nnbrs = 2;
   }
}

/************************************************************/
find_ind_blob_nbrs(li, ri, nnbrs, blob, blobls, rank)
int *li, *ri, *nnbrs;
BLOB *blob;
BLOBLS *blobls;
int *rank;
{
   (*ri) = 0;
   while(((*ri) < blobls->num) && (blobls->blobs[rank[(*ri)]]->cx < blob->cx))
      (*ri)++;
   /* if only one nbr */
   if(((*ri) == 0) || ((*ri) == blobls->num)){
      *nnbrs = 1;
      if((*ri) == blobls->num)
         (*ri)--;
      (*li) = (*ri);
   }
   else{
      (*li) = (*ri) - 1;
      *nnbrs = 2;
   }
}

/************************************************************/
find_first_blob_inside(blob, blobls)
BLOB *blob;
BLOBLS *blobls;
{
   int bi;

   for(bi = 0; bi < blobls->num; bi++){
      if(is_box_inside(blob->x1, blob->y1, blob->x2, blob->y2,
                       blobls->blobs[bi]->x1, blobls->blobs[bi]->y1,
                       blobls->blobs[bi]->x2, blobls->blobs[bi]->y2))
         return(bi);
   }
   return(NOT_FOUND);
}

/*************************************************************************/
blob_hori_overlaps(laps_i, laps_d, nlaps, alaps, blobls, bi)
BLOBLS *blobls;
int **laps_i, **laps_d, *nlaps, *alaps, bi;
{
   int i, d;
   BLOB *cptr, *nptr;

   *nlaps = 0;
   cptr = blobls->blobs[bi];
   for(i = 0; i < blobls->num; i++){
      if(i != bi){
         nptr = blobls->blobs[i];
         if(box_overlap_hori(cptr->x1, cptr->y1, cptr->x2, cptr->y2,
                             nptr->x1, nptr->y1, nptr->x2, nptr->y2)){
            /* reallocate lists if necessary */
            if(*nlaps >= *alaps){
               (*alaps) += BLOB_CHUNKS;
               realloc_int(laps_i, *alaps, "blob_hori_overlaps : realloc : laps_i");
               realloc_int(laps_d, *alaps, "blob_hori_overlaps : realloc : laps_d");
            }
            /* compute and store overlap distance and index */
            d = (cptr->w + nptr->w) -
                   (max(nptr->x2, cptr->x2) - min(nptr->x1, cptr->x1) + 1);
            (*laps_i)[*nlaps] = i;
            (*laps_d)[*nlaps] = d;
            (*nlaps)++;
         }
      }
   }
}
