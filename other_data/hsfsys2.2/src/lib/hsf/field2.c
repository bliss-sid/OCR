/*
# proc: do_digit_fields_Rel2 - processes the digit fields on a registered HSF
# proc:                        form.
# proc: do_alpha_field_Rel2 - processes an alphabetic field (lowercase or
# proc:                       uppercase) on a registered HSF form.
# proc: do_const_fields_Rel2 - processes the Constitution field on a registered
# proc:                        HSF form.
# proc: process_digit_field_Rel2 - isolates, segments, normalizes, feature
# proc:                       extracts, and classifies the specified digit field.
# proc: process_alpha_field_Rel2 - isolates, segments, normalizes, feature
# proc:                 extracts, and classifies the specified alphabetic field.
# proc: process_const_field_Rel2 - isolates, segments, normalizes, feature
# proc:                 extracts, classifies, and conditionally spell-corrects
# proc:                 the specified Constitution field.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>
#include <fet.h>
#include <mfs.h>
#include <hsform.h>
#include <hsfsys.h>
#include <hsfsys2.h>
#include <blobls.h>
#include <klparam.h>
#include <mlpparam.h>
#include <defs.h>

/*************************************************************/
do_digit_fields_Rel2(first, last, kl_param, mlp_param,
                       regcdata, bpi, iw, ih, tmplt, hyps, confs)
int first, last;
KL_PARAM *kl_param;
MLP_PARAM *mlp_param;
unsigned char *regcdata;
int bpi, iw, ih;
HSFORM *tmplt;
FET *hyps, *confs;
{
   int i;
   char *hypstr, *constr;
   int *hyps_i, nhyps;
   float *cons;

   for(i = first; i <= last; i++){

      /* process the next digit field */
      if(process_digit_field_Rel2(&hyps_i, &cons, &nhyps, i,
                         regcdata, bpi, iw, ih, tmplt, kl_param, mlp_param)){
         /* convert and store results */
         string_results(&hypstr, &constr, mlp_param->class_map, mlp_param->nouts,
                        hyps_i, cons, nhyps);

         /* update hypothesis and confidence fet structures */
         result_print(hyps->names[i], hypstr);
         if(nhyps != 0){
            updatefet(hyps->names[i], hypstr, hyps);
            updatefet(confs->names[i], constr, confs);
            free(hyps_i);
            free(cons);
            free(hypstr);
            free(constr);
         }
      }
      else
         fprintf(stderr,
                 "Warning: process_digit_field_Rel2: field %d rejected\n", i);
   }
}

/*************************************************************/
do_alpha_field_Rel2(fld_i, kl_param, mlp_param,
                    regcdata, bpi, iw, ih, tmplt, hyps, confs)
int fld_i;
KL_PARAM *kl_param;
MLP_PARAM *mlp_param;
unsigned char *regcdata;
int bpi, iw, ih;
HSFORM *tmplt;
FET *hyps, *confs;
{
   char *hypstr, *constr;
   int *hyps_i, nhyps;
   float *cons;

   /* process the alphabet field */
   if(process_alpha_field_Rel2(&hyps_i, &cons, &nhyps, fld_i,
                          regcdata, bpi, iw, ih, tmplt, kl_param, mlp_param)){

      /* convert and store results */
      string_results(&hypstr, &constr, mlp_param->class_map, mlp_param->nouts,
                     hyps_i, cons, nhyps);

      /* update hypothesis and confidence fet structures */
      result_print(hyps->names[fld_i], hypstr);
      if(nhyps != 0){
         updatefet(hyps->names[fld_i], hypstr, hyps);
         updatefet(confs->names[fld_i], constr, confs);
         free(hyps_i);
         free(cons);
         free(hypstr);
         free(constr);
      }
   }
   else
      fprintf(stderr,
              "Warning: process_alpha_field_Rel2: field %d rejected\n", fld_i);
}

/*************************************************************/
do_const_field_Rel2(fld_i, do_const, pdict, kl_param, mlp_param,
                    regcdata, bpi, iw, ih, tmplt, hyps, confs)
int fld_i, do_const;
MFS *pdict;
KL_PARAM *kl_param;
MLP_PARAM *mlp_param;
unsigned char *regcdata;
int bpi, iw, ih;
HSFORM *tmplt;
FET *hyps, *confs;
{
   char *hypstr, *constr;

   /* process the Constitution field */
   if(process_const_field_Rel2(&hypstr, &constr, fld_i, regcdata, bpi, iw, ih,
                        tmplt, kl_param, mlp_param, do_const, pdict, stdout)){

      /* update hypothesis and confidence fet structures */
      if(hypstr != NULL){
         updatefet(hyps->names[fld_i], hypstr, hyps);
         updatefet(confs->names[fld_i], constr, confs);
         free(hypstr);
         free(constr);
      }
   }
   else
      fprintf(stderr,
              "Warning: process_const_field_Rel2: field %d rejected\n", fld_i);

}

/*************************************************************/
process_digit_field_Rel2(hyps_i, cons, nhyps, fld_i, icdata, bpi, iw, ih,
                         tmplt, kl_param, mlp_param)
int **hyps_i;
float **cons;
unsigned char *icdata;
int *nhyps, fld_i, bpi, iw, ih;
HSFORM *tmplt;
KL_PARAM *kl_param;
MLP_PARAM *mlp_param;
{
   unsigned char *cfield, *mallocate_image();
   int fpar_x, fpar_y, fw, fh;
   MIS *nmis, *smis;
   float *sfctr, *scal_x, *scal_y;
   float *klts;
   int nklts;
   char msgstr[80];
   BLOBLS *blobls, *noisels;
   int *rlen, rn, ra;

   /* isolate the field */
   set_timer();
   if(!iso_handprint_in_box(&blobls, &cfield, &fpar_x, &fpar_y, &fw, &fh,
                   &rlen, &rn, &ra,
                   icdata, bpi, iw, ih,
                   tmplt->box[fld_i].vertex[0].x,
                   tmplt->box[fld_i].vertex[0].y,
                   tmplt->box[fld_i].vertex[3].x,
                   tmplt->box[fld_i].vertex[3].y)){
      /* field is rejected */
      sprintf(msgstr, "Field %d Isolation Failed", fld_i);
      verbose_print("process_digit_field_Rel2", msgstr);
      /* free field subimage (would be used for manual entry) */
      free(cfield);
      *nhyps = 0;
      return(FALSE);
   }
   time_accum(_field_iso_);
   sprintf(msgstr, "Field %d Isolated", fld_i);
   verbose_print("process_digit_field_Rel2", msgstr);

   /* segment the digit field */
   set_timer();
   blobs2chars8(&blobls, &noisels, rlen, rn);
   time_accum(_field_seg_);
   free(cfield);
   free_blobls(noisels);
   free(rlen);
   sprintf(msgstr, "Field %d Segmented", fld_i);
   verbose_print("process_digit_field_Rel2", msgstr);
   if(blobls->num == 0){
      free_blobls(blobls);
      *nhyps = 0;
      return(TRUE);
   }

   /* normalize the segmented digits */
   set_timer();
   norm_2nd_gen_blobls8(&nmis, &scal_x, &scal_y, blobls);
   free_blobls(blobls);
   free(scal_x);
   free(scal_y);
   verbose_print("process_digit_field_Rel2", "Character Images Normalized");
   if(nmis->ent_num == 0)
      fatalerr("process_digit_field_Rel2",
               "no character images normalized", NULL);

   /* shear the normalized digit images */
   shear_mis8(&smis, &sfctr, nmis);
   time_accum(_char_norm_);
   freemis(nmis);
   free(sfctr);
   verbose_print("process_digit_field_Rel2", "Character Images Sheared");
   if(smis->ent_num == 0)
      fatalerr("process_digit_field_Rel2", "no character images sheared", NULL);

   /* kl transform normalized digit images */
   set_timer();
   kl_transform_mis8(&klts, smis,
                kl_param->basis, kl_param->mu, kl_param->sume, kl_param->nbasis);
   time_accum(_char_feat_);
   nklts = smis->ent_num;
   freemis(smis);
   verbose_print("process_digit_field_Rel2",
                 "Character Images Feature Extracted");

   /* classify kl feature vectors */
   set_timer();
   malloc_int(hyps_i, nklts, "process_digit_field_Rel2 : hyps_i");
   malloc_flt(cons, nklts, "process_digit_field_Rel2 : cons");
   mlphypscons(mlp_param->ninps, mlp_param->nhids, mlp_param->nouts,
               mlp_param->acfunc_hids, mlp_param->acfunc_outs, mlp_param->weights,
               klts, nklts, *hyps_i, *cons);
   time_accum(_char_class_);
   *nhyps = nklts;
   free(klts);
   verbose_print("process_digit_field_Rel2", "Feature Vectors Classified");

   return(TRUE);
}

/*************************************************************/
process_alpha_field_Rel2(hyps_i, cons, nhyps, fld_i, icdata, bpi, iw, ih,
                         tmplt, kl_param, mlp_param)
int **hyps_i;
float **cons;
unsigned char *icdata;
int *nhyps, fld_i, bpi, iw, ih;
HSFORM *tmplt;
KL_PARAM *kl_param;
MLP_PARAM *mlp_param;
{
   unsigned char *cfield;
   int fpar_x, fpar_y, fw, fh;
   MIS *nmis, *smis;
   float *sfctr, *scal_x, *scal_y;
   float *klts;
   int nklts;
   char msgstr[80];
   BLOBLS *blobls, *noisels;
   int *rlen, rn, ra;

   /* isolate the field */
   set_timer();

   if(!iso_handprint_in_box(&blobls, &cfield, &fpar_x, &fpar_y, &fw, &fh,
                   &rlen, &rn, &ra,
                   icdata, bpi, iw, ih,
                   tmplt->box[fld_i].vertex[0].x,
                   tmplt->box[fld_i].vertex[0].y,
                   tmplt->box[fld_i].vertex[3].x,
                   tmplt->box[fld_i].vertex[3].y)){
      /* field is rejected */
      sprintf(msgstr, "Field %d Isolation Failed", fld_i);
      verbose_print("process_alpha_field_Rel2", msgstr);
      /* free field subimage (would be used for manual entry) */
      free(cfield);
      *nhyps = 0;
      return(FALSE);
   }
   time_accum(_field_iso_);
   sprintf(msgstr, "Field %d Isolated", fld_i);
   verbose_print("process_alpha_field_Rel2", msgstr);

   /* segment the alpha field */
   set_timer();
   blobs2chars8(&blobls, &noisels, rlen, rn);
   time_accum(_field_seg_);
   free(cfield);
   free_blobls(noisels);
   free(rlen);
   sprintf(msgstr, "Field %d Segmented", fld_i);
   verbose_print("process_alpha_field_Rel2", msgstr);
   if(blobls->num == 0){
      free_blobls(blobls);
      *nhyps = 0;
      return(TRUE);
   }

   /* normalize the segmented digits */
   set_timer();
   norm_2nd_gen_blobls8(&nmis, &scal_x, &scal_y, blobls);
   free_blobls(blobls);
   free(scal_x);
   free(scal_y);
   verbose_print("process_alpha_field_Rel2", "Character Images Normalized");
   if(nmis->ent_num == 0)
      fatalerr("process_alpha_field_Rel2",
               "no character images normalized", NULL);

   /* shear the normalized alpha images */
   shear_mis8(&smis, &sfctr, nmis);
   time_accum(_char_norm_);
   freemis(nmis);
   free(sfctr);
   verbose_print("process_alpha_field_Rel2", "Character Images Sheared");
   if(smis->ent_num == 0)
      fatalerr("process_alpha_field_Rel2", "no character images sheared", NULL);

   /* kl transform normalized alpha images */
   set_timer();
   kl_transform_mis8(&klts, smis,
                kl_param->basis, kl_param->mu, kl_param->sume, kl_param->nbasis);
   time_accum(_char_feat_);
   nklts = smis->ent_num;
   freemis(smis);
   verbose_print("process_alpha_field_Rel2",
                 "Character Images Feature Extracted");

   /* classify kl feature vectors */
   set_timer();
   malloc_int(hyps_i, nklts, "process_alpha_field_Rel2 : hyps_i");
   malloc_flt(cons, nklts, "process_alpha_field_Rel2 : cons");
   mlphypscons(mlp_param->ninps, mlp_param->nhids, mlp_param->nouts,
               mlp_param->acfunc_hids, mlp_param->acfunc_outs, mlp_param->weights,
               klts, nklts, *hyps_i, *cons);
   time_accum(_char_class_);
   *nhyps = nklts;
   free(klts);
   verbose_print("process_alpha_field_Rel2", "Feature Vectors Classified");

   return(TRUE);
}

/*************************************************************/
process_const_field_Rel2(hypstr, constr, fld_i, icdata, bpi, iw, ih,
                    tmplt, kl_param, mlp_param, dflag, pdict, fpout)
char **hypstr, **constr;
unsigned char *icdata;
int fld_i, bpi, iw, ih;
HSFORM *tmplt;
KL_PARAM *kl_param;
MLP_PARAM *mlp_param;
int dflag;
MFS *pdict;
FILE *fpout;
{
   unsigned char *cfield;
   int i;
   int fpar_x, fpar_y, fw, fh;
   MIS *nmis, *smis;
   float *sfctr, *scal_x, *scal_y;
   float *klts;
   int nklts;
   int *hyps_i, nhyps;
   float *cons;
   int **pi_lists, *pi_lens;
   int nphrases;
   char *phrases2hypstr2(), *phrases2constr2();
   char msgstr[80];
   int *rlen, rn, ra;
   BLOBLS *blobls, *noisels;
   int *noise_lens;

   /* isolate the field */
   set_timer();
   if(!iso_handprint_in_box(&blobls, &cfield, &fpar_x, &fpar_y, &fw, &fh,
                   &rlen, &rn, &ra,
                   icdata, bpi, iw, ih,
                   tmplt->box[fld_i].vertex[0].x,
                   tmplt->box[fld_i].vertex[0].y,
                   tmplt->box[fld_i].vertex[3].x,
                   tmplt->box[fld_i].vertex[3].y)){
      /* field is rejected */
      sprintf(msgstr, "Field %d Isolation Failed", fld_i);
      verbose_print("process_const_field_Rel2", msgstr);
      /* free field subimage (would be used for manual entry) */
      free(cfield);
      *hypstr = NULL;
      *constr = NULL;
      return(FALSE);
   }
   time_accum(_field_iso_);
   sprintf(msgstr, "Field %d Isolated", fld_i);
   verbose_print("process_const_field_Rel2", msgstr);
   free(cfield);

   /* segment the const field */
   set_timer();
   if((blobls->num == 0) ||
      !segment_paragraph_blobls(&blobls, &pi_lists, &pi_lens, &nphrases,
                                &noisels, &noise_lens, rlen, rn, fw, fh)){
      /* field is assumed to be empty */
      sprintf(msgstr, "Field %d Presumed Empty", fld_i);
      verbose_print("process_const_field_Rel2", msgstr);
      free_blobls(blobls);
      *hypstr = NULL;
      *constr = NULL;
      return(TRUE);
   }
   time_accum(_field_seg_);
   free_blobls(noisels);
   free(noise_lens);
   free(rlen);
   sprintf(msgstr, "Field %d Segmented", fld_i);
   verbose_print("process_const_field_Rel2", msgstr);

   /* normalize the segmented const characters */
   set_timer();
   norm_2nd_gen_blobls8(&nmis, &scal_x, &scal_y, blobls);
   free_blobls(blobls);
   free(scal_x);
   free(scal_y);
   verbose_print("process_const_field_Rel2", "Character Images Normalized");
   if(nmis->ent_num == 0)
      fatalerr("process_const_field_Rel2",
               "no character images normalized", NULL);

   /* shear the normalized const character images */
   shear_mis8(&smis, &sfctr, nmis);
   time_accum(_char_norm_);
   freemis(nmis);
   free(sfctr);
   verbose_print("process_const_field_Rel2", "Character Images Sheared");
   if(smis->ent_num == 0)
      fatalerr("process_const_field_Rel2", "no character images sheared", NULL);

   /* kl transform normalized const images */
   set_timer();
   kl_transform_mis8(&klts, smis,
                kl_param->basis, kl_param->mu, kl_param->sume, kl_param->nbasis);
   time_accum(_char_feat_);
   nklts = smis->ent_num;
   freemis(smis);
   verbose_print("process_const_field_Rel2",
                 "Character Images Feature Extracted");

   /* classify kl feature vectors */
   set_timer();
   malloc_int(&hyps_i, nklts, "process_const_field_Rel2 : hyps_i");
   malloc_flt(&cons, nklts, "process_const_field_Rel2 : cons");
   mlphypscons(mlp_param->ninps, mlp_param->nhids, mlp_param->nouts,
               mlp_param->acfunc_hids, mlp_param->acfunc_outs, mlp_param->weights,
               klts, nklts, hyps_i, cons);
   time_accum(_char_class_);
   nhyps = nklts;
   free(klts);
   verbose_print("process_const_field_Rel2", "Feature Vectors Classified");

   if(dflag == DO_DICT){
      /* correct phrase classifications using dictionaries */
      if((!_silent_) && (fpout != NULL)){
         fprintf(fpout, "\nhsf_%d: ", fld_i);
         fflush(fpout);
      }
      set_timer();
      spell_phrases_Rel2(hypstr, hyps_i, nhyps,
                     mlp_param->class_map, mlp_param->nouts,
                     pi_lists, pi_lens, nphrases, pdict,
                     ((!_silent_)?fpout:NULL));
      time_accum(_field_spell_);
      *constr = NULL;
      if((!_silent_) && (fpout != NULL)){
         fprintf(fpout, "\n\n");
         fflush(fpout);
      }
   }
   else{
      /* convert phrases into one long string */
      (*hypstr) = phrases2hypstr2(hyps_i, nhyps,
                                  mlp_param->class_map, mlp_param->nouts,
                                  pi_lists, pi_lens, nphrases);
      (*constr) = phrases2constr2(cons, nhyps, pi_lists, pi_lens, nphrases);
      if((!_silent_) && (fpout != NULL)){
         fprintf(fpout, "\nhsf_%d: %s\n\n", fld_i, (*hypstr));
         fflush(fpout);
      }
   }
   free(hyps_i);
   free(cons);
   for(i = 0; i < nphrases; i++)
      free(pi_lists[i]);
   free(pi_lists);
   free(pi_lens);
   return(TRUE);
}
