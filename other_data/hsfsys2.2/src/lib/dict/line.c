/*
# proc: spell_line2 - takes an integer list of character classes and corrects
# proc:               segmentor and classifier errors and identifies word
# proc:               boundaries using the given phrase dictionary.
# proc: first_0 - locates the position (left-to-right) of the first
# proc:           occurance of a 0 in the alignment string.
# proc: last_0 - locates the position (right-to-left) of the first
# proc:          occurance of a 0 in the alignment string.
# proc: syncline - synchronizes processing of the list of character
# proc:            classifications upon incurring a dictionary match.
*/

#include <stdio.h>
#include <mfs.h>
#include <spell_ln.h>

/*************************************************************/
spell_line2(fpout, lookups, nlookups, cls, ncls, pdict, dumpflag)
FILE *fpout;
char *lookups[];
int *nlookups;
int *cls, ncls;
MFS *pdict;
int dumpflag;
{
   float slope, compute_slope();
   int level;

   slope = compute_slope(X1, Y1, X2, Y2);
   *nlookups = 0;
   level = 0;
   process_alpha_text(fpout, lookups, nlookups,
                      cls, ncls, 0, ncls, MAXWORDLEN,
                      pdict, X1, Y1, slope, dumpflag, level);
}

/*************************************************************/
first_0(alignment, alen)
int *alignment, alen;
{
   int i, first_0;

   first_0 = -1;
   for(i = 0; i < alen; i++){
      if(alignment[i] == 0){
         first_0 = i;
         break;
      }
   }
   if(first_0 == -1)
      fatalerr("first_0", "no correct character found in alignment", NULL);
   return(first_0);
}

/*************************************************************/
last_0(alignment, alen)
int *alignment, alen;
{
   int i, last_0;

   last_0 = -1;
   for(i = alen-1; i >= 0; i--){
      if(alignment[i] == 0){
         last_0 = i;
         break;
      }
   }
   if(last_0 == -1)
      fatalerr("last_0", "no correct character found in alignment",
               NULL);
   return(last_0);
}

/*************************************************************/
syncline(spellst, i)
SPELL_LINE *spellst;
int *i;
{
   int j, last, twos;

   last = last_0(spellst->align, spellst->alen);

   twos = 0;
   for(j = last; j >= 0; j--){
      if(spellst->align[j] == 2)
         twos++;
   }

   (*i) += (last - twos +1);
}
