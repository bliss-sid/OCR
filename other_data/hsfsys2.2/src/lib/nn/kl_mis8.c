/*
# proc: kl_transform_mis8 - takes an mis structure of spatially normalized
# proc:                    (32 X 32) characters formatted 1 pixel per byte
# proc:                    and computes a kl-feature vector for each one.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>


/*************************************************************/
kl_transform_mis8(klts, mis, basis, klmu, klsume, nbasis)
float **klts, *basis, *klmu, *klsume;
MIS *mis;
int nbasis;
{
   if(mis->misd != 8)
      fatalerr("kl_transform_mis8", "MIS entries must be depth 8", NULL);

   if((mis->entw != 32) || (mis->enth != 32))
      fatalerr("kl_transform_mis8", "MIS entries must be 32 X 32", NULL);

   /* compute kl transform on promoted image data */
   kl_transform(mis->data, mis->ent_num, nbasis, basis, klmu, klsume, klts);
}
