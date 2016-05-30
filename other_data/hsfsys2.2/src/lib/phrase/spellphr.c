/*
# proc: spell_phrases2 - takes a lists of phrase indices whose elements are
# proc:                  sorted into reading order and corrects errors and
# proc:                  identifies word boundaries using the specified
# proc:                  dictionaries.
# proc: spell_phrases_Rel2 - takes a lists of phrase indices whose elements are
# proc:                  sorted into reading order and corrects errors and
# proc:                  identifies word boundaries using the specified
# proc:                  dictionaries (passed word list rather than dictionary
# proc:                  file).
*/

#include <stdio.h>
#include <mfs.h>
#include <maxlist.h>

/*************************************************************/
spell_phrases2(hypstr, hyps_i, nhyps, class_map, maplen,
               pi_lists, pi_lens, nphrases, pfile, fp)
char **hypstr;
int *hyps_i, nhyps, maplen;
char **class_map;
int *pi_lists[], *pi_lens;
int nphrases;
char *pfile;
FILE *fp;
{
   int i, j;
   MFS *pdict, *load_phrase_dictionary();
   char *lookups[128];
   int nlookup;
   char *tstr, *ttstr, *tttstr;
   char *concat_strlist(), *concat_strings();
   int clist[MAX_INDEX], *pi_list;

   if(nphrases > MAX_INDEX)
      fatalerr("spell_phrases2", "# of phrases exceeds MAX_INDEX", NULL);

   pdict = load_phrase_dictionary(pfile);

   tstr = NULL;
   ttstr = NULL;
   tttstr = NULL;
   for(i = 0; i < nphrases; i++){
      pi_list = pi_lists[i];
      for(j = 0; j < pi_lens[i]; j++)
         clist[j] = *(class_map[hyps_i[pi_list[j]]]);
      spell_line2(fp, lookups, &nlookup, clist, pi_lens[i], pdict, 0);
      tstr = concat_strlist(lookups, nlookup);
      if(tstr != NULL){
         if(ttstr != NULL){
            tttstr = concat_strings(ttstr, tstr);
            free(tstr);
            free(ttstr);
            ttstr = tttstr;
         }
         else
            ttstr = tstr;
      }
      for(j = 0; j < nlookup; j++)
         free(lookups[j]);
   }

   (*hypstr) = ttstr;

   freemfs(pdict);
}

/*************************************************************/
spell_phrases_Rel2(hypstr, hyps_i, nhyps, class_map, maplen,
               pi_lists, pi_lens, nphrases, pdict, fp)
char **hypstr;
int *hyps_i, nhyps, maplen;
char **class_map;
int *pi_lists[], *pi_lens;
int nphrases;
MFS *pdict;
FILE *fp;
{
   int i, j;
   char *lookups[128];
   int nlookup;
   char *tstr, *ttstr, *tttstr;
   char *concat_strlist(), *concat_strings();
   int clist[MAX_INDEX], *pi_list;

   if(nphrases > MAX_INDEX)
      fatalerr("spell_phrases_Rel2", "# of phrases exceeds MAX_INDEX", NULL);

   tstr = NULL;
   ttstr = NULL;
   tttstr = NULL;
   for(i = 0; i < nphrases; i++){
      pi_list = pi_lists[i];
      for(j = 0; j < pi_lens[i]; j++)
         clist[j] = *(class_map[hyps_i[pi_list[j]]]);
      spell_line2(fp, lookups, &nlookup, clist, pi_lens[i], pdict, 0);
      tstr = concat_strlist(lookups, nlookup);
      if(tstr != NULL){
         if(ttstr != NULL){
            tttstr = concat_strings(ttstr, tstr);
            free(tstr);
            free(ttstr);
            ttstr = tttstr;
         }
         else
            ttstr = tstr;
      }
      for(j = 0; j < nlookup; j++)
         free(lookups[j]);
   }

   (*hypstr) = ttstr;
}
