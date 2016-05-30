/*
# proc: readfetfile - opens an fet file and reads its contents into an
# proc:               fet structure.
*/

#include <stdio.h>
#include <string.h>
#include <fet.h>

FET *readfetfile(file)
char *file;
{
   FILE *fp;
   FET *fet;
   char c,buf[MAXFETLENGTH];

   if ((fp = fopen(file,"rb")) == NULL)
      syserr("readfetfile","fopen",file);

   fet = allocfet(MAXFETS);
   while (fscanf(fp,"%s",buf) != EOF){
      while(((c = getc(fp)) == ' ') || (c == '\t'));
      ungetc(c, fp);
      if (fet->num >= fet->alloc)
         reallocfet(fet, fet->alloc + MAXFETS);
      fet->names[fet->num] = strdup(buf);
      if(fet->names[fet->num] == NULL)
         syserr("readfetfile","strdup","fet->names[]");
      fgets(buf,MAXFETLENGTH-1,fp);
      buf[strlen(buf)-1] = '\0';
      fet->values[fet->num] = strdup(buf);
      if(fet->values[fet->num] == NULL)
         syserr("readfetfile","strdup","fet->values[]");
      (fet->num)++;
   }
   fclose(fp);
   return(fet);
}
