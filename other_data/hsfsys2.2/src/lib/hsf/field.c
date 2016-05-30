/*
# proc: do_digit_fields - processes the digit fields on a registered HSF form.
# proc:
# proc: do_alpha_field - processes an alphabetic field (lowercase or uppercase)
# proc:                  on a registered HSF form.
# proc: do_const_field - processes the Constitution field on a registered HSF
# proc:                  form.
# proc: init_field - reads in the feature extraction basis functions and
# proc:              neural network prototypes assocated with a specific
# proc:              type of entry field.
# proc: process_digit_field - isolates, segments, normalizes, shears, feature
# proc:                       extracts, and classifies the specified digit
# proc:                       field.
# proc: process_alpha_field - isolates, segments, normalizes, shears, feature
# proc:                       extracts, and classifies the specified alpha
# proc:                       field.
# proc: process_const_field2 - isolates, segments, normalizes, shears, feature
# proc:                        extracts, and classifies the given const field
# proc:                        and conditionally does dictionary processing.
# proc: free_field - deallocates basis functions and prototype information
# proc:              related to a specific type of entry field.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <fet.h>
#include <maxlist.h>
#include <hsform.h>
#include <hsfsys.h>
#include <kdtree.h>

static float tree_cuts_digit[] = {0.45, 0.70, 0.90, 1.05, 1.40, 2.00};
static int ncuts_digit = 6;
static float tree_cuts_alpha[] = {0.55, 0.70, 0.90, 1.05, 1.40, 2.00};
static int ncuts_alpha = 6;
static float tree_cuts_const[] = {0.45, 0.70, 0.90, 1.00, 1.40, 2.00};
static int ncuts_const = 6;

/*************************************************************/
do_digit_fields(msg, first, last, evtfile, patfile, medfile, setfile,
                rmdata, iw, ih, tmplt, hyps, confs)
char *msg;
int first, last;
char *evtfile, *patfile, *medfile, *setfile;
unsigned char *rmdata;
int iw, ih;
HSFORM *tmplt;
FET *hyps, *confs;
{
   int i;
   float *klbasis, *klmu, *klsume, *medians;
   int nbasis;
   TREE *tree;
   MFS *class_map;
   char *hypstr, *constr;
   int *hyps_i, nhyps;
   float *cons;

   /* initialize digit eigenvectors and prototypes */
   set_timer();
   init_field(evtfile, patfile, medfile, setfile,
              &klbasis, &klmu, &klsume, &nbasis,
              &tree, &medians, &class_map);
   time_elapsed(msg, "initialize");

   for(i = first; i <= last; i++){

      /* process the next digit field */
      process_digit_field(msg, &hyps_i, &cons, &nhyps, i, rmdata, iw, ih,
                         tmplt, klbasis, klmu, klsume, nbasis,
                         tree, medians, class_map);

      /* convert and store results */
      string_results(&hypstr, &constr, class_map->values, class_map->num,
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

   free_field(klbasis, klmu, klsume, tree, medians, class_map);
}

/*************************************************************/
do_alpha_field(msg, fld_i, evtfile, patfile, medfile, setfile,
               rmdata, iw, ih, tmplt, hyps, confs)
char *msg;
int fld_i;
char *evtfile, *patfile, *medfile, *setfile;
unsigned char *rmdata;
int iw, ih;
HSFORM *tmplt;
FET *hyps, *confs;
{
   float *klbasis, *klmu, *klsume, *medians;
   int nbasis;
   TREE *tree;
   MFS *class_map;
   char *hypstr, *constr;
   int *hyps_i, nhyps;
   float *cons;

   /* initialize alphabetic eigenvectors and prototypes */
   set_timer();
   init_field(evtfile, patfile, medfile, setfile,
              &klbasis, &klmu, &klsume, &nbasis,
              &tree, &medians, &class_map);
   time_elapsed(msg, "initialize");

   /* process the alphabet field */
   process_alpha_field(msg, &hyps_i, &cons, &nhyps, fld_i, rmdata, iw, ih,
                       tmplt, klbasis, klmu, klsume, nbasis,
                       tree, medians, class_map);

   /* convert and store results */
   string_results(&hypstr, &constr, class_map->values, class_map->num,
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

   free_field(klbasis, klmu, klsume, tree, medians, class_map);
}

/*************************************************************/
do_const_field(msg, fld_i, do_const, dictfile, evtfile, patfile, medfile, setfile,
               rmdata, iw, ih, tmplt, hyps, confs)
char *msg;
int fld_i, do_const;
char *dictfile, *evtfile, *patfile, *medfile, *setfile;
unsigned char *rmdata;
int iw, ih;
HSFORM *tmplt;
FET *hyps, *confs;
{
   float *klbasis, *klmu, *klsume, *medians;
   int nbasis;
   TREE *tree;
   MFS *class_map;
   char *hypstr, *constr;

   /* initialize digit eigenvectors and prototypes */
   set_timer();
   init_field(evtfile, patfile, medfile, setfile,
              &klbasis, &klmu, &klsume, &nbasis,
              &tree, &medians, &class_map);

   time_elapsed(msg, "initialize");

   /* process the Constitution field */
   process_const_field2(msg, &hypstr, &constr, fld_i, rmdata, iw, ih,
                        tmplt, klbasis, klmu, klsume, nbasis,
                        tree, medians, class_map,
                        do_const, dictfile, stdout);

   /* update hypothesis and confidence fet structures */
   if(hypstr != NULL){
      updatefet(hyps->names[fld_i], hypstr, hyps);
      updatefet(confs->names[fld_i], constr, confs);
      free(hypstr);
      free(constr);
   }

   free_field(klbasis, klmu, klsume, tree, medians, class_map);
}

/*************************************************************/
init_field(efile, pfile, mfile, sfile,
           klbasis, klmu, klsume, nbasis, tree, medians, class_map)
char *efile, *pfile, *mfile, *sfile;
float **klbasis, **klmu, **klsume, **medians;
int *nbasis;
TREE **tree;
MFS **class_map;
{
   int baslen, protlen;
   float *klmean, *klstdv;
   int nmed, ninp;

   /* load digit eigenvectors for feature extraction */
   read_basis(efile, &baslen, nbasis, 0, &klmean, &klstdv, klbasis);
   kl_premult(*nbasis, klmean, *klbasis, klmu, klsume);
   free(klmean);
   free(klstdv);
   verbose_print("init_field", "Eigenvectors Loaded");

   /* load kl prototypes within kdtree */
   (*tree) = kdtreeread(pfile);
   if(*nbasis != (*tree)->datadimension)
      fatalerr("init_field",
               "length of basis vectors not equal to length of prototypes",
               NULL);
   verbose_print("init_field", "Prototypes Loaded");

   /* load prototype median vectors */
   readmedianfile(mfile, medians, &nmed, &ninp);
   if(ninp != *nbasis)
      fatalerr("init_field",
            "length of median vectors not equal to number of basis functions",
               NULL);
   verbose_print("init_field", "Median Vectors Loaded");

   /* load class map for feature vectors */
   *class_map = readmfsfile(sfile);
   if(nmed != (*class_map)->num)
      fatalerr("init_field",
            "number of median vectors not equal to number of outputclasses",
               NULL);
   verbose_print("init_field", "Class Set Loaded");
}

/*************************************************************/
process_digit_field(msg, hyps_i, cons, nhyps, fld_i, idata, iw, ih, tmplt,
              klbasis, klmu, klsume, nbasis, tree, medians, class_map)
char *msg;
int **hyps_i;
float **cons;
unsigned char *idata;
int *nhyps, fld_i, iw, ih;
HSFORM *tmplt;
float *klbasis, *klmu, *klsume, *medians;
int nbasis;
TREE *tree;
MFS *class_map;
{
   unsigned char *field;
   int fpar_x, fpar_y, fw, fh;
   MIS *mis, *nmis, *smis;
   int *cpar_x, *cpar_y, *prisp_w, *prisp_h;
   float *sfctr, *scal_x, *scal_y;
   float *klts;
   int nklts;
   char msgstr[80];

   /* isolate the field */
   set_timer();
   iso_1line_field(&field, &fpar_x, &fpar_y, &fw, &fh, idata, iw, ih,
                   tmplt->box[fld_i].vertex[0].x,
                   tmplt->box[fld_i].vertex[0].y,
                   tmplt->box[fld_i].vertex[3].x,
                   tmplt->box[fld_i].vertex[3].y);
   time_elapsed(msg, "isolate   ");
   sprintf(msgstr, "Field %d Isolated", fld_i);
   verbose_print("process_digit_field", msgstr);
   if((fw == 0) || (fh == 0)){
      *nhyps = 0;
      return;
   }

   /* segment the digit field */
   set_timer();
   segbinblobdigits(&mis, &cpar_x, &cpar_y, &prisp_w, &prisp_h, field, fw, fh);
   time_elapsed(msg, "segment   ");
   free(field);
   free(cpar_x);
   free(cpar_y);
   sprintf(msgstr, "Field %d Segmented", fld_i);
   verbose_print("process_digit_field", msgstr);
   if(mis->ent_num == 0){
      freemis(mis);
      free(prisp_w);
      free(prisp_h);
      *nhyps = 0;
      return;
   }

   /* normalize the segmented digits */
   set_timer();
   norm_2nd_gen(&nmis, &scal_x, &scal_y, mis, prisp_w, prisp_h);
   time_elapsed(msg, "normalize ");
   freemis(mis);
   free(prisp_w);
   free(prisp_h);
   free(scal_x);
   free(scal_y);
   verbose_print("process_digit_field", "Character Images Normalized");
   if(nmis->ent_num == 0)
      fatalerr("process_digit_field", "no character images normalized", NULL);

   /* shear the normalized digit images */
   set_timer();
   shear_mis(&smis, &sfctr, nmis);
   time_elapsed(msg, "shear     ");
   freemis(nmis);
   free(sfctr);
   verbose_print("process_digit_field", "Character Images Sheared");
   if(smis->ent_num == 0)
      fatalerr("process_digit_field", "no character images sheared", NULL);

   /* kl transform normalized digit images */
   set_timer();
   kl_transform_mis(&klts, smis, klbasis, klmu, klsume, nbasis);
   time_elapsed(msg, "feature   ");
   nklts = smis->ent_num;
   freemis(smis);
   verbose_print("process_digit_field", "Character Images Feature Extracted");

   /* classify kl feature vectors */
   set_timer();
   malloc_int(hyps_i, nklts, "process_digit_field : hyps_i");
   malloc_flt(cons, nklts, "process_digit_field : cons");
   pnnsearch(tree, klts, nklts, class_map->num, medians,
             tree_cuts_digit, ncuts_digit, PNN_SIGMA_DIGIT, *hyps_i, *cons);
   time_elapsed(msg, "classify  ");
   *nhyps = nklts;
   free(klts);
   verbose_print("process_digit_field", "Feature Vectors Classified");
}

/*************************************************************/
process_alpha_field(msg, hyps_i, cons, nhyps, fld_i, idata, iw, ih, tmplt,
              klbasis, klmu, klsume, nbasis, tree, medians, class_map)
char *msg;
int **hyps_i;
float **cons;
unsigned char *idata;
int *nhyps, fld_i, iw, ih;
HSFORM *tmplt;
float *klbasis, *klmu, *klsume, *medians;
int nbasis;
TREE *tree;
MFS *class_map;
{
   unsigned char *field;
   int fpar_x, fpar_y, fw, fh;
   MIS *mis, *nmis, *smis;
   int *cpar_x, *cpar_y, *prisp_w, *prisp_h;
   float *sfctr, *scal_x, *scal_y;
   float *klts;
   int nklts;
   char msgstr[80];

   /* isolate the field */
   set_timer();
   iso_1line_field(&field, &fpar_x, &fpar_y, &fw, &fh, idata, iw, ih,
                   tmplt->box[fld_i].vertex[0].x,
                   tmplt->box[fld_i].vertex[0].y,
                   tmplt->box[fld_i].vertex[3].x,
                   tmplt->box[fld_i].vertex[3].y);
   time_elapsed(msg, "isolate   ");
   sprintf(msgstr, "Field %d Isolated", fld_i);
   verbose_print("process_alpha_field", msgstr);
   if((fw == 0) || (fh == 0)){
      *nhyps = 0;
      return;
   }

   /* segment the alpha field */
   set_timer();
   segbinblob(&mis, &cpar_x, &cpar_y, &prisp_w, &prisp_h, field, fw, fh);
   time_elapsed(msg, "segment   ");
   free(field);
   free(cpar_x);
   free(cpar_y);
   sprintf(msgstr, "Field %d Segmented", fld_i);
   verbose_print("process_alpha_field", msgstr);
   if(mis->ent_num == 0){
      freemis(mis);
      free(prisp_w);
      free(prisp_h);
      *nhyps = 0;
      return;
   }

   /* normalize the segmented alphas */
   set_timer();
   norm_2nd_gen(&nmis, &scal_x, &scal_y, mis, prisp_w, prisp_h);
   time_elapsed(msg, "normalize ");
   freemis(mis);
   free(prisp_w);
   free(prisp_h);
   free(scal_x);
   free(scal_y);
   verbose_print("process_alpha_field", "Character Images Normalized");
   if(nmis->ent_num == 0)
      fatalerr("process_alpha_field", "no character images normalized", NULL);

   /* shear the normalized alpha images */
   set_timer();
   shear_mis(&smis, &sfctr, nmis);
   time_elapsed(msg, "shear     ");
   freemis(nmis);
   free(sfctr);
   verbose_print("process_alpha_field", "Character Images Sheared");
   if(smis->ent_num == 0)
      fatalerr("process_alpha_field", "no character images sheared", NULL);

   /* kl transform normalized alpha images */
   set_timer();
   kl_transform_mis(&klts, smis, klbasis, klmu, klsume, nbasis);
   time_elapsed(msg, "feature   ");
   nklts = smis->ent_num;
   freemis(smis);
   verbose_print("process_alpha_field",
                 "Character Images Feature Extracted");

   /* classify kl feature vectors */
   set_timer();
   malloc_int(hyps_i, nklts, "process_alpha_field : hyps_i");
   malloc_flt(cons, nklts, "process_alpha_field : cons");
   pnnsearch(tree, klts, nklts, class_map->num, medians,
             tree_cuts_alpha, ncuts_alpha, PNN_SIGMA_ALPHA, *hyps_i, *cons);
   time_elapsed(msg, "classify  ");
   *nhyps = nklts;
   free(klts);
   verbose_print("process_alpha_field", "Feature Vectors Classified");
}

/*************************************************************/
process_const_field2(msg, hypstr, constr, fld_i, idata, iw, ih, tmplt,
              klbasis, klmu, klsume, nbasis, tree, medians, class_map,
              dflag, pfile, fpout)
char *msg;
char **hypstr, **constr;
unsigned char *idata;
int fld_i, iw, ih;
HSFORM *tmplt;
float *klbasis, *klmu, *klsume, *medians;
int nbasis, dflag;
TREE *tree;
MFS *class_map;
char *pfile;
FILE *fpout;
{
   unsigned char *field;
   int i;
   int fpar_x, fpar_y, fw, fh;
   MIS *mis, *nmis, *smis;
   int *cpar_x, *cpar_y, *prisp_w, *prisp_h;
   float *sfctr, *scal_x, *scal_y;
   float *klts;
   int nklts;
   int *hyps_i, nhyps;
   float *cons;
   int *pi_lists[MAX_INDEX], pi_lens[MAX_INDEX];
   int nphrases;
   char *phrases2hypstr2(), *phrases2constr2();
   char msgstr[80];

   /* isolate the field */
   set_timer();
   iso_nline_field(&field, &fpar_x, &fpar_y, &fw, &fh, idata, iw, ih,
                   tmplt->box[fld_i].vertex[0].x,
                   tmplt->box[fld_i].vertex[0].y,
                   tmplt->box[fld_i].vertex[3].x,
                   tmplt->box[fld_i].vertex[3].y);
   time_elapsed(msg, "isolate   ");
   sprintf(msgstr, "Field %d Isolated", fld_i);
   verbose_print("process_const_field2", msgstr);
   if((fw == 0) || (fh == 0)){
      *hypstr = NULL;
      *constr = NULL;
      return;
   }

   /* segment the const field */
   set_timer();
   segbinblob(&mis, &cpar_x, &cpar_y, &prisp_w, &prisp_h, field, fw, fh);
   time_elapsed(msg, "segment   ");
   free(field);
   sprintf(msgstr, "Field %d Segmented", fld_i);
   verbose_print("process_const_field2", msgstr);
   if(mis->ent_num == 0){
      freemis(mis);
      free(cpar_x);
      free(cpar_y);
      free(prisp_w);
      free(prisp_h);
      *hypstr = NULL;
      *constr = NULL;
      return;
   }

   /* normalize the segmented const characters */
   set_timer();
   norm_2nd_gen(&nmis, &scal_x, &scal_y, mis, prisp_w, prisp_h);
   time_elapsed(msg, "normalize ");
   freemis(mis);
   free(scal_x);
   free(scal_y);
   verbose_print("process_const_field2", "Character Images Normalized");
   if(nmis->ent_num == 0)
      fatalerr("process_const_field2", "no character images normalized", NULL);

   /* shear the normalized const character images */
   set_timer();
   shear_mis(&smis, &sfctr, nmis);
   time_elapsed(msg, "shear     ");
   freemis(nmis);
   free(sfctr);
   verbose_print("process_const_field2", "Character Images Sheared");
   if(smis->ent_num == 0)
      fatalerr("process_const_field2", "no character images sheared", NULL);

   /* kl transform normalized const images */
   set_timer();
   kl_transform_mis(&klts, smis, klbasis, klmu, klsume, nbasis);
   time_elapsed(msg, "feature   ");
   nklts = smis->ent_num;
   freemis(smis);
   verbose_print("process_const_field2",
                 "Character Images Feature Extracted");

   /* classify kl feature vectors */
   set_timer();
   malloc_int(&hyps_i, nklts, "process_const_field2 : hyps_i");
   malloc_flt(&cons, nklts, "process_const_field2 : cons");
   pnnsearch(tree, klts, nklts, class_map->num, medians,
             tree_cuts_const, ncuts_const, PNN_SIGMA_ALPHA, hyps_i, cons);
   time_elapsed(msg, "classify  ");
   nhyps = nklts;
   free(klts);
   verbose_print("process_const_field2", "Feature Vectors Classified");

   /* sort classified blobs into lines according to reading order */
   set_timer();
   build_pi_lists(pi_lists, pi_lens, &nphrases,
                  cpar_x, cpar_y, prisp_w, prisp_h, nhyps);
   time_elapsed(msg, "sort      ");
   free(cpar_x);
   free(cpar_y);
   free(prisp_w);
   free(prisp_h);
   verbose_print("process_const_field2", "Classifications Sorted");

   if(dflag == DO_DICT){
      /* correct phrase classifications using dictionaries */
      if((!_silent_) && (fpout != NULL)){
         fprintf(fpout, "\nhsf_%d: ", fld_i);
         fflush(fpout);
      }
      set_timer();
      spell_phrases2(hypstr, hyps_i, nhyps,
                     class_map->values, class_map->num,
                     pi_lists, pi_lens, nphrases, pfile,
                     ((!_silent_)?fpout:NULL));
      /* deallocate levenstein globals */
      end_lookup_levenstein();
      time_elapsed(msg, "spell     ");
      *constr = NULL;
      if((!_silent_) && (fpout != NULL)){
         fprintf(fpout, "\n\n");
         fflush(fpout);
      }
   }
   else{
      /* convert phrases into one long string */
      (*hypstr) = phrases2hypstr2(hyps_i, nhyps,
                                  class_map->values, class_map->num,
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
}

/*************************************************************/
free_field(klbasis, klmu, klsume, tree, medians, class_map)
float *klbasis, *klmu, *klsume, *medians;
TREE *tree;
MFS *class_map;
{
   free(klbasis);
   free(klmu);
   free(klsume);
   treefree(tree);
   free(medians);
   freemfs(class_map);
}
