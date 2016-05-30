/*
# pgrm: trainreg - program for training generalized form registration on new
# pgrm:            form type. The output of this program is used in "hsfsys2".
*/

#include <stdio.h>
#include <ihead.h>
#include <deskew.h>

/*****************************************************************************/
main(argc, argv)
int argc;
char *argv[];
{
   char *infile, *ptsfile;
   int w, h, bpi;
   IHEAD *ihead;
   unsigned char *bdata, *cdata, *mallocate_image();
   int left, right, top, bot;

   procargs(argc, argv, &infile, &ptsfile);
   ReadBinaryRaster(infile, &ihead, &bdata, &bpi, &w, &h);
   cdata = mallocate_image(w, h, 8);
   bits2bytes(bdata, cdata, w*h);

   train_genregform8(&left, &right, &top, &bot, cdata, bpi, w, h);

   write_regpts_mfs(ptsfile, left, right, top, bot);

   free(bdata);
   free(cdata);
   free(ihead);

   exit(0);
}


/*****************************************************************************/
write_regpts_mfs(file, left, right, top, bot)
char *file;
int left, right, top, bot;
{
   FILE *fp;

   if((fp = fopen(file, "wb")) == NULL)
      syserr("write_regpts_mfs", "fopen", file);

   fprintf(fp, "%d\n", 4);
   fprintf(fp, "%d\n", left);
   fprintf(fp, "%d\n", right);
   fprintf(fp, "%d\n", top);
   fprintf(fp, "%d\n", bot);

   fclose(fp);
}

/*****************************************************************************/
procargs(argc, argv, infile, ptsfile)
int argc;
char *argv[], **infile, **ptsfile;
{
   if(argc < 2)
      fatalerr("Usage", argv[0], "<form_image> <out_points>");
   *infile = argv[1];
   *ptsfile = argv[2];
}
