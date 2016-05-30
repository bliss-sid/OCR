/*
# proc: one_like_blob - determines if the given blob is shaped similar
# proc:                 to the handprinted character 1.
# proc: one_like_blob - determines if the given blob is shaped similar
# proc:                 to a handprinted horizontal dash.
# proc: is_smallpunct_blob - determines if the given blob is similar in
# proc:                 size and shape to a small punctuation mark.
# proc: is_top_of_5 - determines if the given blob is shaped and positioned
# proc:               similar to the top stroke of a handprinted 5.
# proc: is_top_of_alpha - tests if blob is shaped like a top stroke of alphabetic
# proc:                   character.
*/

#include <math.h>
#include <segchars.h>
#include <blobls.h>
#include <defs.h>

/************************************************************/
one_like_blob(blob, esw, charh)
BLOB *blob;
int esw, charh;
{
   static ocharh = -1, oesw = -1;
   static int hlimit;
   int d1;
   float d;

   if((ocharh != charh) || (oesw != esw)){
      ocharh = charh;
      oesw = esw;
      hlimit = sround(charh * ONE_HLIMIT);
   }

   d1 = (blob->w*blob->w) + (blob->h*blob->h);
   d = sqrt((double)d1);

   if((blob->h < hlimit) || (d < (blob->pixcnt/(float)esw)))
      return(FALSE);
   else
      return(TRUE);
}

/************************************************************/
dash_like_blob(blob, esw, charh)
BLOB *blob;
int esw, charh;
{
   int d1;
   float pw, d;

   /* if one horizontal stroke, then pw ==> diagonal pixel width of blob */
   pw = blob->pixcnt/(float)esw;
   d1 = (blob->w*blob->w) + (blob->h*blob->h);
   d = sqrt((double)d1);
   /* pw must be <= diagonal pixel width or */
   /* not more than one stroke width longer */
   if((pw <= d) || ((pw-d) <= esw))
      return(TRUE);
   else
      return(FALSE);
}

/************************************************************/
is_smallpunct_blob(blob, esw, charh)
BLOB *blob;
int esw, charh;
{
   static int ocharh = -1, oesw = -1;
   static int plimit;

   if((ocharh != charh) || (oesw != esw)){
      ocharh = charh;
      oesw = esw;
      /* compute area of half charh by dot width */
      plimit = (charh>>1) * esw;
   }

   if((!one_like_blob(blob, esw, charh)) && (blob->pixcnt < plimit)){
      return(TRUE);
   }
   else{
      return(FALSE);
   }
}

/************************************************************/
is_top_of_5(fblob, tblob, esw, charh)
BLOB *fblob, *tblob;
int esw, charh;
{
   int xlimit, ylimit;

   xlimit = min(fblob->w>>1, tblob->w>>1);
   ylimit = fblob->h>>1;

   /* top must be less than the hieght of the bottom */
   if((tblob->h < fblob->h) &&
      /* top not too far to the right of bottom */
      ((tblob->x1 - fblob->x2) < xlimit) &&
      /* top not too far down wrt bottom */
      ((tblob->y2 - fblob->y1) < ylimit) &&
      /* top not too far to the left of bottom */
      (fblob->x1 - tblob->x1 < xlimit) &&
      /* top is dash-like */
      dash_like_blob(tblob, esw, charh))
      return(TRUE);
   else
      return(FALSE);
}

/************************************************************/
is_top_of_alpha(fblob, tblob, esw, charh)
BLOB *fblob, *tblob;
int esw, charh;
{
   int xlimit, ylimit;

   xlimit = min(fblob->w>>1, tblob->w>>1);
   ylimit = fblob->h>>2;

   /* top must be less than the hieght of the bottom */
   if((tblob->h < fblob->h) &&
      /* top not too far to the right of bottom */
      ((tblob->x1 - fblob->x2) < xlimit) &&
      /* top not too far down wrt bottom */
      ((tblob->y2 - fblob->y1) < ylimit) &&
      /* top not too far to the left of bottom */
      (fblob->x1 - tblob->x1 < xlimit) &&
      /* top is dash-like */
      ((tblob->w/(float)tblob->h) > DASH_ASPECT) &&
      dash_like_blob(tblob, esw, charh))
      return(TRUE);
   else
      return(FALSE);
}
