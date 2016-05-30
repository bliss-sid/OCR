/*
# proc: free_names - deallocates an array of strings.
# proc:
*/

#include <stdio.h>

free_names(pats, nfiles) 
char **pats;
int nfiles;
{
int i;   
    for (i = 0; i < nfiles ; i++)
      free((char *)pats[i]);
    free(pats);
}
