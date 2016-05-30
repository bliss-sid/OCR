/*
# proc: init_hsf_fet - allocates a new fet structures and initializes it
# proc:                with hsf field names.
*/

#include <stdio.h>
#include <fet.h>

FET *init_hsf_fet(num)
int num;
{
   FET *fet;
   char name[MAXFETLENGTH];
   int i;

   fet = allocfet(num);
   for(i = 0; i < num; i++){
      sprintf(name, "hsf_%d", i);
      fet->names[i] = (char *)strdup(name);
      if(fet->names[i] == NULL)
         syserr("init_hsf_fet", "strdup", "fet->names[i]");
   }
   fet->num = num;
   return(fet);
}
   
