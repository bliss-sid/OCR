/*
# proc: kl_transform_mis - takes an mis structure of spatially normalized
# proc:                    (32 X 32) characters and computes a kl-feature
# proc:                    for each one.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>


/*************************************************************/
kl_transform_mis(klts, mis, basis, klmu, klsume, nbasis)
float **klts, *basis, *klmu, *klsume;
MIS *mis;
int nbasis;
{
   int len;
   unsigned char *cdata, *mallocate_image();

   if((mis->entw != 32) || (mis->enth != 32))
      fatalerr("kl_transform_mis", "MIS entries must be 32 X 32", NULL);

   /* promote transposed data to 1 byte per pixel */
   len = mis->misw * mis->mish;
   cdata = mallocate_image(mis->misw, mis->mish, 8);
   bits2bytes(mis->data, cdata, len);

   /* compute kl transform on promoted image data */
   kl_transform(cdata, mis->ent_num, nbasis, basis, klmu, klsume, klts);
   free(cdata);
}
