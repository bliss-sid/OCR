/*
# proc: substring - determines if the first string is a substring of the
# proc:             the second provided string.
*/

#include <stdio.h>

substring(s1, s2)
char *s1, *s2;
{
   if((s1 == NULL) || (s2 == NULL))
      return(0);
   if(strncmp(s1, s2, strlen(s1)) == 0)
      return(1);
   else
      return(0);
}
