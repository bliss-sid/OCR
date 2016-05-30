/*
# proc: choosemaxpseudoword - determines the best match given a list of
# proc:                       hypthesized words in a fan-out.
# proc: dumppseudowords2 - generates another formated report of a fan-out to
# proc:                    the specified file pointer.
# proc: growword2 - takes a character string and appends a new character
# proc:             stored in a list of integers classes to the end of the
# proc:             string provided there is sufficient memory.
*/

#include <stdio.h>
#include <spell_ln.h>

/*************************************************************/
choosemaxpseudoword(max_c, max_i, spell_list, start, end)
float *max_c;
int *max_i;
SPELL_LINE spell_list[];
int start, end;
{
   int i;

   *max_i = start;
   *max_c = spell_list[start].conf;
   for(i = start+1; i <= end; i++){
      if(spell_list[i].conf > *max_c){
         *max_i = i;
         *max_c = spell_list[i].conf;
      }
      else{
         if((strcmp(spell_list[*max_i].lookup, spell_list[i].lookup) != 0) &&
            substring(spell_list[*max_i].lookup, spell_list[i].lookup) &&
            (spell_list[i].conf > 0.0)){
            *max_i = i;
            *max_c = spell_list[i].conf;
         }
      }
   }
}

/*************************************************************/
dumppseudowords2(fpout, spell_list, nlist)
FILE *fpout;
SPELL_LINE spell_list[];
int nlist;
{
   int s, k;
   char astring[80];

   for(s = 0; s < nlist; s++){
      fprintf(fpout, "%15f   %-12s   %-12s   ",
          spell_list[s].conf, spell_list[s].word, spell_list[s].lookup);
      if(spell_list[s].alen > 0){
         for(k = 0; k < spell_list[s].alen; k++)
            astring[k] = '0' + spell_list[s].align[k];
         astring[k] = '\0';
         fprintf(fpout, "%s\n", astring);
      }
      else
         fprintf(fpout, "%s\n", "<empty>");
   }
}

/*************************************************************/
growword2(word, maxwordlen, cls, i)
char *word;
int *cls;
int maxwordlen, i;
{
   char *wptr;
   int len;

   len = strlen(word);
   if(len > maxwordlen)
      fatalerr("growword2", "maxwordlen exceeded", NULL);

   wptr = word + len;
   *wptr++ = (char)to_upper(cls[i]);
   *wptr = '\0';
}

