/*********************************************************************/
/*          General Purpose Defines                                  */
/*********************************************************************/
#ifndef True
#define True		1
#define False		0
#endif

#ifndef TRUE
#define TRUE		True
#define FALSE		False
#endif

#define DEG2RAD	(double)(57.29578)

#define max(a, b)   ((a) > (b) ? (a) : (b))
#define min(a, b)   ((a) < (b) ? (a) : (b))
#define sround(x) ((int) (((x)<0) ? (x)-0.5 : (x)+0.5))

#ifndef None
#define None		-1
#endif

#define FOUND            1
#define NOT_FOUND       -1

#ifndef CHUNKS
#define CHUNKS          100
#endif
