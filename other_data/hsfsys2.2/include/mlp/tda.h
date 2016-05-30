/* Two-dimensional arrays with dimensions that are variables. */

#ifndef _TDA_INCLUDE
#define _TDA_INCLUDE

typedef struct {
  int dim2;
  char *buf;
} TDA_CHAR;

typedef struct {
  int dim2;
  int *buf;
} TDA_INT;

typedef struct {
  int dim2;
  float *buf;
} TDA_FLOAT;

/* "Element" macro: refers to the (i,j) element of atda, which can
be a TDA_CHAR, a TDA_INT, or a TDA_FLOAT. */
#define e(atda,i,j) (*((atda).buf+(i)*(atda).dim2+(j)))

#endif
