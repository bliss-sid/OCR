/*
# proc: phrases2hypstr2 - appends the classes referenced in lists of phrase
# proc:                   indices into one long character string.
# proc: phrases2constr2 - appends the confidences referenced in lists of phrase
# proc:                   indices into one long character string.
*/

#include <stdio.h>

/*************************************************************/
char *phrases2hypstr2(hyps_i, nhyps, class_map, maplen,
                      pi_lists, pi_lens, nphrases)
int *hyps_i;
char **class_map;
int *pi_lists[], *pi_lens;
int nhyps, nphrases, maplen;
{
   int i, j, clen, *pi_list;
   char *sptr, *str;

   clen = 0;
   for(i = 0; i < nphrases; i++)
      clen += pi_lens[i];

   if(clen == 0)
      return(NULL);

   calloc_char(&str, clen+1, "phrases2hypstr2 : str");
   sptr = str;
   for(i = 0; i < nphrases; i++){
      pi_list = pi_lists[i];
      for(j = 0; j < pi_lens[i]; j++){
         (*sptr) = *(class_map[hyps_i[pi_list[j]]]);
         sptr++;
      }
   }
   return(str);
}

/*************************************************************/
char *phrases2constr2(cons, ncons, pi_lists, pi_lens, nphrases)
float *cons;
int *pi_lists[], *pi_lens;
int ncons, nphrases;
{
   int i, j, clen, *pi_list;
   char *sptr, *str;

   clen = 0;
   for(i = 0; i < nphrases; i++)
      clen += (pi_lens[i] * 5);

   if(clen == 0)
      return(NULL);

   calloc_char(&str, clen+1, "phrases2constr2 : str");
   sptr = str;
   for(i = 0; i < nphrases; i++){
      pi_list = pi_lists[i];
      for(j = 0; j < pi_lens[i]; j++){
         sprintf(sptr, "%4.2f ", cons[pi_list[j]]);
         sptr+=5;
      }
   }
   if(sptr != str)
      *(sptr-1) = '\0';
   return(str);
}
