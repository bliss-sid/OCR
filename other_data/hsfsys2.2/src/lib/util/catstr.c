/*
# proc: concat_strings - concatenates two given strings together, returning one
# proc:                  new string.
# proc: concat_strlist - concatenates all the strings in the specified list of
# proc:                  strings together, returning one new string.
*/

#include <stdio.h>

/*************************************************************/
char *concat_strings(s1, s2)
char *s1, *s2;
{
   int clen;
   char *str, *ptr;

   if(s1 == NULL)
      fatalerr("concat_strings", "first string is NULL", NULL);
   if(s2 == NULL)
      fatalerr("concat_strings", "2nd string is NULL", NULL);

   clen = 0;
   clen += (strlen(s1) + 1);
   clen += (strlen(s2) + 1);

   malloc_char(&str, clen+1, "concat_strings : str");
   ptr = str;

   sprintf(ptr, "%s ", s1);
   ptr += (strlen(s1) + 1);
   sprintf(ptr, "%s ", s2);

   str[strlen(str)-1] = '\0';
   return(str);
}

/*************************************************************/
char *concat_strlist(strlist, nstr)
char *strlist[];
int nstr;
{
   int i, clen;
   char *str, *ptr;

   if(nstr <= 0)
      return(NULL);
   clen = 0;
   for(i = 0; i < nstr; i++)
      clen += (strlen(strlist[i]) + 1);
   malloc_char(&str, clen+1, "concat_strlist : str");
   ptr = str;
   for(i = 0; i < nstr; i++){
      sprintf(ptr, "%s ", strlist[i]);
      ptr += (strlen(strlist[i]) + 1);
   }
   str[strlen(str)-1] = '\0';
   return(str);
}
