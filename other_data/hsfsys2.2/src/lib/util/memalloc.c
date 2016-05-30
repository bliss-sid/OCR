/*
# proc: datadup - mallocs new space and copies supplied data into it, see also
# proc:           ../image/imageops.c imagedup
# proc: malloc_char - allocates (malloc) a char of specified size n
# proc:
# proc: malloc_uchar - allocates (malloc) an unsigned char of specified size n
# proc:
# proc: malloc_shrt - allocates (malloc) a short of specified size n
# proc:
# proc: malloc_int - allocates (malloc) an int of specified size n
# proc:
# proc: malloc_flt - allocates (malloc) a float of specified size n
# proc:
# proc: calloc_char - allocates (calloc) a char of specified size n
# proc:
# proc: calloc_uchar - allocates (calloc) an unsigned char of specified size n
# proc:
# proc: malloc_shrt - allocates (calloc) a short of specified size n
# proc:
# proc: calloc_int - allocates (calloc) an int of specified size n
# proc:
# proc: calloc_flt - allocates (calloc) a float of specified size n
# proc:
# proc: malloc_dbl_char_l1 - allocates (malloc) a set of char pointers of
# proc:                      specified number n
# proc: malloc_dbl_uchar_l1 - allocates (malloc) a set of unsigned char pointers
# proc:                       of specified number n
# proc: malloc_dbl_shrt_l1 - allocates (malloc) a set of short pointers of
# proc:                     specified number n
# proc: malloc_dbl_int_l1 - allocates (malloc) a set of int pointers of specified
# proc:                     number n
# proc: malloc_dbl_flt_l1 - allocates (malloc) a set of float pointers of
# proc:                     specified number n
# proc: realloc_char - reallocates a char to new size n
# proc:
# proc: realloc_uchar - reallocates an unsigned char to new size n
# proc:
# proc: realloc_shrt - reallocates a short to new size n
# proc:
# proc: realloc_int - reallocates an int to new size n
# proc:
# proc: realloc_flt - reallocates an float to new size n
# proc:
# proc: realloc_dbl_char_l1 - reallocates a set of char pointers to a
# proc:                        new number n
# proc: realloc_dbl_uchar_l1 - reallocates a set of unsigned char pointers
# proc:                        to new number n
# proc: realloc_dbl_int_l1 - reallocates a set of int pointers to new number n
# proc:
# proc: realloc_dbl_flt_l1 - reallocates a set of float pointers to new number n
# proc:
*/

#include <stddef.h>
#include <stdlib.h>

/*******************************************************************/
void *datadup(p, nbytes, s)
void *p; int nbytes; char *s;
{
void *q;
   if ( p == NULL )
      return NULL;

   if ((q = (void *)malloc(nbytes)) == NULL)
      syserr("datadup", "malloc", s);

   memcpy(q, p, nbytes);
   return q;
}

/*******************************************************************/
malloc_char(ptr, n, s)
   char **ptr;
   int n;
   char *s;
{
   if(((*ptr) = (char *)malloc(n * sizeof(char))) == NULL)
      syserr("malloc_char","malloc",s);
}

/*******************************************************************/
malloc_uchar(ptr, n, s)
   unsigned char **ptr;
   int n;
   char *s;
{
   if(((*ptr) = (unsigned char *)malloc(n * sizeof(unsigned char))) == NULL)
      syserr("malloc_uchar","malloc",s);
}

/*******************************************************************/
malloc_shrt(ptr, n, s)
   short **ptr, n;
   char *s;
{
   if(((*ptr) = (short *)malloc(n * sizeof(short))) == NULL)
      syserr("malloc_shrt","malloc",s);
}

/*******************************************************************/
malloc_int(ptr, n, s)
   int **ptr, n;
   char *s;
{
   if(((*ptr) = (int *)malloc(n * sizeof(int))) == NULL)
      syserr("malloc_int","malloc",s);
}

/*******************************************************************/
malloc_flt(ptr, n, s)
   float **ptr;
   int n;
   char *s;
{
   if(((*ptr) = (float *)malloc(n * sizeof(float))) == NULL)
      syserr("malloc_float","malloc",s);
}

/*******************************************************************/
calloc_char(ptr, n, s)
   char **ptr;
   int n;
   char *s;
{
   if(((*ptr) = (char *)calloc(n, sizeof(char))) == NULL)
      syserr("calloc_char","calloc",s);
}

/*******************************************************************/
calloc_uchar(ptr, n, s)
   unsigned char **ptr;
   int n;
   char *s;
{
   if(((*ptr) = (unsigned char *)calloc(n, sizeof(unsigned char))) == NULL)
      syserr("calloc_uchar","calloc",s);
}

/*******************************************************************/
calloc_shrt(ptr, n, s)
   short **ptr, n;
   char *s;
{
   if(((*ptr) = (short *)calloc(n, sizeof(short))) == NULL)
      syserr("calloc_shrt","calloc",s);
}

/*******************************************************************/
calloc_int(ptr, n, s)
   int **ptr, n;
   char *s;
{
   if(((*ptr) = (int *)calloc(n, sizeof(int))) == NULL)
      syserr("calloc_int","calloc",s);
}

/*******************************************************************/
calloc_flt(ptr, n, s)
   float **ptr;
   int n;
   char *s;
{
   if(((*ptr) = (float *)calloc(n, sizeof(float))) == NULL)
      syserr("calloc_float","calloc",s);
}

/*******************************************************************/
malloc_dbl_char_l1(ptr, ndbl, s)
   char ***ptr;
   int ndbl;
   char *s;
{
   if(((*ptr) = (char **)malloc(ndbl * sizeof(char *))) == NULL)
      syserr("malloc_dbl_char_l1","malloc",s);
}

/*******************************************************************/
malloc_dbl_uchar_l1(ptr, ndbl, s)
   unsigned char ***ptr;
   int ndbl;
   char *s;
{
   if(((*ptr) = (unsigned char **)malloc(ndbl * sizeof(unsigned char *))) == NULL)
      syserr("malloc_dbl_uchar_l1","malloc",s);
}

/*******************************************************************/
malloc_dbl_shrt_l1(ptr, ndbl, s)
   short ***ptr, ndbl;
   char *s;
{
   if(((*ptr) = (short **)malloc(ndbl * sizeof(short *))) == NULL)
      syserr("malloc_dbl_shrt_l1","malloc",s);
}

/*******************************************************************/
malloc_dbl_int_l1(ptr, ndbl, s)
   int ***ptr, ndbl;
   char *s;
{
   if(((*ptr) = (int **)malloc(ndbl * sizeof(int *))) == NULL)
      syserr("malloc_dbl_int_l1","malloc",s);
}

/*******************************************************************/
malloc_dbl_flt_l1(ptr, ndbl, s)
   float ***ptr;
   int ndbl;
   char *s;
{
   if(((*ptr) = (float **)malloc(ndbl * sizeof(float *))) == NULL)
      syserr("malloc_dbl_flt_l1","malloc",s);
}

/*******************************************************************/
realloc_char(ptr, n, s)
   char **ptr;
   int n;
   char *s;
{
   if((*ptr = (char *)realloc(*ptr, n * sizeof(char))) == NULL)
      syserr("realloc_char","realloc",s);
}

/*******************************************************************/
realloc_uchar(ptr, n, s)
   unsigned char **ptr;
   int n;
   char *s;
{
   if((*ptr = (unsigned char *)realloc(*ptr, n * sizeof(unsigned char))) == NULL)
      syserr("realloc_uchar","realloc",s);
}

/*******************************************************************/
realloc_shrt(ptr, n, s)
   short **ptr, n;
   char *s;
{
   if((*ptr = (short *)realloc(*ptr, n * sizeof(short))) == NULL)
      syserr("realloc_shrt","realloc",s);
}

/*******************************************************************/
realloc_int(ptr, n, s)
   int **ptr, n;
   char *s;
{
   if((*ptr = (int *)realloc(*ptr, n * sizeof(int))) == NULL)
      syserr("realloc_int","realloc",s);
}

/*******************************************************************/
realloc_flt(ptr, n, s)
   float **ptr;
   int n;
   char *s;
{
   if((*ptr = (float *)realloc(*ptr, n * sizeof(float))) == NULL)
      syserr("realloc_flt","realloc",s);
}

/*******************************************************************/
realloc_dbl_int_l1(ptr, ndbl, s)
   int ***ptr, ndbl;
   char *s;
{
   if((*ptr = (int **)realloc(*ptr, ndbl * sizeof(int *))) == NULL)
      syserr("realloc_dbl_int_l1","realloc",s);
}

/*******************************************************************/
realloc_dbl_char_l1(ptr, ndbl, s)
   char ***ptr;
   int ndbl;
   char *s;
{
   if((*ptr = (char **)realloc(*ptr, ndbl * sizeof(char *))) == NULL)
      syserr("realloc_dbl_char_l1","realloc",s);
}

/*******************************************************************/
realloc_dbl_uchar_l1(ptr, ndbl, s)
   unsigned char ***ptr;
   int ndbl;
   char *s;
{
   if((*ptr = (unsigned char **)realloc(*ptr, ndbl * sizeof(unsigned char *))) == NULL)
      syserr("realloc_dbl_uchar_l1","realloc",s);
}

/*******************************************************************/
realloc_dbl_flt_l1(ptr, ndbl, s)
   float ***ptr;
   int ndbl;
   char *s;
{
   if((*ptr = (float **)realloc(*ptr, ndbl * sizeof(float *))) == NULL)
      syserr("realloc_dbl_flt_l1","realloc",s);
}
