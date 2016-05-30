/*
# proc: is_dot_blob - determines if blob is small enough to be a dot based on the
# proc:               estimated stroke width.
# proc: process_dot_blobs - reconnects dots to i's and j's and other larger blobs that are
# proc:               sufficiently close.
# proc: left_closer_to_dot - given two nbring blobs, determines which is closer.
# proc:
# proc: reconnect_dot - determines if nbring blob is below, close, and in line with the
# proc:                 dot blob.
# proc: dist_dot2blob - computes the perpendicular distance from the blob to the linear
# proc:                 projection from a nbring blob.
# proc: count_nondots_blob - counts the number of non-dots (blobs larger than a dot) in a
# proc:                 specified blob image.
*/

#include <stdio.h>
#include <math.h>
#include <blobls.h>
#include <segchars.h>
#include <defs.h>

/************************************************************/
is_dot_blob(blob, esw)
BLOB *blob;
int esw;
{
   static int dot_x1_thresh, dot_y1_thresh;
   static int oesw = -1;

   if(esw != oesw){
      dot_x1_thresh = max(1, sround(esw * DOT_X_FCTR));
      dot_y1_thresh = max(1, sround(esw * DOT_Y_FCTR));
      oesw = esw;
   }

   if((blob->w < dot_x1_thresh) &&
      (blob->h < dot_y1_thresh))
      return(TRUE);
   else
      return(FALSE);
}

/************************************************************/
process_dot_blobs(esw, eh, dots, blobls)
int esw, eh;
BLOBLS *dots, *blobls;
{
   int di, bi, li, ri, nnbrs;
   BLOB *dptr, *lptr, *rptr;
   int a1, a2;

   /* Warning: blobls should already be sorted left to right */

   /* if no original blobs, then done */
   if(blobls->num == 0)
      return;

   di = dots->num - 1;
   while(di >= 0){
      dptr = dots->blobs[di];
      /* if dot-sized blob is inside another blob, then merge */
      if((bi = find_first_blob_inside(dptr, blobls)) != NOT_FOUND){
         /* merge dot with blob it is inside */
         merge_among_blobls(dots, di, blobls, bi);
      }
      /* check the nbrs of the dot */
      else{
         /* locate dot's left and right nbring blobs */
         find_blob_nbrs(&li, &ri, &nnbrs, dptr, blobls);
         /* if only 1 nbring blob found ... */
         if(nnbrs == 1){
            lptr = blobls->blobs[li];
            /* dot must be above, close, and in line with nbr */
            if(reconnect_dot(dptr, lptr, esw, DOT_DIST)){
               /* merge dot with its nbr */
               merge_among_blobls(dots, di, blobls, li);
            }
            /* dot might be top of a 5 for nbr */
            else if(is_top_of_5(lptr, dptr, esw, eh)){
               merge_among_blobls(dots, di, blobls, li);
            }
            /* otherwise not a dot */
         }
         /* otherwise two nbrs */
         else{
            lptr = blobls->blobs[li];
            rptr = blobls->blobs[ri];

            a1 = reconnect_dot(dptr, lptr, esw, DOT_DIST);
            a2 = reconnect_dot(dptr, rptr, esw, DOT_DIST);

            /* if could reconnect dot to both nbrs */
            if(a1 && a2){
               /* choose blob closer to dot */
               if(left_closer_to_dot(dptr, lptr, rptr)){
                  /* merge dot with left nbr */
                  merge_among_blobls(dots, di, blobls, li);
               }
               else{
                  /* merge dot with right nbr */
                  merge_among_blobls(dots, di, blobls, ri);
               }
            }
            else if (a1){
               /* merge dot with left nbr */
               merge_among_blobls(dots, di, blobls, li);
            }
            else if (a2){
               /* merge dot with right nbr */
               merge_among_blobls(dots, di, blobls, ri);
            }
            /* dot might be top of a 5 for nbr */
            else if(is_top_of_5(lptr, dptr, esw, eh)){
               merge_among_blobls(dots, di, blobls, li);
            }
         }
         /* otherwise not a dot */
      } /* end nbr checking */
      di--;
   } /* end while */
}

/************************************************************/
left_closer_to_dot(dot, lblob, rblob)
BLOB *dot, *lblob, *rblob;
{
   float ld, rd, distance();

   /* distance between dot's bottom cx and blobs top cx */
   ld = distance((float)lblob->cx, (float)lblob->y1, (float)dot->cx, (float)dot->cy);
   rd = distance((float)rblob->cx, (float)rblob->y1, (float)dot->cx, (float)dot->cy);

   if(ld <= rd)
      return(TRUE);
   else
      return(FALSE);
}

/************************************************************/
reconnect_dot(dot, blob, esw, thresh)
BLOB *dot, *blob;
int esw;
float thresh;
{
   float d, dist_dot2blob();

   /* if dot is not above, then not a dot */
   if(!is_blob_above(dot, blob))
      return(FALSE);

   /* compute nbr distance */
   d = dist_dot2blob(dot, blob);
   /* normalize distance on estimated line width */
   d /= (float)esw;
   if(d < thresh)
      return(TRUE);
   else
      return(FALSE);
}

/************************************************************/
float dist_dot2blob(dot, blob)
BLOB *dot, *blob;
{
   int x1, y1, x2, y2, ly;
   float m, d, perp_dist();

   y1 = blob->y1;
   ly = blob->h - 1;
   y2 = y1 + ly;
   if(((x1 = left_pix(blob->data, blob->w)) != NOT_FOUND) &&
      ((x2 = left_pix(blob->data+(ly*blob->w), blob->w)) != NOT_FOUND)){
      /* if vertical slope */
      x1 += blob->x1;
      x2 += blob->x1;
      if(x1 == x2){
         d = (float)(dot->cx - x1);
         return(fabs(d));
      }
      else{
         m = (y2 - y1)/(float)(x2 - x1);
         d = perp_dist((float)dot->cx, (float)dot->cy, (float)x1, (float)y1, (float)m);
         return(d);
      }
   }
   else
      fatalerr("dist_dot2blob",
               "black pixel not found in either top or bottom row of blob",
               NULL);
}

/************************************************************/
count_nondots_blob(blob, esw)
BLOB *blob;
int esw;
{
   int i, tot;
   unsigned char *tdata, *imagedup();
   BLOBLS *tblobls;
   static int flags = BLOB_XY1S | BLOB_WHS;

   tdata = imagedup(blob->data, blob->w, blob->h, 8);
   segblobs8(&tblobls, flags, tdata, blob->w, blob->h);
   free(tdata);
   tot = 0;
   for(i = 0; i < tblobls->num; i++){
      /* if a dot or smaller, then ignore */
      if(!is_dot_blob(tblobls->blobs[i], esw))
         tot++;
   }
   free_blobls(tblobls);
   return(tot);
}
