/*
# proc: blob_split_n_test - splits an image along a specified vertical path and then
# proc:              tests to see if any of the resulting pieces is empty or too small
# proc:              in which case cut points are adjusted and a new path is found.
# proc: find_cut_path - given a starting x position, finds a vertical cut path through
# proc:              the image by following the gradient of character stroke edges.
# proc: split_blob_along_path - given a vertical path, actually divides the image into
# proc:              two separate pieces.
# proc: join_2_vert_paths - joins to vertical paths at their closest points and ensures
# proc:              that there is one and only one point in the resulting path for each
# proc:              y-value in the image.
# proc: cut_caused_fragments - determines if cut created spurious fragments of characters
# proc:              by counting the number of non-dot blobs in the resulting pieces.
*/

#include <math.h>
#include <blobls.h>
#include <stdio.h>
#include <defs.h>
#include <segchars.h>

/*******************************************************************/
blob_split_n_test(lblob, rblob, px, py, pn, pa, cut_x, esw, sblob)
BLOB **lblob, **rblob, *sblob;
int **px, **py, *pn, *pa;
int cut_x, esw;
{
   int count_limit, count, next_x, xlimit, found;

   count_limit = max(1,(sblob->w>>1)/esw); /* half the image's width in strokes */
   count = 0;
   next_x = cut_x;
   xlimit = sblob->w-esw;
   *lblob = (BLOB *)NULL;
   *rblob = (BLOB *)NULL;
   found = 0;

   /* if we haven't looped too much, and the left or right piece is too small */
   while((!found) && (count++ < count_limit)){
      /* find cut path from new starting points */
      find_cut_path(px, py, pn, pa, sblob->data, next_x, esw, sblob->w, sblob->h);
      split_blob_along_path(lblob, rblob, *px, *py, *pn, sblob);

      /* left and right piece should never both be too small */
      /* move cut points one stroke width away from piece that's too small */
      if((*lblob == NULL) || is_noise_blob(*lblob, esw)){
         if(*lblob != NULL)
            free_blob(*lblob);
         if(*rblob != NULL)
            free_blob(*rblob);
         free(*px);
         free(*py);
         next_x += esw;
         if(next_x >= xlimit){
            break;
         }
      }
      else if((*rblob == NULL) || is_noise_blob(*rblob, esw)){
         if(*lblob != NULL)
            free_blob(*lblob);
         if(*rblob != NULL)
            free_blob(*rblob);
         free(*px);
         free(*py);
         next_x -= esw;
         if(next_x < esw){
            break;
         }
      }
      else
         found = 1;
   }

   /* if never found successful cut path ... */
   if(!found){
      return(FALSE);
   }
   else if(cut_caused_fragments(sblob, *lblob, *rblob, esw, sblob->h)){
      free(*px);
      free(*py);
      free_blob(*lblob);
      free_blob(*rblob);
      return(FALSE);
   }
   else{
      return(TRUE);
   }
}

/*******************************************************************/
blob_split_n_test_orig(lblob, rblob, px, py, pn, pa, cut_x, esw, sblob)
BLOB **lblob, **rblob, *sblob;
int **px, **py, *pn, *pa;
int cut_x, esw;
{
   int count_limit, count, next_x, xlimit, ltest, found;

   /* find cut path from starting points */
   find_cut_path(px, py, pn, pa, sblob->data, cut_x, esw, sblob->w, sblob->h);
   /* split the image along the path */
   split_blob_along_path(lblob, rblob, *px, *py, *pn, sblob);

   /* test left and right pieces */
   count_limit = max(1,(sblob->w>>1)/esw); /* half the image's width in strokes */
   count = 0;
   next_x = cut_x;
   xlimit = sblob->w-esw;
   found = 1;

   /* if we haven't looped too much, and the left or right piece is too small */
   while((count++ < count_limit) &&
         (ltest = ((*lblob == NULL) || is_noise_blob(*lblob, esw)) ||
          (*rblob == NULL) || is_noise_blob(*rblob, esw))){

      /* left and right piece should never both be too small */
      /* move cut points one stroke width away from piece that's too small */
      if(ltest){
         next_x += esw;
         if(next_x >= xlimit){
            found = 0;
            break;
         }
      }
      else{
         next_x -= esw;
         if(next_x < esw){
            found = 0;
            break;
         }
      }
      if(*lblob != NULL)
         free_blob(*lblob);
      if(*rblob != NULL)
         free_blob(*rblob);
      free(*px);
      free(*py);
      /* find cut path from new starting points */
      find_cut_path(px, py, pn, pa, sblob->data, next_x, esw, sblob->w, sblob->h);
      split_blob_along_path(lblob, rblob, *px, *py, *pn, sblob);
   }

   /* if never found successful cut path ... */
   if((count > count_limit) || (!found)){
      free(*px);
      free(*py);
      if(*lblob != NULL)
         free_blob(*lblob);
      if(*rblob != NULL)
         free_blob(*rblob);
      return(FALSE);
   }
   else if(cut_caused_fragments(sblob, *lblob, *rblob, esw, sblob->h)){
      free(*px);
      free(*py);
      free_blob(*lblob);
      free_blob(*rblob);
      return(FALSE);
   }
   else{
      return(TRUE);
   }
}

/*******************************************************************/
find_cut_path(px, py, pn, pa, cdata, cx, winw, w, h)
int **px, **py, *pn, *pa;
unsigned char *cdata;
int cx, winw, w, h;
{
   int *dx, *dy, dn, da;
   int *ux, *uy, un, ua, ui;

   dribble_down_up(&dx, &dy, &dn, &da, &ux, &uy, &un, &ua,
                     cdata, cx, winw, w, h);

   /* if both paths pass all the way through the image ... */
   if((dn == h) && (un == h)){
      /* choose path whose midpt y is closer to the cut line */
      if(abs(cx - dy[dn>>1]) > abs(cx - uy[un>>1])){
         /* choose upward path */
         dn = 0;
         for(ui = un - 1; ui >= 0; ui--)
            add_point(ux[ui], uy[ui], &dx, &dy, &dn, &da);
      }
      /* otherwise use downward path (by default) */
   }
   /* if downward path cuts all the way through the image ... */
   else if (dn == h){
      /* use downward path by default */
   }
   /* if upward path cuts all the way through the image ... */
   else if (un == h){
      dn = 0;
      for(ui = un - 1; ui >= 0; ui--)
         add_point(ux[ui], uy[ui], &dx, &dy, &dn, &da);
   }
   /* if both paths have some points ... */
   else if((dn > 0) && (un > 0)){
      join_2_vert_paths(&dx, &dy, &dn, &da, &ux, &uy, &un, &ua);
   }
   /* otherwise, should never have no cut points */

   free(ux);
   free(uy);

   *px = dx;
   *py = dy;
   *pn = dn;
   *pa = da;
}

/*******************************************************************/
/* split_blob_along_path returns NULL blob pointers if unsuccessful*/
/*******************************************************************/
split_blob_along_path(lblob, rblob, px, py, pn, sblob)
BLOB **lblob, **rblob, *sblob;
int *px, *py, pn;
{
   unsigned char *ldata, *rdata, *lptr, *rptr, *cptr;
   unsigned char *mallocate_image(), *allocate_image();
   int i, w, h, min_px, max_px;
   int lw, rw;
   int blx, bly, blw, blh;
   int brx, bry, brw, brh;
   static int flags = BLOB_XY1S | BLOB_WHS | BLOB_AS;

   if(pn != sblob->h)
      fatalerr("split_blob_along_path",
               "number of points not equal to height of blob image", NULL);

   w = sblob->w;
   h = sblob->h;

   find_first_max_forward(px, 0, pn, &i, &max_px);
   lw = max_px;
   find_first_min_forward(px, 0, pn, &i, &min_px);
   rw = w - min_px;

   /* if left or right piece empty ... */
   if((lw == 0) || (rw == 0)){
      *lblob = (BLOB *)NULL;
      *rblob = (BLOB *)NULL;
      return;
   }
   ldata = allocate_image(lw, h, 8);
   rdata = allocate_image(rw, h, 8);

   lptr = ldata;
/* more readable, but less efficient */
/*   rptr = rdata; */
   rptr = rdata - min_px;
   cptr = sblob->data;
   for(i = 0; i < pn; i++){
      memcpy(lptr, cptr, px[i]);
      /* more readable, but less efficient */
/*      memcpy(rptr+px[i]-min_px, cptr+px[i], w-px[i]); */
      memcpy(rptr+px[i], cptr+px[i], w-px[i]);
      lptr += lw;
      rptr += rw;
      cptr += w;
   }

   find_corners8(&blx, &bly, &blw, &blh, ldata, lw, h);
   if((blw != 0) && (blh != 0)){
      build_blob(lblob, flags, mallocate_image(blw, blh, 8), blx, bly, blw, blh);
      subimage_copy(ldata, lw, h, (*lblob)->data, blw, blh, blx, bly, blw, blh, 0, 0);
   }
   else
      *lblob = (BLOB *)NULL;

   find_corners8(&brx, &bry, &brw, &brh, rdata, rw, h);
   if((brw != 0) && (brh != 0)){
      build_blob(rblob, flags, mallocate_image(brw, brh, 8),
                 min_px+brx, bry, brw, brh);
      subimage_copy(rdata, rw, h, (*rblob)->data, brw, brh, brx, bry, brw, brh, 0, 0);
   }
   else
      *rblob = (BLOB *)NULL;

   free(ldata);
   free(rdata);
}

/*******************************************************************/
split_blob_along_path_orig(lblob, rblob, px, py, pn, sblob)
BLOB **lblob, **rblob, *sblob;
int *px, *py, pn;
{
   unsigned char *ldata, *rdata, *lptr, *rptr, *cptr;
   unsigned char *mallocate_image(), *allocate_image();
   int i, w, h, min_px, max_px;
   int lw, rw;
   int blx, bly, blw, blh;
   int brx, bry, brw, brh;
   static int flags = BLOB_XY1S | BLOB_WHS | BLOB_AS;

   if(pn != sblob->h)
      fatalerr("split_blob_along_path",
               "number of points not equal to height of blob image", NULL);

   w = sblob->w;
   h = sblob->h;

   find_first_max_forward(px, 0, pn, &i, &max_px);
   lw = max_px;
   ldata = allocate_image(lw, h, 8);

   find_first_min_forward(px, 0, pn, &i, &min_px);
   rw = w - min_px;
   rdata = allocate_image(rw, h, 8);

   lptr = ldata;
/* more readable, but less efficient */
/*   rptr = rdata; */
   rptr = rdata - min_px;
   cptr = sblob->data;
   for(i = 0; i < pn; i++){
      memcpy(lptr, cptr, px[i]);
      /* more readable, but less efficient */
/*      memcpy(rptr+px[i]-min_px, cptr+px[i], w-px[i]); */
      memcpy(rptr+px[i], cptr+px[i], w-px[i]);
      lptr += lw;
      rptr += rw;
      cptr += w;
   }

   find_corners8(&blx, &bly, &blw, &blh, ldata, lw, h);
   if((blw != 0) && (blh != 0)){
      build_blob(lblob, flags, mallocate_image(blw, blh, 8), blx, bly, blw, blh);
      subimage_copy(ldata, lw, h, (*lblob)->data, blw, blh, blx, bly, blw, blh, 0, 0);
   }
   else
      *lblob = (BLOB *)NULL;

   find_corners8(&brx, &bry, &brw, &brh, rdata, rw, h);
   if((brw != 0) && (brh != 0)){
      build_blob(rblob, flags, mallocate_image(brw, brh, 8),
                 min_px+brx, bry, brw, brh);
      subimage_copy(rdata, rw, h, (*rblob)->data, brw, brh, brx, bry, brw, brh, 0, 0);
   }
   else
      *rblob = (BLOB *)NULL;

   free(ldata);
   free(rdata);
}

/*******************************************************************/
join_2_vert_paths(dx, dy, dn, da, ux, uy, un, ua)
int **dx, **dy, *dn, *da;
int **ux, **uy, *un, *ua;
{
   int last_y;
   int *sx, *sy, sn, sa, si, ui;
   int dcx, dcy, dci, ucx, ucy, uci;

   find_closest_pts(&dcx, &dcy, &dci, &ucx, &ucy, &uci, *dx, *dy, *dn, *ux, *uy, *un);

   /* interpolate line between (dcx, dcy) and (ucx, ucy) */
   sa = 0;
   bres_line_alloc(dcx, dcy, ucx, ucy, &sx, &sy, &sn, &sa);

   /* reset down list so point at dci is last point in list */
   *dn = dci + 1;
   /* append interpolated line to down list */
   /* do not copy endpoints (dcx, dcy) and (ucx, ucy) */
   /* do not copy consecutive ponts with same y coord */
   last_y = dcy;
   for(si = 1; si < sn - 1; si++){
      if(sy[si] > last_y){
         add_point(sx[si], sy[si], dx, dy, dn, da);
         last_y = sy[si];
      }
   }
   /* append upward points in reverse order from (ucx, ucy) */
   if(last_y == ucy)
      ui = uci - 1;
   else
      ui = uci;
   for(; ui >= 0; ui--){
      if((*uy)[ui] > last_y){
         add_point((*ux)[ui], (*uy)[ui], dx, dy, dn, da);
         last_y = (*uy)[ui];
      }
   }

   free(sx);
   free(sy);
}

/************************************************************/
cut_caused_fragments(blob, lblob, rblob, esw, charh)
BLOB *blob, *lblob, *rblob;
int esw, charh;
{
   int hlimit;
   int otot, ltot, rtot;

   hlimit = sround(charh * FRAG_HEIGHT);

   /* if left or right piece is a fragment */
   if((lblob->h < hlimit) || (rblob->h < hlimit))
      return(TRUE);

   /* count number of non-dots in original blob */
   otot = count_nondots_blob(blob, esw);

   /* count number of non-dots in left piece */
   ltot = count_nondots_blob(lblob, esw);

   /* count number of non-dots in right piece */
   rtot = count_nondots_blob(rblob, esw);

   if(otot != ltot + rtot - 1)
      return(TRUE);
   else
      return(FALSE);
}
