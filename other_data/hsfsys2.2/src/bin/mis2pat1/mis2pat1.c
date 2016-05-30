/*
# pgrm: mis2pat1 - computes KL feature vectors from 32x32 or 128x128 binary
# pgrm:            images stored in an MIS file and writes out a patterns-tree
# pgrm:            file and a median vector file. These files are used by the
# pgrm:            program "hsfsys1".
*/

#include <stdio.h>
#include <sys/param.h>
#include <string.h>
#include <stdlib.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <kdtree.h>
#include <defs.h>

#define ES  	  32
#define ES4 	 128 		/* always 4 ES 		*/
#define ES2 	1024 		/* always ES * ES 	*/


main(argc, argv)
int argc; char **argv;
{
MFS  *mises, *clses;
char *setfile, *evtfile, *oroot;
char fn[MAXPATHLEN], patfile[MAXPATHLEN], medfile[MAXPATHLEN];
unsigned char *pm1, *allocate_image();
int   i, j, k, tot, tlen, nevts, err;
int   progress, hexclass, writemis, hex;

float *mean, *vari, *evts, *kptr;
float *klts, *klmu, *klsume, *median;
MIS   *inp, *out;
MFS   *cls, *set;
float *allklts, *aptr;
int   *allclas, *cptr;
float *sfctr, *scal_x, *scal_y;

TREE *tree;

   procargs(argc, argv, &mises, &clses, &setfile, &evtfile, &oroot,
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
   if ((allclas = cptr = (int   *)malloc(        tot * sizeof(int  ))) == NULL)
     syserr(argv[0], "malloc", "space for all the classes");

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

      if (hexclass == TRUE)
         for ( j = 0 ; j < cls->num ; j++ )
         {
            if ((err = sscanf(cls->values[j], "%x", &hex)) != 1)
               syserr(argv[0], "sscanf", "failed on hex field of class mfs");
            if ((hex < 0x20) || (hex > 0x7E))
               fatalerr(argv[0], "class hex value", "is not printable ascii");
            sprintf(cls->values[j], "%c", hex);
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

      /* and append the class to a similar buffer 			*/
      for ( j = 0, kptr = klts ; j < inp->ent_num ; j++ )
         *cptr++ = clset2index(set->values, set->num, cls->values[j]);

      free(pm1);
      free(klts);
      freemis(inp);
      freemfs(cls);

      if (progress == TRUE)
         fprintf(stdout, "done %s\r", mises->values[i]),
         fflush(stdout);
   }

   /* build the kdtree from the feature vectors */
   tree = build_tree(allklts, allclas, tot, nevts);

   /* write the kdtree to file */
   sprintf(patfile, "%s.pat", oroot);
   kdtreewrite(tree, patfile);

   /* compute median of feature vectors */
   featsclassmedian(allklts, allclas, tot, nevts, set->num, &median);
   sprintf(medfile, "%s.med", oroot);
   writemedianfile(medfile, median, set->num, nevts);

   free(mean); free(vari); free(evts);
   free(median);
   freemfs(set);
   free(allklts); free(allclas);
   free(klmu); free(klsume);
   treefree(tree);
   freemfs(clses);
   freemfs(mises);
   free(setfile); free(evtfile); free(oroot);

   exit(0);
}


int clset2index(set, nset, element)
char **set, *element; int nset;
{
int i;

   for ( i = 0 ; i < nset ; i++ )
      if (strcmp(set[i], element) == 0)
         return i;

   if (i == nset)
      fatalerr("clset2index", element, "this class is not in class set");
}


procargs(argc, argv, mises, clses, setfile, evtfile, oroot,
        progress, hexclass, writemis)
int	 argc, *progress, *hexclass, *writemis;
MFS     **mises, **clses;
char	**argv, **setfile, **evtfile, **oroot;
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
   *oroot = strdup(argv[optind++]);
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
   fprintf(stderr, "\t    classset evtfile outroot mfs_of_clsfiles mfs_of_misfiles\n");
   exit(1);
}

