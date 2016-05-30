/*
# proc: init_run - allocates and initializes memory required to batch process
# proc:            a group of HSF forms.
# proc: free_run - deallocates the memory required to batch process a group of
# proc:            HSF forms.
*/

#include <stdio.h>
#include <mfs.h>
#include <hsform.h>
#include <hsfsys.h>
#include <hsfsys2.h>
#include <klparam.h>
#include <mlpparam.h>
#include <defs.h>

/*************************************************************/
init_run(tmplt, regcoords, ncoords, digit_kl, digit_mlp,
         lower_kl, lower_mlp, upper_kl, upper_mlp, const_kl, const_mlp, pdict,
         tmpltfile, regfile,
         do_digit, devtfile, dmapfile, dwtfile,
         do_lower, levtfile, lmapfile, lwtfile,
         do_upper, uevtfile, umapfile, uwtfile,
         do_const, cevtfile, cmapfile, cwtfile, dictfile)
HSFORM *tmplt;
int **regcoords, *ncoords;
KL_PARAM *digit_kl, *lower_kl, *upper_kl, *const_kl;
MLP_PARAM *digit_mlp, *lower_mlp, *upper_mlp, *const_mlp;
MFS **pdict;
char *tmpltfile, *regfile;
int do_digit, do_lower, do_upper, do_const;
char *dmapfile, *devtfile, *dwtfile, *lmapfile, *levtfile, *lwtfile;
char *umapfile, *uevtfile, *uwtfile, *cmapfile, *cevtfile, *cwtfile, *dictfile;
{
   int tout, baslen;
   char purpose;
   float *klmean, *klstdv;
   MFS *map, *load_phrase_dictionary();

   set_timer();

   /* read in the HSF spatial template */
   read_hsftmplt(tmpltfile, tmplt);
   verbose_print("init_run", "HSF Template Loaded");

   /* read in the coordinated for generalized form registration */
   readintmfs(regfile, ncoords, regcoords);
   verbose_print("init_run", "HSF Registration Coordinates Loaded");

   if(do_digit){
      /* load digit eigenvectors for feature extraction */
      read_basis(devtfile, &baslen, &(digit_kl->nbasis), 0, &klmean, &klstdv,
                 &(digit_kl->basis));
      kl_premult(digit_kl->nbasis, klmean, digit_kl->basis,
                 &(digit_kl->mu), &(digit_kl->sume));
      free(klmean);
      free(klstdv);
      verbose_print("init_run", "Digit Eigenvectors Loaded");

      /* load digit class map file */
      map = readmfsfile(dmapfile);
      digit_mlp->class_map = map->values;
      tout = map->num;
      free(map);
      verbose_print("init_run", "Digit Class Map Loaded");

      /* load mlp weights for digits */
      readwts_np(dwtfile, &purpose, &(digit_mlp->ninps), &(digit_mlp->nhids),
              &(digit_mlp->nouts), &(digit_mlp->acfunc_hids),
              &(digit_mlp->acfunc_outs), &(digit_mlp->weights));
      if(digit_kl->nbasis != digit_mlp->ninps)
         fatalerr("init_run", "do digits",
                  "length of basis vectors not equal to length of input layer");
      if(tout != digit_mlp->nouts)
         fatalerr("init_run", "do digits",
                  "length of class map not equal to length of output layer");
      verbose_print("init_run", "Digit Weights Loaded");
   }

   if(do_lower){
      /* load lower eigenvectors for feature extraction */
      read_basis(levtfile, &baslen, &(lower_kl->nbasis), 0, &klmean, &klstdv,
                 &(lower_kl->basis));
      kl_premult(lower_kl->nbasis, klmean, lower_kl->basis,
                 &(lower_kl->mu), &(lower_kl->sume));
      free(klmean);
      free(klstdv);
      verbose_print("init_run", "Lower Eigenvectors Loaded");

      /* load lower class map file */
      map = readmfsfile(lmapfile);
      lower_mlp->class_map = map->values;
      tout = map->num;
      free(map);
      verbose_print("init_run", "Lower Class Map Loaded");

      /* load mlp weights for lowers */
      readwts_np(lwtfile, &purpose, &(lower_mlp->ninps), &(lower_mlp->nhids),
              &(lower_mlp->nouts), &(lower_mlp->acfunc_hids),
              &(lower_mlp->acfunc_outs), &(lower_mlp->weights));
      if(lower_kl->nbasis != lower_mlp->ninps)
         fatalerr("init_run", "do lowers",
                  "length of basis vectors not equal to length of input layer");
      if(tout != lower_mlp->nouts)
         fatalerr("init_run", "do lowers",
                  "length of class map not equal to length of output layer");
      verbose_print("init_run", "Lower Weights Loaded");
   }

   if(do_upper){
      /* load upper eigenvectors for feature extraction */
      read_basis(uevtfile, &baslen, &(upper_kl->nbasis), 0, &klmean, &klstdv,
                 &(upper_kl->basis));
      kl_premult(upper_kl->nbasis, klmean, upper_kl->basis,
                 &(upper_kl->mu), &(upper_kl->sume));
      free(klmean);
      free(klstdv);
      verbose_print("init_run", "Upper Eigenvectors Loaded");

      /* load upper class map file */
      map = readmfsfile(umapfile);
      upper_mlp->class_map = map->values;
      tout = map->num;
      free(map);
      verbose_print("init_run", "Upper Class Map Loaded");

      /* load mlp weights for uppers */
      readwts_np(uwtfile, &purpose, &(upper_mlp->ninps), &(upper_mlp->nhids),
              &(upper_mlp->nouts), &(upper_mlp->acfunc_hids),
              &(upper_mlp->acfunc_outs), &(upper_mlp->weights));
      if(upper_kl->nbasis != upper_mlp->ninps)
         fatalerr("init_run", "do uppers",
                  "length of basis vectors not equal to length of input layer");
      if(tout != upper_mlp->nouts)
         fatalerr("init_run", "do uppers",
                  "length of class map not equal to length of output layer");
      verbose_print("init_run", "Upper Weights Loaded");
   }

   if(do_const){
      /* load const eigenvectors for feature extraction */
      read_basis(cevtfile, &baslen, &(const_kl->nbasis), 0, &klmean, &klstdv,
                 &(const_kl->basis));
      kl_premult(const_kl->nbasis, klmean, const_kl->basis,
                 &(const_kl->mu), &(const_kl->sume));
      free(klmean);
      free(klstdv);
      verbose_print("init_run", "Const Eigenvectors Loaded");

      /* load const class map file */
      map = readmfsfile(cmapfile);
      const_mlp->class_map = map->values;
      tout = map->num;
      free(map);
      verbose_print("init_run", "Const Class Map Loaded");

      /* load mlp weights for consts */
      readwts_np(cwtfile, &purpose, &(const_mlp->ninps), &(const_mlp->nhids),
              &(const_mlp->nouts), &(const_mlp->acfunc_hids),
              &(const_mlp->acfunc_outs), &(const_mlp->weights));
      if(const_kl->nbasis != const_mlp->ninps)
         fatalerr("init_run", "do consts",
                  "length of basis vectors not equal to length of input layer");
      if(tout != const_mlp->nouts)
         fatalerr("init_run", "do consts",
                  "length of class map not equal to length of output layer");
      verbose_print("init_run", "Const Weights Loaded");

      *pdict = load_phrase_dictionary(dictfile);
      verbose_print("init_run", "Const Lexicon Loaded");
   }

   time_accum(_run_init_);
}

/*************************************************************/
free_run(regcoords, do_digit, digit_kl, digit_mlp, do_lower, lower_kl, lower_mlp,
         do_upper, upper_kl, upper_mlp, do_const, const_kl, const_mlp, pdict)
int *regcoords;
int do_digit, do_lower, do_upper, do_const;
KL_PARAM *digit_kl, *lower_kl, *upper_kl, *const_kl;
MLP_PARAM *digit_mlp, *lower_mlp, *upper_mlp, *const_mlp;
MFS *pdict;
{
   free(regcoords);
   if(do_digit){
      free(digit_kl->basis);
      free(digit_kl->mu);
      free(digit_kl->sume);
      free_names(digit_mlp->class_map, digit_mlp->nouts);
      free(digit_mlp->weights);
   }
   if(do_lower){
      free(lower_kl->basis);
      free(lower_kl->mu);
      free(lower_kl->sume);
      free_names(lower_mlp->class_map, lower_mlp->nouts);
      free(lower_mlp->weights);
   }
   if(do_upper){
      free(upper_kl->basis);
      free(upper_kl->mu);
      free(upper_kl->sume);
      free_names(upper_mlp->class_map, upper_mlp->nouts);
      free(upper_mlp->weights);
   }
   if(do_const){
      free(const_kl->basis);
      free(const_kl->mu);
      free(const_kl->sume);
      free_names(const_mlp->class_map, const_mlp->nouts);
      free(const_mlp->weights);
      freemfs(pdict);
      end_lookup_levenstein();
   }
}
