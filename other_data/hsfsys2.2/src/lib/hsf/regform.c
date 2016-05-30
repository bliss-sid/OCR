/*
# proc: genregform8 - generalized form registration routine that accounts for
# proc:               rotation and translation by locating dominant vertical
# proc:               and horizontal line structures in the image.
# proc: train_genregform8 - locates registration structures for training
# proc:               generalized form registration code.
*/

#include <stdio.h>
#include <math.h>
#include <regform.h>
#include <deskew.h>
#include <defs.h>

/*****************************************************************************/
genregform8(rdata, cdata, bpi, w, h, refs, nref, zflag)
unsigned char **rdata, *cdata;
int bpi, w, h;
int *refs, nref, zflag;
{
   unsigned char *zdata, *rzdata;
   unsigned char *imagedup(), *allocate_image();
   int i, zw, zh, zbpi, *zrefs;
   int errlimit, rejlimit;
   float theta; double calc_skew();
   int tx, ty;
   float dx, mxx, mxy, dy, myy, myx;

   switch(zflag){
   case 8:
      squashx8_8(cdata, w, h, &zdata, &zw, &zh);
   break;
   case 4:
      zoomx4(cdata, w, h, &zdata, &zw, &zh);
   break;
   case 1:
      zdata = imagedup(cdata, w, h, 8);
      zw = w;
      zh = h;
   break;
   default:
      fatalerr("genregform8", "illegal zoom factor", "must be 1|4|8");
   break;
   }

   /* detect rotation */
   theta = (float)calc_skew(zdata, zw, zh, sumlineexps);
   rzdata = allocate_image(zw, zh, 8);
   rotate_push8_float(zdata, rzdata, zw, zh, zw>>1, zh>>1, theta);
   free(zdata);

   /* detect translation */
   zbpi = sround(bpi/(float)zflag);
   malloc_int(&zrefs, nref, "genregform8 : zrefs");
   for(i = 0; i < nref; i++)
      zrefs[i] = refs[i] / zflag;
   /* if non-valid bpi them use fixed pixel amount */
   if(zbpi <= 0){
      errlimit = ERR_PIX_LIMIT;
      rejlimit = REJ_PIX_LIMIT;
   }
   else{
      /* otherwise, compute pixels per given inches */
      errlimit = sround(zbpi * ERR_INCH_LIMIT);
      rejlimit = sround(zbpi * REJ_INCH_LIMIT);
   }

   if(!calc_trans(&tx, &ty, rzdata, zw, zh, zrefs, nref, errlimit, rejlimit)){
      free(zrefs);
      free(rzdata);
      return(FALSE);
   }

   tx *= zflag;
   ty *= zflag;
   free(zrefs);
   free(rzdata);

   /* if required, apply rotation w/ translation transformation */
   if((fabs(theta) < SMALL_THETA) && (tx == 0) && (ty == 0))
      *rdata = imagedup(cdata, w, h, 8);
   else{
      rotntrans_push_params(theta, w>>1, h>>1, tx, ty,
                            &dx, &mxx, &mxy, &dy, &myy, &myx);
      *rdata = allocate_image(w, h, 8);
      push_param3_chrimage(*rdata, cdata, w, h, dx, mxx, mxy, dy, myy, myx);
   }
   return(TRUE);
}

/*****************************************************************************/
train_genregform8(left, right, top, bot, cdata, bpi, w, h)
int *left, *right, *top, *bot;
unsigned char *cdata;
int bpi, w, h;
{
   unsigned char *rdata, *allocate_image();
   float theta; double calc_skew();

   /* remove rotation */
   theta = (float)calc_skew(cdata, w, h, sumlineexps);
   rdata = allocate_image(w, h, 8);
   rotate_push8_float(cdata, rdata, w, h, w>>1, h>>1, theta);

   /* locate registration structures */
   find_trans_reg_points(left, right, top, bot, rdata, w, h);
   free(rdata);
}
