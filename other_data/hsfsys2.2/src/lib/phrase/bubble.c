/*
# proc: find_bubble_max_y - computes the maximum height of the bubble
# proc:                     given the line parameters for b = L(a).
# proc: nearest_nbor_in_right_bubble - incrementally grows a bubble to the
# proc:                                right, retuning the nearest blob once
# proc:                                blobs are detected within the bubble.
# proc: get_points_within_right_box - initially constrains the search space
# proc:                               to the rectangular region bounding the
# proc:                               largest possible bubble.
# proc: get_list_in_right_bubble - searches the area within the current bubble
# proc:                            for the presence of blobs.
# proc: is_in_right_bubble - determines if the specified point is within the
# proc:                      current bubble.
# proc: nearest_nbor_in_left_bubble - incrementally grows a bubble to the
# proc:                               left, retuning the nearest blob once
# proc:                               blobs are detected within the bubble.
# proc: get_points_within_left_box - initially constrains the search space
# proc:                              to the rectangular region bounding the
# proc:                              largest possible bubble.
# proc: get_list_in_left_bubble - searches the area within the current bubble
# proc:                           for the presence of blobs.
# proc: is_in_left_bubble - determines if the specified point is within the
# proc:                     current bubble.

*/

#include <math.h>
#include <defs.h>
#include <bubble.h>
#include <maxlist.h>

/***************************************************************************/
float find_bubble_max_y(a_limit, a1, b1, m)
float a_limit, a1, b1, m;
{
   float limit, theta, radius;
   float b, compute_y();
   float ymax, y;

   ymax = 0.0;
   b = compute_y(a_limit, m, a1, b1);
   limit = M_PI/(2.0 * b);
   theta = 0;
   while(theta < limit){
      radius = a_limit * cos((double)(b * theta));
      y = (radius * sin(theta));
      if(y > ymax)
         ymax = y;
      theta += RADINC;
   }
   return(ymax);
}

/***************************************************************************/
nearest_nbor_in_right_bubble(near_i, dist, na, nb,
                             px, py, lx, ly, end, m, a1, b1,
                             abegin, alimit, ylimit)
int *near_i, *dist, px, py, *lx, *ly, end;
float m, a1, b1, *na, *nb, abegin, alimit, ylimit;
{
   int *bx, *by, *bi, blen;
   int *inbox, inbox_i;
   int i, b_near_i;
   float a, b, compute_y(), *lr, *lt;

   if(end == 0){
      *near_i = -1;
      *dist = 0;
      *na = 0.0;
      *nb = 0.0;
      return;
   }

   malloc_int(&inbox, end, "nearest_nbor_in_right_bubble : inbox");
   get_points_within_right_box(inbox, &inbox_i, lx, ly, end,
                         px, py, m, a1, b1, alimit, ylimit);
   if(inbox_i == 0){
      free(inbox);
      *near_i = -1;
      *dist = 0;
      *na = 0.0;
      *nb = 0.0;
      return;
   }

   malloc_flt(&lr, inbox_i, "nearest_nbor_in_right_bubble : lr");
   malloc_flt(&lt, inbox_i, "nearest_nbor_in_right_bubble : lt");
   to_rel_polar2(lr, lt, px, py, lx, ly, end, inbox, inbox_i);
   a = abegin;
   b = compute_y(a, m, a1, b1);
   blen = 0;
   malloc_int(&bx, inbox_i, "nearest_nbor_in_right_bubble : bx");
   malloc_int(&by, inbox_i, "nearest_nbor_in_right_bubble : by");
   malloc_int(&bi, inbox_i, "nearest_nbor_in_right_bubble : bi");
   while(a < alimit){
      get_list_in_right_bubble(bi, &blen, px, py, lx, ly, lr, lt, a, b, end,
                               inbox, inbox_i);
      if(blen != 0) break;
      a+=A_INCR;
      b = compute_y(a, m, a1, b1);
   }
   if(blen == 0){
      *near_i = -1;
      *dist = 0;
      *na = 0.0;
      *nb = 0.0;
   }
   else{
      for(i = 0; i < blen; i++){
         bx[i] = lx[bi[i]];
         by[i] = ly[bi[i]];
      }
      nearest_nbor(&b_near_i, dist, px, py, bx, by, blen);
      *near_i = bi[b_near_i];
      *na = a;
      *nb = b;
   }
   free(inbox);
   free(lr);
   free(lt);
   free(bx);
   free(by);
   free(bi);
}

/***************************************************************************/
get_points_within_right_box(inbox, inbox_i, lx, ly, end, px, py,
                            m, a1, b1, alimit, ylimit)
int *inbox, *inbox_i, *lx, *ly, px, py, end;
float m, a1, b1, alimit, ylimit;
{
   int i;
   float lower_x, lower_y, upper_x, upper_y;
   float compute_y();

   *inbox_i = 0;
   lower_x = px;
   upper_x = px + alimit;
   lower_y = py - ylimit;
   upper_y = py + ylimit;

   for(i = 0; i < end; i++){
      if((lx[i] >= lower_x) && (lx[i] <= upper_x) &&
         (ly[i] >= lower_y) && (ly[i] <= upper_y)){
         inbox[*inbox_i] = i;
         (*inbox_i)++;
      }
   }
}

/***************************************************************************/
get_list_in_right_bubble(bi, blen, px, py, lx, ly, lr, lt, a, b, end,
                         inbox, inbox_i)
int *bi, *blen;
int px, py, *lx, *ly, *inbox, inbox_i;
float *lr, *lt, a, b;
int end;
{
   int i;
   float l;

   *blen = 0;
   l = M_PI/(2 * b);
   for(i = 0; i < inbox_i; i++){
      if(is_in_right_bubble(px, py, lx[inbox[i]], ly[inbox[i]], lr[i], lt[i],
                           a, b, l)){
         bi[*blen] = inbox[i];
         (*blen)++;
      }
   }
}

/***************************************************************************/
is_in_right_bubble(px, py, lx, ly, r, t, a, b, l)
int px, py, lx, ly;
float r, t, a, b, l;
{
   float v;

   if((t > l) || (t < -l))
      return(FALSE);
   if(lx < px)
      return(FALSE);
   v = r - (a * cos((double)(b * t)));
   if(v < 0.0){
      return(TRUE);
   }
   else
      return(FALSE);
}

/***************************************************************************/
nearest_nbor_in_left_bubble(near_i, dist, px, py, lx, ly, end, m, a1, b1,
                             abegin, alimit, ylimit)
int *near_i, *dist, px, py, *lx, *ly, end;
float m, a1, b1, abegin, alimit, ylimit;
{
   int *bx, *by, *bi, blen;
   int *inbox, inbox_i;
   int i, b_near_i;
   float a, b, compute_y(), *lr, *lt;

   if(end == 0){
      *near_i = -1;
      *dist = 0;
      return;
   }

   malloc_int(&inbox, end, "nearest_nbor_in_left_bubble : inbox");
   get_points_within_left_box(inbox, &inbox_i, lx, ly, end,
                              px, py, m, a1, b1, alimit, ylimit);
   if(inbox_i == 0){
      free(inbox);
      *near_i = -1;
      *dist = 0;
      return;
   }

   malloc_flt(&lr, inbox_i, "nearest_nbor_in_left_bubble : lr");
   malloc_flt(&lt, inbox_i, "nearest_nbor_in_left_bubble : lt");
   to_rel_polar2(lr, lt, px, py, lx, ly, end, inbox, inbox_i);
   a = abegin;
   b = compute_y(a, m, a1, b1);
   blen = 0;
   malloc_int(&bx, inbox_i, "nearest_nbor_in_left_bubble : bx");
   malloc_int(&by, inbox_i, "nearest_nbor_in_left_bubble : by");
   malloc_int(&bi, inbox_i, "nearest_nbor_in_left_bubble : bi");
   while(a < alimit){
      get_list_in_left_bubble(bi, &blen, px, py, lx, ly, lr, lt, a, b, end,
                                inbox, inbox_i);
      if(blen != 0) break;
      a+=A_INCR;
      b = compute_y(a, m, a1, b1);
   }
   if(blen == 0){
      *near_i = -1;
      *dist = 0;
   }
   else{
      for(i = 0; i < blen; i++){
         bx[i] = lx[bi[i]];
         by[i] = ly[bi[i]];
      }
      nearest_nbor(&b_near_i, dist, px, py, bx, by, blen);
      *near_i = bi[b_near_i];
   }
   free(inbox);
   free(lr);
   free(lt);
   free(bx);
   free(by);
   free(bi);
}

/***************************************************************************/
get_points_within_left_box(inbox, inbox_i, lx, ly, end, px, py,
                            m, a1, b1, alimit, ylimit)
int *inbox, *inbox_i, *lx, *ly, px, py, end;
float m, a1, b1, alimit, ylimit;
{
   int i;
   float lower_x, lower_y, upper_x, upper_y;
   float compute_y();

   *inbox_i = 0;
   lower_x = px - alimit;
   upper_x = px;
   lower_y = py - ylimit;
   upper_y = py + ylimit;

   for(i = 0; i < end; i++){
      if((lx[i] >= lower_x) && (lx[i] <= upper_x) &&
         (ly[i] >= lower_y) && (ly[i] <= upper_y)){
         inbox[*inbox_i] = i;
         (*inbox_i)++;
      }
   }
}

/***************************************************************************/
get_list_in_left_bubble(bi, blen, px, py, lx, ly, lr, lt, a, b, end,
                         inbox, inbox_i)
int *bi, *blen;
int px, py, *lx, *ly, *inbox, inbox_i;
float *lr, *lt, a, b;
int end;
{
   int i;
   float l;

   *blen = 0;
   l = M_PI/(2 * b);
   for(i = 0; i < inbox_i; i++){
      if(is_in_left_bubble(px, py, lx[inbox[i]], ly[inbox[i]], lr[i], lt[i],
                           a, b, l)){
         bi[*blen] = inbox[i];
         (*blen)++;
      }
   }
}

/***************************************************************************/
is_in_left_bubble(px, py, lx, ly, r, t, a, b, l)
int px, py, lx, ly;
float r, t, a, b, l;
{
   float v;

   if((t > l) || (t < -l))
      return(FALSE);
   if(lx > px)
      return(FALSE);
   v = r - (a * cos((double)(b * t)));
   if(v < 0.0){
      return(TRUE);
   }
   else
      return(FALSE);
}

