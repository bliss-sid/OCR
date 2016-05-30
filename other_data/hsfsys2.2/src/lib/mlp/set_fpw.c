/*
# proc: set_fpw - Sets the final pattern-weights.

CAUTION: This version might not produce sensible results if the
frequencies of the classes in the patterns set are unequal.  Perhaps a
later version can correct for this, by dividing by the frequencies or
by the counts by class in the appropriate places. */

#include <stdio.h>
#include <stdlib.h>
#include <mlp/defs.h>
#include <mlp/parms.h>

void
set_fpw(priors, class_wts_infile, nouts, short_classnames,
  pattern_wts_infile, npats, classes, patwts)
char priors, *class_wts_infile, **short_classnames,
  *pattern_wts_infile;
int nouts, npats;
short *classes;
float **patwts;
{
  FILE *fp;
  char str[100];
  static char first = TRUE;
  short *classes_p;
  int i;
  static int nouts_maxsofar;
  float patwts_val, *patwts_p, *patwts_e, aweight, *class_wts;
  static float *patwts_byclass;

  if(first || nouts > nouts_maxsofar) {
    if(!first)
      free((char *)patwts_byclass);
    nouts_maxsofar = nouts;
    if((patwts_byclass = (float *)malloc(nouts * sizeof(float))) ==
      (float *)NULL)
      syserr("set_fpw", "malloc", "patwts_byclass");
    first = FALSE;
  }

  if((*patwts = (float *)malloc(npats * sizeof(float))) ==
    (float *)NULL)
    syserr("set_fpw", "malloc", "*patwts");

  switch(priors) {

  case ALLSAME:
    patwts_val = 1. / (float)npats;
    for(patwts_e = (patwts_p = *patwts) + npats;
      patwts_p < patwts_e; patwts_p++)
      *patwts_p = patwts_val;
    break;

  case CLASS:
    rd_cwts(nouts, short_classnames, class_wts_infile, &class_wts);
    for(i = 0; i < nouts; i++)
      patwts_byclass[i] = class_wts[i] / npats;
    free((char *)class_wts);
    for(patwts_e = (patwts_p = *patwts) + npats,
      classes_p = classes; patwts_p < patwts_e; patwts_p++,
      classes_p++)
      *patwts_p = patwts_byclass[*classes_p];
    break;

  case PATTERN:
    if((fp = fopen(pattern_wts_infile, "rb")) == (FILE *)NULL)
      syserr("set_fpw", "fopen for reading failed",
        pattern_wts_infile);
    for(patwts_e = (patwts_p = *patwts) + npats;
      patwts_p < patwts_e; patwts_p++) {
      if(fscanf(fp, "%f", &aweight) != 1)
	fatalerr("set_fpw", "fscanf of aweight failed (probably not \
enough weights)", pattern_wts_infile);
      *patwts_p = aweight / (float)npats;
    }
    fclose(fp);
    break;

  case BOTH:
    rd_cwts(nouts, short_classnames, class_wts_infile, &class_wts);
    if((fp = fopen(pattern_wts_infile, "rb")) == (FILE *)NULL)
      syserr("set_fpw", "fopen for reading failed",
        pattern_wts_infile);
    for(patwts_e = (patwts_p = *patwts) + npats,
      classes_p = classes; patwts_p < patwts_e; patwts_p++,
      classes_p++) {
      if(fscanf(fp, "%f", &aweight) != 1)
	fatalerr("set_fpw", "fscanf of aweight failed (probably not \
enough weights)", pattern_wts_infile);
      *patwts_p = class_wts[*classes_p] * aweight / (float)npats;
    }
    free((char *)class_wts);
    fclose(fp);
    break;

  default:
    sprintf(str, "priors must be ALLSAME (%d), CLASS (%d), \
PATTERN (%d), or BOTH (%d); it is %d", (int)ALLSAME, (int)CLASS,
      (int)PATTERN, (int)BOTH, (int)priors);
    fatalerr("set_fpw", str, NULL);
    break;

  } /* switch(priors) */
}
