/*
# proc: register_hsf2 - locates the registration points in a binary image of
# proc:                 an HSF form and transforms the image to match the
# proc:                 given reference points quickly pushing pixels.
*/

#include <stdio.h>
#include <hsfsys.h>

/*************************************************************/
register_hsf2(rdata, idata, iw, ih, ptsfile)
unsigned char **rdata, *idata;
int iw, ih;
char *ptsfile;
{
   int *xref, *yref, nref;
   int *xhyp, *yhyp, fill_flag, npts;
   float dx, mxx, mxy, dy, myy, myx;

   readmfsint2(ptsfile, &xref, &yref, &nref);
   malloc_int(&xhyp, nref, "register_hsf2 : xhyp");
   malloc_int(&yhyp, nref, "register_hsf2 : yhyp");
   hsfpoints(idata, iw, ih, xhyp, yhyp);
   chknfindparam3(&npts, &dx, &mxx, &mxy, &dy, &myy, &myx,
                   xref, yref, nref, xhyp, yhyp, nref, RM_LIMIT, ERR_LIMIT);
   fill_flag = 0;
   f_fit_param3_image2(rdata, idata, iw, ih, dx, mxx, mxy, dy, myy, myx,
                       fill_flag);
   free(xref);
   free(yref);
   free(xhyp);
   free(yhyp);
}
