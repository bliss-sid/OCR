/*
# proc: readmedianfile - read the vectors that are the median of
# proc:                  of features for each class
# proc: writemedianfile - write a median file as above. The file generally
# proc:                   stores L text vectors each of N elements.
*/

#include <stdio.h>
#include <malloc.h>

readmedianfile(fn, vctr,      nctr,  ninp)
char *fn; float  **vctr; int *nctr, *ninp;
{
FILE *fp;
int  c, i;
float *vtr;
 
    if ((fp = fopen(fn, "rb")) == NULL)
       syserr("readmedianfile", "fopen", fn);
 
    fscanf(fp, "%d %d", nctr, ninp);
 
    malloc_flt(vctr, (*nctr) * (*ninp), "readmedianfile : vctr");
    vtr = *vctr;
 
    for ( c = 0 ; c < *nctr ; c++ )
       for (i = 0 ; i < *ninp ; i++ )
          fscanf(fp, "%f", vtr++);
 
    fclose(fp);
}


writemedianfile(fn, vctr,     nctr, ninp)
char *fn;   float  *vctr; int nctr, ninp;
{
FILE *fp;
int  c, i;
float *vtr;
 
    if ((fp = fopen(fn, "wb")) == NULL)
       syserr("writemedianfile", "fopen", fn);
 
    fprintf(fp, "%d %d\n", nctr, ninp);
    fprintf(fp, "\n");
 
    for ( c = 0, vtr = vctr ; c < nctr ; c++ )
    {
       for (i = 0 ; i < ninp ; i++ )
          fprintf(fp, "%15.7E%c", *vtr++, ((i+1)%5 == 0) ? '\n' : ' ');
       fprintf(fp, "\n");
       if ((i % 5) != 0)
          fprintf(fp, "\n");
    }
 
    fclose(fp);
}
