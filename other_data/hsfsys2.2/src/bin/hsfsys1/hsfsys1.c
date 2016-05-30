/*
# pgrm: hsfsys1 - updated version of 1st release of program that conducts OCR
# pgrm:           on a binary image of an HSF form.
*/

#include <stdio.h>
#include <sys/param.h>
#include <math.h>
#include <defs.h>
#include <ihead.h>
#include <hsform.h>
#include <fet.h>
#include <hsfsys.h>

main(argc, argv)
int argc;
char *argv[];
{
   int do_digit, do_lower, do_upper, do_const, do_small;
   char *ifile, *oroot;
   char outfile[MAXPATHLEN];
   IHEAD *ihead;
   int iw, ih;
   unsigned char *indata, *regdata, *rmdata;
   HSFORM tmplt;
   FET *hyps, *confs, *init_hsf_fet();

   /* process the command line arguments */
   procargs(argc, argv, &do_digit, &do_lower, &do_upper, &do_const, &do_small,
            &_silent_, &_verbose_, &_time_, &ifile, &oroot);

   init_timer(oroot);
   set_timer2();

   /*****************************************************/
   do_hsf_form("form ", &ihead, &indata, &regdata, &rmdata, &iw, &ih, &tmplt,
               ifile, TMPLT_FIELDS, REG_POINTS, TMPLT_DILATE);

   /* initialize an FET structure to hold system field hypotheses */
   hyps = init_hsf_fet(tmplt.num);
   /* initialize an FET structure to hold system character confidences */
   confs = init_hsf_fet(tmplt.num);

   /*****************************************************/
   if(do_digit){
      if(do_small)
         do_digit_fields("digit", FIRST_DIGIT, LAST_DIGIT,
                  DIGIT_EVT_SM, DIGIT_PAT_SM, DIGIT_MED_SM, DIGIT_SET_SM,
                  rmdata, iw, ih, &tmplt, hyps, confs);
      else
         do_digit_fields("digit", FIRST_DIGIT, LAST_DIGIT,
                         DIGIT_EVT, DIGIT_PAT, DIGIT_MED, DIGIT_SET,
                         rmdata, iw, ih, &tmplt, hyps, confs);
   }

   if(do_lower)
      do_alpha_field("lower", FIRST_LOWER,
                     LOWER_EVT, LOWER_PAT, LOWER_MED, LOWER_SET,
                     rmdata, iw, ih, &tmplt, hyps, confs);

   if(do_upper)
      do_alpha_field("upper", FIRST_UPPER,
                     UPPER_EVT, UPPER_PAT, UPPER_MED, UPPER_SET,
                     rmdata, iw, ih, &tmplt, hyps, confs);

   if(do_const){
      if(do_small)
         do_const_field("const", FIRST_CONST, do_const, PHRASE_DICT,
                CONST_EVT_SM, CONST_PAT_SM, CONST_MED_SM, CONST_SET_SM,
                rmdata, iw, ih, &tmplt, hyps, confs);
      else
         do_const_field("const", FIRST_CONST, do_const, PHRASE_DICT,
                        CONST_EVT, CONST_PAT, CONST_MED, CONST_SET,
                        rmdata, iw, ih, &tmplt, hyps, confs);
   }

   /*****************************************************/
   sprintf(outfile, "%s.hyp", oroot);
   writefetfile(outfile, hyps);
   sprintf(outfile, "%s.con", oroot);
   writefetfile(outfile, confs);

   free_form(ihead, indata, regdata, rmdata);
   freefet(hyps);
   freefet(confs);

   time2_elapsed("total", "user time ");
   close_timer();
   exit(0);
}

/*************************************************************/
procargs(argc, argv, do_digit, do_lower, do_upper, do_const, do_small,
            silent, verbose, time, ifile, oroot)
int argc, *do_digit, *do_lower, *do_upper, *do_const, *do_small;
int *silent, *verbose, *time;
char **ifile, **oroot, *argv[];
{
   int c, errflg = 0;

   *do_digit = FALSE;
   *do_lower = FALSE;
   *do_upper = FALSE;
   *do_const = FALSE;
   *do_small = FALSE;
   *silent = FALSE;
   *verbose = FALSE;
   *time = FALSE;
   while(((c = getopt(argc, argv, "c:dlmstuv")) != -1) && (!errflg)){
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
       case 'm':
         *do_small = TRUE;
         break;
      case 's':
         *silent = TRUE;
         break;
      case 't':
         *time = TRUE;
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
   if((errflg) || ((argc - optind) != 2)){
      fprintf(stderr, "Usage:\n");
      fprintf(stderr, "   %s [options] <hsf file> <output root>\n", argv[0]);
      fprintf(stderr, "\t-d\t\tprocess digit fields\n");
      fprintf(stderr, "\t-l\t\tprocess lower case fields\n");
      fprintf(stderr, "\t-u\t\tprocess upper case fields\n");
      fprintf(stderr, "\t-c nodict\tprocess Constitution field without dictionary\n");
      fprintf(stderr, "\t-c dict\t\tprocess Constitution field using dictionary\n");
      fprintf(stderr, "\t-m\t\tsmall memory mode\n");
      fprintf(stderr, "\t-s\t\tsilent mode\n");
      fprintf(stderr, "\t-v\t\tverbose mode\n");
      fprintf(stderr, "\t-t\t\tcompute and report timings\n");
      exit(-1);
   }

   *ifile = argv[optind++];
   *oroot = argv[optind];
}
