/*
# pgrm: mis2evt - produces the eigenvectors from the covariance matrix of 32x32
# pgrm:           or 128X128 binary characters stored in an MIS file.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>
#include <defs.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>

#define ES  	  32
#define ES4      128            /* always 4 ES          */
#define ES2     1024            /* always ES * ES       */

main(argc, argv)
int argc; char **argv;
{
   char  fn[MAXPATHLEN], *evtfile;
   MFS   *names;
   IHEAD *ihead;
   int   progress, writemis;
   int   reqevts, rtnevts;
   FILE  *fp;
   int   entw, enth, vlen;
   float *mean, *stdv, *covar;
   float *eigvals, *eigvctrs;

   procargs(argc, argv, &names, &reqevts, &evtfile, &progress, &writemis);

   /* get mis images' entry width and height */
   if((fp = fopen(names->values[0], "rb")) == NULL)
      fatalerr("main", "fopen", names->values[0]);
   ihead = readihdr(fp);
   fclose(fp);
   entw = get_par_x(ihead);
   enth = get_par_y(ihead);
   free(ihead);

   /* build covariance matrix from images */
   if((entw == ES4) && (enth == ES4)){
      /* special case if images are 128 X 128 */
      make_misnorm_covar(names->values, names->num, &mean, &stdv, &covar,
                         progress, writemis);
      vlen = ES2;
   }
   else{
      make_mis_covar(names->values, names->num, &mean, &stdv, &covar, progress);
      vlen = entw * enth;
   }

/*
   write_basis("test.cov", vlen, vlen, vlen, mean, stdv, covar);
*/

   if (reqevts > vlen)
   {
      fprintf(stderr, "Incorrect usage: the number of eigenvectors cannot\n");
      fprintf(stderr, "possibly exceed the dimensionality of the images.\n"); 
      fprintf(stderr, "Continuing anyway...requesting %d vectors", vlen);
      reqevts = vlen;
   }

   /* compute eigenvectors from covariance matrix */
   if(!diag(covar, vlen, reqevts, progress, &rtnevts, &eigvals, &eigvctrs))
      fatalerr("main", "diag", "failed to compute eigenvectors");

   if (rtnevts < reqevts){
       fprintf(stderr, "Warning: found less eigenvectors than requested\n");
       fflush(stderr);
   }

   if(progress){
      fprintf(stdout, "%d Eigenvectors found, writing %d,\n", rtnevts, reqevts);
      fprintf(stdout, "  corresponding to highest eigenvalues\n");
   }

   /* write the eigenvectors to file */
   write_basis(evtfile, vlen, reqevts, vlen, mean, stdv, eigvctrs);

   free(mean);
   free(stdv);
   free(covar);
   free(eigvals);
   free(eigvctrs);
   freemfs(names);
   free(evtfile);

   exit(0);
}



procargs(argc, argv, names, nevts, evtfile, progress, writemis)
int	  argc, *nevts, *progress, *writemis;
char	**argv, **evtfile; MFS **names;
{
extern int getopt(), optind;
int c;

   if (argc < 4)
      usage(argv);

   *writemis = FALSE;
   *progress = FALSE;

   while ((c = getopt(argc, argv, "nv")) != -1)
      switch (c)
      {  
          case 'n' : *writemis = TRUE; break;
          case 'v' : *progress = TRUE; break;
          default  :  usage(argv);
      }

   if (argc - optind != 3)
      usage(argv);

   sscanf(argv[optind++], "%d", nevts);
   *evtfile = strdup(argv[optind++]);
   *names = readmfsfile(argv[optind]);
}

usage(argv)
char **argv;
{
   fprintf(stderr, "Usage: %s:\n", argv[0]);
   fprintf(stderr, "\t-n  for 128x128 input write normed+sheared 32x32 intermediate misfiles\n");
   fprintf(stderr, "\t-v  be verbose - notify completion of each image\n");
   fprintf(stderr, "\t    nrequiredevts evtfile mfs_of_misfiles\n");
   exit(1);
}
