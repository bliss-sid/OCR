/*
# pgrm: mis2pat2 - computes KL feature vectors from 32x32 or 128x128 binary
# pgrm:           images stored in an MIS file and writes out a patterns file.
# pgrm:           This file is used by the programs "hsfsys2" and "mlp".
*/

#include <stdio.h>
#include <sys/param.h>
#include <string.h>
#include <stdlib.h>
#include <defs.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>

#define ES  	  32
#define ES4 	 128 		/* always 4 ES 		*/
#define ES2 	1024 		/* always ES * ES 	*/


main(argc, argv)
int argc; char **argv;
{
MFS  *mises, *clses;
char *evtfile, *patfile, *setfile, fn[MAXPATHLEN];
unsigned char *pm1, *allocate_image();
int   i, j, k, tot, tlen, nevts, err;
int   progress, hexclass, writemis, hex;
float *mean, *vari, *evts, *kptr;
float *klts, *klmu, *klsume;
MIS   *inp, *out;
MFS   *cls, *set;
MFS   *allcls, *allocmfs();
float *allklts, *aptr;
float *sfctr, *scal_x, *scal_y;

   procargs(argc, argv, &mises, &clses, &setfile, &evtfile, &patfile,
            &progress, &hexclass, &writemis);

   set = readmfsfile(setfile);
   read_basis(evtfile, &tlen, &nevts, ES2, &mean, &vari, &evts); 

   if (tlen != ES2)
      fatalerr(argv[0], evtfile, "eigenvectors are of wrong dimension");

   kl_premult(nevts, mean, evts, &klmu, &klsume);

   for ( i = 0, tot = 0 ; i < mises->num ; i++)
      inp = readmishdr(mises->values[i]),
      tot += inp->ent_num,
      freemishdr(inp);

   if ((allklts = aptr = (float *)malloc(nevts * tot * sizeof(float))) == NULL)
     syserr(argv[0], "malloc", "space for all the features");

   allcls = allocmfs(tot);

   for ( i = 0 ; i < mises->num ; i++ )
   {
      inp = readmisfile(mises->values[i]);
      if ((inp->misd != 1))
       fatalerr(argv[0], mises->values[i], "not binary data");
      if (!((inp->enth == ES ) && (inp->entw == ES )) &&
          !((inp->enth == ES4) && (inp->entw == ES4))  )
       fatalerr(argv[0], mises->values[i], "not 32x32 nor 128x128 mis entries");

      cls = readmfsfile(clses->values[i]);
      if (inp->ent_num != cls->num)
        fatalerr(argv[0], "diff num entries", "in image and class files");

      for ( j = 0 ; j < cls->num ; j++ ){
         if (hexclass == TRUE){
            if ((err = sscanf(cls->values[j], "%x", &hex)) != 1)
               syserr(argv[0], "sscanf", "failed on hex field of class mfs");
            if ((hex < 0x20) || (hex > 0x7E))
               fatalerr(argv[0], "class hex value", "is not printable ascii");
            sprintf(cls->values[j], "%c", hex);
         }
         appendmfs(allcls, cls->values[j]);
      }

      if (inp->entw == ES4)
      {
         norm_2nd_gen2(&out, &scal_x, &scal_y, inp);
         freemis(inp);
         free(scal_x);
         free(scal_y);
         shear_mis(&inp, &sfctr, out); 
         freemis(out);
         free(sfctr);
         if (writemis == TRUE)
           sprintf(fn, "%s.32", mises->values[i]),
           writemisfile(fn, inp);
      }

      /* --------------------------------------------------------------- */
      /* expand the binary data by a factor of eight so that 1 bit is    */
      /* now represented as one char 1 or 0                              */
      pm1 = allocate_image(inp->misw, inp->mish, 8);
      bits2bytes(inp->data, pm1, inp->misw*inp->mish);
      /* --------------------------------------------------------------- */

      kl_transform(pm1, inp->ent_num, nevts, evts, klmu, klsume, &klts);

      /* append these features to the buffer of all the kl transforms 	*/
      for ( j = 0, kptr = klts ; j < inp->ent_num ; j++ )
         for ( k = 0 ; k < nevts ; k++ )
            *aptr++ = *kptr++;

      free(pm1);
      free(klts);
      freemis(inp);
      freemfs(cls);

      if (progress == TRUE)
         fprintf(stdout, "done %s\r", mises->values[i]),
         fflush(stdout);
   }

   write_bin_patterns(patfile, allklts, allcls, set->values, nevts, tot,
                      nevts, set->num);

   free(mean); free(vari); free(evts);
   freemfs(set);
   free(klmu); free(klsume);
   free(allklts); freemfs(allcls);
   freemfs(clses);
   freemfs(mises);
   free(setfile); free(evtfile); free(patfile);

   exit(0);
}

/***********************************************************************/
procargs(argc, argv, mises, clses, setfile, evtfile, patfile,
        progress, hexclass, writemis)
int	 argc, *progress, *hexclass, *writemis;
MFS     **mises, **clses;
char	**argv, **evtfile, **setfile, **patfile;
{
extern int getopt(), optind;
int c;

   if (argc < 6)
      usage(argv);

   *progress = *writemis = *hexclass = FALSE;
   while ((c = getopt(argc, argv, "hnv")) != -1)
      switch (c)
      {
          case 'h' : *hexclass = TRUE; break;
          case 'n' : *writemis = TRUE; break;
          case 'v' : *progress = TRUE; break;
          default  :  usage(argv);
      }

   if (argc - optind != 5)
      usage(argv);

   *setfile = strdup(argv[optind++]);
   *evtfile = strdup(argv[optind++]);
   *patfile = strdup(argv[optind++]);
   *clses   = readmfsfile(argv[optind++]);
   *mises   = readmfsfile(argv[optind++]);

   if ((*clses)->num != (*mises)->num)
      fatalerr(argv[0], "ncls+nmis","unequal numbers of class and image files");
}

usage(argv)
char **argv;
{
   fprintf(stderr, "Usage: %s:\n", argv[0]);
   fprintf(stderr, "\t-h  accept hexadecimal class files\n");
   fprintf(stderr, "\t-n  with 128x128 images write normed+sheared 32x32 intermediate misfiles\n");
   fprintf(stderr, "\t-v  be verbose - notify completion of each misfile\n");
   fprintf(stderr, "\t    classset evtfile outfile mfs_of_clsfiles mfs_of_misfiles\n");
   exit(1);
}
