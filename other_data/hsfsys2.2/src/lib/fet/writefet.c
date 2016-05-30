/*
# proc: writefetfile - write the contents of an fet structure to the
# proc:                specified file.
*/

#include <stdio.h>
#include <fet.h>

void writefetfile(file,fet)
char *file;
FET *fet;
{
  FILE *fp = NULL;
  int item;
  if ((fp = fopen(file,"wb")) ==NULL)
     syserr("writefetfile","fopen",file);
   for (item = 0; item<fet->num; item++){
       if(fet->values[item] == NULL)
          fprintf(fp,"%s\n",fet->names[item]);
       else
          fprintf(fp,"%s %s\n",fet->names[item],fet->values[item]);
   }
   fclose(fp);

}
