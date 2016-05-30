/*
# proc: do_hsf_form - reads in an HSF form image, its spatial template,
# proc:               registers the form image, and conducts form removal.
# proc: init_form - reads in an HSF form image and its spatial template.
# proc:
# proc: process_form2 - registers an HSF form image (quickly) and conducts form
# proc:                 removal.
# proc: free_form - deallocates images and IHead structures associated with
# proc:             a full-page HSF form.
*/

#include <stdio.h>
#include <ihead.h>
#include <hsform.h>
#include <hsfsys.h>

/*************************************************************/
do_hsf_form(msg, ihead, indata, regdata, rmdata, iw, ih, tmplt,
            hsffile, tmpltfile, ptsfile, maskfile)
char *msg;
IHEAD **ihead;
unsigned char **indata, **regdata, **rmdata;
int *iw, *ih;
HSFORM *tmplt;
char *hsffile, *tmpltfile, *ptsfile, *maskfile;
{
   /* initialize form template and form image variables */
   set_timer();
   init_form(hsffile, tmpltfile, ihead, indata, iw, ih, tmplt);
   time_elapsed(msg, "initialize");

   /* register the form image and conduct form removal */
   process_form2(msg, regdata, rmdata, (*indata), *iw, *ih, ptsfile, maskfile);
}

/*************************************************************/
init_form(ifile, tfile, ihead, idata, iw, ih, tmplt)
char *ifile, *tfile;
IHEAD **ihead;
unsigned char **idata;
int *iw, *ih;
HSFORM *tmplt;
{
   int bpi;

   /* read in the HSF form image */
   ReadBinaryRaster(ifile, ihead, idata, &bpi, iw, ih);
   verbose_print("init_form", "HSF Form Loaded");

   /* read in the HSF spatial template */
   read_hsftmplt(tfile, tmplt);
   verbose_print("init_form", "HSF Template Loaded");
}

/*************************************************************/
process_form2(msg, regdata, rmdata, indata, iw, ih, pfile, dfile)
char *msg;
unsigned char **regdata, **rmdata, *indata;
int iw, ih;
char *pfile, *dfile;
{

   /* register the HSF form image */
   set_timer();
   register_hsf2(regdata, indata, iw, ih, pfile);
   time_elapsed(msg, "register  ");
   verbose_print("process_form", "HSF Form Registered");

   /* remove the form information from the registered image */
   set_timer();
   remove_form(rmdata, *regdata, iw, ih, dfile);
   time_elapsed(msg, "remove    ");
   verbose_print("process_form", "HSF Form Removed");
}

/*************************************************************/
free_form(ihead, indata, regdata, rmdata)
IHEAD *ihead;
unsigned char *indata, *regdata, *rmdata;
{
   free(indata);
   free(ihead);
   free(regdata);
   free(rmdata);
   /* deallocate findblob() memory */
   end_findblobs();
}

