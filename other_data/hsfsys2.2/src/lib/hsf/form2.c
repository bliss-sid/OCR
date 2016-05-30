/*
# proc: load_form_image - loads a binary IHead image into memory as 1 pixel
# proc:                   per byte.
# proc: register_form - registers an HSF form using a generalized method.
# proc:
*/

#include <stdio.h>
#include <ihead.h>
#include <hsfsys.h>
#include <hsfsys2.h>
#include <defs.h>

/*************************************************************/
load_form_image(ihead, incdata, bpi, iw, ih, ifile)
IHEAD **ihead;
unsigned char **incdata;
int *bpi, *iw, *ih;
char *ifile;
{
   unsigned char *inbdata, *mallocate_image();

   set_timer();

   ReadBinaryRaster(ifile, ihead, &inbdata, bpi, iw, ih);
   *incdata = mallocate_image(*iw, *ih, 8);
   bits2bytes(inbdata, *incdata, (*iw)*(*ih));
   free(inbdata);

   time_accum(_form_load_);
   result_print("Form Image Loaded", ifile);
}

/*************************************************************/
register_form(regcdata, incdata, bpi, iw, ih, regcoords, ncoords, zoomflag)
unsigned char *incdata, **regcdata;
int bpi, iw, ih, *regcoords, ncoords, zoomflag;
{
   set_timer();

   if(!genregform8(regcdata, incdata, bpi, iw, ih, regcoords, ncoords, zoomflag)){
      fprintf(stderr, "Warning : register_form : Form Registration Failed\n");
      verbose_print("register_form", "Form Registration Failed");
      return(FALSE);
   }

   time_accum(_form_reg_);
   verbose_print("register_form", "Form Registered");

   return(TRUE);
}
