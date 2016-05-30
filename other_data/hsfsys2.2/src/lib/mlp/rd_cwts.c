/*
# proc: rd_cwts - Reads a class-weights file.

Input args:
  nouts: Number of output nodes, which is the number of classes.
  short_classnames: The short names of the classes, in their standard
    order, in their form as provided by lng2shrt, i.e. padded with a
    space in front if original short name read from file had only one
    char.
  class_wts_infile: The class-weights file to be read.  Each line
    consists of a short class-name, non-newline whitespace char(s),
    then the class-weight (prior weight) for this class.  The order of
    the lines of this file does not matter.  Whether or not the short
    class-names in this file have a padding blank in front when only
    1 char long, or not, does not matter.

Output arg:
  class_wts: Buffer containing the class-weights, malloced by this
    routine.  On return, for each 0 <= i < nouts, (*class_wts)[i] will
    contain the class-weight that goes with the class whose short name
    is short_classnames[i]; so, these weights will be in the standard
    order, since the elts of short_classnames (supposedly) are.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mlp/defs.h>

void
rd_cwts(nouts, short_classnames, class_wts_infile, class_wts)
int nouts;
char **short_classnames, *class_wts_infile;
float **class_wts;
{
  FILE *fp;
  char str[100], line[100], *set, ascn[50], yowchar;
  int nlines, iline, i;
  float awt;

  if((fp = fopen(class_wts_infile, "rb")) == (FILE *)NULL)
    syserr("rd_cwts", "fopen for reading failed", class_wts_infile);
  for(nlines = 0; fgets(line, 100, fp); nlines++);
  rewind(fp);
  if(nlines != nouts) {
    sprintf(str, "No. of lines in %s, %d, does not equal\n\
nouts arg, %d", class_wts_infile, nlines, nouts);
    fatalerr("rd_cwts", str, NULL);
  }
  if((set = calloc(nouts, sizeof(char))) == (char *)NULL)
    syserr("rd_cwts", "calloc", "set");
  if((*class_wts = (float *)malloc(nouts * sizeof(float))) ==
    (float *)NULL)
    syserr("rd_cwts", "malloc", "*class_wts");
  for(iline = 1; iline <= nouts; iline++) {
    fgets(line, 100, fp);
    if(sscanf(line, "%s %f", ascn, &awt) != 2) {
      sprintf(str, "line %d of %s does not consist of a string (a\n\
short class-name) and a floating-point no. (class-weight), as \
required", iline, class_wts_infile);
      fatalerr("rd_cwts", str, NULL);
    }
    if(strlen(ascn) > 2) {
      sprintf(str, "line %d of %s contains short name %s with\n> 2 \
characters", iline, class_wts_infile, ascn);
      fatalerr("rd_cwts", str, NULL);
    }
    /* Since the names in short_classnames have already been padded
    to 2 chars (blank in front) when necessary, do the same to the
    current supposed short class-name before comparing. */
    if(strlen(ascn) == 1) {
      yowchar = ascn[0];
      sprintf(ascn, " %c", yowchar);
    }
    for(i = 0; i < nouts; i++)
      if(!strcmp(ascn, short_classnames[i])) {
	(*class_wts)[i] = awt;
	set[i] = TRUE;
	break;
      }
  }
  fclose(fp);
  for(i = 0; i < nouts; i++)
    if(!set[i]) {
      sprintf(str, "%s does not set a class-weight for\n\
short class-name %s", class_wts_infile, short_classnames[i]);
      fatalerr("rd_cwts", str, NULL);
    }
  free(set);
}
