/*
# proc: process_alpha_text - takes an integer list of character classes and
# proc:                      recursively processes them into words provided in
# proc:                      a phrase dictionary by generating fan-outs of
# proc:                      hypothesized words.
# proc: process_fanout - takes a fan-out of hypothesized words and recursively
# proc:                  processes it into words provided in a phrase dic-
# proc:                  tionary using classes stored in an integer list.
# proc: build_fanout - fills in a fan-out structure with hypothesized words
# proc:                from the provided integer list of classes and matches
# proc:                them to the phrase dictionary.
# proc: spell_fanout_word- mathes a hypothesized word in a fanout structure
# proc:                    to the provided phrase dictionary storing the
# proc:                    resulting match and fanout signal.
# proc: fanout_signal - given a hypothesized word match, its alignment, and
# proc:                 the parameters for a linear translation function,
# proc:                 computes a fanout signal.
*/

#include <stdio.h>
#include <mfs.h>
#include <spell_ln.h>

/*************************************************************/
process_alpha_text(fpout, lookups, nlookups,
                   cls, ncls, start, len, maxwordlen,
                   pdict, x, y, slope, dumpflag, level)
FILE *fpout;
char *lookups[];
int *nlookups;
int *cls, ncls;
MFS *pdict;
int dumpflag;
int start, len, maxwordlen, level;
float x, y, slope;
{
   int i, nlist;
   SPELL_LINE spell_list[MAXWORDLEN];
   int choices[MAXWORDLEN], nchosen;
   int old_i;

   i = start;
   while(i < len){
      build_fanout(spell_list, &nlist, cls, ncls, i, maxwordlen,
                        pdict, x, y, slope);
      if(dumpflag){
         dumppseudowords2(stdout, spell_list, nlist);
      }
      nchosen = 0;
      process_fanout(cls, i, ncls, pdict, x, y, slope,
                         dumpflag, fpout, lookups, nlookups,
                         choices, &nchosen, spell_list, 0, nlist-1, level);
      if(nchosen > 0){
         old_i = i;
         syncline(&spell_list[choices[nchosen-1]], &i);
         if(level > 0)
            maxwordlen -= (i - old_i);
      }
      else{
         i++;
         if(level > 0)
            maxwordlen--;
      }
   }
}

/*************************************************************/
process_fanout(cls, cls_i, ncls, pdict, x, y, slope,
                   dumpflag, fpout, lookups, nlookups,
                   choices, nchosen, spell_list, start, end, level)
int *cls, cls_i, ncls;
MFS *pdict;
int dumpflag;
float x, y, slope;
int *choices, *nchosen;
FILE *fpout;
char *lookups[];
int *nlookups;
SPELL_LINE spell_list[];
int start, end, level;
{
   int max_i, first;
   float max_c;
   int maxwordlen, cls_len;

   choosemaxpseudoword(&max_c, &max_i, spell_list, start, end);

   /* if no word found in range, then done */
   if(max_c <= 0.0)
      return;

   /* if word found is not left justified in range, then process left range */
   if(!is_in_int_set(spell_list[max_i].align[0], "02")){
      /* get new left range */
      first = first_0(spell_list[max_i].align, spell_list[max_i].alen);
      maxwordlen = first;
      cls_len = cls_i + maxwordlen;

      /* process new left range */
      process_alpha_text(fpout, lookups, nlookups,
                     cls, ncls, cls_i, cls_len, maxwordlen,
                     pdict, x, y, slope, dumpflag, ++level);
   }

   /* store current word info */
   choices[*nchosen] = max_i;
   (*nchosen)++;
   if((lookups[*nlookups] = (char *)strdup(spell_list[max_i].lookup)) == NULL)
      syserr("process_fanout", "strdup", "lookups");
   if(fpout != NULL){
      fprintf(fpout, "%s ", lookups[*nlookups]);
      fflush(fpout);
   }
   if(dumpflag){
      fprintf(stdout, "\n");
      fflush(stdout);
   }
   (*nlookups)++;
}

/*************************************************************/
build_fanout(spell_list, nlist, cls, ncls, start, maxwordlen,
                  pdict, x, y, slope)
SPELL_LINE spell_list[];
int *cls, ncls;
MFS *pdict;
int start, maxwordlen, *nlist;
float x, y, slope;
{
   int j, s;

   (*nlist) = 0;
   for(j = start, s = 0;
       (j < (start + maxwordlen)) && (j < ncls);
       j++, s++){
      if(s == 0){
         *(spell_list[s].word) = '\0';
         growword2(spell_list[s].word, maxwordlen, cls, j);
      }
      else{
         strcpy(spell_list[s].word, spell_list[s-1].word);
         growword2(spell_list[s].word, maxwordlen, cls, j);
      }
      spell_fanout_word(&(spell_list[s]), pdict, x, y, slope);
      (*nlist)++;
   }
}

/*************************************************************/
spell_fanout_word(spellst, pdict, x, y, slope)
SPELL_LINE *spellst;
MFS *pdict;
float x, y, slope;
{
   float conf, fanout_signal();
   int winner, *alignment, alen;

   lookup_levenstein(spellst->word, pdict, &winner, &conf,
                     &alignment, &alen);

   if(alen == 0){
      spellst->conf = 0.0;
      spellst->alen = alen;
      *(spellst->lookup) = '\0';
      return;
   }

   spellst->alen = alen;
   strcpy(spellst->lookup, pdict->values[winner]);
   memcpy(spellst->align, alignment, alen * sizeof(int));
   spellst->conf = fanout_signal(spellst->lookup,alignment,alen,x,y,slope);
   free(alignment);
}

/*************************************************************/
float fanout_signal(hypstr, alignment, alen, x, y, slope)
char *hypstr;
int *alignment, alen;
float x, y, slope;
{
   float e, s, t, p, compute_y();
   int i, in, ig;

   in = 0;
   for(i = 0; i < alen; i++){
      if(alignment[i] != 0)
         in++;
   }
   ig = int_groups(alignment, alen, "13");
   if(alen == ig)
      e = 1.0;
   else
      e = in/(float)(alen - ig);
   p = (float)strlen(hypstr);
   t = compute_y(p, slope, x, y);
   s = (1.0 - e) - t;
   return(s);
}
