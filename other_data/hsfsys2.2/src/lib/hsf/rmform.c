/*
# proc: remove_form - subtracts the form information from a registered binary
# proc:               image, given an image of a blank form.
# proc:               
*/

#include <stdio.h>
#include <ihead.h>

remove_form(rmdata, regdata, w, h, formfile)
unsigned char **rmdata, *regdata;
int w, h;
char *formfile;
{
   unsigned char *fdata;
   int bpi, fw, fh;
   IHEAD *fhead;

   ReadBinaryRaster(formfile, &fhead, &fdata, &bpi, &fw, &fh);
   if(w != fw)
      fatalerr("remove_form", "form and template image widths differ", NULL);
   if(h != fh)
      fatalerr("remove_form", "form and template image heights differ", NULL);
   nandbinimage(rmdata, fdata, regdata, w, h);
   free(fdata);
   free(fhead);
}
