/*
# proc: read_hsftmplt - reads the contents of the specified file into an
# proc:                 hsform structure.
*/

#include <stdio.h>
#include <hsform.h>

read_hsftmplt(file,hsform)
char *file;
HSFORM *hsform;
{
   FILE *fpd;
   int i;
   if ((fpd = fopen(file,"rb")) == NULL){
     printf("Unable to open template file: %s",file);
     fflush(stdout);
     exit(-1);
   }
   fscanf(fpd,"%d",&(hsform->num));
   for(i=0;i<hsform->num;i++){  
      fscanf(fpd,"%d",&hsform->box[i].vertex[0].x);
      fscanf(fpd,"%d",&hsform->box[i].vertex[0].y);
      fscanf(fpd,"%d",&hsform->box[i].vertex[1].x);
      fscanf(fpd,"%d",&hsform->box[i].vertex[1].y);
      fscanf(fpd,"%d",&hsform->box[i].vertex[2].x);
      fscanf(fpd,"%d",&hsform->box[i].vertex[2].y);
      fscanf(fpd,"%d",&hsform->box[i].vertex[3].x);
      fscanf(fpd,"%d",&hsform->box[i].vertex[3].y);
   }
   fclose(fpd);
}
