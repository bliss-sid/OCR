/*
# proc: detect_multi_char_blob - determines if the blob passed contains multiple
# proc:              characters based on a simple 2D-linear discrimination function.
# proc: detect_multi_alpha_blob - determines if the blob passed contains multiple
# proc:    alphabetic characters based on a simple 2D-linear discrimination function.
*/

#include <blobls.h>
#include <defs.h>

/*******************************************************************/
detect_multi_char_blob(blob, esw, charh, dlm, dlx, dly)
BLOB *blob;
int esw, charh;
float dlm, dlx, dly;
{
   float hvb_fet, hvw_fet, thresh_y, compute_y();
   static int oesw = -1, ocharh = -1, std_stroke;

   if((oesw != esw) || (ocharh != charh)){
      oesw = esw;
      ocharh = charh;
      std_stroke = esw * charh;
   }

   /* compute left subimage features */
   /* first feature measures density in the image in termes of */
   /* number of "standard" strokes */
   hvb_fet = blob->pixcnt/(float)std_stroke;
   /* second feature simple measures aspect ratio of the image */
   hvw_fet = blob->w/(float)charh;

   thresh_y = compute_y(hvw_fet, dlm, dlx, dly);

   if(hvb_fet > thresh_y)
      return(TRUE);
   else
      return(FALSE);
}

/*******************************************************************/
detect_multi_alpha_blob(blob, esw, charh, dlm, dlx, dly)
BLOB *blob;
int esw, charh;
float dlm, dlx, dly;
{
   float hvb_fet, hvw_fet, thresh_y, compute_y();
   static int oesw = -1, ocharh = -1, std_stroke;

   if((oesw != esw) || (ocharh != charh)){
      oesw = esw;
      ocharh = charh;
      std_stroke = esw * charh;
   }

   /* compute left subimage features */
   /* first feature measures density in the image in termes of */
   /* number of "standard" strokes */
   hvb_fet = blob->pixcnt/(float)std_stroke;
   /* second feature measures aspect ratio of the image */
   /* relative to component height */
   hvw_fet = blob->w/(float)blob->h;

   thresh_y = compute_y(hvw_fet, dlm, dlx, dly);

   if(hvb_fet > thresh_y)
      return(TRUE);
   else
      return(FALSE);
}
