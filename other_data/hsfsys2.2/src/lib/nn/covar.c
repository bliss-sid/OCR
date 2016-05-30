/*
# proc: make_covar - calculates the mean vector and covariance matrix
# proc:              of floating feature vector sets.
# proc: make_mis_covar - calculates the mean vector and covariance matrix
# proc:                  of binary mis image sets.
# proc: make_misnorm_covar - calculates the mean vector and covariance matrix
# proc:                  of the normalized and sheared binary mis image sets.
*/


#include <stdio.h>
#include <sys/param.h>
#include <ihead.h>
#include <mis.h>

/*****************************************************************/
make_covar(feats, nPats, nInps, mean, stdv, cova)
float *feats, **mean, **stdv, **cova;
int nPats, nInps;
{
float *ptr, *ftr, *mtr, *ctr;
int   j, k, l;

   if (((*mean) = (float *)calloc(nInps, sizeof(float))) == NULL)
      syserr("make_covar", "space for mean vector", "calloc");
   if (((*stdv) = (float *)calloc(nInps, sizeof(float))) == NULL)
      syserr("make_covar", "space for stdv vector", "calloc");
   if (((*cova) = (float *)calloc(nInps*nInps, sizeof(float))) == NULL)
      syserr("make_covar", "space for covariance matrix", "calloc");

   /* compute the mean vector */
   for (j = 0, ptr = feats ; j < nPats ; j++)
      for (k = 0, mtr = *mean ;  k < nInps ; k++)
         *mtr++ += *ptr++;
   for (j = 0, mtr = *mean ;  j < nInps ; j++)
      *mtr++ /= (float)nPats;
 
   /* and the correlation matrix */
   for (j = 0, ptr = feats ; j < nPats ; j++)
      for (k = 0, ctr = *cova, mtr = ptr  ; k < nInps ; k++, ptr++)
         for (l = 0, ftr = mtr   ; l < nInps ; l++)
            *ctr++ += *ftr++ * *ptr;
   for (j = 0, ctr = *cova ;  j < nInps*nInps ; j++)
      *ctr++ /= (float)nPats;

   /* and subtract the outer product of the mean */
   /* to yield the covariance matrix             */
   for (k = 0, ctr = *cova, ptr = *mean ;  k < nInps ; k++, ptr++)
      for (l = 0, mtr = *mean ; l < nInps ; l++)
         *ctr++ -= *mtr++ * *ptr;
}

/*****************************************************************/
/* makes the covariance matrix of binary mis images     */
/* the binary input image vectors must be 1-bit/pixel   */
/* pixel values are regarded for mathematical purposes  */
/* as +1 and -1                                         */
/* the code is a little more cryptic than would be the case for */
/* for grey level images since large efficiency gains are       */
/* available for binary data                                    */

/* speed is data dependent - faster for fewer true ink pixels   */

/* Covariance is R = (1/N) \sum_{i} (u_{i} - \mu) (u_{i} - \mu)^{T}     */
/*                 = (1/N) \sum_{i} u_{i}u_{i}^{T} - \mu \mu^{T}        */
/* the products are skipped as 1.1 = 1 and -1.1 = -1 and -1.-1 = 1      */
/* are reducible to equality tests andd initialization to -1 of all     */

/* the routine returns the dimensionality of the matrix */

int make_mis_covar(misfiles, nmisfiles, mmean, mstdv, mcova, progflag)
char **misfiles; int nmisfiles;
float **mmean, **mstdv, **mcova;
int progflag;
{
float *cova, *mean, *frow, fotal, *stdv;
int   *covi, *meai, *irow, total, nPixs, nPats, nByte, i, j, k, n;
unsigned char *pm1, *data, *mallocate_image();
MIS *mis;

   for ( i = total = 0 ; i < nmisfiles ; i++)
   {
      mis = readmisfile(misfiles[i]);
      if (progflag){
         fprintf(stdout, "doing %4d chars of %s\r", mis->ent_num, misfiles[i]);
         fflush(stdout);
      }
      if (get_depth(mis->head) != 1)
         fatalerr("make_mis_covar", misfiles[i], "depth != 1 bit"); 
      if (i == 0)
      {
         nPixs = mis->entw * mis->enth;
         nByte = nPixs / 8;
         pm1 = mallocate_image(mis->entw, mis->enth, 8);
         if ((mean = *mmean = (float *)calloc(nPixs, sizeof(float))) == NULL)
            syserr("make_mis_covar", "float mean vector", "calloc");
         if ((stdv = *mstdv = (float *)calloc(nPixs, sizeof(float))) == NULL)
            syserr("make_mis_covar", "stdv vector", "calloc");
         if ((cova = *mcova = (float *)calloc(nPixs*nPixs,sizeof(float)))==NULL)
            syserr("make_mis_covar", "covariance matrix", "calloc");
         meai = (int *)mean;	/* same space for different purposes at	*/
         covi = (int *)cova;	/* different times.			*/
      }
      else
      {
         if (nPixs != mis->entw * mis->enth)
            fatalerr("make_mis_covar", misfiles[i], "inconsistent w and h"); 
      }
      total += nPats = mis->ent_num;

      /* accumulate the mean vector of all chars of all misfiles */
      for ( n = 0, data = mis->data ; n < nPats ; n++, data += nByte )
      {
         bits2bytes(data, pm1, nPixs);
         for ( j = 0 ; j < nPixs ; j++ )
            if (pm1[j])
               meai[j]++;

         /* all the time goes in this loop */
         /* the if statement makes the execution time data dependent  */
         /* the != is quicker than the == since more pixels are equal */
         /* calculate the correlation matrix of all chars of all misfiles */
         for ( j = 0, irow = covi ; j < nPixs ; j++, irow += nPixs )
            for ( k = 0 ; k <= j ; k++ )	/* I used pointers and a */
               if (pm1[j] != pm1[k])            /* a = pm1[j] here for speed */
                  irow[k]--;                    /* but the cc -O2 does it */
      }
      freemis(mis);
   }
   fotal = (float)total;

   for ( j = 0 ; j < nPixs ; j++ )
      mean[j] = (float)((meai[j]<<1) - total) / fotal;

   for ( j = 0, irow = covi, frow = cova ; j < nPixs ;
      j++, irow += nPixs, frow += nPixs )
      for ( k = 0 ; k <= j ; k++ )
         frow[k] = (float)((irow[k]<<1) + total) / fotal - mean[j] * mean[k];

   for ( j = 0 ; j < nPixs ; j++ )
      for ( k = 0 ; k < j ; k++ )
         *(cova + k*nPixs + j) = *(cova + j*nPixs + k);

   if (progflag){
      fprintf(stdout, "\nCovariance computed from %d images\n", total);
      fflush(stdout);
   }

   free(pm1);
   return nPixs;
}

/*****************************************************************/
int make_misnorm_covar(misfiles, nmisfiles, mmean, mstdv, mcova,
                       progflag, writeflag)
char **misfiles; int nmisfiles;
float **mmean, **mstdv, **mcova;
int progflag, writeflag;
{
float *cova, *mean, *frow, fotal, *stdv;
int   *covi, *meai, *irow, total, nPixs, nPats, nByte, i, j, k, n;
unsigned char *pm1, *data, *mallocate_image();
MIS *mis, *nmis;
float *sfctr, *scal_x, *scal_y;
char fn[MAXPATHLEN];

   for ( i = total = 0 ; i < nmisfiles ; i++)
   {
      mis = readmisfile(misfiles[i]);
      if (get_depth(mis->head) != 1)
         fatalerr("make_mis_covar", misfiles[i], "depth != 1 bit"); 
      if (progflag){
         fprintf(stdout, "doing %4d chars of %s\r", mis->ent_num, misfiles[i]);
         fflush(stdout);
      }

      norm_2nd_gen2(&nmis, &scal_x, &scal_y, mis);
      freemis(mis);
      free(scal_x);
      free(scal_y);
      shear_mis(&mis, &sfctr, nmis);
      freemis(nmis);
      free(sfctr);
      if (writeflag){
         sprintf(fn, "%s.32", misfiles[i]);
         writemisfile(fn, mis);
      }

      if (i == 0)
      {
         nPixs = mis->entw * mis->enth;
         nByte = nPixs / 8;
         pm1 = mallocate_image(mis->entw, mis->enth, 8);
         if ((mean = *mmean = (float *)calloc(nPixs, sizeof(float))) == NULL)
            syserr("make_mis_covar", "float mean vector", "calloc");
         if ((stdv = *mstdv = (float *)calloc(nPixs, sizeof(float))) == NULL)
            syserr("make_mis_covar", "stdv vector", "calloc");
         if ((cova = *mcova = (float *)calloc(nPixs*nPixs,sizeof(float)))==NULL)
            syserr("make_mis_covar", "covariance matrix", "calloc");
         meai = (int *)mean;	/* same space for different purposes at	*/
         covi = (int *)cova;	/* different times.			*/
      }
      else
      {
         if (nPixs != mis->entw * mis->enth)
            fatalerr("make_mis_covar", misfiles[i], "inconsistent w and h"); 
      }
      total += nPats = mis->ent_num;

      /* accumulate the mean vector of all chars of all misfiles */
      for ( n = 0, data = mis->data ; n < nPats ; n++, data += nByte )
      {
         bits2bytes(data, pm1, nPixs);
         for ( j = 0 ; j < nPixs ; j++ )
            if (pm1[j])
               meai[j]++;

         /* all the time goes in this loop */
         /* the if statement makes the execution time data dependent  */
         /* the != is quicker than the == since more pixels are equal */
         /* calculate the correlation matrix of all chars of all misfiles */
         for ( j = 0, irow = covi ; j < nPixs ; j++, irow += nPixs )
            for ( k = 0 ; k <= j ; k++ )	/* I used pointers and a */
               if (pm1[j] != pm1[k])            /* a = pm1[j] here for speed */
                  irow[k]--;                    /* but the cc -O2 does it */
      }
      freemis(mis);
   }
   fotal = (float)total;

   for ( j = 0 ; j < nPixs ; j++ )
      mean[j] = (float)((meai[j]<<1) - total) / fotal;

   for ( j = 0, irow = covi, frow = cova ; j < nPixs ;
      j++, irow += nPixs, frow += nPixs )
      for ( k = 0 ; k <= j ; k++ )
         frow[k] = (float)((irow[k]<<1) + total) / fotal - mean[j] * mean[k];

   for ( j = 0 ; j < nPixs ; j++ )
      for ( k = 0 ; k < j ; k++ )
         *(cova + k*nPixs + j) = *(cova + j*nPixs + k);

   if (progflag){
      fprintf(stdout, "\nCovariance computed from %d images\n", total);
      fflush(stdout);
   }

   free(pm1);
   return nPixs;
}

