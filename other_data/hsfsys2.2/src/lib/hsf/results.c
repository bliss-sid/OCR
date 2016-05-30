/*
# proc: string_results - takes a class map, a list of class indices, and a
# proc:                  list of confidences, and builds a hypothesis string
# proc:                  and a confidence string.
*/

#include <stdio.h>

/*************************************************************/
string_results(hypstr, constr, class_map, maplen, hyps_i, cons, nhyps)
char **hypstr, **constr, **class_map;
int *hyps_i;
float *cons;
int maplen, nhyps;
{
   int i, hyplen, conlen;
   char *hptr, *cptr;

   if(nhyps == 0){
      (*hypstr) = NULL;
      (*constr) = NULL;
      return;
   }

   hyplen = nhyps+1;
   conlen = (nhyps * 5)+1;

   calloc_char(hypstr, hyplen, "string_results : hypstr");
   calloc_char(constr, conlen, "string_results : constr");
   hptr = (*hypstr);
   cptr = (*constr);

   for(i = 0; i < nhyps; i++){
      if(hyps_i[i] >= maplen)
         fatalerr("string_results",
                  "hypothesis index exceeds class map length", NULL);
      if(strlen(class_map[hyps_i[i]]) != 1)
         fatalerr("string_results",
                  "string length of class not 1",
                  "class map must contain strings of single characters");
      sprintf(hptr, "%s", class_map[hyps_i[i]]);
      hptr++;
      sprintf(cptr, "%4.2f ", cons[i]);
      cptr+=5;
   }
   if(nhyps > 0)
      *(cptr-1) = '\0';
}
