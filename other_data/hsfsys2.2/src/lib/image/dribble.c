/*
# proc: dribble_down_up - given an image and a starting x position, follows the gradients
# proc:                   of character stroke edges from the top of the image down and 
# proc:                   from the bottom of the image up, stopping at minima points.
# proc: dribble_down - starts at the top of the image at a specified x position and follows
# proc:                edges downwards, stopping at a local minima.
# proc: dribble_up - starts at the bottom of the image at a specified x position and follows
# proc:                edges upwards, stopping at a local minima.
# proc: sum_dys_left_down - given a starting point in an image, increments to the left a
# proc:                specified window width measuring the distance down to the first
# proc:                black pixel.
# proc: sum_dys_right_down - given a starting point in an image, increments to the right a
# proc:                specified window width measuring the distance down to the first
# proc:                black pixel.
# proc: sum_dys_left_up - given a starting point in an image, increments to the left a
# proc:                specified window width measuring the distance up to the first
# proc:                black pixel.
# proc: sum_dys_right_up - given a starting point in an image, increments to the right a
# proc:                specified window width measuring the distance up to the first
# proc:                black pixel.
# proc: send_feelers_look_down - from a specified point looks left and right to follow
# proc:                the edge of a character stroke downwards.
# proc: send_feelers_look_up - from a specified point looks left and right to follow
# proc:                the edge of a character stroke upwards.
# proc: look_left_down - looks for next step to the left down the edge of a
# proc:                  character stroke.
# proc: look_right_down - looks for next step to the right down the edge of a
# proc:                   character stroke.
# proc: look_left_up - looks for next step to the left up the edge of a
# proc:                  character stroke.
# proc: look_right_up - looks for next step to the right up the edge of a
# proc:                   character stroke.
*/

#include <defs.h>
#define SMALL_MAX_DY  2     /* threshold to determine if change in dys is flat */

/*******************************************************************/
dribble_down_up(dx, dy, dn, da, ux, uy, un, ua, cdata, cx, winw, w, h)
int **dx, **dy, *dn, *da, **ux, **uy, *un, *ua;
unsigned char *cdata;
int cx, winw, w, h;
{

   /* initialize and compute cut statistics */
   malloc_points(dx, dy, dn, da);
   dribble_down(dx, dy, dn, da, cdata, cx, winw, w, h);

   /* initialize and compute cut statistics */
   malloc_points(ux, uy, un, ua);
   dribble_up(ux, uy, un, ua, cdata, cx, winw, w, h);
}

/*******************************************************************/
dribble_down(dx, dy, dn, da, cdata, cx, winw, w, h)
int **dx, **dy, *dn, *da;
unsigned char *cdata;
int cx, winw, w, h;
{
   int px, py;
   int by, wx, wy;
   int ldys, lmdy, lmdy_x;
   int rdys, rmdy, rmdy_x;
   int lasth, lasty, lastp;

   px = cx;
   py = 0;

   /* if starting from a black pixel ... */
   if(*(cdata+px) == 1){
      /* choose initial direction */
      sum_dys_left_down(&ldys, &lmdy, &lmdy_x, cdata, px, py, winw, w, h);
      sum_dys_right_down(&rdys, &rmdy, &rmdy_x, cdata, px+1, py, winw, w, h);

      /* if both max dys too small or sum of dys both == 0 ... */
      if(((lmdy < SMALL_MAX_DY) && (rmdy < SMALL_MAX_DY)) ||
         ((ldys == 0) && (rdys == 0))){
         /* simply set the starting point */
         add_point(px, py, dx, dy, dn, da);
         return;
      }
      /* otherwise, choose direction of larger mdy */
      if(ldys > rdys){
         /* initially look left from largest dy */
         px = lmdy_x;
      }
      else{
         /* otherwise initially look right */
         px = rmdy_x;
      }
   }

   lasth = h-1;
   while(py < lasth){
      if((by = find_pix_south(1, px, py, cdata, w, h)) == NOT_FOUND){
         fill_points_down(px, py, lasth, dx, dy, dn, da);
         break;
      }
      else{
         lasty = by-1;
         fill_points_down(px, py, lasty, dx, dy, dn, da);
         py = lasty;
      }
      lastp = (*dn)-1;
      if(send_feelers_look_down(&wx, &wy, cdata,
              &((*dx)[lastp]), &((*dy)[lastp]), winw, w, h) == NOT_FOUND)
         break;
      else{
         px = wx;
         py = wy;
      }
   }
}

/*******************************************************************/
dribble_up(ux, uy, un, ua, cdata, cx, winw, w, h)
int **ux, **uy, *un, *ua;
unsigned char *cdata;
int cx, winw, w, h;
{
   int px, py;
   int by, wx, wy;
   int ldys, lmdy, lmdy_x;
   int rdys, rmdy, rmdy_x;
   int lasty, lastp;

   px = cx;
   py = h-1;

   /* if starting from a black pixel ... */
   if(*(cdata+(py*w)+px) == 1){
      /* choose initial direction */
      sum_dys_left_up(&ldys, &lmdy, &lmdy_x, cdata, px, py, winw, w, h);
      sum_dys_right_up(&rdys, &rmdy, &rmdy_x, cdata, px+1, py, winw, w, h);

      /* if both max dys too small or sum of dys both == 0 ... */
      if(((lmdy < SMALL_MAX_DY) && (rmdy < SMALL_MAX_DY)) ||
         ((ldys == 0) && (rdys == 0))){
         /* simply set the starting point */
         add_point(px, py, ux, uy, un, ua);
         return;
      }
      /* otherwise, choose direction of larger mdy */
      if(ldys > rdys){
         /* initially look left from largest dy */
         px = lmdy_x;
      }
      else{
         /* otherwise initially look right */
         px = rmdy_x;
      }
   }

   while(py > 0){
      if((by = find_pix_north(1, px, py, cdata, w, h)) == NOT_FOUND){
         fill_points_up(px, py, 0, ux, uy, un, ua);
         break;
      }
      else{
         lasty = by+1;
         fill_points_up(px, py, lasty, ux, uy, un, ua);
         py = lasty;
      }
      lastp = (*un)-1;
      if(send_feelers_look_up(&wx, &wy, cdata,
              &((*ux)[lastp]), &((*uy)[lastp]), winw, w, h) == NOT_FOUND)
         break; 
      else{
         px = wx;
         py = wy;
      }
   }
}

/*******************************************************************/
sum_dys_left_down(dys, mdy, mdy_x, cdata, cx, cy, winw, w, h)
int *dys, *mdy, *mdy_x;
unsigned char *cdata;
int cx, cy, winw, w, h;
{
   int x, by, delta;

   *dys = 0;
   *mdy = -1;
   *mdy_x = -1;
   for(x = cx; x >= max(0, cx-winw); x--){
      if((by = find_pix_south(1, x, cy, cdata, w, h)) == NOT_FOUND)
         break;
      delta = by - cy;
      *dys += delta;
      if(delta > *mdy){
         *mdy = delta;
         *mdy_x = x;
      }
   }
}

/*******************************************************************/
sum_dys_right_down(dys, mdy, mdy_x, cdata, cx, cy, winw, w, h)
int *dys, *mdy, *mdy_x;
unsigned char *cdata;
int cx, cy, winw, w, h;
{
   int x, by, delta;

   *dys = 0;
   *mdy = -1;
   *mdy_x = -1;

   *dys = 0;
   for(x = cx; x <= min(w-1, cx+winw); x++){
      if((by = find_pix_south(1, x, cy, cdata, w, h)) == NOT_FOUND)
         break;
      delta = by - cy;
      *dys += delta;
      if(delta > *mdy){
         *mdy = delta;
         *mdy_x = x;
      }
   }
}

/*******************************************************************/
sum_dys_left_up(dys, mdy, mdy_x, cdata, cx, cy, winw, w, h)
int *dys, *mdy, *mdy_x;
unsigned char *cdata;
int cx, cy, winw, w, h;
{
   int x, by, delta;

   *dys = 0;
   *mdy = -1;
   *mdy_x = -1;

   *dys = 0;
   for(x = cx; x >= max(0, cx-winw); x--){
      if((by = find_pix_north(1, x, cy, cdata, w, h)) == NOT_FOUND)
         break;
      delta = cy - by;
      *dys += delta;
      if(delta > *mdy){
         *mdy = delta;
         *mdy_x = x;
      }
   }
}

/*******************************************************************/
sum_dys_right_up(dys, mdy, mdy_x, cdata, cx, cy, winw, w, h)
int *dys, *mdy, *mdy_x;
unsigned char *cdata;
int cx, cy, winw, w, h;
{
   int x, by, delta;

   *dys = 0;
   *mdy = -1;
   *mdy_x = -1;

   *dys = 0;
   for(x = cx; x <= min(w-1, cx+winw); x++){
      if((by = find_pix_north(1, x, cy, cdata, w, h)) == NOT_FOUND)
         break;
      delta = cy - by;
      *dys += delta;
      if(delta > *mdy){
         *mdy = delta;
         *mdy_x = x;
      }
   }
}

/*******************************************************************/
send_feelers_look_down(wx, wy, cdata, cx, cy, winw, w, h)
int *wx, *wy;
unsigned char *cdata;
int *cx, *cy, winw, w, h;
{
   if(*cy == h-1)
      return(NOT_FOUND);

   if(look_left_down(wx, wy, cdata, cx, cy, winw, w, h) == FOUND){
      return(FOUND);
   }
   else if(look_right_down(wx, wy, cdata, cx, cy, winw, w, h) == FOUND){
      return(FOUND);
   }
   else
      return(NOT_FOUND);
}

/*******************************************************************/
send_feelers_look_up(wx, wy, cdata, cx, cy, winw, w, h)
int *wx, *wy;
unsigned char *cdata;
int *cx, *cy, winw, w, h;
{
   if(*cy == 0)
      return(NOT_FOUND);

   if(look_left_up(wx, wy, cdata, cx, cy, winw, w, h) == FOUND)
      return(FOUND);
   else if(look_right_up(wx, wy, cdata, cx, cy, winw, w, h) == FOUND)
      return(FOUND);
   else
      return(NOT_FOUND);
}

/*******************************************************************/
look_left_down(wx, wy, cdata, cx, cy, winw, w, h)
int *wx, *wy;
unsigned char *cdata;
int *cx, *cy, winw, w, h;
{
   int x, nx, xlim;
   unsigned char *sptr, *cptr, *nptr;

   sptr = cdata + ((*cy) * w) + (*cx);
   cptr = sptr;
   nptr = cptr + w;
   xlim = max(0, (*cx)-winw);
   for(x = (*cx), nx = (*cx) - 1; x >= xlim; x--, nx--){
      /* if current row runs into black, then ... */
      if(*cptr == 1){
         /* if only a single black bump */
         if(((*cy) > 0) && (*(cptr-w) == 0) &&
            (nx >= xlim) && (*(cptr-1) == 0)){
            /* then ignore bump */
            cptr--;
            nptr--;
            /* reset current point's x position passed bump */
            *cx = nx;
         }
         else
            /* otherwise stop searching in current direction */
            break;
      }
      /* if next row runs into white, then ... */
      else if(*nptr == 0){
         /* if only a single white well */
         if(((*cy)+2 < h) && (*(nptr+w) == 1) &&
            (nx >= xlim) && (*(nptr-1) == 1)){
            /* then ignore well */
            cptr--;
            nptr--;
         }
         else{
            /* otherwise found next white step */
            *wy = (*cy) + 1;
            *wx = x;
            return(FOUND);
         }
      }
      /* otherwise current row pix is white and next row pix is black */
      else{
         cptr--;
         nptr--;
      }
   }
   return(NOT_FOUND);
}

/*******************************************************************/
look_right_down(wx, wy, cdata, cx, cy, winw, w, h)
int *wx, *wy;
unsigned char *cdata;
int *cx, *cy, winw, w, h;
{
   int x, nx, xlim;
   unsigned char *sptr, *cptr, *nptr;

   sptr = cdata + ((*cy) * w) + (*cx);
   cptr = sptr;
   nptr = cptr + w;
   xlim = min(w-1, (*cx)+winw);
   for(x = (*cx), nx = (*cx)+1; x <= xlim; x++, nx++){
      /* if current row runs into black, then ... */
      if(*cptr == 1){
         /* if only a single black bump */
         if(((*cy) > 0) && (*(cptr-w) == 0) &&
            (nx <= xlim) && (*(cptr+1) == 0)){
            /* then ignore bump */
            cptr++;
            nptr++;
            /* reset current point's x position passed bump */
            *cx = nx;
         }
         else
            /* otherwise stop searching in current direction */
            break;
      }
      /* if next row runs into white, then ... */
      else if(*nptr == 0){
         /* if only a single white well */
         if(((*cy)+2 < h) && (*(nptr+w) == 1) &&
            (nx <= xlim) && (*(nptr+1) == 1)){
            /* then ignore well */
            cptr++;
            nptr++;
         }
         else{
            /* otherwise found next white step */
            *wy = (*cy) + 1;
            *wx = x;
            return(FOUND);
         }
      }
      /* otherwise current row pix is white and next row pix is black */
      else{
         cptr++;
         nptr++;
      }
   }
   return(NOT_FOUND);
}

/*******************************************************************/
look_left_up(wx, wy, cdata, cx, cy, winw, w, h)
int *wx, *wy;
unsigned char *cdata;
int *cx, *cy, winw, w, h;
{
   int x, nx, xlim;
   unsigned char *sptr, *cptr, *nptr;

   sptr = cdata + ((*cy) * w) + (*cx);
   cptr = sptr;
   nptr = cptr - w;
   xlim = max(0, (*cx)-winw);
   for(x = (*cx), nx = (*cx)-1; x >= xlim; x--, nx--){
      /* if current row runs into black, then ... */
      if(*cptr == 1){
         /* if only a single black bump */
         if(((*cy) < h-1) && (*(cptr+w) == 0) &&
            (nx >= xlim) && (*(cptr-1) == 0)){
            /* then ignore bump */
            cptr--;
            nptr--;
            /* reset current point's x position passed bump */
            *cx = nx;
         }
         else
            /* otherwise stop searching in current direction */
            break;
      }
      /* if next row runs into white, then ... */
      else if(*nptr == 0){
         /* if only a single white well */
         if(((*cy) > 2) && (*(nptr-w) == 1) &&
            (nx >= xlim) && (*(nptr-1) == 1)){
            /* then ignore well */
            cptr--;
            nptr--;
         }
         else{
            /* otherwise found next white step */
            *wy = (*cy) - 1;
            *wx = x;
            return(FOUND);
         }
      }
      /* otherwise current row pix is white and next row pix is black */
      else{
         cptr--;
         nptr--;
      }
   }
   return(NOT_FOUND);
}

/*******************************************************************/
look_right_up(wx, wy, cdata, cx, cy, winw, w, h)
int *wx, *wy;
unsigned char *cdata;
int *cx, *cy, winw, w, h;
{
   int x, nx, xlim;
   unsigned char *sptr, *cptr, *nptr;

   sptr = cdata + ((*cy) * w) + (*cx);
   cptr = sptr;
   nptr = cptr - w;
   xlim = min(w-1, (*cx)+winw);
   for(x = (*cx), nx=(*cx)+1; x <= min(w-1, (*cx)+winw); x++, nx++){
      /* if current row runs into black, then ... */
      if(*cptr == 1){
         /* if only a single black bump */
         if(((*cy) < h-1) && (*(cptr+w) == 0) &&
            (nx <= xlim) && (*(cptr+1) == 0)){
            /* then ignore bump */
            cptr++;
            nptr++;
            /* reset current point's x position passed bump */
            *cx = nx;
         }
         else
            /* otherwise stop searching in current direction */
            break;
      }
      /* if next row runs into white, then ... */
      else if(*nptr == 0){
         /* if only a single white well */
         if(((*cy) > 2) && (*(nptr-w) == 1) &&
            (nx <= xlim) && (*(nptr+1) == 1)){
            /* then ignore well */
            cptr++;
            nptr++;
         }
         else{
            /* otherwise found next white step */
            *wy = (*cy) - 1;
            *wx = x;
            return(FOUND);
         }
      }
      /* otherwise current row pix is white and next row pix is black */
      else{
         cptr++;
         nptr++;
      }
   }
   return(NOT_FOUND);
}
