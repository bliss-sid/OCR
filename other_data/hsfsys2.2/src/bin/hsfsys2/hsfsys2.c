/*
# pgrm: hsfsys2 - 2nd release of program that conducts OCR on a binary image
# pgrm:           of an HSF form.
*/

#include <stdio.h>
#include <sys/param.h>
#include <string.h>
#include <ihead.h>
#include <mfs.h>
#include <hsform.h>
#include <fet.h>
#include <hsfsys.h>
#include <hsfsys2.h>
#include <klparam.h>
#include <mlpparam.h>
#include <defs.h>

main(argc, argv)
int argc;
char *argv[];
{
   int do_digit, do_lower, do_upper, do_const;
   int retflag;
   char *timefile, *listfile, ifile[MAXPATHLEN], oroot[MAXPATHLEN];
   char hypfile[MAXPATHLEN];
   char confile[MAXPATHLEN];
   IHEAD *ihead;
   int bpi, iw, ih;
   int *regcoords, ncoords;
   unsigned char *incdata, *regcdata;
   HSFORM tmplt;
   FET *hyps, *confs, *init_hsf_fet();
   FILE *lfp;
   KL_PARAM digit_kl, lower_kl, upper_kl, const_kl;
   MLP_PARAM digit_mlp, lower_mlp, upper_mlp, const_mlp;
   MFS *pdict;

   /* process the command line arguments */
   procargs(argc, argv, &do_digit, &do_lower, &do_upper, &do_const,
            &_silent_, &_verbose_, &_time_, &timefile, &listfile);

   set_timer2();

   /* open list file */
   if((lfp = fopen(listfile, "rb")) == NULL)
      syserr("main", "fopen", listfile);

   /* initialize run */
   init_run(&tmplt, &regcoords, &ncoords, &digit_kl, &digit_mlp,
            &lower_kl, &lower_mlp, &upper_kl, &upper_mlp, &const_kl, &const_mlp,
            &pdict, TMPLT_FIELDS, GEN_REG_POINTS,
            do_digit, DIGIT_EVT_REL2, DIGIT_SET_REL2, DIGIT_WT_REL2,
            do_lower, LOWER_EVT_REL2, LOWER_SET_REL2, LOWER_WT_REL2,
            do_upper, UPPER_EVT_REL2, UPPER_SET_REL2, UPPER_WT_REL2,
            do_const, CONST_EVT_REL2, CONST_SET_REL2, CONST_WT_REL2, PHRASE_DICT);

   /* get next form image to process */
   while(fscanf(lfp, "%s %s", ifile, oroot) == 2){

      /* load next form image */
      load_form_image(&ihead, &incdata, &bpi, &iw, &ih, ifile);

      /* register form */
      retflag = register_form(&regcdata, incdata, bpi, iw, ih,
                              regcoords, ncoords, ZOOMFLAG);

      if(retflag){
         /* initialize an FET structure to hold system field hypotheses */
         hyps = init_hsf_fet(tmplt.num);
         /* initialize an FET structure to hold system character confidences */
         confs = init_hsf_fet(tmplt.num);
         /* create output file names */
         sprintf(hypfile, "%s.hyp", oroot);
         sprintf(confile, "%s.con", oroot);

         if(do_digit)
            do_digit_fields_Rel2(FIRST_DIGIT, LAST_DIGIT,
                                 &digit_kl, &digit_mlp, regcdata, bpi, iw, ih,
                                 &tmplt, hyps, confs);

         if(do_lower)
            do_alpha_field_Rel2(FIRST_LOWER,
                                &lower_kl, &lower_mlp, regcdata, bpi, iw, ih,
                                &tmplt, hyps, confs);

         if(do_upper)
            do_alpha_field_Rel2(FIRST_UPPER,
                                &upper_kl, &upper_mlp, regcdata, bpi, iw, ih,
                                &tmplt, hyps, confs);

         if(do_const)
            do_const_field_Rel2(FIRST_CONST, do_const, pdict,
                                &const_kl, &const_mlp,
                                regcdata, bpi, iw, ih, &tmplt, hyps, confs);

         writefetfile(hypfile, hyps);
         writefetfile(confile, confs);

         free(ihead);
         free(incdata);
         free(regcdata);
         freefet(hyps);
         freefet(confs);

         _total_forms_++;
      }
      else{
         fprintf(stderr, "Warning: main: Form Rejected\n");
         free(ihead);
         free(incdata);
      }

   }

   free_run(regcoords, do_digit, &digit_kl, &digit_mlp,
            do_lower, &lower_kl, &lower_mlp,
            do_upper, &upper_kl, &upper_mlp,
            do_const, &const_kl, &const_mlp, pdict);

   fclose(lfp);

   time2_accum(_total_time_);
   if(timefile != (char *)NULL){
      print_time_report(timefile);
      free(timefile);
   }

   exit(0);
}

/*************************************************************/
procargs(argc, argv, do_digit, do_lower, do_upper, do_const,
            silent, verbose, time, timefile, listfile)
int argc, *do_digit, *do_lower, *do_upper, *do_const;
int *silent, *verbose, *time;
char **timefile, **listfile, *argv[];
{
   int c, errflg = 0;

   *do_digit = FALSE;
   *do_lower = FALSE;
   *do_upper = FALSE;
   *do_const = FALSE;
   *silent = FALSE;
   *verbose = FALSE;
   *time = FALSE;
   *timefile = (char *)NULL;
   while(((c = getopt(argc, argv, "c:dlst:uv")) != -1) && (!errflg)){
      switch(c){
      case 'c':
         if(strcmp(optarg, "dict") == 0)
            *do_const = DO_DICT;
         else if(strcmp(optarg, "nodict") == 0)
            *do_const = NO_DICT;
         else{
            fprintf(stderr,"Error: Illegal argument given with -c option\n\n");
            errflg++;
         }
         break;
      case 'd':
         *do_digit = TRUE;
         break;
      case 'l':
         *do_lower = TRUE;
         break;
      case 's':
         *silent = TRUE;
         break;
      case 't':
         *time = TRUE;
         *timefile = strdup(optarg);
         break;
      case 'u':
         *do_upper = TRUE;
         break;
      case 'v':
         *verbose = TRUE;
         break;
      case '?':
         errflg++;
         break;
      }
   }
   if(!(*do_digit|*do_lower|*do_upper|*do_const)){
      *do_digit = TRUE;
      *do_lower = TRUE;
      *do_upper = TRUE;
      *do_const = DO_DICT;
   }
   if((errflg) || ((argc - optind) != 1)){
      fprintf(stderr, "Usage:\n");
      fprintf(stderr, "   %s [options] <list file>\n", argv[0]);
      fprintf(stderr, "\t-d\t\tprocess digit fields\n");
      fprintf(stderr, "\t-l\t\tprocess lower case fields\n");
      fprintf(stderr, "\t-u\t\tprocess upper case fields\n");
      fprintf(stderr, "\t-c nodict\tprocess Constitution field without dictionary\n");
      fprintf(stderr, "\t-c dict\t\tprocess Constitution field using dictionary\n");
      fprintf(stderr, "\t-s\t\tsilent mode\n");
      fprintf(stderr, "\t-v\t\tverbose mode\n");
      fprintf(stderr, "\t-t <time file>\tcompute and report timings\n");
      exit(-1);
   }

   *listfile = argv[optind];
}
